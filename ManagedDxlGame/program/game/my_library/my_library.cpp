#include <random>
#include "../../dxlib_ext/dxlib_ext.h"
#include "my_library.h"

namespace mtl {

	// 指定された範囲からランダムに値を返す
	int getRandomValue(int start, int end) {

		// ランダムなシードを生成
		std::random_device rd;
		std::mt19937 gen(rd());

		// 一様分布を持つ整数型の乱数ジェネレータ
		std::uniform_int_distribution<> dis(start, end);
		// ランダムな整数を取得
		int random_value = dis(gen);

		return random_value;
	}

	// 指定した文字列を右揃えにする
	std::string rightAlign(const std::string& str, int width) {
		if (str.size() >= width) return str;
		return std::string(width - str.size(), ' ') + str;
	}
	
	// 指定した浮動小数点数を固定小数点表記にし、指定した桁数に変換して文字列で返します。
	std::string toStringWithPrecision(double value, int precision) {
		std::ostringstream out;
		out << std::fixed << std::setprecision(precision) << value;
		return out.str();
	}
}