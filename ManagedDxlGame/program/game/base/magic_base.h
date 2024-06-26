#pragma once
#include "../common/enum.h"

class Camera;
class Character;
class Animation;

// =====================================================================================
// 魔法を作成するためのベースクラス
// =====================================================================================
class MagicBase {
public:
	MagicBase();
	virtual ~MagicBase();

	virtual void update(float delta_time) = 0;
	virtual void draw(const std::shared_ptr<Camera> camera) = 0;

protected:
	// 魔法エフェクト
	std::shared_ptr<Animation> magic_effect_;
	// エフェクト画像
	std::vector<int> magic_effect_gpc_hdl_;
	// 魔法名
	std::string magic_name_;
	// 魔法の効果対象
	eMagicTarget magic_target_;
	// エフェクトの表示フラグ
	bool is_draw_effect_;
	// 消費MP
	int consumed_mp_;
	// 魔法効果量
	int magic_effect_amount_;

	// 魔法の説明
	std::vector<std::string> magic_explantion_;

public:
	inline bool isDrawEffect() const { return is_draw_effect_; }
	
	// 消費MPをセット
	inline int getConsumedMP() const { return consumed_mp_; }

	// 魔法効果量を取得（ 回復量、ダメージ量など ）
	inline int getMagicEffectAmount() const { return magic_effect_amount_; }

	inline const std::string& getMagicName() const { return magic_name_; }
	// 魔法の効果対象を返す
	inline eMagicTarget getMagicTarget() const { return magic_target_; }
	// 魔法の説明文を返す
	inline const std::vector<std::string>& getMagicExplantion() const { return magic_explantion_; }

	// 使う魔法の準備をする
	virtual void setupToUseMagic(const std::shared_ptr<Character> owner) = 0;
	virtual void startDrawEffect() = 0;
	virtual void useMagic(std::shared_ptr<Character> owner) = 0;

};