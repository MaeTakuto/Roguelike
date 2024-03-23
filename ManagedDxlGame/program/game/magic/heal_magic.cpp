#include "../../dxlib_ext/dxlib_ext.h"
#include "../manager/gm_manager.h"
#include "../manager/resource_manager.h"
#include "../scene/scene_play.h"
#include "../base/character_base.h"
#include "../common/camera.h"
#include "../common/chara_status.h"
#include "../common/animation.h"
#include "heal_magic.h"

HealMagic::HealMagic() : heal_amount_(0), draw_elapsed_time_(0.0f), effect_draw_time_(1.0f){
	magic_name_ = "ヒール";
	magic_target_ = eMagicTarget::OWNER;
	consumed_mp_ = 6;

	CsvData& atk_effect_data = ResourceManager::getInstance()->loadCsvData("csv/effect_gpc_data.csv");

	std::vector<tnl::CsvCell> start_cell = atk_effect_data[GameManager::CSV_CELL_ROW_START + 2];

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

void HealMagic::startDrawEffectOnOwner(tnl::Vector2i pos, tnl::Vector2i size) {
	is_draw_effect_ = true;
	magic_effect_->startAnimation();
	magic_effect_->setDrawPos(pos);
	magic_effect_->setDrawSize(size);
}

void HealMagic::useMagic(std::shared_ptr<Character> owner) {

	if (!owner) {
		return;
	}

	auto scene_play = std::dynamic_pointer_cast<ScenePlay>(GameManager::GetInstance()->getSceneInstance());
	if (!scene_play) {
		return;
	}

	heal_amount_ = owner->getStatus().getMaxHP() * 0.4f;
	owner->getStatus().healHP(heal_amount_);
	scene_play->setMessage(owner->getName() + "は" + std::to_string(heal_amount_) + "回復した");

}