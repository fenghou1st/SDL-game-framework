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

	// ��һ��ʵ�ַ����ǣ�ֻ��������·����Ȼ���ڲ���ȡbase_path���ٽ��н����������ⲿ�͸�������֪��DataPath������ˡ�
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
