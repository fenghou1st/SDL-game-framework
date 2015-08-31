
/**************************************************************************
* The following libs (and corresponding headers) are needed to compile and to link:
* CEGUIBase
* CEGUIOpenGLRenderer
* CEGUICoreWindowRendererSet
* default CEGUI xml parser (and dependencies)
* SDL2main
* SDL2
* OpengGL
* glm headers (as part of CEGUIBase)
***************************************************************************/

#include <iostream>

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GL3Renderer.h>

#include <SDL.h>
#include <SDL_opengl.h>

static SDL_Window* window;
static SDL_GLContext context;


uint8_t _key_map[SDL_NUM_SCANCODES];

void init_key_map()
{
	memset(_key_map, CEGUI::Key::Unknown, SDL_NUM_SCANCODES);
	_key_map[SDL_SCANCODE_A] = CEGUI::Key::A;
	_key_map[SDL_SCANCODE_B] = CEGUI::Key::B;
	_key_map[SDL_SCANCODE_C] = CEGUI::Key::C;
	_key_map[SDL_SCANCODE_D] = CEGUI::Key::D;
	_key_map[SDL_SCANCODE_E] = CEGUI::Key::E;
	_key_map[SDL_SCANCODE_F] = CEGUI::Key::F;
	_key_map[SDL_SCANCODE_G] = CEGUI::Key::G;
	_key_map[SDL_SCANCODE_H] = CEGUI::Key::H;
	_key_map[SDL_SCANCODE_I] = CEGUI::Key::I;
	_key_map[SDL_SCANCODE_J] = CEGUI::Key::J;
	_key_map[SDL_SCANCODE_K] = CEGUI::Key::K;
	_key_map[SDL_SCANCODE_L] = CEGUI::Key::L;
	_key_map[SDL_SCANCODE_M] = CEGUI::Key::M;
	_key_map[SDL_SCANCODE_N] = CEGUI::Key::N;
	_key_map[SDL_SCANCODE_O] = CEGUI::Key::O;
	_key_map[SDL_SCANCODE_P] = CEGUI::Key::P;
	_key_map[SDL_SCANCODE_Q] = CEGUI::Key::Q;
	_key_map[SDL_SCANCODE_R] = CEGUI::Key::R;
	_key_map[SDL_SCANCODE_S] = CEGUI::Key::S;
	_key_map[SDL_SCANCODE_T] = CEGUI::Key::T;
	_key_map[SDL_SCANCODE_U] = CEGUI::Key::U;
	_key_map[SDL_SCANCODE_V] = CEGUI::Key::V;
	_key_map[SDL_SCANCODE_W] = CEGUI::Key::W;
	_key_map[SDL_SCANCODE_X] = CEGUI::Key::X;
	_key_map[SDL_SCANCODE_Y] = CEGUI::Key::Y;
	_key_map[SDL_SCANCODE_Z] = CEGUI::Key::Z;
	_key_map[SDL_SCANCODE_1] = CEGUI::Key::One;
	_key_map[SDL_SCANCODE_2] = CEGUI::Key::Two;
	_key_map[SDL_SCANCODE_3] = CEGUI::Key::Three;
	_key_map[SDL_SCANCODE_4] = CEGUI::Key::Four;
	_key_map[SDL_SCANCODE_5] = CEGUI::Key::Five;
	_key_map[SDL_SCANCODE_6] = CEGUI::Key::Six;
	_key_map[SDL_SCANCODE_7] = CEGUI::Key::Seven;
	_key_map[SDL_SCANCODE_8] = CEGUI::Key::Eight;
	_key_map[SDL_SCANCODE_9] = CEGUI::Key::Nine;
	_key_map[SDL_SCANCODE_0] = CEGUI::Key::Zero;
	_key_map[SDL_SCANCODE_RETURN] = CEGUI::Key::Return;
	_key_map[SDL_SCANCODE_ESCAPE] = CEGUI::Key::Escape;
	_key_map[SDL_SCANCODE_BACKSPACE] = CEGUI::Key::Backspace;
	_key_map[SDL_SCANCODE_TAB] = CEGUI::Key::Tab;
	_key_map[SDL_SCANCODE_SPACE] = CEGUI::Key::Space;
	_key_map[SDL_SCANCODE_MINUS] = CEGUI::Key::Minus;
	_key_map[SDL_SCANCODE_EQUALS] = CEGUI::Key::Equals;
	_key_map[SDL_SCANCODE_LEFTBRACKET] = CEGUI::Key::LeftBracket;
	_key_map[SDL_SCANCODE_RIGHTBRACKET] = CEGUI::Key::RightBracket;
	_key_map[SDL_SCANCODE_BACKSLASH] = CEGUI::Key::Backslash;
	_key_map[SDL_SCANCODE_NONUSHASH] = CEGUI::Key::Backslash;
	_key_map[SDL_SCANCODE_SEMICOLON] = CEGUI::Key::Semicolon;
	_key_map[SDL_SCANCODE_APOSTROPHE] = CEGUI::Key::Apostrophe;
	_key_map[SDL_SCANCODE_GRAVE] = CEGUI::Key::Grave;
	_key_map[SDL_SCANCODE_COMMA] = CEGUI::Key::Comma;
	_key_map[SDL_SCANCODE_PERIOD] = CEGUI::Key::Period;
	_key_map[SDL_SCANCODE_SLASH] = CEGUI::Key::Slash;
	_key_map[SDL_SCANCODE_CAPSLOCK] = CEGUI::Key::Capital;
	_key_map[SDL_SCANCODE_F1] = CEGUI::Key::F1;
	_key_map[SDL_SCANCODE_F2] = CEGUI::Key::F2;
	_key_map[SDL_SCANCODE_F3] = CEGUI::Key::F3;
	_key_map[SDL_SCANCODE_F4] = CEGUI::Key::F4;
	_key_map[SDL_SCANCODE_F5] = CEGUI::Key::F5;
	_key_map[SDL_SCANCODE_F6] = CEGUI::Key::F6;
	_key_map[SDL_SCANCODE_F7] = CEGUI::Key::F7;
	_key_map[SDL_SCANCODE_F8] = CEGUI::Key::F8;
	_key_map[SDL_SCANCODE_F9] = CEGUI::Key::F9;
	_key_map[SDL_SCANCODE_F10] = CEGUI::Key::F10;
	_key_map[SDL_SCANCODE_F11] = CEGUI::Key::F11;
	_key_map[SDL_SCANCODE_F12] = CEGUI::Key::F12;
	_key_map[SDL_SCANCODE_PRINTSCREEN] = CEGUI::Key::Unknown;
	_key_map[SDL_SCANCODE_SCROLLLOCK] = CEGUI::Key::ScrollLock;
	_key_map[SDL_SCANCODE_PAUSE] = CEGUI::Key::Pause;
	_key_map[SDL_SCANCODE_INSERT] = CEGUI::Key::Insert;
	_key_map[SDL_SCANCODE_HOME] = CEGUI::Key::Home;
	_key_map[SDL_SCANCODE_PAGEUP] = CEGUI::Key::PageUp;
	_key_map[SDL_SCANCODE_DELETE] = CEGUI::Key::Delete;
	_key_map[SDL_SCANCODE_END] = CEGUI::Key::End;
	_key_map[SDL_SCANCODE_PAGEDOWN] = CEGUI::Key::PageDown;
	_key_map[SDL_SCANCODE_RIGHT] = CEGUI::Key::ArrowRight;
	_key_map[SDL_SCANCODE_LEFT] = CEGUI::Key::ArrowLeft;
	_key_map[SDL_SCANCODE_DOWN] = CEGUI::Key::ArrowDown;
	_key_map[SDL_SCANCODE_UP] = CEGUI::Key::ArrowUp;
	_key_map[SDL_SCANCODE_NUMLOCKCLEAR] = CEGUI::Key::NumLock;
	_key_map[SDL_SCANCODE_KP_DIVIDE] = CEGUI::Key::Divide;
	_key_map[SDL_SCANCODE_KP_MULTIPLY] = CEGUI::Key::Multiply;
	_key_map[SDL_SCANCODE_KP_MINUS] = CEGUI::Key::Subtract;
	_key_map[SDL_SCANCODE_KP_PLUS] = CEGUI::Key::Add;
	_key_map[SDL_SCANCODE_KP_ENTER] = CEGUI::Key::NumpadEnter;
	_key_map[SDL_SCANCODE_KP_1] = CEGUI::Key::Numpad1;
	_key_map[SDL_SCANCODE_KP_2] = CEGUI::Key::Numpad2;
	_key_map[SDL_SCANCODE_KP_3] = CEGUI::Key::Numpad3;
	_key_map[SDL_SCANCODE_KP_4] = CEGUI::Key::Numpad4;
	_key_map[SDL_SCANCODE_KP_5] = CEGUI::Key::Numpad5;
	_key_map[SDL_SCANCODE_KP_6] = CEGUI::Key::Numpad6;
	_key_map[SDL_SCANCODE_KP_7] = CEGUI::Key::Numpad7;
	_key_map[SDL_SCANCODE_KP_8] = CEGUI::Key::Numpad8;
	_key_map[SDL_SCANCODE_KP_9] = CEGUI::Key::Numpad9;
	_key_map[SDL_SCANCODE_KP_0] = CEGUI::Key::Numpad0;
	_key_map[SDL_SCANCODE_KP_PERIOD] = CEGUI::Key::Decimal;
	_key_map[SDL_SCANCODE_APPLICATION] = CEGUI::Key::AppMenu;
	_key_map[SDL_SCANCODE_POWER] = CEGUI::Key::Power;
	_key_map[SDL_SCANCODE_KP_EQUALS] = CEGUI::Key::NumpadEquals;
	_key_map[SDL_SCANCODE_F13] = CEGUI::Key::F13;
	_key_map[SDL_SCANCODE_F14] = CEGUI::Key::F14;
	_key_map[SDL_SCANCODE_F15] = CEGUI::Key::F15;
	_key_map[SDL_SCANCODE_KP_COMMA] = CEGUI::Key::NumpadComma;
	_key_map[SDL_SCANCODE_LCTRL] = CEGUI::Key::LeftControl;
	_key_map[SDL_SCANCODE_LSHIFT] = CEGUI::Key::LeftShift;
	_key_map[SDL_SCANCODE_LALT] = CEGUI::Key::LeftAlt;
	_key_map[SDL_SCANCODE_LGUI] = CEGUI::Key::LeftWindows;
	_key_map[SDL_SCANCODE_RCTRL] = CEGUI::Key::RightControl;
	_key_map[SDL_SCANCODE_RSHIFT] = CEGUI::Key::RightShift;
	_key_map[SDL_SCANCODE_RALT] = CEGUI::Key::RightAlt;
	_key_map[SDL_SCANCODE_RGUI] = CEGUI::Key::RightWindows;
}



