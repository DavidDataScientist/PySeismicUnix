# suchart

SUCHART - prepare data for x vs. y plot suchart <stdin >stdout key1=sx key2=gx

## Synopsis

```bash
suchart <stdin >stdout key1=sx key2=gx
```

## Required Parameters

none

## Optional Parameters

key1=sx  	abscissa
key2=gx		ordinate
outpar=null	name of parameter file
The output is the (x, y) pairs of binary floats

## Examples

```
suchart < sudata outpar=pfile >plot_data
psgraph <plot_data par=pfile title="CMG" \
linewidth=0 marksize=2 mark=8 | ...
rm plot_data
suchart < sudata | psgraph n=1024 d1=.004 \
linewidth=0 marksize=2 mark=8 | ...
fold chart:
suchart < stacked_data key1=cdp key2=nhs |
psgraph n=NUMBER_OF_TRACES d1=.004 \
linewidth=0 marksize=2 mark=8 > chart.ps
```

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
