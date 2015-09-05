# SDL-game-framework
A game framework using SDL2, CEGUI, and OpenGL.

Compile with Visual Studio Community 2015 (VC++14).


Dependencies:
  - Boost 1.59 (http://www.boost.org/)
  - SDL 2.03 (https://www.libsdl.org)
  - yaml-cpp 0.5.2 (https://github.com/jbeder/yaml-cpp)
  - CEGUI 0.8.4 (http://cegui.org.uk/)
  - GLEW 1.13.0 (http://glew.sourceforge.net/)


Pitfalls:
  - yaml-cpp: If you extract a number with 1-byte-length type (int8_t, int_fast8_t, ...),
    it will convert the number into character, so '3' in yml file will be extracted as 51,
    which is the value of character '3'.
  - CEGUI: Be sure to use debug version of .lib files in the Debug environment, otherwise you will
    get several weird bugs such as "Can't create CEGUI::String with std::string as parameter",
    "Function injectMousePosition throw 'memory access violation' exception", and so on.
