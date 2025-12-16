# succfilt

SUCCFILT -  FX domain Correlation Coefficient FILTER

## Synopsis

```bash
succfilt sucff < stdin > stdout [optional parameters]
```

## Optional Parameters

cch=1.0		Correlation coefficient high pass value
ccl=0.3		Correlation coefficient low pass value
key=ep		ensemble identifier
padd=25		FFT padding in percentage

## Examples

```
susort ep offset < data.su > datasorted.su
suputgthr dir=Data verbose=1 < datasorted.su
sugetgthr dir=Data verbose=1 > dataupdated.su
succfilt  < dataupdated.su > ccfiltdata.su
(Work in progress, editing required)
```

## Notes

This program uses "get_gather" and "put_gather" so requires that
the  data be sorted into ensembles designated by "key", with the ntr
field set to the number of traces in each respective ensemble.

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
