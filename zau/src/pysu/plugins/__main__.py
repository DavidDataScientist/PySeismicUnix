"""
Command-line entry point for SU flow plugins.

This module allows plugins to be invoked from the command line:
    python -m zau.src.pysu.plugins <plugin_name> [args...]

Or as a standalone script:
    python -m zau.src.pysu.plugins pysuflipsign < input.su > output.su
"""

import sys
from .factory import SuPluginFactory, create_plugin

# Import plugins to register them
from . import pysuflipsign  # noqa: F401


def main() -> int:
    """Main entry point for plugin execution."""
    if len(sys.argv) < 2:
        sys.stderr.write("Usage: python -m zau.src.pysu.plugins <plugin_name> [args...]\n")
        sys.stderr.write("\nAvailable plugins:\n")
        for name in SuPluginFactory.list_plugins():
            sys.stderr.write(f"  {name}\n")
        return 1
    
    plugin_name = sys.argv[1]
    plugin_args = sys.argv[2:] if len(sys.argv) > 2 else []
    
    # Create and run plugin
    plugin = create_plugin(plugin_name, args=plugin_args)
    if plugin is None:
        sys.stderr.write(f"Error: Unknown plugin '{plugin_name}'\n")
        sys.stderr.write("\nAvailable plugins:\n")
        for name in SuPluginFactory.list_plugins():
            sys.stderr.write(f"  {name}\n")
        return 1
    
    # Run plugin (reads from stdin, writes to stdout)
    return plugin.run()


if __name__ == '__main__':
    sys.exit(main())
