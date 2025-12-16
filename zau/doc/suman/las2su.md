# las2su

LAS2SU - convert las2 format well log curves to su traces

## Synopsis

```bash
las2su <stdin nskip= ncurve= >stdout [optional params]
```

## Required Parameters

none

## Optional Parameters

ncurve=automatic	number of log curves in the las file
dz=0.5		input depth sampling (ft)
m=0			output (d1,f1) in feet
=1 output (d1,f1) in meters
ss=0			do not subsample (unless nz > 32767 )
=1 pass every other sample
verbose=0		=1 to echo las header lines to screen
outhdr=las_header.asc	name of file for las headers

## Notes

1. It is recommended to run LAS_CERTIFY available from CWLS
at http://cwls.org.
2. First log curve MUST BE depth.
3. If number of depth levels > 32767 (segy NT limit)
then input is subsampled by factor of 2 or 4 as needed
4. Logs may be isolated using tracl header word (1,2,...,ncurve)
tracl=1 is depth
If the input LAS file contains sonic data as delta t or interval
transit time and you plan to use these data for generating a
reflection coefficient time series in suwellrf, convert the sonic
trace to velocity using suop with op=s2v (sonic to velocity)
before input of the velocity trace to suwellrf.
Caveat:
No trap is made for the commonly used null value in LAS files
(-999.25). The null value will be output as ?999.25, which
really messes up a suxwigb display of density data because the
?999.25 skews the more or less 2.5 values of density.
The user needs to edit out null values (-999.25) before running
other programs, such as "suwellrf".

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
