# su3dchart

SU3DCHART - plot x-midpoints vs. y-midpoints for 3-D data su3dchart <stdin >stdout

## Synopsis

```bash
su3dchart <stdin >stdout
```

## Optional Parameters

outpar=null	name of parameter file
degree=0	=1 convert seconds of arc to degrees
The output is the (x, y) pairs of binary floats

## Examples

```
su3dchart <segy_data outpar=pfile >plot_data
psgraph <plot_data par=pfile \
linewidth=0 marksize=2 mark=8 | ...
rm plot_data
su3dchart <segy_data | psgraph n=1024 d1=.004 \
linewidth=0 marksize=2 mark=8 | ...
```

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
