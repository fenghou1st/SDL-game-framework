//----------------------------------------------------------------------------------------------------------------------
// Data path class that is compatiable with both packed and unpacked file structure
//
// base_path:
//   the data path, e.g. "/usr/local/MyGame/data/".
// rel_path:
//   - data file, e.g. "textures/scene1/outdoor1.tex"
//   - packed file + packed item, e.g. in "textures/scene1/outdoor1.tex",
//     packed file is "texture.gpk", packed item is "scene1/outdoor1.tex".
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include <cstdio>
#include <cassert>
#include <stdexcept>
#include <memory>

// User Interface //////////////////////////////////////////////////////////////////////////////////////////////////////

namespace fenghou { namespace game
{
	using namespace std;

	// Classes /////////////////////////////////////////////////////////////////////////////////////////////////////////

	// 另一种实现方法是，只传入完整路径，然后内部获取base_path，再进行解析。这样外部就根本不用知道DataPath这个类了。
	class DataPath
	{
	public:
		DataPath();
		DataPath(const string & base_path, const string & rel_path);
		~DataPath();

		void set(const string & base_path, const string & rel_path);
		const string & unpacked_file() const;
		const string & packed_file() const;
		const string & packed_item() const;

	private:
		class Impl; unique_ptr<Impl> impl;
	};

	// Functions ///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Global Variables ////////////////////////////////////////////////////////////////////////////////////////////////
}}
