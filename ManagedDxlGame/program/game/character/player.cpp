#include "../../dxlib_ext/dxlib_ext.h"
#include "../manager/gm_manager.h"
#include "../common/camera.h"
#include "../manager/resource_manager.h"
#include "../scene/scene_play.h"
#include "enemy.h"
#include "player.h"


Player::Player() {

	gpc_hdl_data_ = tnl::LoadCsv("csv/player_gpc_data.csv");

	chara_gpc_hdl_.resize(4);

	for (int i = 1; i < gpc_hdl_data_.size(); i++) {
		
		chara_gpc_hdl_[i - 1].resize( gpc_hdl_data_[i][1].getInt() );

		chara_gpc_hdl_[i - 1] = ResourceManager::getInstance()->loadAnimation
		(gpc_hdl_data_[i][0].getString(),
		 gpc_hdl_data_[i][1].getInt(),
		 gpc_hdl_data_[i][2].getInt(),
		 gpc_hdl_data_[i][3].getInt(),
		 GameManager::CHIP_SIZE,
		 GameManager::CHIP_SIZE
		);
	}

	select_cell_blue_gpc_hdl_ = ResourceManager::getInstance()->loadGraph("graphics/blue.bmp");
	select_cell_red_gpc_hdl_ = ResourceManager::getInstance()->loadGraph("graphics/red.bmp");

	name_ = "�v���C���[";
	status_.setStatus(1, 15, 5, 0, 0);
	anim_dir_ = eDir_4::DOWN;
	act_state_ = eActState::IDLE;
	is_collision_ = false;

}

Player::~Player() {
	tnl::DebugTrace("Player�̃f�X�g���N�^�����s����܂���\n");
}

void Player::update(float delta_time) {

	sequence_.update(delta_time);
}

// ====================================================
// �v���C���[�֘A�̕`��
// ====================================================
void Player::draw(std::shared_ptr<Camera> camera) {

	// �`��ʒu����
	tnl::Vector3 player_draw_pos = pos_ * GameManager::DRAW_CHIP_SIZE - camera->getPos() + tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);

	DrawExtendGraph(player_draw_pos.x, player_draw_pos.y, player_draw_pos.x + GameManager::DRAW_CHIP_SIZE, player_draw_pos.y + GameManager::DRAW_CHIP_SIZE,
		chara_gpc_hdl_[static_cast<int>(anim_dir_)][0], true);

	if (act_state_ != eActState::IDLE) return;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	// ---------------- �w�肵�Ă���Z���̕`�� ----------------
	if (tnl::Input::IsKeyDown(eKeys::KB_LSHIFT)) {
		tnl::Vector3 select_cell_draw_pos = (pos_ + DIR_POS[static_cast<int>(looking_dir_)]) * GameManager::DRAW_CHIP_SIZE
			- camera->getPos() + tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);
		if (isEnableDir(looking_dir_)) {
			DrawExtendGraph(select_cell_draw_pos.x, select_cell_draw_pos.y, select_cell_draw_pos.x + GameManager::DRAW_CHIP_SIZE, select_cell_draw_pos.y + GameManager::DRAW_CHIP_SIZE,
				select_cell_blue_gpc_hdl_, true);
		}
		else {
			DrawExtendGraph(select_cell_draw_pos.x, select_cell_draw_pos.y, select_cell_draw_pos.x + GameManager::DRAW_CHIP_SIZE, select_cell_draw_pos.y + GameManager::DRAW_CHIP_SIZE,
				select_cell_red_gpc_hdl_, true);
		}
	}
	// ---------------- �΂߈ړ��ł�������̃Z���`�� ----------------
	else if (tnl::Input::IsKeyDown(eKeys::KB_RSHIFT)) {
		for (int i = static_cast<int>(eDir_8::UP_LEFT); i < static_cast<int>(eDir_8::MAX); ++i) {
			eDir_8 dir = static_cast<eDir_8>(i);
			if (isEnableDir(dir) && !checkMapDataFromPos(pos_ + DIR_POS[static_cast<int>(dir)], eMapData::ENEMY)) {
				tnl::Vector3 select_cell_draw_pos = ( pos_ + DIR_POS[static_cast<int>(dir)]) * GameManager::DRAW_CHIP_SIZE
					- camera->getPos() + tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);
				DrawExtendGraph(select_cell_draw_pos.x, select_cell_draw_pos.y, select_cell_draw_pos.x + GameManager::DRAW_CHIP_SIZE, select_cell_draw_pos.y + GameManager::DRAW_CHIP_SIZE,
					select_cell_blue_gpc_hdl_, true);
			}
		}
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
}

