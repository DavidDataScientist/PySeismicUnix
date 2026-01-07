/*
 * suflow.c - Binary pipe wrapper for SU workflows on Windows
 * 
 * Usage: suflow "cmd1 | cmd2 | cmd3" < input.su > output.su
 *        suflow "cmd1 | cmd2 | cmd3" input.su output.su
 * 
 * This program creates binary-mode pipes between SU programs,
 * avoiding Windows' default text-mode CR/LF translation that
 * corrupts binary seismic data.
 * 
 * Copyright (c) 2025 ZAU
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <process.h>
#include <windows.h>

#define MAX_COMMANDS 32
#define MAX_CMD_LEN 4096
#define PIPE_BUFSIZE 65536

/* Forward declarations */
static int parse_pipeline(char *pipeline, char *commands[], int max_cmds);
static int run_pipeline(char *commands[], int num_cmds, const char *infile, const char *outfile);
static void print_usage(const char *progname);

int main(int argc, char *argv[])
{
    char *commands[MAX_COMMANDS];
    int num_cmds;
    const char *infile = NULL;
    const char *outfile = NULL;
    char *pipeline;
    int result;

    /* Set stdin/stdout to binary mode */
    _setmode(_fileno(stdin), _O_BINARY);
    _setmode(_fileno(stdout), _O_BINARY);
    _setmode(_fileno(stderr), _O_BINARY);

    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    pipeline = argv[1];

    /* Optional input/output files */
    if (argc >= 3) {
        infile = argv[2];
    }
    if (argc >= 4) {
        outfile = argv[3];
    }

    /* Parse the pipeline string */
    num_cmds = parse_pipeline(pipeline, commands, MAX_COMMANDS);
    if (num_cmds <= 0) {
        fprintf(stderr, "suflow: No valid commands in pipeline\n");
        return 1;
    }

    /* Run the pipeline */
    result = run_pipeline(commands, num_cmds, infile, outfile);

    /* Free command strings */
    for (int i = 0; i < num_cmds; i++) {
        free(commands[i]);
    }

    return result;
}

/*
 * Parse pipeline string into array of command strings
 * Returns number of commands parsed
 */
static int parse_pipeline(char *pipeline, char *commands[], int max_cmds)
{
    char *p = pipeline;
    char *start;
    int count = 0;
    int in_quotes = 0;

    while (*p && count < max_cmds) {
        /* Skip leading whitespace */
        while (*p == ' ' || *p == '\t') p++;
        if (!*p) break;

        start = p;

        /* Find end of command (pipe or end of string) */
        while (*p) {
            if (*p == '"') {
                in_quotes = !in_quotes;
            } else if (*p == '|' && !in_quotes) {
                break;
            }
            p++;
        }

        /* Extract command */
        int len = (int)(p - start);
        
        /* Trim trailing whitespace */
        while (len > 0 && (start[len-1] == ' ' || start[len-1] == '\t')) {
            len--;
        }

        if (len > 0) {
            commands[count] = (char *)malloc(len + 1);
            if (!commands[count]) {
                fprintf(stderr, "suflow: Memory allocation failed\n");
                return -1;
            }
            strncpy(commands[count], start, len);
            commands[count][len] = '\0';
            count++;
        }

        if (*p == '|') p++;  /* Skip pipe character */
    }

    return count;
}

/*
 * Run a pipeline of commands using temp files for reliable binary transfer
 * This approach works around Windows pipe limitations
 */
static int run_pipeline(char *commands[], int num_cmds, const char *infile, const char *outfile)
{
    char **tempfiles = NULL;
    int i;
    int result = 0;
    char cmd_line[MAX_CMD_LEN];
    char temp_dir[MAX_PATH];
    
    /* Get temp directory */
    if (!GetTempPathA(MAX_PATH, temp_dir)) {
        strcpy(temp_dir, ".");
    }

    /* Allocate temp file names (we need num_cmds - 1 temp files for intermediate results) */
    if (num_cmds > 1) {
        tempfiles = (char **)calloc(num_cmds - 1, sizeof(char *));
        if (!tempfiles) {
            fprintf(stderr, "suflow: Memory allocation failed\n");
            return 1;
        }
        
        for (i = 0; i < num_cmds - 1; i++) {
            tempfiles[i] = (char *)malloc(MAX_PATH);
            if (!tempfiles[i]) {
                fprintf(stderr, "suflow: Memory allocation failed\n");
                result = 1;
                goto cleanup;
            }
            snprintf(tempfiles[i], MAX_PATH, "%ssuflow_tmp_%d_%d.su", 
                     temp_dir, GetCurrentProcessId(), i);
        }
    }

    /* Run each command */
    for (i = 0; i < num_cmds; i++) {
        const char *input_file;
        const char *output_file;
        
        /* Determine input for this command */
        if (i == 0) {
            input_file = infile;  /* First command uses provided input (or stdin if NULL) */
        } else {
            input_file = tempfiles[i - 1];  /* Use output of previous command */
        }
        
        /* Determine output for this command */
        if (i == num_cmds - 1) {
            output_file = outfile;  /* Last command uses provided output (or stdout if NULL) */
        } else {
            output_file = tempfiles[i];  /* Use temp file */
        }
        
        /* Build command line with redirections */
        if (input_file && output_file) {
            snprintf(cmd_line, MAX_CMD_LEN, "cmd /c \"%s < \"%s\" > \"%s\"\"",
                     commands[i], input_file, output_file);
        } else if (input_file) {
            snprintf(cmd_line, MAX_CMD_LEN, "cmd /c \"%s < \"%s\"\"",
                     commands[i], input_file);
        } else if (output_file) {
            snprintf(cmd_line, MAX_CMD_LEN, "cmd /c \"%s > \"%s\"\"",
                     commands[i], output_file);
        } else {
            snprintf(cmd_line, MAX_CMD_LEN, "%s", commands[i]);
        }
        
        /* Execute command */
        result = system(cmd_line);
        if (result != 0) {
            fprintf(stderr, "suflow: Command failed with exit code %d: %s\n", 
                    result, commands[i]);
            goto cleanup;
        }
    }

cleanup:
    /* Delete temp files */
    if (tempfiles) {
        for (i = 0; i < num_cmds - 1; i++) {
            if (tempfiles[i]) {
                DeleteFileA(tempfiles[i]);
                free(tempfiles[i]);
            }
        }
        free(tempfiles);
    }

    return result;
}

static void print_usage(const char *progname)
{
    fprintf(stderr, "suflow - Binary pipe wrapper for SU workflows on Windows\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "Usage: %s \"cmd1 | cmd2 | cmd3\" [input.su] [output.su]\n", progname);
    fprintf(stderr, "\n");
    fprintf(stderr, "Examples:\n");
    fprintf(stderr, "  %s \"sugain agc=1 | sufilter\" input.su output.su\n", progname);
    fprintf(stderr, "  %s \"sugain agc=1 | sufilter | sustack\" data.su result.su\n", progname);
    fprintf(stderr, "\n");
    fprintf(stderr, "This program handles binary data transfer between SU programs\n");
    fprintf(stderr, "by using temporary files, avoiding Windows pipe corruption.\n");
}
