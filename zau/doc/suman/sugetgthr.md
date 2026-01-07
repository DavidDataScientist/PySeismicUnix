# sugetgthr

SUGETGTHR - Gets su files from a directory and put them throught the unix pipe. This creates continous data flow. sugetgthr  <stdin >sdout

## Synopsis

```bash
sugetgthr <stdin >sdout
```

## Required Parameters

dir=            Name of directory to fetch data from
Every file in the directory is treated as an su file

## Optional Parameters

verbose=0		=1 more chatty
vt=0			=1 allows gathers with variable length traces
no header checking is done!
ns=			must be specified if vt=1; number of samples to read

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
