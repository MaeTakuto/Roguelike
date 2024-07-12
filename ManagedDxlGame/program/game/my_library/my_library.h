#pragma once
#include "../../dxlib_ext/dxlib_ext.h"

namespace mtl {

	// �w�肳�ꂽ�͈͂��烉���_���ɒl��Ԃ�
	// arg1...�͈͂̎n�܂�
	// arg2...�͈͂̏I���
	int getRandomValue(int start, int end);

	// �w�肵����������E�����ɂ���
	std::string rightAlign(const std::string& str, int width);

	// �w�肵�����������_�����Œ菬���_�\�L�ɂ��A�w�肵�������ɕϊ����ĕ�����ŕԂ��܂��B
	// arg1...������ɕϊ����镂�������_��
	// arg2...����
	std::string toStringWithPrecision(double value, int precision = 6);

}