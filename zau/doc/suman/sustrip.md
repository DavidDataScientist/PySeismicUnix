# sustrip

SUSTRIP - remove the SEGY headers from the traces sustrip <stdin >stdout head=/dev/null outpar=/dev/tty ftn=0

## Synopsis

```bash
sustrip <stdin >stdout head=/dev/null outpar=/dev/tty ftn=0
```

## Required Parameters

none

## Optional Parameters

head=/dev/null		file to save headers in
outpar=/dev/tty		output parameter file, contains:
number of samples (n1=)
number of traces (n2=)
sample rate in seconds (d1=)
ftn=0			Fortran flag
0 = write unformatted for C
1 = ... for Fortran

## Notes

Invoking head=filename will write the trace headers into filename.
You may paste the headers back onto the traces with supaste
See:  sudoc  supaste 	 for more information
Related programs: supaste, suaddhead

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