// ====================================================
// �w�肵���ʒu�̃Z�����ǂ�����
// ====================================================
bool Player::checkMapDataFromPos(const tnl::Vector3& pos, eMapData map_data) {

	std::shared_ptr<ScenePlay> scene_play = std::dynamic_pointer_cast<ScenePlay>(GameManager::GetInstance()->getSceneInstance());
	if (!scene_play) return false;

	return scene_play->getMapData(pos) == map_data;
}

// ====================================================
// �ҋ@�A���͑҂��V�[�P���X
// ====================================================
bool Player::seqIdle(const float delta_time) {

	if (act_state_ != eActState::IDLE) return true;

	// ======== �����]�� ========
	if (tnl::Input::IsKeyDown(eKeys::KB_LSHIFT)) {
		if (tnl::Input::IsKeyDown(eKeys::KB_W) && tnl::Input::IsKeyDown(eKeys::KB_A)) {
			looking_dir_ = eDir_8::UP_LEFT;
			anim_dir_ = eDir_4::UP;
		}
		else if (tnl::Input::IsKeyDown(eKeys::KB_W) && tnl::Input::IsKeyDown(eKeys::KB_D)) {
			looking_dir_ = eDir_8::UP_RIGHT;
			anim_dir_ = eDir_4::UP;
		}
		else if (tnl::Input::IsKeyDown(eKeys::KB_S) && tnl::Input::IsKeyDown(eKeys::KB_A)) {
			looking_dir_ = eDir_8::DOWN_LEFT;
			anim_dir_ = eDir_4::DOWN;
		}
		else if (tnl::Input::IsKeyDown(eKeys::KB_S) && tnl::Input::IsKeyDown(eKeys::KB_D)) {
			looking_dir_ = eDir_8::DOWN_RIGHT;
			anim_dir_ = eDir_4::DOWN;
		}
		else if (tnl::Input::IsKeyDown(eKeys::KB_W)) {
			looking_dir_ = eDir_8::UP;
			anim_dir_ = eDir_4::UP;
		}
		else if (tnl::Input::IsKeyDown(eKeys::KB_S)) {
			looking_dir_ = eDir_8::DOWN;
				anim_dir_ = eDir_4::DOWN;
		}
		else if (tnl::Input::IsKeyDown(eKeys::KB_A)) {
			looking_dir_ = eDir_8::LEFT;
			anim_dir_ = eDir_4::LEFT;
		}
		else if (tnl::Input::IsKeyDown(eKeys::KB_D)) {
			looking_dir_ = eDir_8::RIGHT;
			anim_dir_ = eDir_4::RIGHT;
		}
	}
	// ======== �΂߈ړ� ========
	else if (tnl::Input::IsKeyDown(eKeys::KB_RSHIFT)) {
		// �����]��
		if (tnl::Input::IsKeyDown(eKeys::KB_W) && tnl::Input::IsKeyDown(eKeys::KB_A)) {
			if (isEnableDir(eDir_8::UP_LEFT)) {
				setNextPosInDir(eDir_8::UP_LEFT);
				act_state_ = eActState::MOVE;
				sequence_.change(&Player::seqMove);
				return true;
			}
		}
		else if (tnl::Input::IsKeyDown(eKeys::KB_W) && tnl::Input::IsKeyDown(eKeys::KB_D)) {
			if (isEnableDir(eDir_8::UP_RIGHT)) {
				setNextPosInDir(eDir_8::UP_RIGHT);
				act_state_ = eActState::MOVE;
				sequence_.change(&Player::seqMove);
				return true;
			}
		}
		else if (tnl::Input::IsKeyDown(eKeys::KB_S) && tnl::Input::IsKeyDown(eKeys::KB_A)) {
			if (isEnableDir(eDir_8::DOWN_LEFT)) {
				setNextPosInDir(eDir_8::DOWN_LEFT);
				act_state_ = eActState::MOVE;
				sequence_.change(&Player::seqMove);
				return true;
			}
		}
		else if (tnl::Input::IsKeyDown(eKeys::KB_S) && tnl::Input::IsKeyDown(eKeys::KB_D)) {
			if (isEnableDir(eDir_8::DOWN_RIGHT)) {
				setNextPosInDir(eDir_8::DOWN_RIGHT);
				act_state_ = eActState::MOVE;
				sequence_.change(&Player::seqMove);
				return true;
			}
		}
	}
	// ======== �㉺���E�ړ��@========
	else {
		if (tnl::Input::IsKeyDown(eKeys::KB_W)) {
			setNextPosInDir(eDir_8::UP);
			act_state_ = eActState::MOVE;
			sequence_.change(&Player::seqMove);
			return true;
		}
		if (tnl::Input::IsKeyDown(eKeys::KB_S)) {
			setNextPosInDir(eDir_8::DOWN);
			act_state_ = eActState::MOVE;
			sequence_.change(&Player::seqMove);
			return true;
		}
		if (tnl::Input::IsKeyDown(eKeys::KB_A)) {
			setNextPosInDir(eDir_8::LEFT);
			act_state_ = eActState::MOVE;
			sequence_.change(&Player::seqMove);
			return true;
		}
		if (tnl::Input::IsKeyDown(eKeys::KB_D)) {
			setNextPosInDir(eDir_8::RIGHT);
			act_state_ = eActState::MOVE;
			sequence_.change(&Player::seqMove);
			return true;
		}
	}

	// ======== �^�[���X�L�b�v ========
	if (tnl::Input::IsKeyDown(eKeys::KB_Z)) {
		act_state_ = eActState::MOVE;
		sequence_.change(&Player::seqMove);
		return true;
	}

	// ======== �U������ ========
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		act_state_ = eActState::ATTACK;
		sequence_.change(&Player::seqAttack);
		attack_dir_ = pos_ + DIR_POS[static_cast<int>(looking_dir_)];
	}

	return true;
}

