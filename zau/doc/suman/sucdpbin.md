# sucdpbin

SUCDPBIN - Compute CDP bin number sucdpbin <stdin >stdout xline= yline= dcdp=

## Synopsis

```bash
sucdpbin <stdin >stdout xline= yline= dcdp=
```

## Required Parameters

xline=		array of X defining the CDP line
yline=		array of Y defining the CDP line
dcdp=			distance between bin centers

## Optional Parameters

verbose=0		<>0 output informations
cdpmin=1001		min cdp bin number
distmax=dcdp		search radius
xline,yline defines the CDP line made of continuous straight lines.
If a smoother line is required, use unisam to interpolate.
Bin centers are located at dcdp constant interval on this line.
Each trace will be numbered with the number of the closest bin. If no
bin center is found within the search radius. cdp is set to 0

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
