#!/usr/bin/env python3
"""
Module documentation.
"""

# Imports
import sys
# Import os

# Global variables

# Class declarations

# Function declarations

def main():
    args = sys.argv[1:]

    if not args:
        print('usage: [--flags options] [inputs] ')
        sys.exit(1)

# Main body
if __name__ == '__main__':
    try:
        main()
    except (SystemExit, KeyboardInterrupt, GeneratorExit, Exception) as err:
        # Disabled for production, maintained for debugging
        print("Error: ", err)
        print("Error.__cause__", err.__cause__)
        print("Error.__class__", err.__class__.__name__)
        print("Error.with_traceback", err.with_traceback)
        # pass exists only for when debug off, so last line ends with a \n
        pass


