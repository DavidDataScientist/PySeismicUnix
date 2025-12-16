# suimpedance

SUIMPEDANCE - Convert reflection coefficients to impedances.

## Synopsis

```bash
suimpedance <stdin >stdout [optional parameters]
```

## Optional Parameters

v0=1500.	Velocity at first sample (m/sec)
rho0=1.0e6	Density at first sample  (g/m^3)

## Notes

Implements recursion [1-R(k)]Z(k) = [1+R(k)]Z(k-1).
The input traces are assumed to be reflectivities, and thus are
expected to have amplitude values between -1.0 and 1.0.

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
