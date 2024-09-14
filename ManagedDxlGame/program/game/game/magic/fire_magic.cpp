#include "../../dxlib_ext/dxlib_ext.h"
#include "../manager/gm_manager.h"
#include "../manager/resource_manager.h"
#include "../my_library/my_library.h"
#include "../base/character_base.h"
#include "../scene/scene_play.h"
#include "../object/projectile.h"
#include "../common/camera.h"
#include "../common/magic_status.h"
#include "fire_magic.h"


FireMagic::FireMagic() : atk_target_(nullptr), fire_ball_(std::make_shared<Projectile>()) {

	CsvData& magic_data = ResourceManager::getInstance()->loadCsvData("csv/magic_data.csv");

	int index = GameManager::CSV_CELL_ROW_START + std::underlying_type<eMagicName>::type(eMagicName::FIRE);

	magic_icon_gpc_hdl_ = ResourceManager::getInstance()->loadGraph("graphics/magic_icon/fire_magic_icon.png");

	// ----------------------------------- �X�e�[�^�X�ݒ� -------------------------------------------------------------------------

	magic_status_->setMagicID(magic_data[index][0].getInt());
	magic_status_->setMagicName(magic_data[index][1].getString());
	magic_status_->setConsumeMP(magic_data[index][2].getInt());
	magic_status_->setMagicRange(magic_data[index][3].getInt());
	magic_status_->setMagicEffectMultiplier(magic_data[index][4].getFloat());

	level_up_bonus_status_->setConsumeMP(2);
	level_up_bonus_status_->setMagicRange(2);
	level_up_bonus_status_->setMagicEffectMultiplier(0.1f);

	tnl::DebugTrace("Multiplier = %f\n", magic_status_->getMagicEffectMultiplier());
	tnl::DebugTrace("Multiplier = %f\n", level_up_bonus_status_->getMagicEffectMultiplier());

	// ----------------------------------------------------------------------------------------------------------------------------

	magic_status_explantion_[0] = "���x���@�@�F";
	magic_status_explantion_[1] = "����MP�@�@�F";
	magic_status_explantion_[2] = "�_���[�W���F";
	magic_status_explantion_[3] = "�˒��@�@�@�F";

	magic_explantion_[0] = magic_status_explantion_[0] + std::to_string(magic_status_->getNowLevel());
	magic_explantion_[1] = magic_status_explantion_[1] + std::to_string(magic_status_->getConsumeMP());
	magic_explantion_[2] = magic_status_explantion_[2] + std::to_string( static_cast<int>( std::round( magic_status_->getMagicEffectMultiplier() * 100.0f ) ) ) + "��";
	std::string str, s;

	str = std::to_string(magic_status_->getMagicRange()) + magic_data[index][5].getString();
	std::stringstream ss{ str };

	for (int i = 3; std::getline(ss, s, '/'); ++i) {
		magic_explantion_[i] = s;
	}

	magic_target_ = eMagicTarget::OTHER;


	fire_ball_gpc_hdl_.resize( std::underlying_type<eDir_8>::type( eDir_8::MAX ) );
	fire_ball_gpc_hdl_ = ResourceManager::getInstance()->loadAnimation(
		"graphics/fire_ball.png",
		8,
		8,
		1,
		GameManager::CHIP_SIZE,
		GameManager::CHIP_SIZE
		);
}

FireMagic::~FireMagic() {

}

void FireMagic::update(float delta_time) {

	if (!is_draw_effect_) {
		return;
	}

	fire_ball_->update(delta_time);

	if (!fire_ball_->isEnable()) {
		is_draw_effect_ = false;
		atk_target_ = fire_ball_->getHitCharacter();

	}
}

void FireMagic::draw(const std::shared_ptr<Camera> camera) {
	fire_ball_->draw(camera);
}

std::shared_ptr<MagicBase> FireMagic::createClone() {
	auto clone = std::make_shared<FireMagic>();
	return clone;
}

void FireMagic::levelUpMagic() {

	magic_status_->levelUp(level_up_bonus_status_);

	// ���������X�V
	updateMagicExplantion();

}

// =====================================================================================
// ���@�����̏�������
// =====================================================================================
void FireMagic::setupToUseMagic(const std::shared_ptr<Character> owner) {

	auto scene_play = std::dynamic_pointer_cast<ScenePlay>(GameManager::getInstance()->getSceneInstance());
	if (!scene_play) {
		return;
	}

	scene_play->addAttacker(owner);

	// �΂̋ʂ̉摜�Ɣ��ˈʒu��ݒ肷��
	fire_ball_->setProjectileGpcHdl(fire_ball_gpc_hdl_[std::underlying_type<eDir_8>::type(owner->getLookingDir())]);
	fire_ball_->setupToLaunchProjectile(owner->getPos(), owner->getLookingDir(), magic_status_->getMagicRange());
}

// =====================================================================================
// �΂̋ʃG�t�F�N�g�𔭎˂���
// =====================================================================================
void FireMagic::startDrawEffect() {

	is_draw_effect_ = true;
	fire_ball_->startToLaunchProjectile();
	ResourceManager::getInstance()->playSound("sound/springin/fire.mp3", DX_PLAYTYPE_BACK);
}

// =====================================================================================
// ���@���g�p
// =====================================================================================
void FireMagic::useMagic(std::shared_ptr<Character> owner) {

	if (!atk_target_) {
		return;
	}

	auto scene_play = std::dynamic_pointer_cast<ScenePlay>(GameManager::getInstance()->getSceneInstance());
	if (!scene_play) {
		return;
	}

	magic_effect_amount_ = owner->getStatus().getAtk() * magic_status_->getMagicEffectMultiplier();

	scene_play->addAttackTarget(atk_target_);

}

void FireMagic::updateMagicExplantion() {

	CsvData& magic_data = ResourceManager::getInstance()->loadCsvData("csv/magic_data.csv");
	int index = GameManager::CSV_CELL_ROW_START + std::underlying_type<eMagicName>::type(eMagicName::FIRE);

	magic_explantion_[0] = magic_status_explantion_[0] + std::to_string(magic_status_->getNowLevel());
	magic_explantion_[1] = magic_status_explantion_[1] + std::to_string(magic_status_->getConsumeMP());
	magic_explantion_[2] = magic_status_explantion_[2] + std::to_string(static_cast<int>(std::round(magic_status_->getMagicEffectMultiplier() * 100.0f))) + "��";
	std::string str, s;

	str = std::to_string(magic_status_->getMagicRange()) + magic_data[index][5].getString();
	std::stringstream ss{ str };

	for (int i = 3; std::getline(ss, s, '/'); ++i) {
		magic_explantion_[i] = s;
	}
}
