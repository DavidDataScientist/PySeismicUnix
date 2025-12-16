"""
SU Flow Plugin Factory

Provides factory pattern for plugin registration and instantiation.
Plugins are registered by name and can be created dynamically.
"""

from typing import Dict, Type, Optional
from .base import SuFlowPlugin


class SuPluginFactory:
    """
    Factory for creating SU flow plugins.
    
    Plugins are registered by name and can be instantiated with arguments.
    """
    
    _registry: Dict[str, Type[SuFlowPlugin]] = {}
    
    @classmethod
    def register(cls, name: str, plugin_class: Type[SuFlowPlugin]) -> None:
        """
        Register a plugin class with a name.
        
        Args:
            name: Plugin name (used in command line)
            plugin_class: Plugin class (subclass of SuFlowPlugin)
        """
        if not issubclass(plugin_class, SuFlowPlugin):
            raise TypeError(f"Plugin class must be a subclass of SuFlowPlugin")
        cls._registry[name] = plugin_class
    
    @classmethod
    def create(cls, name: str, args: Optional[list[str]] = None) -> Optional[SuFlowPlugin]:
        """
        Create a plugin instance by name.
        
        Args:
            name: Registered plugin name
            args: Arguments to pass to plugin constructor
            
        Returns:
            Plugin instance, or None if not found
        """
        plugin_class = cls._registry.get(name)
        if plugin_class is None:
            return None
        return plugin_class(args=args)
    
    @classmethod
    def list_plugins(cls) -> list[str]:
        """Return list of registered plugin names."""
        return list(cls._registry.keys())
    
    @classmethod
    def is_registered(cls, name: str) -> bool:
        """Check if a plugin name is registered."""
        return name in cls._registry


# Convenience functions
def register_plugin(name: str, plugin_class: Type[SuFlowPlugin]) -> None:
    """Register a plugin class."""
    SuPluginFactory.register(name, plugin_class)


def create_plugin(name: str, args: Optional[list[str]] = None) -> Optional[SuFlowPlugin]:
    """Create a plugin instance."""
    return SuPluginFactory.create(name, args)
