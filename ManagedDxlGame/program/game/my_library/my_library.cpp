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
}