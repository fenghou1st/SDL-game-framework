#include "cegui.h"

using namespace std;
using namespace sdl;

// Implementer Interface ///////////////////////////////////////////////////////////////////////////////////////////////
// Local Field /////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace
{
	// Global Variables ////////////////////////////////////////////////////////////////////////////////////////////////
	uint8_t _key_map[SDL_NUM_SCANCODES];

	// Classes /////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Functions ///////////////////////////////////////////////////////////////////////////////////////////////////////

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

	// Global Variables ////////////////////////////////////////////////////////////////////////////////////////////////
}

// Global Variables ////////////////////////////////////////////////////////////////////////////////////////////////////

// Functions ///////////////////////////////////////////////////////////////////////////////////////////////////////////

CEGUI::Key::Scan cegui::toCEGUIKey(SDL_Scancode key)
{
	return static_cast<CEGUI::Key::Scan>(_key_map[key]);
}


void cegui::initCEGUI(const string & data_path, const string & log_path, int width, int height)
{
	using namespace CEGUI;

	init_key_map();

	auto & renderer = OpenGL3Renderer::create(Sizef(static_cast<float>(width), static_cast<float>(height)));
	renderer.enableExtraStateSettings(true);

	auto & system = CEGUI::System::create(renderer, 0, 0, 0, 0, "", log_path + "CEGUI.log");

	//
	auto rp = static_cast<DefaultResourceProvider *>(System::getSingleton().getResourceProvider());
	rp->setResourceGroupDirectory("fonts", data_path + "cegui/fonts/");
	rp->setResourceGroupDirectory("schemes", data_path + "cegui/schemes/");
	rp->setResourceGroupDirectory("layouts", data_path + "cegui/layouts/");
	rp->setResourceGroupDirectory("looknfeels", data_path + "cegui/looknfeel/");
	rp->setResourceGroupDirectory("lua_scripts", data_path + "cegui/lua_scripts/");
	rp->setResourceGroupDirectory("imagesets", data_path + "cegui/imagesets/");
	rp->setResourceGroupDirectory("schemas", data_path + "cegui/xml_schemas/");

	Font::setDefaultResourceGroup("fonts");
	Scheme::setDefaultResourceGroup("schemes");
	WindowManager::setDefaultResourceGroup("layouts");
	WidgetLookManager::setDefaultResourceGroup("looknfeels");
	ScriptModule::setDefaultResourceGroup("lua_scripts");

	ImageManager::setImagesetDefaultResourceGroup("imagesets");

	auto parser = System::getSingleton().getXMLParser();
	if (parser->isPropertyPresent("SchemaDefaultResourceGroup"))
		parser->setProperty("SchemaDefaultResourceGroup", "schemas");

	//
	SchemeManager::getSingleton().createFromFile("TaharezLook.scheme", "schemes");
	FontManager::getSingleton().createFromFile("DejaVuSans-10.font");

	System::getSingleton().getDefaultGUIContext().setDefaultFont("DejaVuSans-10");
	System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");
	System::getSingleton().getDefaultGUIContext().setDefaultTooltipType("TaharezLook/Tooltip");
}


void cegui::initWindows()
{
	using namespace CEGUI;

	auto root = WindowManager::getSingleton().loadLayoutFromFile("test.layout");
	System::getSingleton().getDefaultGUIContext().setRootWindow(root);
}


CEGUI::MouseButton cegui::SDLtoCEGUIMouseButton(const Uint8 & button)
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
