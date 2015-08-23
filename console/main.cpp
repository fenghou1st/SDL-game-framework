#include <cstdio>
#include <cassert>
#include <stdexcept>

#include "common/common.h"

#include "game.h"

using namespace std;
using namespace sdl;


int main(int argc, char * argv [])
{
	try
	{
		Game game;
		if (!game.is_ok()) throw runtime_error("Failed to initialize Game instance");
		if (!game.init()) throw runtime_error("Failed to initialize Game");
		if (!game.run()) throw runtime_error("Failed to run Game");
	}
	catch (logic_error & e) { log::fatal(e.what()); }
	catch (runtime_error & e) { log::fatal(e.what()); }
	catch (...) { log::fatal("Catched unknown exception."); }

	return 0;
}
