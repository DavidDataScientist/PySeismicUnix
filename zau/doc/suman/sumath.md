# sumath

SUMATH - do math operation on su data suop <stdin >stdout op=mult

## Synopsis

```bash
sumath suop <stdin >stdout op=mult
```

## Required Parameters

none
Optional parameter:
op=mult		operation flag
--------- operations -----------------
add   : o = i + a    (o=out; i=in)
sub   : o = i - a
mult  : o = i * a
div   : o = i / a
pow   : o = i ^ a
spow  : o = sgn(i) * abs(i) ^ a
--------- operation parameter --------
a=1
copy=1		n>1 copy each trace n times

## Notes

There is overlap between this program and "sugain" and
"suop"

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
