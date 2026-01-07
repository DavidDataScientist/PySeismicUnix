# Include Directory - Windows Port Modifications

## Copyright

Copyright (c) 2025 ZAU.Energy Asia Limited, MIT License

## Overview

This directory contains header files for CWP/SU (Seismic Unix). During the Windows port, several headers were modified to enable compilation with Visual Studio 2022 and MSVC. This document describes all Windows-specific changes made to the include directory.

**Note:** The `win32_compat/` subdirectory contains a complete Windows compatibility layer. See [`win32_compat/README.md`](win32_compat/README.md) for detailed documentation of that component.

---

## Modified Headers

### 1. `cwp.h` - Core CWP Header

**File:** `include/cwp.h`  
**Purpose:** Main header for CWP (Center for Wave Phenomena) core functionality

#### Windows Compatibility Changes

##### 1.1 NOMINMAX Definition

**Problem:** Windows SDK defines `min` and `max` as macros in `windows.h`, which conflicts with CWP code that uses `min()` and `max()` as function names.

**Solution:**
```c
#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
/* Undefine min/max if already defined (by minwindef.h) */
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#endif
```

**Location:** Lines 21-37  
**Impact:** Prevents macro expansion conflicts with function names

##### 1.2 WIN32_LEAN_AND_MEAN

**Problem:** Including `windows.h` pulls in many unnecessary headers, including `winsock.h` which defines `SOCKET` as a type, conflicting with CWP's `SOCKET` enum value.

**Solution:**
```c
#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#endif
```

**Location:** Lines 24-26, 51-53  
**Impact:** Reduces header bloat and prevents `SOCKET` type conflict

##### 1.3 Complex Type Handling

**Problem:** MSVC's `<complex.h>` may define `complex` as a macro, conflicting with CWP's complex number type definitions.

**Solution:**
```c
#ifdef _WIN32
/* After including standard headers, undefine complex if it was defined as a macro */
#ifdef complex
#undef complex
#endif
#ifdef _Complex
#undef _Complex
#endif
#endif
```

**Location:** Lines 38-46, 57-63, 99-117  
**Impact:** Ensures CWP's `cwp_complex` type is used instead of compiler macros

##### 1.4 Windows Compatibility Headers

**Problem:** Unix headers (`fcntl.h`, `unistd.h`, `sys/types.h`) are not available on Windows.

**Solution:**
```c
#ifdef _WIN32
#include "win32_compat/fcntl.h"
#include "win32_compat/unistd.h"
#include "win32_compat/sys/types.h"
#else
#include <fcntl.h>      /* non-ANSI */
#include <unistd.h>     /* non-ANSI */
#include <sys/types.h>  /* non-ANSI */
#endif
```

**Location:** Lines 49-68  
**Impact:** Provides POSIX-compatible headers on Windows

##### 1.5 SOCKET Enum Conflict

**Problem:** On Windows, `SOCKET` is a type defined in `winsock.h`, conflicting with CWP's `FileType` enum that uses `SOCKET` as a value.

**Solution:**
```c
#ifdef _WIN32
/* On Windows, SOCKET is a type defined in winsock.h, so use a different name */
typedef enum {BADFILETYPE = -1,
        TTY, DISK, DIRECTORY, TAPE, PIPE, FIFO, SOCKET_FILE, SYMLINK} FileType;
#else
typedef enum {BADFILETYPE = -1,
        TTY, DISK, DIRECTORY, TAPE, PIPE, FIFO, SOCKET, SYMLINK} FileType;
#endif
```

**Location:** Lines 78-85  
**Impact:** Uses `SOCKET_FILE` instead of `SOCKET` on Windows to avoid type conflict

##### 1.6 CSIZE Macro Definition

**Problem:** The `CSIZE` macro (size of complex number type) was not correctly defined for Windows builds.

