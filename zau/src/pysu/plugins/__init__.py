"""
SU Flow Python Plugins - Factory and Registration System

This module provides:
- Abstract base class for SU flow plugins
- Factory pattern for plugin registration and instantiation
- Plugin discovery and loading mechanism
"""

from .base import SuFlowPlugin, SuTrace
from .factory import SuPluginFactory, register_plugin, create_plugin

__all__ = [
    'SuFlowPlugin',
    'SuTrace',
    'SuPluginFactory',
    'register_plugin',
    'create_plugin',
]
