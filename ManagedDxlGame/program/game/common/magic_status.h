#pragma once


class MagicStatus {
public:
	MagicStatus();
	~MagicStatus();

private:
	// ���@��
	std::string magic_name_;
	// �ő僌�x��
	int max_level_;
	// ���݂̃��x��
	int now_level_;
	// ����MP
	int consume_mp_;
	// ���@�̎˒�����
	int magic_range_;
	// ���@���ʔ{��
	float magic_effect_multiplier_;

public:
	// ���@�����擾
	const std::string& getMagicName() const { return magic_name_; }
	// �ő僌�x�����擾
	int getMaxLevel() const { return max_level_; }
	// ���݂̃��x�����擾
	int getNowLevel() const { return now_level_; }
	// ���@�̏���MP���擾
	int getConsumeMP() const { return consume_mp_; }
	// ���@�̎˒��������擾
	int getMagicRange() const { return magic_range_; }
	// ���@�̌��ʔ{�����擾
	float getMagicEffectMultiplier() const { return magic_effect_multiplier_; }

	// ���@����ݒ肷��
	void setMagicName(const std::string& name) { magic_name_ = name; }
	// �ő僌�x����ݒ肷��
	void setMaxLevel(int max_level) { max_level_ = max_level; }
	// ���݃��x����ݒ肷��
	void setNowLevel(int now_level) { now_level_ = now_level; }
	// ����MP��ݒ肷��
	void setConsumeMP(int consume_mp) { consume_mp_ = consume_mp; }
	// ���@�̎˒�������ݒ肷��
	void setMagicRange(int magic_range) { magic_range_ = magic_range; }
	// ���@�̌��ʔ{����ݒ肷��
	void setMagicEffectMultiplier(float magic_effect_multiplier) { magic_effect_multiplier_ = magic_effect_multiplier; }

	// ���@�̃��x�����グ��i �ő�̏ꍇ"false"��Ԃ� �j
	// �����F����MP�A���₷���@�̎˒��l�A���₷���@�̌��ʔ{��
	bool levelUp(int consume_mp, int add_magic_range, float add_magic_effect_multiplier);

	MagicStatus& operator=(const MagicStatus& other) {
		magic_name_ = other.magic_name_;
		this->max_level_ = other.max_level_;
		this->now_level_ = other.now_level_;
		this->consume_mp_ = other.consume_mp_;
		this->magic_range_ = other.magic_range_;
		this->magic_effect_multiplier_ = other.magic_effect_multiplier_;
		return *this;
	}

};