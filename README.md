# SDL-game-framework
A game framework using SDL2.

Compile with Visual Studio Community 2015 (VC++14).


Dependencies:
  - Boost 1.59 (http://www.boost.org/)
  - SDL 2.03 (https://www.libsdl.org)
  - yaml-cpp 0.5.2 (https://github.com/jbeder/yaml-cpp)
  - guisan 0.9.0 (https://github.com/kallisti5/guisan)


Pitfalls:
  - yaml-cpp: If you extract a number with 1-byte-length type (int8_t, int_fast8_t, ...), it will convert the number into character, so '3' in yml file will be extracted as 51, which is the value of character '3'.
  - boost.serialization: Be sure using ios_base::binary flag when opening file streams, otherwise you will encounter a very obscure bug, that sometimes your save file will be loaded correctly, but sometimes it will be partially loaded, and you will get an input stream error.
