#pragma once


class MagicStatus {
public:
	MagicStatus();
	~MagicStatus();

private:
	// 魔法名
	std::string magic_name_;
	// 最大レベル
	int max_level_;
	// 現在のレベル
	int now_level_;
	// 消費MP
	int consume_mp_;
	// 魔法の射程距離
	int magic_range_;
	// 魔法効果倍率
	float magic_effect_multiplier_;

public:
	// 魔法名を取得
	const std::string& getMagicName() const { return magic_name_; }
	// 最大レベルを取得
	int getMaxLevel() const { return max_level_; }
	// 現在のレベルを取得
	int getNowLevel() const { return now_level_; }
	// 魔法の消費MPを取得
	int getConsumeMP() const { return consume_mp_; }
	// 魔法の射程距離を取得
	int getMagicRange() const { return magic_range_; }
	// 魔法の効果倍率を取得
	float getMagicEffectMultiplier() const { return magic_effect_multiplier_; }

	// 魔法名を設定する
	void setMagicName(const std::string& name) { magic_name_ = name; }
	// 最大レベルを設定する
	void setMaxLevel(int max_level) { max_level_ = max_level; }
	// 現在レベルを設定する
	void setNowLevel(int now_level) { now_level_ = now_level; }
	// 消費MPを設定する
	void setConsumeMP(int consume_mp) { consume_mp_ = consume_mp; }
	// 魔法の射程距離を設定する
	void setMagicRange(int magic_range) { magic_range_ = magic_range; }
	// 魔法の効果倍率を設定する
	void setMagicEffectMultiplier(float magic_effect_multiplier) { magic_effect_multiplier_ = magic_effect_multiplier; }

	// 魔法のレベルを上げる（ 最大の場合"false"を返す ）
	// 引数：消費MP、増やす魔法の射程値、増やす魔法の効果倍率
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