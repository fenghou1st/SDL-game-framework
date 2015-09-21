#include <cstdio>
#include <cassert>
#include <stdexcept>

#include <boost/filesystem.hpp>

#include <yaml-cpp/yaml.h>

#include "game/common.h"
#include "game/sdl/sdl_log.h"

#include "game_base.h"
#include "state.h"
#include "actions/actions.h"

using namespace std;
using namespace sdl;
using namespace fenghou;


void load_default_state();
void load(YAML::Node & node, State & state);


int main(int argc, char * argv [])
{
	try
	{
		load_default_state();
	}
	catch (exception & e)
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


void load_default_state()
{
	GameBase game_base(ORG_NAME, APP_NAME);
	if (!game_base.is_ok()) throw runtime_error("GameBase::is_ok");

	string default_save_bin = GameBase::get_data_path() + "defaults" + PATH_SEP + SAVE_FILE;
	State state(default_save_bin);

	string default_save_txt = GameBase::get_data_path() + "defaults" + PATH_SEP + "save.txt";
	if (!boost::filesystem::exists(default_save_txt) || boost::filesystem::file_size(default_save_txt) <= 0)
		throw runtime_error("save txt file not exists");

	auto node = YAML::LoadFile(default_save_txt);

	load(node, state);

	if (!state.save()) throw runtime_error("State::save");
}


void load(YAML::Node & node, State & state)
{
	auto & chars = node["chars"];
	for (size_t i = 0; i < chars.size(); ++i)
	{
		auto & character = chars[i];
		auto c = new Character();
		state.chars.push_back(c);

		c->appearance = character["appearance"].as<int_fast32_t>();
		c->hp = character["hp"].as<double>();

		auto & pos = character["pos"];
		c->pos.x = pos["x"].as<double>();
		c->pos.y = pos["y"].as<double>();
		c->pos.z = pos["z"].as<double>();

		auto & vel = character["vel"];
		c->vel.x = vel["x"].as<double>();
		c->vel.y = vel["y"].as<double>();
		c->vel.z = vel["z"].as<double>();

		c->ani = character["ani"].as<int_fast32_t>();
		c->ani_progress = character["ani_progress"].as<double>();
		c->name = character["name"].as<string>();
		c->exp = character["exp"].as<int_fast32_t>();
		c->lvl = character["lvl"].as<int_fast16_t>();
		c->str = character["str"].as<double>();
		c->dex = character["dex"].as<double>();
		c->wiz = character["wiz"].as<double>();
		c->stamina = character["stamina"].as<double>();
		c->mana = character["mana"].as<double>();

		auto & actions = character["actions"];
		for (size_t i = 0; i < actions.size(); ++i)
		{
			auto & action = actions[i];
			auto type = action["type"].as<string>();

			if (type == "move")
			{
				auto phi = action["phi"].as<double>();
				auto theta = action["theta"].as<double>();
				auto power_ratio = action["power_ratio"].as<double>();
				c->actions.push_back(new Move(phi, theta, power_ratio));
			}
		}
	}

	state.screen_tips = node["screen_tips"].as<string>();
}
