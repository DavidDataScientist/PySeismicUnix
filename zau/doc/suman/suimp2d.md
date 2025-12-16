# suimp2d

SUIMP2D - generate shot records for a line scatterer embedded in three dimensions using the Born integral equation

## Synopsis

```bash
suimp2d [optional parameters] >stdout
```

## Optional Parameters

nshot=1		number of shots
nrec=1		number of receivers
c=5000		speed
dt=.004		sampling rate
nt=256		number of samples
x0=1000		point scatterer location
z0=1000		point scatterer location
sxmin=0		first shot location
szmin=0		first shot location
gxmin=0		first receiver location
gzmin=0		first receiver location
dsx=100		x-step in shot location
dsz=0	 	z-step in shot location
dgx=100		x-step in receiver location
dgz=0		z-step in receiver location

## Examples

```
suimp2d nrec=32 | sufilter | supswigp | ...
```

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
