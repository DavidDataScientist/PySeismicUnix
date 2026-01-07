# sugausstaper

SUGAUSSTAPER - Multiply traces with gaussian taper

## Synopsis

```bash
sugausstaper < stdin > stdout [optional parameters]
```

## Required Parameters

<none>

## Optional Parameters

key=offset    keyword of header field to weight traces by
x0=300        key value defining the center of gaussian window
xw=50         width of gaussian window in units of key value

## Notes

Traces are multiplied with a symmetrical gaussian taper
w(t)=exp(-((key-x0)/xw)**2)
unlike "sutaper" the value of x0 defines center of the taper
rather than the edges of the data.

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
