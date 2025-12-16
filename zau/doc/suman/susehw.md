# susehw

SUSEHW - Set the value the Header Word denoting trace number within an Ensemble defined by the value of another header word susehw <stdin >stdout [options]

## Synopsis

```bash
susehw <stdin >stdout [options]
```

## Required Parameters

none

## Optional Parameters

key1=cdp	Key header word defining the ensemble
key2=cdpt	Key header word defining the count within the ensemble
a=1		starting value of the count in the ensemble
b=1		increment or decrement within the ensemble

## Examples

```
susehw < cdpgathers.su a=1 b=1 key1=cdp key2=cdpt > new.su
```

## Notes

This code was written because suresstat requires cdpt to be set.
The computation is
val(key2) = a + b*i
The input data must first be sorted into constant key1 gathers.

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
