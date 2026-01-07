# sugprfb

SUGPRFB - SU program to remove First Breaks from GPR data sugprfb < radar traces >outfile nx=51		number of traces to sum to create pilot trace (odd)

## Synopsis

```bash
sugprfb < radar traces >outfile
```

## Notes

The first fbt samples from nx traces are stacked to form a pilot
first break trace, this is fitted to the actual traces by shifting
and scaling.		 The nx traces long spatial window is
slided along the section and a new pilot traces is formed for each
position. The scalers in percent and the time shifts are stored in
header words trwf and grnors.

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
