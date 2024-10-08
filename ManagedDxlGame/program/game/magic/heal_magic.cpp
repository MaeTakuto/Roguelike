#include "../../dxlib_ext/dxlib_ext.h"
#include "../manager/gm_manager.h"
#include "../manager/resource_manager.h"
#include "../my_library/my_library.h"
#include "../scene/scene_play.h"
#include "../base/character_base.h"
#include "../common/camera.h"
#include "../common/chara_status.h"
#include "../common/magic_status.h"
#include "../common/animation.h"
#include "heal_magic.h"

HealMagic::HealMagic() : heal_amount_(0), draw_elapsed_time_(0.0f), effect_draw_time_(1.0f){

	CsvData& magic_data = ResourceManager::getInstance()->loadCsvData("csv/magic_data.csv");

	int index = GameManager::CSV_CELL_ROW_START + std::underlying_type<eMagicName>::type(eMagicName::HEAL);

	magic_icon_gpc_hdl_ = ResourceManager::getInstance()->loadGraph("graphics/magic_icon/heal_magic_icon.png");

	// ----------------------------------- ステータス設定 -------------------------------------------------------------------------

	magic_status_->setMagicID(magic_data[index][0].getInt());
	magic_status_->setMagicName(magic_data[index][1].getString());
	magic_status_->setConsumeMP(magic_data[index][2].getInt());
	magic_status_->setMagicRange(magic_data[index][3].getInt());
	magic_status_->setMagicEffectMultiplier(magic_data[index][4].getFloat());

	level_up_bonus_status_->setConsumeMP(2);
	level_up_bonus_status_->setMagicRange(0);
	level_up_bonus_status_->setMagicEffectMultiplier(0.2f);

	// ----------------------------------------------------------------------------------------------------------------------------

	magic_status_explantion_.resize(3);

	magic_status_explantion_[0] = "レベル　　：";
	magic_status_explantion_[1] = "消費MP　　：";
	magic_status_explantion_[2] = "回復率　　：";

	magic_explantion_[0] = magic_status_explantion_[0] + std::to_string(magic_status_->getNowLevel());
	magic_explantion_[1] = magic_status_explantion_[1] + std::to_string(magic_status_->getConsumeMP());
	magic_explantion_[2] = magic_status_explantion_[2] + std::to_string(static_cast<int>(magic_status_->getMagicEffectMultiplier() * 100.0f )) + "％";

	std::string str, s;
	str = magic_data[index][5].getString();

	std::stringstream ss{ str };

	for (int i = 3; std::getline(ss, s, '/'); ++i) {
		magic_explantion_[i] = s;
	}

	magic_target_ = eMagicTarget::OWNER;

	CsvData& atk_effect_data = ResourceManager::getInstance()->loadCsvData("csv/effect_gpc_data.csv");

	std::vector<tnl::CsvCell> start_cell = atk_effect_data[GameManager::CSV_CELL_ROW_START + 3];

	// -------------------------- 魔法エフェクトの設定 -----------------------------------------
	magic_effect_gpc_hdl_ = ResourceManager::getInstance()->loadAnimation(
		start_cell[1].getString(),								// 画像パス,
		start_cell[2].getInt() * start_cell[3].getInt(),		// フレーム総数
		start_cell[2].getInt(),									// 横方向の分割数
		start_cell[3].getInt(),									// 縦方向の分割数
		start_cell[4].getInt(),									// 横方向の分割サイズ
		start_cell[5].getInt()									// 縦方向の分割サイズ
		);

	magic_effect_ = std::make_shared<Animation>();
	magic_effect_->setAnimGraphicHandle(magic_effect_gpc_hdl_);
	magic_effect_->setFrameChangeInterval(0.05f);
	magic_effect_->setBlendMode(DX_BLENDMODE_ADD);
}

HealMagic::~HealMagic() {

}

void HealMagic::update(float delta_time) {
	
	magic_effect_->update(delta_time);

	if (!magic_effect_->isEnable()) {
		is_draw_effect_ = false;
	}
}

void HealMagic::draw(const std::shared_ptr<Camera> camera) {
	magic_effect_->draw(camera);
}

std::shared_ptr<MagicBase> HealMagic::createClone() {
	auto clone = std::make_shared<HealMagic>();
	return clone;
}

void HealMagic::levelUpMagic() {

	magic_status_->levelUp(level_up_bonus_status_);

	// 説明文を更新
	updateMagicExplantion();
}

void HealMagic::setupToUseMagic(const std::shared_ptr<Character> owner) {

	auto& owner_pos = owner->getPos();

	// エフェクトの表示位置を設定
	tnl::Vector2i effect_draw_pos = tnl::Vector2i(static_cast<int>(owner_pos.x), static_cast<int>(owner_pos.y)) * GameManager::DRAW_CHIP_SIZE
		+ tnl::Vector2i(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1);

	tnl::Vector2i effect_draw_size = tnl::Vector2i(GameManager::DRAW_CHIP_SIZE, GameManager::DRAW_CHIP_SIZE);

	magic_effect_->setDrawPos(effect_draw_pos);
	magic_effect_->setDrawSize(effect_draw_size);
}

void HealMagic::startDrawEffect() {
	
	is_draw_effect_ = true;
	magic_effect_->startAnimation();
	ResourceManager::getInstance()->playSound("sound/se/heal_type2.mp3", DX_PLAYTYPE_BACK);
}

void HealMagic::useMagic(std::shared_ptr<Character> owner) {

	if (!owner) {
		return;
	}

	auto scene_play = std::dynamic_pointer_cast<ScenePlay>(GameManager::getInstance()->getSceneInstance());
	if (!scene_play) {
		return;
	}

	heal_amount_ = owner->getStatus().getMaxHP() * magic_status_->getMagicEffectMultiplier();
	owner->healHP(heal_amount_);
	scene_play->setMessage(owner->getName() + "は" + std::to_string(heal_amount_) + "回復した");

}

void HealMagic::updateMagicExplantion() {

	CsvData& magic_data = ResourceManager::getInstance()->loadCsvData("csv/magic_data.csv");
	int index = GameManager::CSV_CELL_ROW_START + std::underlying_type<eMagicName>::type(eMagicName::HEAL);

	magic_explantion_[0] = magic_status_explantion_[0] + std::to_string(magic_status_->getNowLevel());
	magic_explantion_[1] = magic_status_explantion_[1] + std::to_string(magic_status_->getConsumeMP());
	magic_explantion_[2] = magic_status_explantion_[2] + std::to_string(static_cast<int>(magic_status_->getMagicEffectMultiplier() * 100.0f)) + "％";
	std::string str, s;
	str = magic_data[index][5].getString();

	std::stringstream ss{ str };

	for (int i = 3; std::getline(ss, s, '/'); ++i) {
		magic_explantion_[i] = s;
	}
}
