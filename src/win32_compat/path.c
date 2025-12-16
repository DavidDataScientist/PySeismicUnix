/* Windows path compatibility utilities implementation */

#ifdef _WIN32

#include "../include/win32_compat/path.h"
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

/* Convert Unix-style path to Windows-style path */
char *win32_path_from_unix(const char *unix_path)
{
    if (unix_path == NULL) {
        return NULL;
    }
    
    size_t len = strlen(unix_path);
    char *win_path = (char *)malloc(len + 1);
    if (win_path == NULL) {
        return NULL;
    }
    
    strcpy(win_path, unix_path);
    win32_normalize_path_separators(win_path);
    
    /* Handle absolute paths starting with / */
    if (len > 0 && win_path[0] == '\\') {
        /* Convert / to C:\ (or current drive) */
        char drive[4];
        if (GetCurrentDirectoryA(4, drive) > 0 && drive[1] == ':') {
            /* Use current drive */
            char *new_path = (char *)malloc(len + 3);
            if (new_path != NULL) {
                sprintf(new_path, "%c:%s", drive[0], win_path);
                free(win_path);
                win_path = new_path;
            }
        } else {
            /* Default to C: */
            char *new_path = (char *)malloc(len + 3);
            if (new_path != NULL) {
                sprintf(new_path, "C:%s", win_path);
                free(win_path);
                win_path = new_path;
            }
        }
    }
    
    return win_path;
}

/* Convert Unix-style path to Windows-style path (in-place) */
char *win32_path_from_unix_inplace(char *path, size_t bufsize)
{
    if (path == NULL || bufsize == 0) {
        return NULL;
    }
    
    win32_normalize_path_separators(path);
    
    /* Handle absolute paths starting with / */
    if (path[0] == '\\' && bufsize >= 4) {
        /* Convert / to C:\ (or current drive) */
        char drive[4];
        if (GetCurrentDirectoryA(4, drive) > 0 && drive[1] == ':') {
            /* Shift path and prepend drive */
            size_t len = strlen(path);
            if (len + 2 < bufsize) {
                memmove(path + 2, path, len + 1);
                path[0] = drive[0];
                path[1] = ':';
            }
        } else {
            /* Default to C: */
            size_t len = strlen(path);
            if (len + 2 < bufsize) {
                memmove(path + 2, path, len + 1);
                path[0] = 'C';
                path[1] = ':';
            }
        }
    }
    
    return path;
}

/* Normalize path separators */
char *win32_normalize_path_separators(char *path)
{
    if (path == NULL) {
        return NULL;
    }
    
    /* Convert forward slashes to backslashes */
    for (char *p = path; *p != '\0'; p++) {
        if (*p == '/') {
            *p = '\\';
        }
    }
    
    return path;
}

/* Get temporary directory path */
char *win32_get_temp_dir(void)
{
    char temp_path[MAX_PATH + 1];
    DWORD len = GetTempPathA(MAX_PATH, temp_path);
    
    if (len == 0 || len > MAX_PATH) {
        return NULL;
    }
    
    /* Remove trailing backslash if present */
    if (len > 0 && temp_path[len - 1] == '\\') {
        temp_path[len - 1] = '\0';
        len--;
    }
    
    char *result = (char *)malloc(len + 1);
    if (result != NULL) {
        strcpy(result, temp_path);
    }
    
    return result;
}

/* Get user home directory path */
char *win32_get_home_dir(void)
{
    const char *home = getenv("USERPROFILE");
    if (home == NULL) {
        home = getenv("HOMEDRIVE");
        if (home != NULL) {
            const char *homepath = getenv("HOMEPATH");
            if (homepath != NULL) {
                size_t len = strlen(home) + strlen(homepath) + 1;
                char *result = (char *)malloc(len);
                if (result != NULL) {
                    sprintf(result, "%s%s", home, homepath);
                    return result;
                }
            }
        }
        return NULL;
    }
    
    size_t len = strlen(home) + 1;
    char *result = (char *)malloc(len);
    if (result != NULL) {
        strcpy(result, home);
    }
    
    return result;
}

