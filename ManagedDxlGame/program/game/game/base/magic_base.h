#pragma once
#include "../common/enum.h"

class Camera;
class Character;
class Animation;
class MagicStatus;

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
	// 魔法のアイコン画像
	int magic_icon_gpc_hdl_;
	// 魔法エフェクト
	std::shared_ptr<Animation> magic_effect_;
	// エフェクト画像
	std::vector<int> magic_effect_gpc_hdl_;
	// 魔法のステータス
	std::shared_ptr<MagicStatus> magic_status_;
	// レベルアップ時の上昇ステータス
	std::shared_ptr<MagicStatus> level_up_bonus_status_;
	// 魔法の効果対象
	eMagicTarget magic_target_;
	// エフェクトの表示フラグ
	bool is_draw_effect_;
	// 魔法効果量
	int magic_effect_amount_;

	// 魔法の説明
	std::vector<std::string> magic_explantion_;

	// 魔法の説明のベース項目
	std::vector<std::string> magic_status_explantion_;

public:
	// エフェクトを描画しているか判定する
	inline bool isDrawEffect() const { return is_draw_effect_; }
	
	// 魔法のアイコン画像を返す
	inline int getMagicIconGpcHdl() const { return magic_icon_gpc_hdl_; }

	// 魔法のステータスを取得する
	inline const std::shared_ptr<MagicStatus> getMagicStatus() const { return magic_status_; }

	// 魔法効果量を取得（ 回復量、ダメージ量など ）
	inline int getMagicEffectAmount() const { return magic_effect_amount_; }
	
	// 魔法の効果対象を返す
	inline eMagicTarget getMagicTarget() const { return magic_target_; }
	// 魔法の説明文を返す
	inline const std::vector<std::string>& getMagicExplantion() const { return magic_explantion_; }
	
	//
	std::vector<std::string> getStatusComparisonText() const;

	// 
	std::vector<std::string> getLevelUpStatusComparisonText() const;

	// 自身のクローン魔法を生成する
	virtual std::shared_ptr<MagicBase> createClone() = 0;

	// 魔法のレベルを上げる
	virtual void levelUpMagic() = 0;

	// 使う魔法の準備をする
	virtual void setupToUseMagic(const std::shared_ptr<Character> owner) = 0;
	virtual void startDrawEffect() = 0;
	virtual void useMagic(std::shared_ptr<Character> owner) = 0;

protected:
	// 魔法説明を更新
	virtual void updateMagicExplantion() = 0;

};