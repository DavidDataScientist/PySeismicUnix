# succepstrum

SUCCEPSTRUM - Compute the complex CEPSTRUM of a seismic trace sucepstrum < stdin > stdout

## Synopsis

```bash
succepstrum sucepstrum < stdin > stdout
```

## Required Parameters

none

## Optional Parameters

sign1=1		sign of real to complex transform
sign2=-1		sign of complex to complex (inverse) transform
...phase unwrapping .....
mode=ouphase		Oppenheim's algorithm for phase unwrapping
=suphase  simple unwrap phase
unwrap=1	 |dphase| > pi/unwrap constitutes a phase wrapping
(operative only for mode=suphase)
trend=1		deramp the phase, =0 do not deramp the phase
zeromean=0		assume phase starts at 0,  =1 phase is zero mean

## Notes

The cepstrum is defined as the fourier transform of the the decibel
spectrum, as though it were a time domain signal.
CC(t) = FT[ln[T(omega)] ] = FT[ ln|T(omega)| + i phi(omega) ]
T(omega) = |T(omega)| exp(i phi(omega))
phi(omega) = unwrapped phase of T(omega)
Phase unwrapping:
The mode=ouphase uses the phase unwrapping method of Oppenheim and
Schaffer, 1975, which operates integrating the derivative of the phase
The mode=suphase generates unwrapped phase assuming that jumps
in phase larger than dphase=pi/unwrap constitute a phase wrapping. In this case
the jump in phase is replaced with the average of the jumps in phase
on either side of the location where the suspected phase wrapping occurs.
In either mode, the user has the option of de-ramping the phase, by
removing its linear trend via trend=1 and of deciding whether the
phase starts at phase=0 or is of  zero mean via zeromean=1.

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
