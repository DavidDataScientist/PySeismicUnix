/*
 * dirent.h - Windows compatibility header for Unix directory operations
 * 
 * This header provides POSIX-like directory operations on Windows
 * using the Windows API internally.
 */

#ifndef _WIN32_COMPAT_DIRENT_H
#define _WIN32_COMPAT_DIRENT_H

#ifdef _WIN32

#include <windows.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Maximum path length */
#ifndef PATH_MAX
#define PATH_MAX MAX_PATH
#endif

/* Directory entry structure */
struct dirent {
    ino_t d_ino;              /* Inode number (not used on Windows) */
    char d_name[MAX_PATH];    /* File name */
    unsigned char d_type;     /* Type of file */
};

/* Directory stream structure */
typedef struct {
    HANDLE handle;
    WIN32_FIND_DATAA find_data;
    struct dirent entry;
    int first;
    char path[MAX_PATH];
} DIR;

/* File type constants for d_type */
#define DT_UNKNOWN  0
#define DT_FIFO     1
#define DT_CHR      2
#define DT_DIR      4
#define DT_BLK      6
#define DT_REG      8
#define DT_LNK      10
#define DT_SOCK     12

/* Function declarations */
DIR *opendir(const char *name);
struct dirent *readdir(DIR *dir);
int closedir(DIR *dir);
void rewinddir(DIR *dir);

/* Inline implementations */

static inline DIR *opendir(const char *name)
{
    DIR *dir;
    char search_path[MAX_PATH];
    
    if (!name || !*name) {
        return NULL;
    }
    
    dir = (DIR *)malloc(sizeof(DIR));
    if (!dir) {
        return NULL;
    }
    
    /* Build search path with wildcard */
    strncpy(search_path, name, MAX_PATH - 3);
    search_path[MAX_PATH - 3] = '\0';
    
    /* Remove trailing slash/backslash if present */
    size_t len = strlen(search_path);
    if (len > 0 && (search_path[len-1] == '/' || search_path[len-1] == '\\')) {
        search_path[len-1] = '\0';
    }
    
    strcat(search_path, "\\*");
    
    dir->handle = FindFirstFileA(search_path, &dir->find_data);
    if (dir->handle == INVALID_HANDLE_VALUE) {
        free(dir);
        return NULL;
    }
    
    dir->first = 1;
    strncpy(dir->path, name, MAX_PATH - 1);
    dir->path[MAX_PATH - 1] = '\0';
    
    return dir;
}

static inline struct dirent *readdir(DIR *dir)
{
    if (!dir || dir->handle == INVALID_HANDLE_VALUE) {
        return NULL;
    }
    
    if (dir->first) {
        dir->first = 0;
    } else {
        if (!FindNextFileA(dir->handle, &dir->find_data)) {
            return NULL;
        }
    }
    
    /* Fill in dirent structure */
    dir->entry.d_ino = 0;  /* Not used on Windows */
    strncpy(dir->entry.d_name, dir->find_data.cFileName, MAX_PATH - 1);
    dir->entry.d_name[MAX_PATH - 1] = '\0';
    
    /* Determine file type */
    if (dir->find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
        dir->entry.d_type = DT_DIR;
    } else if (dir->find_data.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) {
        dir->entry.d_type = DT_LNK;
    } else {
        dir->entry.d_type = DT_REG;
    }
    
    return &dir->entry;
}

static inline int closedir(DIR *dir)
{
    if (!dir) {
        return -1;
    }
    
    if (dir->handle != INVALID_HANDLE_VALUE) {
        FindClose(dir->handle);
    }
    
    free(dir);
    return 0;
}

static inline void rewinddir(DIR *dir)
{
    if (!dir) {
        return;
    }
    
    if (dir->handle != INVALID_HANDLE_VALUE) {
        FindClose(dir->handle);
    }
    
    char search_path[MAX_PATH];
    strncpy(search_path, dir->path, MAX_PATH - 3);
    search_path[MAX_PATH - 3] = '\0';
    
    size_t len = strlen(search_path);
    if (len > 0 && (search_path[len-1] == '/' || search_path[len-1] == '\\')) {
        search_path[len-1] = '\0';
    }
    strcat(search_path, "\\*");
    
    dir->handle = FindFirstFileA(search_path, &dir->find_data);
    dir->first = 1;
}

#ifdef __cplusplus
}
#endif

#endif /* _WIN32 */

#endif /* _WIN32_COMPAT_DIRENT_H */

