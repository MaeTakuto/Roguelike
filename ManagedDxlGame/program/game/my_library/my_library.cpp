#include <random>
#include "../dxlib_ext/dxlib_ext.h"
#include "my_library.h"

namespace mtl {

	// Žw’è‚³‚ê‚½”ÍˆÍ‚©‚çƒ‰ƒ“ƒ_ƒ€‚É’l‚ð•Ô‚·
	int getRandomValue(int start, int end) {

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(start, end);
		int random_value = dis(gen);

		return random_value;
	}
}