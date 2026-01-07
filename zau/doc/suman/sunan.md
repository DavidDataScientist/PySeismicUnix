# sunan

SUNAN - remove NaNs & Infs from the input stream sunan < in.su >out.su

## Synopsis

```bash
sunan < in.su >out.su
```

## Optional Parameters

verbose=1	echo locations of NaNs or Infs to stderr
=0 silent
...user defined ...
value=0.0	NaNs and Inf replacement value
... and/or....
interp=0	=1 replace NaNs and Infs by interpolating
neighboring finite values

## Notes

A simple program to remove NaNs and Infs from an input stream.
The program sets NaNs and Infs to "value" if interp=0. When
interp=1 NaNs are replaced with the average of neighboring values
provided that the neighboring values are finite, otherwise
NaNs and Infs are replaced by "value".

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
