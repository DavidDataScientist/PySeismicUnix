"""
pysuflipsign - Flip the sign of SU trace data

This plugin flips the sign of all samples in each trace,
effectively multiplying the data by -1.

Usage:
    python -m zau.src.pysu.plugins pysuflipsign < input.su > output.su
    Or in SU flow:
    sugain | python -m zau.src.pysu.plugins pysuflipsign | surange
"""

from .base import SuFlowPlugin, SuTrace


class PySuFlipSign(SuFlowPlugin):
    """
    Plugin that flips the sign of all trace samples.
    
    This is a simple test plugin that demonstrates the plugin system.
    """
    
    def process_trace(self, trace: SuTrace) -> SuTrace:
        """
        Flip the sign of all samples in the trace.
        
        Args:
            trace: Input trace
            
        Returns:
            Trace with flipped signs
        """
        # Flip sign of all data samples
        trace.data = [-x for x in trace.data]
        
        return trace


# Register this plugin
from .factory import register_plugin
register_plugin('pysuflipsign', PySuFlipSign)
