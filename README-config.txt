Emerald Dreamway configuration v2
=================================

Replace:
  src\EmeraldDreamway.cpp
  src\EmeraldDreamway_loader.cpp

Add:
  conf\mod_emerald_dreamway.conf.dist

Because the conf file is new, rerun CMake Configure and Generate, then build worldserver.

After building, locate the copied module config beneath the worldserver configuration
directory, normally:

  Build\bin\RelWithDebInfo\configs\modules\mod_emerald_dreamway.conf.dist

Copy it to:

  mod_emerald_dreamway.conf

Edit the non-.dist copy. The .dist file documents defaults and should remain unchanged.

The C++ code reads configuration values when a pedestal is clicked, so changes become
effective after AzerothCore reloads its configuration. Use:

  .reload config

or restart worldserver.
