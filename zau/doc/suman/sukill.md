# sukill

SUKILL - zero out traces

## Synopsis

```bash
sukill <stdin >stdout [optional parameters]
```

## Optional Parameters

key=trid	header name to select traces to kill
a=2		header value identifying tracces to kill
or
min= 		first trace to kill (one-based)
count=1		number of traces to kill

## Notes

If min= is set it overrides selecting traces by header.

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
