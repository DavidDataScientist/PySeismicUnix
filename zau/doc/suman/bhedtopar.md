# bhedtopar

BHEDTOPAR - convert a Binary tape HEaDer file to PAR file format bhedtopar < stdin outpar=parfile

## Synopsis

```bash
bhedtopar < stdin outpar=parfile
```

## Optional Parameters

swap=0 			=1 to swap bytes
outpar=/dev/tty		=parfile  name of output param file

## Notes

This program dumps the contents of a SEGY binary tape header file, as
would be produced by segyread and segyhdrs to a file in "parfile" format.
A "parfile" is an ASCII file containing entries of the form param=value.
Here "param" is the keyword for the binary tape header field and
"value" is the value of that field. The parfile may be edited as
any ASCII file. The edited parfile may then be made into a new binary tape
header file via the program    setbhed.

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
