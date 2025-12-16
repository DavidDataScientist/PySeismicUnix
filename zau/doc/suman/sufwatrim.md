# sufwatrim

SUFWATRIM - FX domain Alpha TRIM

## Synopsis

```bash
sufwatrim <stdin > stdout [optional parameters]
```

## Required Parameters

key=key1,key2,..	Header words defining mixing dimesnion
dx=d1,d2,..		Distance units for each header word

## Optional Parameters

keyg=ep		Header word indicating the start of gather
vf=0			=1 Do a frequency dependent mix
vmin=5000		Velocity of the reflection slope
than should not be attenuated

## Notes

Trace with the header word mark set to one will be
the output trace

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
