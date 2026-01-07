# sudctcomp

DCTCOMP - Compression by Discrete Cosine Transform

## Synopsis

```bash
sudctcomp dctcomp < stdin n1= n2=   [optional parameter] > sdtout
```

## Required Parameters

n1=			number of samples in the fast (first) dimension
n2=			number of samples in the slow (second) dimension

## Optional Parameters

blocksize1=16		blocksize in direction 1
blocksize2=16		blocksize in direction 2
error=0.01		acceptable error

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
