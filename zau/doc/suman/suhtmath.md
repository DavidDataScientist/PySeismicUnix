# suhtmath

SUHTMATH - do unary arithmetic operation on segy traces with headers values suhtmath <stdin >stdout

## Synopsis

```bash
suhtmath <stdin >stdout
```

## Required Parameters

none
Optional parameter:
key=tracl	header word to use
op=nop		operation flag
nop   : no operation
add   : add header to trace
mult  : multiply trace with header
div   : divide trace by header
scale=1.0	scalar multiplier for header value
const=0.0	additive constant for header value
Operation order:
op=add:	out(t) = in(t) + (scale * key + const)
op=mult:	out(t) = in(t) * (scale * key + const)
op=div:	out(t) = in(t) / (scale * key + const)

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
