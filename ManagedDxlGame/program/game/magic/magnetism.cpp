#include "../../dxlib_ext/dxlib_ext.h"
#include "../manager/gm_manager.h"
#include "../manager/resource_manager.h"
#include "../base/character_base.h"
#include "../scene/scene_play.h"
#include "../object/projectile.h"
#include "../common/camera.h"
#include "magnetism_magic.h"

// =====================================================================================
// コンストラクタ
// =====================================================================================
MagnetismMagic::MagnetismMagic() : launch_dir_{0}, magic_bullet_(std::make_shared<Projectile>()), target_moving_position_(nullptr) {

	CsvData& magic_data = ResourceManager::getInstance()->loadCsvData("csv/magic_data.csv");

	int index = GameManager::CSV_CELL_ROW_START + std::underlying_type<eMagicName>::type(eMagicName::MAGNETISM);

	magic_name_ = magic_data[index][0].getString();
	consumed_mp_ = magic_data[index][1].getInt();
	magic_explantion_[0] = "消費MP：" + std::to_string(consumed_mp_);
	std::string str, s;
	str = magic_data[index][2].getString();

	std::stringstream ss{ str };

	for (int i = 1; std::getline(ss, s, '/'); ++i) {
		magic_explantion_[i] = s;
	}

	magic_bullet_gpc_hdl_.resize(std::underlying_type<eDir_8>::type(eDir_8::MAX));
	magic_bullet_gpc_hdl_ = ResourceManager::getInstance()->loadAnimation(
		"graphics/fire_ball.png",
		8,
		8,
		1,
		GameManager::CHIP_SIZE,
		GameManager::CHIP_SIZE
	);
}

// =====================================================================================
// 
// =====================================================================================
MagnetismMagic::~MagnetismMagic() {

}

// =====================================================================================
// 
// =====================================================================================
void MagnetismMagic::update(float delta_time) {

	if (!is_draw_effect_) {
		return;
	}

	magic_bullet_->update(delta_time);

	if (!magic_bullet_->isEnable()) {
		is_draw_effect_ = false;

		if (magic_bullet_->getHitCharacter()) {
			target_moving_position_
				= std::make_shared<tnl::Vector3>(magic_bullet_->getPos() - DIR_POS[std::underlying_type<eDir_8>::type(launch_dir_)]);
		}
		else if (magic_bullet_->isCollision()) {
			target_moving_position_
				= std::make_shared<tnl::Vector3>(magic_bullet_->getPos());
		}
	}
}

// =====================================================================================
// 
// =====================================================================================
void MagnetismMagic::draw(std::shared_ptr<Camera> camera) {
	magic_bullet_->draw(camera);
}

// =====================================================================================
// 
// =====================================================================================
void MagnetismMagic::setupToUseMagic(const std::shared_ptr<Character> owner) {

	launch_dir_ = owner->getLookingDir();
	target_moving_position_ = nullptr;

	// 魔法弾の画像と発射位置を設定する
	magic_bullet_->setProjectileGpcHdl(magic_bullet_gpc_hdl_[std::underlying_type<eDir_8>::type(owner->getLookingDir())]);
	magic_bullet_->setupToLaunchProjectile(owner->getPos(), launch_dir_, 10);
}

// =====================================================================================
// 
// =====================================================================================
void MagnetismMagic::startDrawEffect() {

	is_draw_effect_ = true;
	magic_bullet_->startToLaunchProjectile();
	ResourceManager::getInstance()->playSound("sound/springin/magnetism.mp3", DX_PLAYTYPE_BACK);
}

// =====================================================================================
// 
// =====================================================================================
void MagnetismMagic::useMagic(std::shared_ptr<Character> owner) {

	auto scene_play = std::dynamic_pointer_cast<ScenePlay>(GameManager::GetInstance()->getSceneInstance());
	if (!scene_play) {
		return;
	}

	if (!target_moving_position_) {
		std::string message = "";
		scene_play->setMessage("魔法弾の届く場所に何もなかった");
		return;
	}

	owner->moveToTargetPos(*target_moving_position_);
	scene_play->modifyEnemyAction();

}