**Solution:**
```c
#if defined(CRAY) || defined(OVERRIDE_CWP_COMPLEX) || defined(_WIN32)
/* Use custom cwp_complex type to avoid conflicts with compiler-specific complex macros */
typedef struct _complexStruct { /* complex number */
	float r,i;
}  cwp_complex;

/* Define CSIZE for Windows/CRAY - use cwp_complex */
#ifndef CSIZE
#define CSIZE sizeof(cwp_complex)
#endif
#endif
```

**Location:** Lines 87-124, 166-172  
**Impact:** Ensures `CSIZE` is correctly defined for Windows using `cwp_complex`

##### 1.7 Custom Complex Type for Windows

**Problem:** MSVC's complex number handling differs from Unix compilers.

**Solution:**
- Windows uses `cwp_complex` and `cwp_dcomplex` custom types
- These types are aliased as `complex` and `dcomplex` via macros
- All complex operations use the custom types

**Location:** Lines 87-149  
**Impact:** Provides consistent complex number handling across platforms

---

### 2. `su_xdr.h` - SU XDR Header

**File:** `include/su_xdr.h`  
**Purpose:** Header for XDR (eXternal Data Representation) support in SU

#### Windows Compatibility Changes

##### 2.1 Windows RPC/XDR Headers

**Problem:** Standard Unix RPC/XDR headers (`<rpc/types.h>`, `<rpc/xdr.h>`) are not available on Windows.

**Solution:**
```c
#ifdef SUXDR
#ifdef _WIN32
#include "win32_compat/rpc/types.h"
#include "win32_compat/rpc/xdr.h"
#elif defined(SUTIRPC)
#include <tirpc/rpc/types.h>
#include <tirpc/rpc/xdr.h>
#include <tirpc/netconfig.h>
#else
#include <rpc/types.h>
#include <rpc/xdr.h>
#endif
#endif
```

**Location:** Lines 17-29  
**Impact:** Provides Windows-compatible RPC/XDR type definitions

**Note:** XDR functionality is typically disabled on Windows builds (see `CHANGES.md` section 6). The headers are provided for compatibility but XDR code paths are not compiled.

---

## New Directories

### `win32_compat/` - Windows Compatibility Layer

**Location:** `include/win32_compat/`  
**Purpose:** Complete POSIX compatibility layer for Windows

**Documentation:** See [`win32_compat/README.md`](win32_compat/README.md) for comprehensive documentation.

**Contents:**
- **Headers:** POSIX-compatible headers (`unistd.h`, `fcntl.h`, `dirent.h`, `signal.h`, etc.)
- **Types:** Unix type definitions (`sys/types.h`, `sys/stat.h`, etc.)
- **RPC:** RPC/XDR compatibility (`rpc/types.h`, `rpc/xdr.h`)
- **Utilities:** Path conversion and Windows-specific utilities

**Key Features:**
- File operations (`open`, `read`, `write`, `close`, etc.)
- Directory operations (`opendir`, `readdir`, `closedir`)
- File status (`stat`, `fstat`, `lstat`)
- Process operations (`getpid`, `fork`, `pipe`)
- Environment variables (`setenv`, `unsetenv`, `getlogin`)
- Path handling (Unix ↔ Windows conversion)
- Signal handling (Unix signal mapping)
- Temporary files (`mkstemp`)

---

## Implementation Details

### Header Inclusion Strategy

The Windows port uses conditional compilation to include appropriate headers:

```c
#ifdef _WIN32
    /* Windows-specific includes */
    #include "win32_compat/..."
#else
    /* Unix-specific includes */
    #include <...>
#endif
```

This approach:
- Maintains compatibility with original Unix code
- Provides Windows equivalents transparently
- Requires no changes to source files that include these headers

### Macro Protection

Windows headers are protected with multiple safeguards:

1. **NOMINMAX** - Prevents `min`/`max` macro conflicts
2. **WIN32_LEAN_AND_MEAN** - Reduces header bloat
3. **Complex macro undefining** - Prevents type conflicts
4. **Conditional includes** - Platform-specific header selection

