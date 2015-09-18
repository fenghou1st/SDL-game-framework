#include <cstdio>
#include <cassert>
#include <stdexcept>

#include <boost/locale.hpp>

#include "game/common.h"
#include "game/sdl/sdl_log.h"

#include "game_base.h"
#include "state.h"

using namespace std;
using namespace sdl;
using namespace fenghou;


void make_default_state();


int main(int argc, char * argv [])
{
	try
	{
		make_default_state();
	}
	catch (logic_error & e)
	{
		log::fatal(e.what());
		return 1;
	}
	catch (runtime_error & e)
	{
		log::fatal(e.what());
		return 1;
	}
	catch (...)
	{
		log::fatal("Catched unknown exception.");
		return 1;
	}

	return 0;
}


void make_default_state()
{
	GameBase game_base(ORG_NAME, APP_NAME);
	if (!game_base.is_ok()) throw runtime_error("GameBase::is_ok");

	string default_save_file = GameBase::get_data_path() + "defaults" + PATH_SEP + SAVE_FILE;

	Character c;
	c.appearance = 0;
	c.hp = 100;
	c.pos = {1000, 1000, 0};
	c.vel = {10, 10, 0};
	c.ani = 0;
	c.ani_progress = 0;
	c.name = boost::locale::conv::to_utf<char>("Ö÷½Ç", "GB2312");
	c.exp = 100;
	c.lvl = 1;
	c.str = 10;
	c.dex = 10;
	c.wiz = 10;
	c.stamina = 100;
	c.mana = 100;
	c.actions;

	State state(default_save_file);
	state.chars.push_back(c);
	state.screen_tips = boost::locale::conv::to_utf<char>("ÖÐÎÄUTF-8", "GB2312");

	if (!state.save()) throw runtime_error("State::save");
}
