# suattributes

SUATTRIBUTES - instantaneous trace ATTRIBUTES suattributes <stdin >stdout mode=amp

## Synopsis

```bash
suattributes <stdin >stdout mode=amp
```

## Required Parameters

none
Optional parameter:
mode=amp	output flag
=amp envelope traces
=phase phase traces
=uphase unwrapped phase traces
=freq frequency traces
=freqw Frequency Weighted Envelope
=thin  Thin-Bed (inst. freq - average freq)
=bandwith Instantaneous bandwidth
=normamp Normalized Phase (Cosine Phase)
=fdenv 1st envelope traces derivative
=sdenv 2nd envelope traces derivative
=q Ins. Q Factor
unwrap=		default unwrap=0 for mode=phase
default unwrap=1 for freq, uphase, freqw, Q
dphase_min=PI/unwrap
wint=		windowing for freqw
windowing for thin
default=1
o--------o--------o
data-1	data	data+1

## Examples

```
suvibro f1=10 f2=50 t1=0 t2=0 tv=1 | suattributes2 mode=amp | ...
suvibro f1=10 f2=50 t1=0 t2=0 tv=1 | suattributes2 mode=phase | ...
suvibro f1=10 f2=50 t1=0 t2=0 tv=1 | suattributes2 mode=freq | ...
suplane | suattributes mode=... | supswigb |...
```

## Notes

This program performs complex trace attribute analysis. The first three
attributes, amp,phase,freq are the classical Taner, Kohler, and
Sheriff, 1979 attributes.
The unwrap parameter is active only for mode=freq and mode=phase. The
quantity dphase_min is the minimum change in the phase angle taken to be
the result of phase wrapping, rather than natural phase variation in the
data. Setting unwrap=0 turns off phase-unwrapping altogether. Choosing
unwrap > 1 makes the unwrapping function more sensitive to phase changes.
Setting unwrap > 1 may be necessary to resolve higher frequencies in
data (or sample data more finely). The phase unwrapping is crude. The
differentiation needed to compute the instantaneous frequency
freq(t)= d(phase)/dt is a simple centered difference.
The mode=uphase generates uwrapped phase traces by integrating the
instantaneous amplitude traces.

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
