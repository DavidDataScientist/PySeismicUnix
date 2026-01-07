# sulfaf

SULFAF -  Low frequency array forming

## Synopsis

```bash
sulfaf < stdin > stdout [optional parameters]
```

## Optional Parameters

key=ep	header keyword describing the gathers
f1=3		lower frequency	cutof
f2=20		high frequency cutof
fr=5		frequency ramp
vel=1000	surface wave velocity
dx=10		trace spacing
maxmix=tr.ntr	default is the entire gather
adb=1.0	add back ratio 1.0=pure filtered 0.0=origibal
tri=0		1 Triangular weight in mixing window

## Examples

```
susort ep offset < data.su > datasorted.su
suputgthr dir=Data verbose=1 < datasorted.su
sugetgthr dir=Data verbose=1 > dataupdated.su
sulfaf  < dataupdated.su > ccfiltdata.su
(Work in progress, editing required)
```

## Notes

The traces transformed into the freqiency domain
where a trace mix is performed in the specifed frequency range
as Mix=vel/(freq*dx)
This program uses "get_gather" and "put_gather" so requires that
the  data be sorted into ensembles designated by "key", with the ntr
field set to the number of traces in each respective ensemble.

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
