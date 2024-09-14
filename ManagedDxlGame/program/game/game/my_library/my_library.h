#pragma once
#include "../../dxlib_ext/dxlib_ext.h"

namespace mtl {

	// 指定された範囲からランダムに値を返す
	// arg1...範囲の始まり
	// arg2...範囲の終わり
	int getRandomValue(int start, int end);

	// 指定した文字列を右揃えにする
	std::string rightAlign(const std::string& str, int width);

	// 指定した浮動小数点数を固定小数点表記にし、指定した桁数に変換して文字列で返します。
	// arg1...文字列に変換する浮動小数点数
	// arg2...桁数
	std::string toStringWithPrecision(double value, int precision = 6);

}