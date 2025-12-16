# suradon

SURADON - compute forward or reverse Radon transform or remove multiples by using the parabolic Radon transform to estimate multiples and subtract.

## Synopsis

```bash
suradon <stdin >stdout [Optional Parameters]
```

## Optional Parameters

choose=0    0  Forward Radon transform
1  Compute data minus multiples
2  Compute estimate of multiples
3  Compute forward and reverse transform
4  Compute inverse Radon transform
igopt=1     1  parabolic transform: g(x) = offset**2
2  Foster/Mosher psuedo hyperbolic transform
g(x) = sqrt(depth**2 + offset**2)
3  Linear tau-p: g(x) = offset
4  abs linear tau-p: g(x) = abs(offset)
offref=2000.    reference maximum offset to which maximum and minimum
moveout times are associated
interoff=0.     intercept offset to which tau-p times are associated
pmin=-200       minimum moveout in ms on reference offset
pmax=400        maximum moveout in ms on reference offset
anderson=1	  p values in ms, =0 pvalues in horizontal slowness
dp=16           moveout increment in ms on reference offset
pmula=80        moveout in ms on reference offset where multiples begin
at maximum time
pmulb=200       moveout in ms on reference offset where multiples begin
at zero time
depthref=500.   Reference depth for Foster/Mosher hyperbolic transform
nwin=1          number of windows to use through the mute zone
f1=60.          High-end frequency before taper off
f2=80.          High-end frequency
prewhite=0.1    Prewhitening factor in percent.
cdpkey=cdp      name of header word for defining ensemble
offkey=offset   name of header word with spatial information
nxmax=240       maximum number of input traces per ensemble
ltaper=7	  taper (integer) for mute tapering function
Optimizing Parameters:
The following parameters are occasionally used to avoid spatial aliasing
problems on the linear tau-p transform.  Not recommended for other
transforms...
ninterp=0      number of traces to interpolate between each input trace
prior to computing transform
freq1=4.0      low-end frequency in Hz for picking (good default: 3 Hz)
(Known bug: freq1 cannot be zero)
freq2=20.0     high-end frequency in Hz for picking (good default: 20 Hz)
lagc=400       length of AGC operator for picking (good default: 400 ms)
lent=5         length of time smoother in samples for picker
(good default: 5 samples)
lenx=7         length of space smoother in samples for picker
(good default: 1 sample)
xopt=1         1 = use differences for spatial derivative
(works with irregular spacing)
0 = use FFT derivative for spatial derivatives
(more accurate but requires regular spacing and
at least 16 input tracs--will switch to differences
automatically if have less than 16 input traces)

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
