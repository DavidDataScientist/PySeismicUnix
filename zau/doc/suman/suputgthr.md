# suputgthr

key parameter.

## Synopsis

```bash
suputgthr SUPUTGTHR - split the stdout flow to gathers on the bases of given
```

## Required Parameters

dir=		Name of directory where to put the gathers

## Optional Parameters

key=ep		header key word to watch
suffix=".hsu"	extension of the output files
verbose=0		verbose = 1 echos information
numlength=7		Length of numeric part of filename

## Notes

The name of the file is constructed from the key parameter. Traces
are put into a temporary disk file, and renamed when key parameter
changes in the input flow to "key.suffix". The result is that the
directory "dir" contains separate files by "key" ensemble.
Header field modified:  ntr  to be the number of traces in a given
ensemble.
Related programs: sugetgthr, susplit

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
