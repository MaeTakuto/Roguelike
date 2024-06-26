#pragma once
#include "../../dxlib_ext/dxlib_ext.h"

namespace mtl {

	// 指定された範囲からランダムに値を返す
	// arg1...範囲の始まり
	// arg2...範囲の終わり
	int getRandomValue(int start, int end);

	// 指定した文字列を右揃えにする
	std::string rightAlign(const std::string& str, int width);
}