CEGUI::Key::Scan toCEGUIKey(SDL_Scancode key)
{
	return static_cast<CEGUI::Key::Scan>(_key_map[key]);
}


void initSDL()
{
	// init everything from SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cerr << "SDL could not be initialized!" << std::endl
			<< "Error message: " << SDL_GetError() << std::endl;
		exit(1);
	}

	// create opengl window with size of 800x600px
	window = SDL_CreateWindow("CEGUI + SDL2 window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (!window)
	{
		std::cerr << "Could not create SDL window: " << SDL_GetError() << std::endl;
		SDL_Quit();
		exit(1);
	}

	// disable native mouse cursor
	SDL_ShowCursor(0);

	// setup opengl rendering context
	context = SDL_GL_CreateContext(window);
}


void initCEGUI()
{
	using namespace CEGUI;

	// create renderer and enable extra states
	OpenGL3Renderer& cegui_renderer = OpenGL3Renderer::create(Sizef(800.f, 600.f));
	cegui_renderer.enableExtraStateSettings(true);

	// create CEGUI system object
	CEGUI::System::create(cegui_renderer);

	// setup resource directories
	DefaultResourceProvider* rp = static_cast<DefaultResourceProvider*>(System::getSingleton().getResourceProvider());
	rp->setResourceGroupDirectory("schemes", "datafiles/schemes/");
	rp->setResourceGroupDirectory("imagesets", "datafiles/imagesets/");
	rp->setResourceGroupDirectory("fonts", "datafiles/fonts/");
	rp->setResourceGroupDirectory("layouts", "datafiles/layouts/");
	rp->setResourceGroupDirectory("looknfeels", "datafiles/looknfeel/");
	rp->setResourceGroupDirectory("lua_scripts", "datafiles/lua_scripts/");
	rp->setResourceGroupDirectory("schemas", "datafiles/xml_schemas/");

	// set default resource groups
	ImageManager::setImagesetDefaultResourceGroup("imagesets");
	Font::setDefaultResourceGroup("fonts");
	Scheme::setDefaultResourceGroup("schemes");
	WidgetLookManager::setDefaultResourceGroup("looknfeels");
	WindowManager::setDefaultResourceGroup("layouts");
	ScriptModule::setDefaultResourceGroup("lua_scripts");

	XMLParser* parser = System::getSingleton().getXMLParser();
	if (parser->isPropertyPresent("SchemaDefaultResourceGroup"))
		parser->setProperty("SchemaDefaultResourceGroup", "schemas");

	// load TaharezLook scheme and DejaVuSans-10 font
	SchemeManager::getSingleton().createFromFile("TaharezLook.scheme", "schemes");
	FontManager::getSingleton().createFromFile("DejaVuSans-10.font");

	// set default font and cursor image and tooltip type
	System::getSingleton().getDefaultGUIContext().setDefaultFont("DejaVuSans-10");
	System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");
	System::getSingleton().getDefaultGUIContext().setDefaultTooltipType("TaharezLook/Tooltip");
}

