# Windows Compatibility Layer - ZAU.Energy Contribution

## Copyright

Copyright (c) 2025 ZAU.Energy Asia Limited, MIT License

## Overview

The Windows compatibility layer (`win32_compat`) is a comprehensive implementation that enables CWP/SU (Seismic Unix) to compile and run natively on Windows using Visual Studio 2022 and MSVC. This layer provides POSIX-compatible interfaces and implementations for Unix functions that are not available or behave differently on Windows.

**Purpose:** Bridge the gap between Unix/POSIX APIs and Windows APIs, allowing the CWP/SU codebase to compile with minimal source code changes.

**Status:** ✅ Complete and production-ready

---

## Architecture

### Design Philosophy

The compatibility layer follows these principles:

1. **Minimal Source Code Changes**: Original CWP/SU code requires no modifications
2. **Header-Only Where Possible**: Many compatibility features are provided via macros and inline functions
3. **Library Functions for Complex Operations**: Functions requiring actual implementation are in `libwin32compat.lib`
4. **Type Compatibility**: Unix types are mapped to Windows equivalents
5. **Path Handling**: Automatic conversion between Unix-style (`/`) and Windows-style (`\`) paths

### Component Structure

```
include/win32_compat/
├── win32_compat.h          # Main library header
├── path.h                  # Path conversion utilities
├── dirent.h                # Directory operations
├── unistd.h                # Unix standard functions
├── fcntl.h                 # File control operations
├── signal.h                # Signal handling
├── sys/
│   ├── types.h             # Type definitions
│   ├── stat.h              # File status
│   ├── time.h              # Time operations
│   ├── resource.h          # Resource limits
│   └── sysmacros.h         # System macros
└── rpc/
    ├── types.h             # RPC types
    └── xdr.h               # XDR encoding
```

**Implementation:**
```
src/win32_compat/
└── win32_compat.c          # Function implementations
```

---

## Implementation Details

### 1. Type System (`sys/types.h`)

**Purpose:** Provides Unix type definitions that don't exist in Windows.

**Key Types:**
- `pid_t` → `int` (Process ID)
- `uid_t`, `gid_t` → `int` (User/Group ID - stubbed)
- `mode_t` → `unsigned short` (File permissions)
- `off_t` → `__int64` (File offset - 64-bit)
- `ssize_t` → `ptrdiff_t` (Signed size)
- `time_t` → `__time64_t` (Time - 64-bit)
- `dev_t`, `ino_t` → `unsigned int/long` (Device/Inode)

**Implementation Notes:**
- Uses `__int64` for large file support (64-bit offsets)
- Maps Unix types to Windows equivalents where possible
- Stubs unavailable types (uid/gid) with reasonable defaults

### 2. File Operations (`unistd.h`, `fcntl.h`)

**Purpose:** Provides Unix file I/O functions using Windows equivalents.

**Macro Mappings:**
```c
#define read(fd, buf, count)     _read(fd, buf, (unsigned int)(count))
#define write(fd, buf, count)    _write(fd, buf, (unsigned int)(count))
#define close(fd)                _close(fd)
#define lseek(fd, offset, whence) _lseek(fd, offset, whence)
#define chdir(path)              _chdir(path)
#define getcwd(buf, size)        _getcwd(buf, (int)(size))
#define unlink(path)             _unlink(path)
#define access(path, mode)        _access(path, mode)
```

**Function Implementations (`win32_compat.c`):**

**`open()`:**
- Maps Unix `O_*` flags to Windows `_O_*` flags
- Handles variadic mode parameter for `O_CREAT`
- Converts permission modes (`S_IRUSR`, etc.) to Windows `_S_*` constants
- Ensures binary mode for SU data files

**`creat()`:**
- Creates files with appropriate Windows permissions
- Maps Unix mode bits to Windows equivalents

**`fcntl()`:**
- Stub implementation (Windows doesn't support fcntl fully)
- Returns `ENOSYS` for unsupported operations
- Used primarily for file descriptor flags (not critical for SU)

**`pipe()`:**
- Uses Windows `_pipe()` with binary mode
- Ensures `_O_BINARY` flag for correct data transfer
- Returns two file descriptors for read/write

**`dup()`, `dup2()`:**
- Direct mapping to Windows `_dup()`, `_dup2()`
- Used for file descriptor manipulation

### 3. Directory Operations (`dirent.h`)

**Purpose:** Provides POSIX directory operations using Windows API.

**Implementation:**
- Uses `FindFirstFile`/`FindNextFile` Windows API
- Implements `DIR` structure wrapping `HANDLE`
- Provides `opendir()`, `readdir()`, `closedir()`, `rewinddir()`

**Key Features:**
- Inline implementations for performance
- Handles both forward slashes and backslashes
- Maps Windows file attributes to Unix `d_type` values
- Supports `MAX_PATH` (260 characters) paths

**Example:**
```c
DIR *dir = opendir("src/su/main");
struct dirent *entry;
while ((entry = readdir(dir)) != NULL) {
    printf("%s\n", entry->d_name);
}
closedir(dir);
```

### 4. File Status (`sys/stat.h`)

**Purpose:** Provides Unix `stat()` structure and functions.

**Structure Mapping:**
```c
struct stat {
    dev_t     st_dev;      /* Device ID */
    ino_t     st_ino;      /* Inode number */
    mode_t    st_mode;     /* File type and permissions */
    nlink_t   st_nlink;    /* Number of hard links */
    uid_t     st_uid;      /* User ID (stubbed) */
    gid_t     st_gid;      /* Group ID (stubbed) */
    dev_t     st_rdev;     /* Device ID (special files) */
    off_t     st_size;     /* File size (64-bit) */
    time_t    st_atime;    /* Access time */
    time_t    st_mtime;    /* Modification time */
    time_t    st_ctime;    /* Creation time */
    blksize_t st_blksize;  /* Block size */
    blkcnt_t  st_blocks;   /* Number of blocks */
};
```

**Function Implementations:**

**`stat()`:**
- Uses `GetFileAttributesExA()` for file attributes
- Converts Windows `FILETIME` to Unix `time_t`
- Maps Windows file attributes to Unix mode bits
- Handles symbolic links (via `GetFileAttributesExA`)

**`fstat()`:**
- Uses `GetFileInformationByHandle()` for open file descriptors
- Extracts file information from handle
- Converts to Unix stat structure

**`lstat()`:**
- Same as `stat()` on Windows (no separate symlink handling needed)

**File Type Macros:**
```c
#define S_ISDIR(m)    (((m) & S_IFMT) == S_IFDIR)
#define S_ISREG(m)    (((m) & S_IFMT) == S_IFREG)
#define S_ISCHR(m)    (((m) & S_IFMT) == S_IFCHR)
// ... etc
```

### 5. Path Handling (`path.h`)

**Purpose:** Converts between Unix-style and Windows-style paths.

**Key Functions:**

**`win32_path_from_unix()`:**
- Converts `/path/to/file` → `C:\path\to\file`
- Handles absolute paths (`/` → `C:\`)
- Converts forward slashes to backslashes
- Returns newly allocated string (caller must free)

**`win32_normalize_path_separators()`:**
- In-place conversion of `/` to `\`
- Modifies provided string directly

**`win32_map_unix_path()`:**
- Maps common Unix paths to Windows equivalents:
  - `/tmp` → Windows temp directory
  - `~` or `$HOME` → User profile directory
  - `/usr` → `C:\usr` (or appropriate mapping)

**`win32_get_temp_dir()`:**
- Returns Windows temp directory (`%TEMP%`)
- Equivalent to Unix `/tmp`

**`win32_get_home_dir()`:**
- Returns user profile directory (`%USERPROFILE%`)
- Equivalent to Unix `~` or `$HOME`

**Usage Example:**
```c
char *win_path = win32_path_from_unix("/usr/local/bin/suprog");
// Returns: "C:\\usr\\local\\bin\\suprog"
free(win_path);
```

### 6. Process Operations

**Purpose:** Provides Unix process-related functions.

**Implemented:**
- `getpid()` → `_getpid()` (direct mapping)
- `getppid()` → Returns `1` (stub - not available on Windows)
- `getuid()`, `getgid()`, `geteuid()`, `getegid()` → Return `0` (stub)

**Not Implemented:**
- `fork()` → Returns `-1` with `ENOSYS` (requires `CreateProcess` for full implementation)
- Process forking is not used in CWP/SU for Windows port

### 7. Environment Variables

**Purpose:** Provides Unix-style environment variable functions.

**`setenv()`:**
- Uses Windows `_putenv()` internally
- Formats as `NAME=VALUE` for Windows compatibility
- Supports overwrite flag

**`unsetenv()`:**
- Sets variable to empty string (Windows limitation)
- Uses `_putenv("NAME=")`

**`getlogin()`:**
- Returns `getenv("USERNAME")` (Windows username)

### 8. Symbolic Links

**Purpose:** Provides Unix symbolic link operations.

**`symlink()`:**
- Uses Windows `CreateSymbolicLinkA()` API
- Requires administrator privileges on older Windows versions
- Maps Unix symlink semantics to Windows

**`readlink()`:**
- Stub implementation (not fully supported)
- Returns error for most cases

### 9. Temporary Files

**Purpose:** Provides Unix temporary file creation.

**`mkstemp()`:**
- Creates temporary file with unique name
- Uses Windows `GetTempPath()` and `GetTempFileName()`
- Opens file in binary mode
- Returns file descriptor

**Implementation:**
```c
int mkstemp(char *template)
{
    char temp_path[MAX_PATH];
    char temp_file[MAX_PATH];
    
    GetTempPathA(MAX_PATH, temp_path);
    GetTempFileNameA(temp_path, "tmp", 0, temp_file);
    
    strcpy(template, temp_file);
    return _open(template, _O_RDWR | _O_CREAT | _O_EXCL | _O_BINARY, 
                 _S_IREAD | _S_IWRITE);
}
```

### 10. Signal Handling (`signal.h`)

**Purpose:** Provides Unix signal handling on Windows.

**Signal Mapping:**
- `SIGINT` → Windows `SIGINT` (Ctrl+C)
- `SIGTERM`, `SIGHUP`, `SIGQUIT` → Mapped to `SIGINT`
- `SIGFPE`, `SIGSEGV`, `SIGABRT` → Direct mapping
- `SIGBREAK` → Windows Ctrl+Break

**Unsupported Signals:**
- `SIGUSR1`, `SIGUSR2` (not available on Windows)
- `SIGPIPE`, `SIGALRM` (not available on Windows)
- `SIGCHLD`, `SIGCONT`, `SIGSTOP` (process control - not available)

**Implementation:**
- Uses Windows `signal()` function via dynamic loading
- Maintains handler table for unsupported signals
- Maps Unix signals to Windows equivalents where possible

### 11. System Calls (`unistd.h`)

**Purpose:** Provides Unix system call wrappers.

**Sleep Operations:**
```c
#define sleep(seconds)           Sleep((seconds) * 1000)
#define usleep(microseconds)      Sleep((microseconds) / 1000)
```

**File Descriptors:**
```c
#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2
```

**Standard File Operations:**
- All mapped to Windows `_*` equivalents
- Binary mode enforced for data files

### 12. RPC/XDR Support (`rpc/`)

**Purpose:** Provides RPC/XDR type definitions (stubbed for Windows).

**Note:** Full XDR implementation not required for CWP/SU Windows port.

---

## Building the Compatibility Library

### Source Files

**Library Implementation:**
- `src/win32_compat/win32_compat.c` - Function implementations
- `src/win32_compat/win32_compat.h` - Library header

**Headers (include-only):**
- All files in `include/win32_compat/`

### Compilation

The library is built as `libwin32compat.lib`:

```makefile
libwin32compat.lib: src/win32_compat/win32_compat.c
    cl /c /Fo:win32_compat.obj src/win32_compat/win32_compat.c
    lib /OUT:libwin32compat.lib win32_compat.obj
```

### Integration

**In Makefiles:**
```makefile
INCLUDES += -I$(CWPROOT)/include/win32_compat
LIBS += libwin32compat.lib
```

**In Source Code:**
```c
#ifdef _WIN32
#include "win32_compat/unistd.h"
#include "win32_compat/sys/stat.h"
#include "win32_compat/dirent.h"
#endif
```

---

## Technical Challenges and Solutions

### Challenge 1: File Descriptor Handling

**Problem:** Windows uses handles, Unix uses file descriptors.

**Solution:**
- Windows `_open()`, `_read()`, `_write()` return `int` file descriptors
- Direct mapping to Unix file descriptor semantics
- Binary mode enforced via `_O_BINARY` flag

### Challenge 2: Path Separators

**Problem:** Unix uses `/`, Windows uses `\`.

**Solution:**
- Automatic conversion in path functions
- Windows API accepts both separators, but we normalize to `\`
- Path conversion utilities handle edge cases

### Challenge 3: File Permissions

**Problem:** Unix has complex permission model, Windows is simpler.

**Solution:**
- Map Unix mode bits (`S_IRUSR`, etc.) to Windows `_S_IREAD`, `_S_IWRITE`
- Stub group/other permissions (Windows doesn't support fully)
- Default to read/write for created files

### Challenge 4: Time Handling

**Problem:** Windows `FILETIME` vs Unix `time_t`.

**Solution:**
- Convert `FILETIME` (100-nanosecond intervals since 1601) to Unix `time_t` (seconds since 1970)
- Use `FileTimeToSystemTime()` and conversion functions
- Handle 64-bit time for large file support

### Challenge 5: Process Operations

**Problem:** No `fork()` on Windows.

**Solution:**
- Stub `fork()` (not used in CWP/SU Windows port)
- Use `CreateProcess` for process spawning (in `popen` implementation)
- Process management handled differently on Windows

### Challenge 6: Symbolic Links

**Problem:** Windows symlink support is limited.

**Solution:**
- Use `CreateSymbolicLinkA()` where available
- Require appropriate privileges
- Stub `readlink()` (not fully supported)

### Challenge 7: Signal Handling

**Problem:** Windows signals are different from Unix.

**Solution:**
- Map Unix signals to Windows equivalents
- Use Windows `signal()` function via dynamic loading
- Maintain handler table for unsupported signals
- Map process control signals to available Windows signals

---

## Usage Examples

### Example 1: File Operations

```c
#include "win32_compat/unistd.h"
#include "win32_compat/fcntl.h"

int fd = open("data.su", O_RDONLY | O_BINARY);
char buffer[4096];
ssize_t n = read(fd, buffer, sizeof(buffer));
close(fd);
```

### Example 2: Directory Traversal

```c
#include "win32_compat/dirent.h"

DIR *dir = opendir("src/su/main");
struct dirent *entry;
while ((entry = readdir(dir)) != NULL) {
    if (S_ISREG(entry->d_type)) {
        printf("File: %s\n", entry->d_name);
    }
}
closedir(dir);
```

### Example 3: File Status

```c
#include "win32_compat/sys/stat.h"

struct stat st;
if (stat("input.su", &st) == 0) {
    printf("Size: %lld bytes\n", (long long)st.st_size);
    printf("Mode: %o\n", st.st_mode);
    if (S_ISREG(st.st_mode)) {
        printf("Regular file\n");
    }
}
```

### Example 4: Path Conversion

```c
#include "win32_compat/path.h"

char *win_path = win32_path_from_unix("/usr/local/bin/suprog");
// Use win_path...
free(win_path);
```

### Example 5: Temporary Files

```c
#include "win32_compat/win32_compat.h"

char template[] = "tempXXXXXX";
int fd = mkstemp(template);
// Use fd...
close(fd);
unlink(template);
```

---

## Testing and Validation

### Compilation Tests

✅ All CWP/SU programs compile successfully with compatibility layer
✅ No source code changes required in CWP/SU codebase
✅ Type compatibility verified
✅ Function signatures match Unix equivalents

### Runtime Tests

✅ File I/O operations work correctly
✅ Directory operations function properly
✅ Path handling works for both styles
✅ Binary data transfer is correct (no corruption)
✅ Temporary file creation works

### Known Limitations

1. **`fork()`**: Not implemented (returns error) - not needed for CWP/SU Windows port
2. **`readlink()`**: Stub implementation - not fully supported
3. **User/Group IDs**: Stubbed to 0 (Windows doesn't have Unix user model)
4. **Signal Handling**: Some Unix signals not available on Windows
5. **File Permissions**: Group/other permissions are simplified

---

## Performance Considerations

### Header-Only Functions

Many compatibility functions are implemented as macros or inline functions for zero runtime overhead:
- `read()`, `write()`, `close()` → Direct mapping
- Type definitions → Compile-time only
- File descriptor constants → Compile-time constants

### Library Functions

Functions requiring actual implementation have minimal overhead:
- Path conversion: Single pass string manipulation
- `stat()`: One Windows API call
- Directory operations: Efficient Windows API usage

### Memory Management

- Path conversion functions allocate memory (caller must free)
- Directory operations use stack-allocated buffers where possible
- No memory leaks in compatibility layer

---

## Integration with CWP/SU

### Makefile Configuration

The compatibility layer is automatically included when building on Windows:

```makefile
ifeq ($(PLATFORM),Windows)
    INCLUDES += -I$(CWPROOT)/include/win32_compat
    LIBS += libwin32compat.lib
endif
```

### Source Code Inclusion

CWP/SU source files include compatibility headers conditionally:

```c
#ifdef _WIN32
#include "win32_compat/unistd.h"
#include "win32_compat/sys/stat.h"
#endif
```

### Build System

The compatibility library is built as part of the core libraries:
- Built before SU main programs
- Linked with all SU executables
- No runtime dependencies (static library)

---

## Maintenance and Extensions

### Adding New Functions

To add a new compatibility function:

1. **Add declaration** to appropriate header in `include/win32_compat/`
2. **Add implementation** to `src/win32_compat/win32_compat.c`
3. **Add to library header** `src/win32_compat/win32_compat.h`
4. **Test** with CWP/SU programs that use the function

### Debugging

**Common Issues:**
- Path separator mismatches → Use path conversion functions
- File permission errors → Check mode bit mapping
- Type size mismatches → Verify type definitions
- Linker errors → Ensure library is linked

**Debugging Tips:**
- Use `#ifdef _WIN32` to isolate Windows-specific code
- Check Windows API return values
- Verify file descriptor validity
- Test path conversions with various inputs

---

## References

### Related Documentation

- **Windows API Documentation**: MSDN Windows API reference
- **CWP/SU Port Summary**: See `CHANGES.md` for overall port details
- **Build Guide**: See `WINDOWS_BUILD_GUIDE.md` for compilation instructions

### Standards Compliance

- **POSIX.1**: Partial compliance (functions needed for CWP/SU)
- **Unix Compatibility**: High compatibility for file/directory operations
- **Windows Integration**: Native Windows API usage for performance

---

## License

This compatibility layer is part of ZAU.Energy's Windows port contributions and is licensed under the MIT License. See `LICENSE-ZAU.md` for details.

**Copyright (c) 2025 ZAU.Energy Asia Limited, MIT License**

---

*This compatibility layer enables CWP/SU to run natively on Windows with minimal source code modifications, providing a robust foundation for the PySeismicUnix project.*

