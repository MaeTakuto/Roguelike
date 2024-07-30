#include "../../dxlib_ext/dxlib_ext.h"
#include "../manager/gm_manager.h"
#include "../manager/resource_manager.h"
#include "../base/character_base.h"
#include "../scene/scene_play.h"
#include "../common/magic_status.h"
#include "../object/projectile.h"
#include "../common/camera.h"
#include "../common/magic_status.h"
#include "magnetism_magic.h"

// =====================================================================================
// コンストラクタ
// =====================================================================================
MagnetismMagic::MagnetismMagic() : launch_dir_{0}, magic_bullet_(std::make_shared<Projectile>()), target_moving_position_(nullptr) {

	CsvData& magic_data = ResourceManager::getInstance()->loadCsvData("csv/magic_data.csv");

	int index = GameManager::CSV_CELL_ROW_START + std::underlying_type<eMagicName>::type(eMagicName::MAGNETISM);

	magic_icon_gpc_hdl_ = ResourceManager::getInstance()->loadGraph("graphics/magic_icon/magnetism_magic_icon.png");

	// ----------------------------------- ステータス設定 -------------------------------------------------------------------------

	magic_status_->setMagicID(magic_data[index][0].getInt());
	magic_status_->setMagicName(magic_data[index][1].getString());
	magic_status_->setConsumeMP(magic_data[index][2].getInt());
	magic_status_->setMagicRange(magic_data[index][3].getInt());
	magic_status_->setMagicEffectMultiplier(magic_data[index][4].getFloat());

	level_up_bonus_status_->setConsumeMP(1);
	level_up_bonus_status_->setMagicRange(2);
	level_up_bonus_status_->setMagicEffectMultiplier(0.f);

	// ----------------------------------------------------------------------------------------------------------------------------

	magic_status_explantion_.resize(3);

	magic_status_explantion_[0] = "レベル　　：";
	magic_status_explantion_[1] = "消費MP　　：";
	magic_status_explantion_[2] = "射程　　　：";

	magic_explantion_[0] = magic_status_explantion_[0] + std::to_string(magic_status_->getNowLevel());
	magic_explantion_[1] = magic_status_explantion_[1] + std::to_string(magic_status_->getConsumeMP());
	magic_explantion_[2] = "";
	std::string str, s;

	str = std::to_string(magic_status_->getMagicRange()) + magic_data[index][5].getString();
	std::stringstream ss{ str };

	for (int i = 3; std::getline(ss, s, '/'); ++i) {
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

std::shared_ptr<MagicBase> MagnetismMagic::createClone() {
	auto clone = std::make_shared<MagnetismMagic>();
	return clone;
}

void MagnetismMagic::levelUpMagic() {

	magic_status_->levelUp(level_up_bonus_status_);

	// 説明文を更新
	updateMagicExplantion();
}

// =====================================================================================
// 
// =====================================================================================
void MagnetismMagic::setupToUseMagic(const std::shared_ptr<Character> owner) {

	launch_dir_ = owner->getLookingDir();
	target_moving_position_ = nullptr;

	// 魔法弾の画像と発射位置を設定する
	magic_bullet_->setProjectileGpcHdl(magic_bullet_gpc_hdl_[std::underlying_type<eDir_8>::type(owner->getLookingDir())]);
	magic_bullet_->setupToLaunchProjectile(owner->getPos(), launch_dir_, magic_status_->getMagicRange());
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

	auto scene_play = std::dynamic_pointer_cast<ScenePlay>(GameManager::getInstance()->getSceneInstance());
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

void MagnetismMagic::updateMagicExplantion() {

	CsvData& magic_data = ResourceManager::getInstance()->loadCsvData("csv/magic_data.csv");
	int index = GameManager::CSV_CELL_ROW_START + std::underlying_type<eMagicName>::type(eMagicName::MAGNETISM);

	magic_explantion_[0] = "現在レベル：" + std::to_string(magic_status_->getNowLevel());
	magic_explantion_[1] = "消費MP：" + std::to_string(magic_status_->getConsumeMP());
	magic_explantion_[2] = "";
	std::string str, s;

	str = std::to_string(magic_status_->getMagicRange()) + magic_data[index][5].getString();
	std::stringstream ss{ str };

	for (int i = 3; std::getline(ss, s, '/'); ++i) {
		magic_explantion_[i] = s;
	}
}
