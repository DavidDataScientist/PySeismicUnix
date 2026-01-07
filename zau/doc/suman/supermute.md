# supermute

SUPERMUTE - permute or transpose a 3d datacube supermute <stdin >sdout

## Synopsis

```bash
supermute <stdin >sdout
```

## Required Parameters

none

## Optional Parameters

n1=ns from header		number of samples in the fast direction
n2=ntr from header		number of samples in the med direction
n3=1				number of samples in the slow direction
o1=1				new fast direction
o2=2				new med direction
o3=3				new slow direction
d1=1				output interval in new fast direction
d2=1				output interval in new med direction
d3=1				output interval in new slow direction

## Notes

header fields d1 and d2 default to d1=1.0 and d2=1.0

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
