"""
SU Flow Plugin Base Classes

Defines the abstract interface for SU flow plugins that process
seismic data in pipeline workflows.
"""

import sys
import struct
from abc import ABC, abstractmethod
from typing import Optional, Dict, Any
from dataclasses import dataclass


# SU format constants
HDRBYTES = 240  # Bytes in trace header
FLOAT_SIZE = 4  # Bytes per float sample


@dataclass
class SuTrace:
    """Represents a single SU trace with header and data."""
    header: bytes  # 240-byte header
    data: list[float]  # Trace samples as floats
    
    @property
    def ns(self) -> int:
        """Number of samples in trace."""
        return len(self.data)
    
    def get_header_int(self, offset: int) -> int:
        """Extract 32-bit signed integer from header at offset."""
        return struct.unpack('>i', self.header[offset:offset+4])[0]
    
    def set_header_int(self, offset: int, value: int) -> None:
        """Set 32-bit signed integer in header at offset."""
        header_list = bytearray(self.header)
        struct.pack_into('>i', header_list, offset, value)
        self.header = bytes(header_list)
    
    def get_header_float(self, offset: int) -> float:
        """Extract 32-bit float from header at offset."""
        return struct.unpack('>f', self.header[offset:offset+4])[0]
    
    def set_header_float(self, offset: int, value: float) -> None:
        """Set 32-bit float in header at offset."""
        header_list = bytearray(self.header)
        struct.pack_into('>f', header_list, offset, value)
        self.header = bytes(header_list)
    
    def to_bytes(self) -> bytes:
        """Convert trace to binary SU format."""
        header_bytes = self.header
        data_bytes = struct.pack('>%df' % len(self.data), *self.data)
        return header_bytes + data_bytes


class SuFlowPlugin(ABC):
    """
    Abstract base class for SU flow plugins.
    
    Plugins process SU traces in a pipeline. They read traces from stdin
    and write processed traces to stdout.
    
    Subclasses must implement:
    - process_trace(): Process a single trace
    - Optional: initialize(): Setup before processing
    - Optional: finalize(): Cleanup after processing
    """
    
    def __init__(self, args: Optional[list[str]] = None):
        """
        Initialize plugin.
        
        Args:
            args: Command-line arguments (parsed from sys.argv if None)
        """
        self.args = args if args is not None else sys.argv[1:]
        self._initialized = False
    
    def initialize(self) -> None:
        """
        Called once before processing any traces.
        Override to perform setup operations.
        """
        pass
    
    def finalize(self) -> None:
        """
        Called once after all traces are processed.
        Override to perform cleanup operations.
        """
        pass
    
    @abstractmethod
    def process_trace(self, trace: SuTrace) -> Optional[SuTrace]:
        """
        Process a single trace.
        
        Args:
            trace: Input trace to process
            
        Returns:
            Processed trace, or None to skip this trace
        """
        pass
    
    def run(self) -> int:
        """
        Main processing loop. Reads traces from stdin, processes them,
        and writes to stdout.
        
        Returns:
            Exit code (0 for success, non-zero for error)
        """
        try:
            self.initialize()
            self._initialized = True
            
            while True:
                # Read trace header (240 bytes)
                header = sys.stdin.buffer.read(HDRBYTES)
                if len(header) == 0:
                    break  # EOF
                if len(header) < HDRBYTES:
                    sys.stderr.write(f"Error: Incomplete header (got {len(header)} bytes, expected {HDRBYTES})\n")
                    return 1
                
                # Extract number of samples from header (offset 72, 4 bytes)
                ns = struct.unpack('>i', header[72:76])[0]
                if ns <= 0 or ns > 1000000:  # Sanity check
                    sys.stderr.write(f"Error: Invalid ns value: {ns}\n")
                    return 1
                
                # Read trace data (ns floats, 4 bytes each)
                data_bytes = sys.stdin.buffer.read(ns * FLOAT_SIZE)
                if len(data_bytes) < ns * FLOAT_SIZE:
                    sys.stderr.write(f"Error: Incomplete trace data (got {len(data_bytes)} bytes, expected {ns * FLOAT_SIZE})\n")
                    return 1
                
                # Unpack trace data
                data = list(struct.unpack('>%df' % ns, data_bytes))
                
                # Create trace object
                trace = SuTrace(header=header, data=data)
                
                # Process trace
                result = self.process_trace(trace)
                
                # Write result if not skipped
                if result is not None:
                    output = result.to_bytes()
                    sys.stdout.buffer.write(output)
                    sys.stdout.buffer.flush()
            
            self.finalize()
            return 0
            
        except KeyboardInterrupt:
            sys.stderr.write("\nInterrupted by user\n")
            return 130
        except Exception as e:
            sys.stderr.write(f"Error: {e}\n")
            import traceback
            traceback.print_exc()
            return 1
