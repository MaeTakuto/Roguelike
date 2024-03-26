#include "../../dxlib_ext/dxlib_ext.h"
#include "../manager/gm_manager.h"
#include "../manager/resource_manager.h"
#include "../base/character_base.h"
#include "../scene/scene_play.h"
#include "../object/projectile.h"
#include "../common/camera.h"
#include "fire_magic.h"


FireMagic::FireMagic() : atk_target_(nullptr), fire_ball_(std::make_shared<Projectile>()) {
	magic_name_ = "ファイア";
	magic_target_ = eMagicTarget::OTHER;
	consumed_mp_ = 7;

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

// =====================================================================================
// エフェクトを表示する
// =====================================================================================
void FireMagic::setupToUseMagic(const std::shared_ptr<Character> user) {

	fire_ball_->setProjectileGpcHdl(fire_ball_gpc_hdl_[std::underlying_type<eDir_8>::type(user->getLookingDir())]);
	fire_ball_->setupToLaunchProjectile(user->getPos(), user->getLookingDir(), 10);
}

// =====================================================================================
// エフェクトを表示する
// =====================================================================================
void FireMagic::startDrawEffectOnOther(const tnl::Vector2i& pos, const tnl::Vector2i& size, eDir_8 other_dir) {

	is_draw_effect_ = true;
	fire_ball_->startToLaunchProjectile();
}

// =====================================================================================
// 魔法を使用
// =====================================================================================
void FireMagic::useMagic(std::shared_ptr<Character> owner) {

	if (!atk_target_) {
		return;
	}

	auto scene_play = std::dynamic_pointer_cast<ScenePlay>(GameManager::GetInstance()->getSceneInstance());
	if (!scene_play) {
		return;
	}

	int damage = owner->getStatus().getAtk() * 1.15f;
	scene_play->applyDamage(owner, atk_target_, damage);

}
