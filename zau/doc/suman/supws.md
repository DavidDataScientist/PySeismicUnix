# supws

SUPWS - Phase stack or phase-weighted stack (PWS) of adjacent traces having the same key header word

## Synopsis

```bash
supws <stdin >stdout [optional parameters]
```

## Required Parameters

none

## Optional Parameters

key=cdp	   key header word to stack on
pwr=1.0	   raise phase stack to power pwr
dt=(from header)  time sampling intervall in seconds
sl=0.0		window length in seconds used for smoothing
of the phase stack (weights)
ps=0		0 = output is PWS, 1 = output is phase stack
verbose=0	 1 = echo additional information

## Notes

Phase weighted stacking is a tool for efficient incoherent noise
reduction. An amplitude-unbiased coherency measure is designed
based on the instantaneous phase, which is used to weight the
samples of an ordinary, linear stack. The result is called the
phase-weighted stack (PWS) and is cleaned from incoherent noise.
PWS thus permits detection of weak but coherent arrivals.
The phase-stack (coherency measure) has values between 0 and 1.
If the stacking is over cdp and the PWS option is set, then the
offset header field is set to zero. Otherwise, output traces get
their headers from the first trace of each data ensemble to stack,
including the offset field. Use "sushw" afterwards, if this is
not acceptable.

## See Also

- [su](su.md)
- [segyread](segyread.md)
- [segywrite](segywrite.md)

---
*Generated from CWP/SU Windows port*