### Type Compatibility

Windows types are mapped to Unix equivalents:

- `pid_t` → `int`
- `off_t` → `__int64` (64-bit file offsets)
- `time_t` → `__time64_t` (64-bit time)
- `mode_t` → `unsigned short`
- Custom complex types for MSVC compatibility

---

## Build Integration

### Compiler Flags

Windows builds include the compatibility layer via compiler flags:

```makefile
INCLUDES += -I$(CWPROOT)/include/win32_compat
```

### Library Linking

The compatibility layer implementation is in `libwin32compat.lib`:

```makefile
LIBS += libwin32compat.lib
```

### Conditional Compilation

All Windows-specific code is guarded:

```c
#ifdef _WIN32
    /* Windows code */
#endif
```

This ensures:
- Unix builds are unaffected
- Windows builds get necessary compatibility
- No runtime overhead on Unix

---

## Testing and Validation

### Compilation Tests

✅ All CWP/SU programs compile successfully with modified headers
✅ No source code changes required in CWP/SU codebase
✅ Type compatibility verified across platforms
✅ Macro conflicts resolved

### Runtime Tests

✅ Complex number operations work correctly
✅ File operations function properly
✅ Type sizes match expectations
✅ No header inclusion conflicts

---

## Known Limitations

### XDR Support

- XDR (eXternal Data Representation) is disabled on Windows
- RPC headers are provided for compatibility but not used
- XDR code paths are excluded from Windows builds

### Signal Handling

- Some Unix signals are not available on Windows
- Signal mapping provided but limited functionality
- See `win32_compat/signal.h` for details

### Process Operations

- `fork()` is not available (stubbed)
- Process forking not used in CWP/SU Windows port
- Process spawning uses `CreateProcess` where needed

---

## Maintenance Guidelines

### Adding New Windows Compatibility

When adding new Windows-specific code:

1. **Use conditional compilation:**
   ```c
   #ifdef _WIN32
       /* Windows code */
   #else
       /* Unix code */
   #endif
   ```

2. **Protect macros:**
   - Check for existing definitions
   - Undefine conflicting macros
   - Define new macros safely

3. **Include compatibility headers:**
   - Use `win32_compat/` headers where available
   - Don't duplicate functionality

4. **Test on both platforms:**
   - Verify Unix builds still work
   - Test Windows builds thoroughly

### Modifying Existing Headers

When modifying existing headers:

1. **Preserve original functionality** for Unix
2. **Add Windows support** conditionally
3. **Document changes** in this README
4. **Test compatibility** with existing code

---

## Related Documentation

- **Windows Compatibility Layer:** [`win32_compat/README.md`](win32_compat/README.md)
- **Port Summary:** `CHANGES.md` (sections 7, 8)
- **Build Guide:** `WINDOWS_BUILD_GUIDE.md`
- **License:** `LICENSE-ZAU.md`

---

## File Summary

### Modified Files

| File | Changes | Purpose |
|------|---------|---------|
| `cwp.h` | NOMINMAX, CSIZE, complex types, SOCKET enum, win32_compat includes | Core CWP functionality |
| `su_xdr.h` | Windows RPC/XDR header includes | XDR support (disabled on Windows) |

### New Directories

| Directory | Purpose | Documentation |
|-----------|---------|---------------|
| `win32_compat/` | Complete Windows compatibility layer | [`win32_compat/README.md`](win32_compat/README.md) |

---

## License

These modifications are part of ZAU.Energy's Windows port contributions and are licensed under the MIT License. See `LICENSE-ZAU.md` for details.

**Copyright (c) 2025 ZAU.Energy Asia Limited, MIT License**

---

*These header modifications enable CWP/SU to compile and run natively on Windows while maintaining full compatibility with the original Unix codebase.*

