# susort

SUSORT - sort on any segy header keywords susort <stdin >stdout [[+-]key1 [+-]key2 ...] Susort supports any number of (secondary) keys with either

## Synopsis

```bash
susort <stdin >stdout [[+-]key1 [+-]key2 ...]
```

## Examples

```
To sort traces by cdp gather and within each gather
by offset with both sorts in ascending order:
susort <INDATA >OUTDATA cdp offset
Caveat: In the case of Pipe input a temporary file is made
to hold the ENTIRE data set.  This temporary is
either an actual disk file (usually in /tmp) or in some
implementations, a memory buffer.  It is left to the
user to be SENSIBLE about how big a file to pipe into
susort relative to the user's computer.
```

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
