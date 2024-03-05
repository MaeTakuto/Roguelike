#include <random>
#include "../dxlib_ext/dxlib_ext.h"
#include "my_library.h"

namespace mtl {

	// 指定された範囲からランダムに値を返す
	int getRandomValue(int start, int end) {

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(start, end);
		int random_value = dis(gen);

		return random_value;
	}
}