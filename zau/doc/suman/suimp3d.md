# suimp3d

SUIMP3D - generate inplane shot records for a point scatterer embedded in three dimensions using the Born integral equation

## Synopsis

```bash
suimp3d [optional parameters] >stdout
```

## Optional Parameters

nshot=1		number of shots
nrec=1		number of receivers
c=5000		speed
dt=.004		sampling rate
nt=256		number of samples
x0=1000		point scatterer location
y0=0		point scatterer location
z0=1000		point scatterer location
dir=0		do not include direct arrival
=1 include direct arrival
sxmin=0		first shot location
symin=0		first shot location
szmin=0		first shot location
gxmin=0		first receiver location
gymin=0		first receiver location
gzmin=0		first receiver location
dsx=100		x-step in shot location
dsy=0	 	y-step in shot location
dsz=0	 	z-step in shot location
dgx=100		x-step in receiver location
dgy=0		y-step in receiver location
dgz=0		z-step in receiver location

## Examples

```
suimp3d nrec=32 | sufilter | supswigp | ...
```

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
