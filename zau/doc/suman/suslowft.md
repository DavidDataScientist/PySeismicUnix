# suslowft

SUSLOWFT - Fourier Transforms by a (SLOW) DFT algorithm (Not an FFT) suslowft <stdin >sdout sign=1

## Synopsis

```bash
suslowft <stdin >sdout sign=1
```

## Required Parameters

none

## Optional Parameters

sign=1			sign in exponent of fft
dt=from header		sampling interval
Trace header fields accessed: ns, dt
Trace header fields modified: ns, dt, trid
Notes: To facilitate further processing, the sampling interval
in frequency and first frequency (0) are set in the
output header.
Warning: This program is *not* fft based. Use only for demo
purposes, *not* for large data processing.
No check is made that the data are real time traces!
suslowft | suslowift is not quite a no-op since the trace
length will usually be longer due to fft padding.
Caveats:
No check is made that the data IS real time traces!
Output is type complex. To view amplitude, phase or real, imaginary
parts, use    suamp

## Examples

```
suslowft < stdin | suamp mode=amp | ....
suslowft < stdin | suamp mode=phase | ....
suslowft < stdin | suamp mode=real | ....
suslowft < stdin | suamp mode=imag | ....
```

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
