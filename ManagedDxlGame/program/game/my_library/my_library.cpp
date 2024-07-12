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

	// �w�肵����������E�����ɂ���
	std::string rightAlign(const std::string& str, int width) {
		if (str.size() >= width) return str;
		return std::string(width - str.size(), ' ') + str;
	}
	
	// �w�肵�����������_�����Œ菬���_�\�L�ɂ��A�w�肵�������ɕϊ����ĕ�����ŕԂ��܂��B
	std::string toStringWithPrecision(double value, int precision) {
		std::ostringstream out;
		out << std::fixed << std::setprecision(precision) << value;
		return out.str();
	}
}