// ====================================================
// �ړ��V�[�P���X
// ====================================================
bool Player::seqMove(const float delta_time) {

	if (abs(next_pos_.y - pos_.y) > 0.1f || abs(next_pos_.x - pos_.x) > 0.1f) {
		pos_ += (next_pos_ - pos_ ) * MOVE_SPEED;
	}
	else {
		status_.healHP(regenerating_hp_);
		pos_ = next_pos_;
		act_state_ = eActState::END;
		sequence_.change(&Player::seqIdle);
	}
	//if (abs(next_pos_.x - pos_.x) > 0.1f) {
	//	pos_.x += MOVE_SPEED;
	//}

	return true;
}

// ====================================================
// �U���V�[�P���X
// ====================================================
bool Player::seqAttack(const float delta_time) {

	// �U���������L���������ꍇ
	if (isEnableDir(looking_dir_)) {
		std::shared_ptr<ScenePlay> scene_play = std::dynamic_pointer_cast<ScenePlay>(GameManager::GetInstance()->getSceneInstance());

		if (!scene_play) return true;

		std::shared_ptr<Player> player = scene_play->getPlayer();
		std::shared_ptr<Enemy> target = scene_play->findEnemy(attack_dir_);

		if (target) scene_play->applyDamage(player, target);

		// ���x���A�b�v����
		if (status_.getLevel() < MAX_LEVEL) {
			if (status_.getExp() >= LEVEL_TABLE[status_.getLevel() - 1]) {
				status_.levelUP(4, 3, 3);
				scene_play->charaLevelUpProcess(player);
			}
		}
	}
	act_state_ = eActState::END;
	sequence_.change(&Player::seqIdle);
	
	return true;
}