void initWindows()
{
	using namespace CEGUI;

	/////////////////////////////////////////////////////////////
	// Add your gui initialisation code in here.
	// You can use the following code as an inspiration for
	// creating your own windows.
	// But you should preferably use layout loading because you won't
	// have to recompile everytime you change the layout.
	/////////////////////////////////////////////////////////////

	// load layout
	Window* root = WindowManager::getSingleton().loadLayoutFromFile("application_templates.layout");
	System::getSingleton().getDefaultGUIContext().setRootWindow(root);
}

// convert SDL mouse button to CEGUI mouse button
CEGUI::MouseButton SDLtoCEGUIMouseButton(const Uint8& button)
{
	using namespace CEGUI;

	switch (button)
	{
	case SDL_BUTTON_LEFT:
		return LeftButton;

	case SDL_BUTTON_MIDDLE:
		return MiddleButton;

	case SDL_BUTTON_RIGHT:
		return RightButton;

	default:
		return NoButton;
	}
}

int _SDL_main(int argc, char* argv[])
{
	using namespace CEGUI;

	// init SDL
	initSDL();

	// init cegui
	initCEGUI();

	// notify system of the window size
	System::getSingleton().notifyDisplaySizeChanged(Sizef(800.f, 600.f));

	// initialise windows and setup layout
	initWindows();

	// set gl clear color
	glClearColor(0, 0, 0, 255);

	bool quit = false;
	SDL_Event event;
	float time = SDL_GetTicks() / 1000.f;

	OpenGL3Renderer* renderer = static_cast<OpenGL3Renderer*>(System::getSingleton().getRenderer());

	// repeat until a quit is requested
	while (!quit && !SDL_QuitRequested())
	{
		// query and process events
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				quit = true;
				break;

			case SDL_MOUSEMOTION:
				CEGUI::System::getSingleton().getDefaultGUIContext().injectMousePosition(static_cast<float>(event.motion.x),
					static_cast<float>(event.motion.y));
				break;

			case SDL_MOUSEBUTTONDOWN:
				CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(SDLtoCEGUIMouseButton(event.button.button));
				break;

			case SDL_MOUSEBUTTONUP:
				CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(SDLtoCEGUIMouseButton(event.button.button));
				break;

			case SDL_MOUSEWHEEL:
				CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseWheelChange(static_cast<float>(event.wheel.y));
				break;

			case SDL_KEYDOWN:
				CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyDown(toCEGUIKey(event.key.keysym.scancode));
				CEGUI::System::getSingleton().getDefaultGUIContext().injectChar(event.key.keysym.sym);
				break;

			case SDL_KEYUP:
				CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp(toCEGUIKey(event.key.keysym.scancode));
				break;

			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					System::getSingleton().notifyDisplaySizeChanged(Sizef(static_cast<float>(event.window.data1),
						static_cast<float>(event.window.data2)));
					glViewport(0, 0, event.window.data1, event.window.data2);
				}
				break;

			default:
				break;

			}
		}

		glClear(GL_COLOR_BUFFER_BIT);

		// inject time pulses
		const float newtime = SDL_GetTicks() / 1000.f;
		const float time_elapsed = newtime - time;
		System::getSingleton().injectTimePulse(time_elapsed);
		System::getSingleton().getDefaultGUIContext().injectTimePulse(time_elapsed);
		time = newtime;

		// render gui
		renderer->beginRendering();
		System::getSingleton().renderAllGUIContexts();
		renderer->endRendering();

		// swap buffers
		SDL_GL_SwapWindow(window);
	}

	// destroy system and renderer
	System::destroy();
	OpenGL3Renderer::destroy(*renderer);
	renderer = 0;

	// delete SDL GL context
	SDL_GL_DeleteContext(context);

	// destroy SDL window
	SDL_DestroyWindow(window);

	// cleanup SDL
	SDL_Quit();

	return 0;
}