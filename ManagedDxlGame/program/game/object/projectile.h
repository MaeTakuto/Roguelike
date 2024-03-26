#pragma once

class Camera;

// =====================================================================================
// キャラクターが発射する投擲物（ target_pos_の方向に向かう ）
// =====================================================================================
class Projectile {
public:
	Projectile();
	~Projectile();

	// アップデート
	void update(float delta_time);
	// 描画
	void draw(const std::shared_ptr<Camera> camera);

private:
	// 現在位置
	tnl::Vector3 pos_;
	// 移動する方向
	eDir_8 move_dir_;

	// 目標の位置
	tnl::Vector3 target_pos_;
	// 発射体がヒットしたキャラクター
	std::shared_ptr<Character> hit_character_;

	// 有効か判定
	bool is_enable_;
	// 投擲物の画像
	int projectile_gpc_hdl_;
	// 発射体が届く最大セル数
	int max_cell_reach_;
	// 
	int cell_count_;

public:
	void setupToLaunchProjectile(const tnl::Vector3& start_pos, eDir_8 move_dir, int max_cell_reach);
	void startToLaunchProjectile();
	void checkCellInMoveDir(const tnl::Vector3& pos);

	// -------------- ゲッター ------------------
	// 有効になっているか判定
	inline bool isEnable() { return is_enable_; }
	// 発射体に当たったキャラクターを取得、いなければ "nullptr" を返す。
	inline std::shared_ptr<Character> getHitCharacter() { return hit_character_; }

	// -------------- セッター ------------------
	// 投擲物の位置をセット
	inline void setPos(const tnl::Vector3& pos) { pos_ = pos; }
	// 目標の位置
	inline void setTargetPos(const tnl::Vector3& target_pos) { target_pos_ = target_pos; }
	// 投擲物の画像をセット
	inline void setProjectileGpcHdl(int gpc_hdl) { projectile_gpc_hdl_ = gpc_hdl; }
	// 有効にするか設定
	inline void setEnable(bool is_enable) { is_enable_ = is_enable; }
};