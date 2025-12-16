# sudivstack

SUDIVSTACK -  Diversity Stacking using either average power or peak power within windows

## Synopsis

```bash
sudivstack susort < field.data tracf |  > stack.data
```

## Required Parameters

none

## Optional Parameters

key=tracf        key header word to stack on
winlen=0.064     window length in seconds.
typical choices: 0.064, 0.128, 0.256,
0.512, 1.024, 2.048, 4.096
if not specified the entire trace is used
peak=1           peak power option default is average power

## Examples

```
For duplicate field shot records:
susort < field.data tracf | sudivstack > stack.data
For CDP ordered data:
sudivstack < cdp.data key=cdp > stack.data
```

## Notes

Diversity stacking is a noise reduction technique used in the
summation of duplicate data. Each trace is scaled by the inverse
of its average power prior to stacking.  The composite trace is
then renormalized by dividing by the sum of the scalers used.
This program stacks adjacent traces having the same key header
word, which can be specified by the key parameter. The default
is "tracf" (trace number within field record).
For more information on key header words, type "sukeyword -o".

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
