#include "../../dxlib_ext/dxlib_ext.h"
#include "../manager/gm_manager.h"
#include "../common/camera.h"
#include "../scene/scene_play.h"
#include "../base/character_base.h"
#include "../base/enemy_base.h"
#include "projectile.h"


namespace {
	// 発射物のスピード
	const float PROJECTILE_SPEED = 10.0f;
}

// ====================================================
// コンストラクタ
// ====================================================
Projectile::Projectile() : pos_(0, 0, 0), move_dir_{0}, hit_character_(nullptr), target_pos_(0, 0, 0), projectile_gpc_hdl_(0), is_enable_(false),
	max_cell_reach_(0), cell_count_(0)
{

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

void Projectile::launchProjectile(const tnl::Vector3& start_pos, eDir_8 move_dir, int max_cell_reach) {
	is_enable_ = true;
	pos_ = start_pos;
	move_dir_ = move_dir;
	hit_character_ = nullptr;
	max_cell_reach_ = max_cell_reach;
	cell_count_ = 1;
	checkCellInMoveDir(pos_ + DIR_POS[std::underlying_type<eDir_8>::type(move_dir_) ]);
}

void Projectile::checkCellInMoveDir(const tnl::Vector3& pos) {

	std::shared_ptr<ScenePlay> scene_play = std::dynamic_pointer_cast<ScenePlay>(GameManager::GetInstance()->getSceneInstance());
	if (!scene_play) {
		return;
	}
	// 最大射程マスを超えたら終了
	if (cell_count_ > max_cell_reach_) {
		target_pos_ = pos - DIR_POS[std::underlying_type<eDir_8>::type(move_dir_)];
		return;
	}

	++cell_count_;

	if (scene_play->getMapData(pos) == eMapData::WALL) {
		target_pos_ = pos - DIR_POS[ std::underlying_type<eDir_8>::type( move_dir_ ) ];
		return;
	}

	if (scene_play->getMapData(pos) == eMapData::ENEMY) {
		target_pos_ = pos;
		std::shared_ptr<EnemyBase> enemy = scene_play->findEnemy(pos);
		if (!enemy) {
			return;
		}
		hit_character_ = std::dynamic_pointer_cast<Character>(enemy);
		return;
	}
	if (scene_play->getMapData(pos) == eMapData::PLAYER) {
		target_pos_ = pos;
		hit_character_ = scene_play->getPlayer();
		return;
	}

	checkCellInMoveDir( pos + DIR_POS[ std::underlying_type<eDir_8>::type( move_dir_ ) ] );
}
