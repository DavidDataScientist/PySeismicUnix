# sufnzero

SUFNZERO - get Time of First Non-ZERO sample by trace

## Synopsis

```bash
sufnzero <stdin >stdout [optional parameters]
```

## Required Parameters

none

## Optional Parameters

mode=first   	Output time of first non-zero sample
=last for last non-zero sample
=both for both first & last non-zero samples
min=0   	Threshold value for considering as zero
Any abs(sample)<min is considered to be zero
key=key1,...	Keyword(s) to print

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
