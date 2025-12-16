# sufilter

SUFILTER - applies a zero-phase, sine-squared tapered filter

## Synopsis

```bash
sufilter <stdin >stdout [optional parameters]
```

## Required Parameters

if dt is not set in header, then dt is mandatory

## Optional Parameters

f=f1,f2,...             array of filter frequencies(HZ)
amps=a1,a2,...          array of filter amplitudes
dt = (from header)      time sampling interval (sec)
verbose=0		=1 for advisory messages
Defaults:f=.10*(nyquist),.15*(nyquist),.45*(nyquist),.50*(nyquist)
(nyquist calculated internally)
amps=0.,1.,...,1.,0.  trapezoid-like bandpass filter

## Examples

```
Bandpass:   sufilter <data f=10,20,40,50 | ...
Bandreject: sufilter <data f=10,20,30,40 amps=1.,0.,0.,1. | ..
Lowpass:    sufilter <data f=10,20,40,50 amps=1.,1.,0.,0. | ...
Highpass:   sufilter <data f=10,20,40,50 amps=0.,0.,1.,1. | ...
Notch:      sufilter <data f=10,12.5,35,50,60 amps=1.,.5,0.,.5,1. |..
```

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
