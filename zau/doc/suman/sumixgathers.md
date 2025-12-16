# sumixgathers

SUMIXGATHERS - mix two gathers

## Synopsis

```bash
sumixgathers file1 file2 > stdout [optional parameters]
```

## Required Parameters

ntr=tr.ntr	if ntr header field is not set, then ntr is mandatory

## Notes

Mixes two gathers keeping only the traces of the first file
if the offset is the same. The purpose is to substitute only
traces non existing in file1 by traces interpolated store in file2.
Example. If file1 is original data file and file 2 is obtained by
resampling with Radon transform, then the output contains original
traces with gaps filled

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
