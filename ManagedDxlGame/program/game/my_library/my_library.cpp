#include <random>
#include "../../dxlib_ext/dxlib_ext.h"
#include "my_library.h"

namespace mtl {

	// �w�肳�ꂽ�͈͂��烉���_���ɒl��Ԃ�
	int getRandomValue(int start, int end) {

		// �����_���ȃV�[�h�𐶐�
		std::random_device rd;
		std::mt19937 gen(rd());

		// ��l���z���������^�̗����W�F�l���[�^
		std::uniform_int_distribution<> dis(start, end);
		// �����_���Ȑ������擾
		int random_value = dis(gen);

		return random_value;
	}
}