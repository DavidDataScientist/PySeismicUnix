/* Windows path compatibility utilities
 * Provides functions to convert between Unix and Windows paths
 */

#ifndef _WIN32_COMPAT_PATH_H
#define _WIN32_COMPAT_PATH_H

#ifdef _WIN32

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Convert Unix-style path to Windows-style path
 * - Converts forward slashes to backslashes
 * - Handles absolute paths (/ -> C:\, /usr -> C:\usr, etc.)
 * - Returns a newly allocated string that must be freed by caller
 */
char *win32_path_from_unix(const char *unix_path);

/* Convert Unix-style path to Windows-style path (in-place)
 * - Modifies the provided buffer
 * - Returns pointer to buffer on success, NULL on failure
 */
char *win32_path_from_unix_inplace(char *path, size_t bufsize);

/* Normalize path separators (convert / to \ on Windows)
 * - Modifies the provided string in-place
 * - Returns pointer to the string
 */
char *win32_normalize_path_separators(char *path);

/* Get temporary directory path
 * - Returns Windows temp directory (equivalent to /tmp)
 * - Returns a newly allocated string that must be freed by caller
 */
char *win32_get_temp_dir(void);

/* Get user home directory path
 * - Returns Windows user profile directory (equivalent to ~ or $HOME)
 * - Returns a newly allocated string that must be freed by caller
 */
char *win32_get_home_dir(void);

/* Check if path is absolute
 * - Returns 1 if absolute, 0 if relative
 */
int win32_is_absolute_path(const char *path);

/* Join path components
 * - Joins multiple path components with appropriate separator
 * - Returns a newly allocated string that must be freed by caller
 */
char *win32_path_join(const char *base, ...);

/* Convert common Unix paths to Windows equivalents
 * - /tmp -> Windows temp directory
 * - /usr -> C:\usr (or appropriate mapping)
 * - /etc -> C:\etc (or appropriate mapping)
 * - ~ or $HOME -> User profile directory
 * - Returns a newly allocated string that must be freed by caller
 */
char *win32_map_unix_path(const char *unix_path);

#ifdef __cplusplus
}
#endif

#endif /* _WIN32 */

#endif /* _WIN32_COMPAT_PATH_H */

