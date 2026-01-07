# supofilt

SUPOFILT - POlarization FILTer for three-component data

## Synopsis

```bash
supofilt <stdin >stdout [optional parameters]
```

## Required Parameters

dfile=polar.dir   file containing the 3 components of the
direction of polarization
wfile=polar.rl    file name of weighting polarization parameter

## Optional Parameters

dt=(from header)  time sampling intervall in seconds
smooth=1          1 = smooth filter operators, 0 do not
sl=0.05           smoothing window length in seconds
wpow=1.0          raise weighting function to power wpow
dpow=1.0          raise directivity functions to power dpow
verbose=0         1 = echo additional information

## Notes

Three adjacent traces are considered as one three-component
dataset.
This program SUPOFILT is an extension to the polarization analysis
program supolar. The files wfile and dfile are SU files as written
by SUPOLAR.

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
