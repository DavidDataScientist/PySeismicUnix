# suvibro

SUVIBRO - Generates a Vibroseis sweep (linear, linear-segment, dB per Octave, dB per Hertz, T-power)

## Synopsis

```bash
suvibro [optional parameters] > out_data_file
```

## Optional Parameters

dt=0.004		time sampling interval
sweep=1	  	linear sweep
=2 linear-segment
=3 decibel per octave
=4 decibel per hertz
=5 t-power
swconst=0.0		sweep constant (see note)
f1=10.0		sweep frequency at start
f2=60.0		sweep frequency at end
tv=10.0		sweep length
phz=0.0		initial phase (radians=1 default)
radians=1		=0 degrees

## Notes

The default tapers are linear envelopes. To eliminate the
taper, choose t1=t2=0.0.
"swconst" is active only with nonlinear sweeps, i.e. when
sweep=3,4,5.
"tseg" and "fseg" arrays are used when only sweep=2
Sweep is a modulated cosine function.

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
