# suminphase

SUMINPHASE - convert input to minimum phase

## Synopsis

```bash
suminphase <stdin >stdout [optional parameters]
```

## Required Parameters

if dt is not set in header, then dt is mandatory

## Optional Parameters

sign1=1		sign of first transform	(1 or -1)
sign2=-1	sign of second transform (-1 or 1)
pnoise=1.e-9	   white noise in spectral routine
verbose=0		=1 for advisory messages

## Examples

```
suclogfft < shotgather | suamp mode=real | sustack key=dt > real.su
suclogfft < shotgather | suamp mode=imag | sustack key=dt > imag.su
suop2 real.su imag.su op=zipper | suiclogfft | suminphase > wavelet.su
```

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
