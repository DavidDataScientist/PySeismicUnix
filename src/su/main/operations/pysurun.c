/* Copyright (c) Zau.ENERGY ASIA aka Zau.AI 2025 .*/
/* All rights reserved.                       */

/* PYSRUN: $Revision: 1.0 $ ; $Date: 2025/12/07 $		*/

#include "su.h"
#include "segy.h"
#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*********************** self documentation ******************************/
char *sdoc[] = {
" 									",
" PYSRUN - run Python SU flow plugins from SU pipeline			",
" 									",
" pysurun <stdin >stdout plugin=name [args...]				",
" 									",
" Required parameters:							",
" 	plugin=name		name of Python plugin to execute		",
" 									",
" Optional parameters:							",
" 	python=python		Python interpreter command (default: python)",
" 	venv=path		Path to virtual environment (optional)	",
" 				If specified, uses venv/Scripts/python.exe (Windows)",
" 				or venv/bin/python (Unix) instead of python=	",
" 	module=zau.src.pysu.plugins	Python module path (default: zau.src.pysu.plugins)",
" 	verbose=0		=1 for echoing useful information	",
" 									",
" Notes:								",
" This program provides a bridge between SU pipelines and Python-based	",
" plugins. It parses SU-style arguments, launches Python with the	",
" specified plugin, and pipes SU trace data through the plugin.		",
" 									",
" The plugin must be registered in the Python plugin factory and must	",
" follow the SuFlowPlugin interface (read from stdin, write to stdout).	",
" 									",
" Examples:								",
" 	pysurun plugin=pysuflipsign < input.su > output.su		",
" 	sugain | pysurun plugin=pysuflipsign | surange			",
" 	pysurun plugin=pysuflipsign venv=./venv < input.su > output.su",
" 									",
" 									",
NULL};

/* Credits:
 *	CWP: Based on SU program structure (uses CWP-SU License)
 *	ZAU: Python plugin integration (uses Zau.ENERGY ASIA License)
 */

/**************** end self doc *******************************************/

/* Default Python interpreter */
#define DEFAULT_PYTHON "python"
#define DEFAULT_MODULE "zau.src.pysu.plugins"

int
main(int argc, char **argv)
{
	cwp_String plugin_name = NULL;	/* Plugin name (required)	*/
	cwp_String python_cmd = NULL;	/* Python interpreter command	*/
	cwp_String venv_path = NULL;	/* Virtual environment path	*/
	cwp_String module_path = NULL;	/* Python module path		*/
	int verbose = 0;		/* Verbose flag			*/
	char command[BUFSIZ];		/* Command to execute		*/
	char python_path[BUFSIZ];	/* Full path to Python executable */
	FILE *pipe_fp = NULL;		/* Pipe to Python process	*/
	char buffer[BUFSIZ];		/* Buffer for data transfer	*/
	size_t bytes_read;		/* Bytes read from stdin	*/
	int exit_code = 0;		/* Exit code from Python	*/

	/* Initialize */
	initargs(argc, argv);
	requestdoc(1);

	/* Get parameters */
	if (!getparstring("plugin", &plugin_name)) {
		err("plugin=name is required");
	}
	
	/* Check for venv parameter first */
	if (getparstring("venv", &venv_path)) {
		/* Construct Python path from venv */
#ifdef _WIN32
		snprintf(python_path, sizeof(python_path), "%s\\Scripts\\python.exe", venv_path);
#else
		snprintf(python_path, sizeof(python_path), "%s/bin/python", venv_path);
#endif
		python_cmd = python_path;
	} else if (!getparstring("python", &python_cmd)) {
		/* No venv and no python specified, use default */
		python_cmd = DEFAULT_PYTHON;
	}
	/* If both venv= and python= are specified, venv= takes precedence */
	
	if (!getparstring("module", &module_path)) {
		module_path = DEFAULT_MODULE;
	}
	if (!getparint("verbose", &verbose)) {
		verbose = 0;
	}

	/* Build Python command */
	/* Format: python -m module_path plugin_name [remaining args] */
	snprintf(command, sizeof(command), "%s -m %s %s", 
		 python_cmd, module_path, plugin_name);

	/* Append remaining command-line arguments (excluding plugin=, python=, venv=, module=, verbose=) */
	{
		int i;
		for (i = 1; i < argc; i++) {
			/* Skip parameters we've already handled */
			if (strncmp(argv[i], "plugin=", 7) == 0) continue;
			if (strncmp(argv[i], "python=", 7) == 0) continue;
			if (strncmp(argv[i], "venv=", 5) == 0) continue;
			if (strncmp(argv[i], "module=", 7) == 0) continue;
			if (strncmp(argv[i], "verbose=", 8) == 0) continue;
			/* Skip help flags - they're handled by requestdoc */
			if (strcmp(argv[i], "-?") == 0) continue;
			if (strcmp(argv[i], "-h") == 0) continue;
			if (strcmp(argv[i], "--help") == 0) continue;
			
			/* Append argument to command */
			if (strlen(command) + strlen(argv[i]) + 2 < sizeof(command)) {
				strcat(command, " ");
				strcat(command, argv[i]);
			}
		}
	}

	if (verbose) {
		warn("Executing: %s", command);
	}

	/* 
	 * Open pipe to Python process for writing.
	 * When using popen(command, "w"), we write to the process's stdin,
	 * and the process's stdout should be inherited by our stdout.
	 * This works on Unix and should work on Windows with _popen.
	 */
	pipe_fp = epopen(command, "w");
	if (pipe_fp == NULL) {
		err("Failed to launch Python process: %s", command);
	}

	/* Copy stdin to Python process's stdin */
	while ((bytes_read = fread(buffer, 1, sizeof(buffer), stdin)) > 0) {
		if (fwrite(buffer, 1, bytes_read, pipe_fp) != bytes_read) {
			err("Error writing to Python process");
		}
		/* Flush to ensure data is sent immediately */
		efflush(pipe_fp);
	}

	/* Close pipe (this sends EOF to Python's stdin) and get exit code */
	exit_code = epclose(pipe_fp);
	
	/* 
	 * Note: Python's stdout should be connected to our stdout automatically
	 * when using popen(). The Python plugin writes to sys.stdout which
	 * should be inherited from our process.
	 */
	
	if (exit_code != 0) {
		if (verbose) {
			warn("Python plugin exited with code: %d", exit_code);
		}
		/* Return the exit code, but don't use err() as that would be fatal */
		return exit_code;
	}

	return CWP_Exit();
}
