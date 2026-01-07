# sucmp

SUCMP   - CoMPare two seismic data sets, returns 0 to the shell if the same and 1 if different sucmp file_A file_B

## Synopsis

```bash
sucmp suxwigb <tst1.su &
```

## Required Parameters

none

## Optional Parameters

limit=1.0e-4    normalized difference threshold value

## Notes

This program is the seismic equivalent of the Unix cmp(1)
command.  However, unlike cmp(1), it understands seismic data
and will consider files which have only small numerical
differences to be the same.
Sucmp first checks that the number of traces and number of samples
are the same. It then compares the trace headers bit for bit.
Finally it checks that the fractional difference of A & B is
less than limit.
This program is intended as an aid in regression testing changes to
seismic processing programs.
Expected usage is in shell scripts or Makefiles, e.g.
#!/bin/sh
#-------------------------------------------------------
# Run a test data set and verify the result is correct
# If the data doesn't match show the data on the screen.
#-------------------------------------------------------
./fubar par=tst1.par
sucmp tst1.su ref/tst1.su
if [ $? ]
then
suxwigb <tst1.su &
suxwigb <ref/tst1.su &    fi

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
