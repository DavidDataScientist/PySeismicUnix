# sufft

SUFFT - fft real time traces to complex frequency traces suftt <stdin >sdout sign=1

## Synopsis

```bash
sufft suftt <stdin >sdout sign=1
```

## Required Parameters

none

## Optional Parameters

sign=1			sign in exponent of fft
dt=from header		sampling interval
verbose=1		=0 to stop advisory messages
Notes: To facilitate further processing, the sampling interval
in frequency and first frequency (0) are set in the
output header.
sufft | suifft is not quite a no-op since the trace
length will usually be longer due to fft padding.
Caveats:
No check is made that the data IS real time traces!
Output is type complex. To view amplitude, phase or real, imaginary
parts, use    suamp

## Examples

```
sufft < stdin | suamp mode=amp | ....
sufft < stdin | suamp mode=phase | ....
sufft < stdin | suamp mode=uphase | ....
sufft < stdin | suamp mode=real | ....
sufft < stdin | suamp mode=imag | ....
```

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
