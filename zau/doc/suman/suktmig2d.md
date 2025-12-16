# suktmig2d

SUKTMIG2D - prestack time migration of a common-offset section with the double-square root (DSR) operator suktmig2d < infile vfile= [parameters]  > outfile

## Synopsis

```bash
suktmig2d < infile vfile= [parameters]  > outfile
```

## Required Parameters

vfile=	rms velocity file (units/s) v(t,x) as a function
of time
dx=		distance (units) between consecutive traces

## Optional Parameters

fcdpdata=tr.cdp	first cdp in data
firstcdp=fcdpdata	first cdp number in velocity file
lastcdp=from header	last cdp number in velocity file
dcdp=from header	number of cdps between consecutive traces
angmax=40	maximum aperture angle for migration (degrees)
hoffset=.5*tr.offset		half offset (m)
nfc=16	number of Fourier-coefficients to approximate
low-pass
filters. The larger nfc the narrower the filter
fwidth=5 	high-end frequency increment for the low-pass
filters
in Hz. The lower this number the more the number
of lowpass filters to be calculated for each
input trace.
Caveat: this code may need some work

## Notes

Data must be preprocessed with sufrac to correct for the
wave-shaping factor using phasefac=.25 for 2D migration.
Input traces must be sorted into offset and cdp number.
The velocity file consists of rms velocities for all CMPs as a
function of vertical time and horizontal position v(t,x)
in C-style binary floating point numbers.  It's easiest to
supply v(t,x) that has the same dimensions as the input data to
be migrated. Note that time t is the fast dimension in these
the input velocity file.
The units may be feet or meters, as long as these are
consistent.
Antialias filter is performed using (Gray,1992, Geoph. Prosp),
using nc low- pass filtered copies of the data. The cutoff
frequencies are calculated  as fractions of the Nyquist
frequency.
The maximum allowed angle is 80 degrees(a 10 degree taper is
applied to the end of the aperture)

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
