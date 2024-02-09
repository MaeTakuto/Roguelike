#pragma once
#include <string>

namespace dxe {
	extern std::string g_x_file_hedder_str;

	enum class eDepthStenclil {
		DEPTH_W_ON_T_ON
		, DEPTH_W_ON_T_OFF
		, DEPTH_W_OFF_T_ON
		, DEPTH_W_OFF_T_OFF
		, MAX
	};
	enum class eBlendState {
		ALPHA
		, ADD
		, SUB
		, MUL
		, NORMAL
		, MAX
	};

}

