# suslowift

SUSLOWIFT - Fourier Transforms by (SLOW) DFT algorithm (Not an FFT) complex frequency to real time domain traces suslowift <stdin >sdout sign=-1

## Synopsis

```bash
suslowift <stdin >sdout sign=-1
```

## Required Parameters

none

## Optional Parameters

sign=-1			sign in exponent of fft
dt=from header		sampling interval
Trace header fields accessed: ns, dt
Trace header fields modified: ns, dt, trid
Notes: To facilitate further processing, the sampling interval
in frequency and first frequency (0) are set in the
output header.
Warning: This program is *not* fft based. Use only for demo
purposes, *not* for large data processing.
No check is made that the data are real time traces!

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
