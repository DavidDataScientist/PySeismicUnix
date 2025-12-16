# suclogfft

SUCLOGFFT - fft real time traces to complex log frequency domain traces suclogftt <stdin >sdout sign=1

## Synopsis

```bash
suclogfft suclogftt <stdin >sdout sign=1
```

## Required Parameters

none

## Optional Parameters

sign=1			sign in exponent of fft
dt=from header		sampling interval
verbose=1		=0 to stop advisory messages
.... phase unwrapping options .....
mode=suphase	simple jump detecting phase unwrapping
=ouphase  Oppenheim's phase unwrapping
unwrap=1       |dphase| > pi/unwrap constitutes a phase wrapping
=0 no phase unwrapping	(in mode=suphase  only)
trend=1	remove linear trend from the unwrapped phase
zeromean=0     assume phase(0)=0.0, else assume phase is zero mean

## Examples

```
suclogfft < shotgather | suamp mode=real | sustack key=dt > real.su
suclogfft < shotgather | suamp mode=imag | sustack key=dt > imag.su
suop2 real.su imag.su op=zipper | suiclogfft | suminphase > wavelet.su
```

## Notes

clogfft(F(t)) = log(FFT(F(t)) = log|F(omega)| + iphi(omega)
where phi(omega) is the unwrapped phase. Note that 0< unwrap<= 1.0
allows phase unwrapping to be tuned, if desired.
To facilitate further processing, the sampling interval
in frequency and first frequency (0) are set in the
output header.
suclogfft unwrap=0 | suiclogfft is not quite a no-op since the trace
length will usually be longer due to fft padding.
Caveats:
No check is made that the data ARE real time traces!
Output is type complex. To view amplitude, phase or real, imaginary
parts, use    suamp
PI/unwrap = minimum dphase is assumed to constitute a wrap in phase
for suphase unwrapping only

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
