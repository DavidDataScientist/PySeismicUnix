# suamp

SUAMP - output amp, phase, real or imag trace from (frequency, x) domain data suamp <stdin >stdout mode=amp

## Synopsis

```bash
suamp <stdin >stdout mode=amp
```

## Required Parameters

none
Optional parameter:
mode=amp	output flag
=amp	output amplitude traces
=logamp	output log(amplitude) traces
=phase	output phase traces
=ouphase output unwrapped phase traces (oppenheim)
=suphase output unwrapped phase traces (simple)
=real	output real parts
=imag	output imag parts
jack=0	=1  divide value at zero frequency by 2
(operative only for mode=amp)
.... phase unwrapping options	.....
unwrap=1	 |dphase| > pi/unwrap constitutes a phase wrapping
(operative only for mode=suphase)
trend=1	remove linear trend from the unwrapped phase
zeromean=0	assume phase(0)=0.0, else assume phase is zero mean
smooth=0	apply damped least squares smoothing to unwrapped phase
r=10.0	    ... damping coefficient, only active when smooth=1

## Examples

```
sufft < data > complex_traces
suamp < complex_traces mode=real > real_traces
suamp < complex_traces mode=imag > imag_traces
```

## Notes

The mode=ouphase uses the phase unwrapping method of Oppenheim and
Schaffer, 1975.
The mode=suphase generates unwrapped phase assuming that jumps
in phase larger than pi/unwrap constitute a phase wrapping.

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
