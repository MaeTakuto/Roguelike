#include "../../dxlib_ext/dxlib_ext.h"
#include "../manager/gm_manager.h"
#include "../common/camera.h"
#include "projectile.h"


namespace {
	// 発射物のスピード
	const float PROJECTILE_SPEED = 10.0f;
}

// ====================================================
// コンストラクタ
// ====================================================
Projectile::Projectile() : pos_(0, 0, 0), target_pos_(0, 0, 0), projectile_gpc_hdl_(0), is_enable_(false) {

}

// ====================================================
// デストラクタ
// ====================================================
Projectile::~Projectile() {

	tnl::DebugTrace("Projectileのデストラクタが実行されました。\n");
}

// ====================================================
// アップデート
// ====================================================
void Projectile::update(float delta_time) {

	if (!is_enable_) {
		return;
	}

	// 移動する方向の単位ベクトル
	tnl::Vector3 direction = tnl::Vector3::Normalize(target_pos_ - pos_);
	tnl::Vector3 displacement =  direction * PROJECTILE_SPEED * delta_time;
	pos_ += displacement;

	if ( (target_pos_ - pos_).length() < 0.1f ) {
		is_enable_ = false;
	}

}

// ====================================================
// 描画
// ====================================================
void Projectile::draw(const std::shared_ptr<Camera> camera) {

	if (!is_enable_) {
		return;
	}

	// 描画位置調整
	tnl::Vector3 draw_pos = tnl::Vector3(pos_.x * GameManager::DRAW_CHIP_SIZE, pos_.y * GameManager::DRAW_CHIP_SIZE, 0)
		- camera->getPos() + tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);

	DrawExtendGraph(draw_pos.x, draw_pos.y, draw_pos.x + GameManager::DRAW_CHIP_SIZE, draw_pos.y + GameManager::DRAW_CHIP_SIZE,
		projectile_gpc_hdl_, true);
}
