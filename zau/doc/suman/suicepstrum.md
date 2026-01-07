# suicepstrum

SUICEPSTRUM - fft of complex log frequency traces to real time traces suicepstrum <stdin >sdout sign2=-1

## Synopsis

```bash
suicepstrum <stdin >sdout sign2=-1
```

## Required Parameters

none
Optional parameter:
sign1=1		sign in exponent of first fft
sign2=-1	sign in exponent of inverse fft
sym=0		=1 center  output
dt=tr.dt	time sampling interval (s) from header
if not set assumed to be .004s
Output traces are normalized by 1/N where N is the fft size.

## Notes

The forward  cepstral transform is the
F(t_c) = InvFFT[ln[FFT(F(t))]]
The inverse  cepstral transform is the
F(t) = InvFFT[exp[FFT(F(t_c))]]
Here t_c is the cepstral time (quefrency) domain

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