/* Check if path is absolute */
int win32_is_absolute_path(const char *path)
{
    if (path == NULL || path[0] == '\0') {
        return 0;
    }
    
    /* Windows absolute path: C:\ or \\ */
    if ((path[0] >= 'A' && path[0] <= 'Z' || path[0] >= 'a' && path[0] <= 'z') &&
        path[1] == ':' && (path[2] == '\\' || path[2] == '/')) {
        return 1;
    }
    
    /* UNC path: \\ */
    if (path[0] == '\\' && path[1] == '\\') {
        return 1;
    }
    
    /* Unix-style absolute path starting with / or \ */
    if (path[0] == '/' || path[0] == '\\') {
        return 1;
    }
    
    return 0;
}

/* Join path components */
char *win32_path_join(const char *base, ...)
{
    if (base == NULL) {
        return NULL;
    }
    
    va_list args;
    va_start(args, base);
    
    size_t total_len = strlen(base) + 1;
    const char *component;
    
    /* Calculate total length */
    while ((component = va_arg(args, const char *)) != NULL) {
        total_len += strlen(component) + 1; /* +1 for separator */
    }
    va_end(args);
    
    /* Allocate buffer */
    char *result = (char *)malloc(total_len);
    if (result == NULL) {
        return NULL;
    }
    
    strcpy(result, base);
    win32_normalize_path_separators(result);
    
    /* Remove trailing separator from base if present */
    size_t base_len = strlen(result);
    if (base_len > 0 && (result[base_len - 1] == '\\' || result[base_len - 1] == '/')) {
        result[base_len - 1] = '\0';
        base_len--;
    }
    
    /* Append components */
    va_start(args, base);
    while ((component = va_arg(args, const char *)) != NULL) {
        /* Add separator */
        result[base_len] = '\\';
        base_len++;
        
        /* Copy component */
        strcpy(result + base_len, component);
        win32_normalize_path_separators(result + base_len);
        base_len += strlen(component);
        
        /* Remove trailing separator if present */
        if (base_len > 0 && (result[base_len - 1] == '\\' || result[base_len - 1] == '/')) {
            result[base_len - 1] = '\0';
            base_len--;
        }
    }
    va_end(args);
    
    return result;
}

/* Convert common Unix paths to Windows equivalents */
char *win32_map_unix_path(const char *unix_path)
{
    if (unix_path == NULL) {
        return NULL;
    }
    
    /* Handle /tmp */
    if (strcmp(unix_path, "/tmp") == 0 || strncmp(unix_path, "/tmp/", 5) == 0) {
        char *temp_dir = win32_get_temp_dir();
        if (temp_dir != NULL) {
            if (strlen(unix_path) > 5) {
                /* Append rest of path */
                char *result = win32_path_join(temp_dir, unix_path + 5, NULL);
                free(temp_dir);
                return result;
            }
            return temp_dir;
        }
    }
    
    /* Handle ~ or $HOME */
    if (unix_path[0] == '~' || strcmp(unix_path, "$HOME") == 0 || 
        strncmp(unix_path, "$HOME/", 6) == 0) {
        char *home_dir = win32_get_home_dir();
        if (home_dir != NULL) {
            if (unix_path[0] == '~' && unix_path[1] != '\0') {
                /* Append rest of path after ~ */
                char *result = win32_path_join(home_dir, unix_path + 1, NULL);
                free(home_dir);
                return result;
            } else if (strncmp(unix_path, "$HOME/", 6) == 0) {
                /* Append rest of path after $HOME/ */
                char *result = win32_path_join(home_dir, unix_path + 6, NULL);
                free(home_dir);
                return result;
            }
            return home_dir;
        }
    }
    
    /* Handle /usr/local/cwp or similar - map to C:\usr\local\cwp */
    if (unix_path[0] == '/') {
        return win32_path_from_unix(unix_path);
    }
    
    /* For relative paths, just normalize separators */
    size_t len = strlen(unix_path) + 1;
    char *result = (char *)malloc(len);
    if (result != NULL) {
        strcpy(result, unix_path);
        win32_normalize_path_separators(result);
    }
    
    return result;
}

#endif /* _WIN32 */

