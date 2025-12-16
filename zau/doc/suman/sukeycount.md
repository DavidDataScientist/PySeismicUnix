# sukeycount

SUKEYCOUNT - sukeycount writes a count of a selected key sukeycount key=keyword < infile [> outfile]

## Synopsis

```bash
sukeycount key=keyword < infile [> outfile]
```

## Required Parameters

key=keyword      One key word.

## Optional Parameters

verbose=0  quiet
=1  chatty
Writes the key and the count to the terminal or a text
file when a change of key occurs. This does not provide
a unique key count (see SUCOUNTKEY for that).
Note that for key values  1 2 3 4 2 5
value 2 is counted once per occurrence since this program
only recognizes a change of key, not total occurrence.

## Examples

```
sukeycount < stdin key=fldr
sukeycount < stdin key=fldr > out.txt
```

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
