# suanalytic

SUANALYTIC - use the Hilbert transform to generate an ANALYTIC (complex) trace suanalytic <stdin >sdout

## Synopsis

```bash
suanalytic <stdin >sdout
```

## Notes

The output are complex valued traces. The analytic trace is defined as
ctr[ i ] = indata[i] + i hilb[indata[t]]
where the imaginary part is the hilbert tranform of the original trace
The Hilbert transform is computed in the direct (time) domain
If phaserot is set, then a phase rotated complex trace is produced
ctr[ i ] = cos[phaserot]*indata[i] + i sin[phaserot]* hilb[indata[t]]
Use "suamp" to extract real, imaginary, amplitude (modulus), etc
Exmple:
suanalytic < sudata | suamp mode=amp | suxgraph
Use "suattributes" for instantaneous phase, frequency, etc.

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
