# suascii

SUASCII - print non zero header values and data in various formats suascii <stdin >ascii_file

## Synopsis

```bash
suascii <stdin >ascii_file
```

## Notes

The programs suwind and suresamp provide trace selection and
subsampling, respectively.
With bare=0 and bare=1 traces are separated by a blank line.
With bare=3 a maximum of ntr traces are output in .csv format
("comma-separated value"), e.g. for import into spreadsheet
applications like Excel.
With bare=4 a maximum of ntr traces are output in as tab delimited
columns. Use bare=4 for plotting in GnuPlot.
With bare=5 traces are written as "x y z" triples as required
by certain plotting programs such as the Generic Mapping Tools
(GMT). If sep= is set, traces are separated by a line containing
the string provided, e.g. sep=">" for GMT multisegment files.
"option=" is an acceptable alias for "bare=".
Related programs: sugethw, sudumptrace

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
