# Package

version       = "0.1.0"
author        = "Gustavo Freitas"
description   = "test bench"
license       = "MIT"

bin           = @["example"]
skipExt = @["nim"]

# Dependencies

requires "nim >= 0.20.0"
requires "jester >= 0.4.3"
#requires "xander >= 0.6.0"
#requires "regex"