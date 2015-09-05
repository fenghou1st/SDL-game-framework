#pragma once

#include <cstdio>
#include <cassert>
#include <iostream>

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GL3Renderer.h>


namespace sdl { namespace cegui
{
	void initCEGUI(const std::string & data_path, const std::string & log_path, int width, int height);

	void initWindows();

	CEGUI::MouseButton SDLtoCEGUIMouseButton(const Uint8 & button);

	CEGUI::Key::Scan toCEGUIKey(SDL_Scancode key);
}}