# sushw

SUSHW - Set one or more Header Words using trace number, mod and integer divide to compute the header word values or input the header word values from a file

## Synopsis

```bash
sushw <stdin >stdout key=cdp,.. a=0,..  b=0,.. c=0,.. d=0,.. j=..,..
```

## Optional Parameters

key=cdp,...			header key word(s) to set
a=0,...			value(s) on first trace
b=0,...			increment(s) within group
c=0,...			group increment(s)
d=0,...			trace number shift(s)
j=ULONG_MAX,ULONG_MAX,...	number of elements in group

## Examples

```
sushw < sudata=key1,key2 ... infile=binary_file [Optional Parameters]
```

## Notes

Fields that are getparred must have the same number of entries as key
words being set. Any field that is not getparred is set to the default
value(s) above. Explicitly setting j=0 will set j to ULONG_MAX.
The value of each header word key is computed using the formula:
i = itr + d
val(key) = a + b * (i % j) + c * (int(i / j))
where itr is the trace number (first trace has itr=0, NOT 1)

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
