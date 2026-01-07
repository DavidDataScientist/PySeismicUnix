# sutrcount

SUTRCOUNT - SU program to count the TRaces in infile sutrcount < infile

## Synopsis

```bash
sutrcount < infile
```

## Required Parameters

none
Optional parameter:
outpar=stdout

## Notes

Once you have the value of ntr, you may set the ntr header field
via:
sushw key=ntr a=NTR < datain.su  > dataout.su
Where NTR is the value of the count obtained with sutrcount

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
