# suiclogfft

SUICLOGFFT - fft of complex log frequency traces to real time traces suiclogftt <stdin >sdout sign=-1

## Synopsis

```bash
suiclogfft suiclogftt <stdin >sdout sign=-1
```

## Required Parameters

none
Optional parameter:
sign=-1		sign in exponent of inverse fft
sym=0		=1 center  output
Output traces are normalized by 1/N where N is the fft size.

## Examples

```
suclogfft < shotgather | suamp mode=real | sustack key=dt > real.su
suclogfft < shotgather | suamp mode=imag | sustack key=dt > imag.su
suop2 real.su imag.su op=zipper | suiclogfft | suminphase > wavelet.su
```

## Notes

Nominally this is the inverse to the complex log fft, but
suclogfft | suiclogfft is not quite a no-op since the trace
length will usually be longer due to fft padding.

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
