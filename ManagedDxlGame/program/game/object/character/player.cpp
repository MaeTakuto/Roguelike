#include "../../../dxlib_ext/dxlib_ext.h"
#include "../../manager/gm_manager.h"
#include "../../common/camera.h"
#include "../../manager/resource_manager.h"
#include "../../scene/scene_play.h"
#include "../../base/enemy_base.h"
#include "../../magic/heal_magic.h"
#include "../../magic/fire_magic.h"
#include "../../magic/magnetism_magic.h"
#include "../../common/animation.h"
#include "../projectile.h"
#include "player.h"


namespace {
	// 最大レベル
	const int MAX_LEVEL = 6;
	// 経験値テーブル
	const int LEVEL_TABLE[MAX_LEVEL - 1] = { 12, 50, 100, 180, 280 };
}


Player::Player() : regenerating_mp_(1), sequence_(tnl::Sequence<Player>(this, &Player::seqIdle)), magic_chanting_effect_(nullptr), is_use_magic_(false), use_magic_index_(0),
	select_cell_blue_gpc_hdl_(0), select_cell_red_gpc_hdl_(0)
{

	// リソースマネージャーのインスタンスを取得
	auto rm_instance = ResourceManager::getInstance();

	// ------------------- プレイヤー画像を設定 --------------------------------------------------------------------
	CsvData& gpc_hdl_data = rm_instance->loadCsvData("csv/player_gpc_data.csv");

	chara_gpc_hdl_.resize(gpc_hdl_data[GameManager::CSV_CELL_ROW_START].size() - GameManager::CSV_CELL_ROW_START);

	for (int i = GameManager::CSV_CELL_ROW_START; i < gpc_hdl_data[GameManager::CSV_CELL_ROW_START].size(); i++) {
		
		chara_gpc_hdl_[i - 1].resize(CHARA_GPC_X_NUM);

		chara_gpc_hdl_[i - 1] = rm_instance->loadAnimation
		(gpc_hdl_data[GameManager::CSV_CELL_ROW_START][i].getString(),			// 画像パス
			CHARA_GPC_MAX_NUM,													// 切り取る数
			CHARA_GPC_X_NUM,													// x方向に切り取る数
			CHARA_GPC_Y_NUM,													// y方向に切り取る数
			GameManager::CHIP_SIZE,												// x方向に 1フレームごとの切り取る画像サイズ
			GameManager::CHIP_SIZE												// y方向に 1フレームごとの切り取る画像サイズ
		);
	}

	// ------------------- プレイヤーの攻撃方向のセル画像を設定 ---------------------------------------------------
	select_cell_blue_gpc_hdl_ = rm_instance->loadGraph("graphics/blue.bmp");
	select_cell_red_gpc_hdl_ = rm_instance->loadGraph("graphics/red.bmp");

	// ------------------- エフェクトの設定 -------------------------------------------------------------------
	CsvData& effect_data = rm_instance->loadCsvData("csv/effect_gpc_data.csv");

	std::vector<tnl::CsvCell> start_cell = effect_data[GameManager::CSV_CELL_ROW_START + 1];

	magic_chanting_effect_ = std::make_shared<Animation>();

	magic_chanting_effect_->setAnimGraphicHandle(
		rm_instance->loadAnimation(
			start_cell[1].getString(),								// 画像パス,
			start_cell[2].getInt() * start_cell[3].getInt(),		// フレーム総数
			start_cell[2].getInt(),									// 横方向の分割数
			start_cell[3].getInt(),									// 縦方向の分割数
			start_cell[4].getInt(),									// 横方向の分割サイズ
			start_cell[5].getInt()									// 縦方向の分割サイズ
		)
	);

	magic_chanting_effect_->setBlendMode(DX_BLENDMODE_ADD);
	magic_chanting_effect_->setFrameChangeInterval(0.05f);

	// ------------------- エフェクトの設定 -------------------------------------------------------------------
	start_cell = effect_data[GameManager::CSV_CELL_ROW_START + 2];

	atk_effect_ = std::make_shared<Animation>();

	atk_effect_->setAnimGraphicHandle(
		rm_instance->loadAnimation(
			start_cell[1].getString(),								// 画像パス,
			start_cell[2].getInt() * start_cell[3].getInt(),		// フレーム総数
			start_cell[2].getInt(),									// 横方向の分割数
			start_cell[3].getInt(),									// 縦方向の分割数
			start_cell[4].getInt(),									// 横方向の分割サイズ
			start_cell[5].getInt()									// 縦方向の分割サイズ
		)
	);

	atk_effect_->setFrameChangeInterval(0.025f);

	// ------------------- ステータスセット -----------------------------------------------------------------------
	CsvData& status_data = rm_instance->loadCsvData("csv/player_status_data.csv");

	name_ = status_data[GameManager::CSV_CELL_ROW_START][1].getString();
	status_.setStatus(
		status_data[GameManager::CSV_CELL_ROW_START][2].getInt(),		// レベル
		status_data[GameManager::CSV_CELL_ROW_START][3].getInt(),		// HP
		status_data[GameManager::CSV_CELL_ROW_START][4].getInt(),		// MP
		status_data[GameManager::CSV_CELL_ROW_START][5].getInt(),		// ATK
		status_data[GameManager::CSV_CELL_ROW_START][6].getInt(),		// DEF
		status_data[GameManager::CSV_CELL_ROW_START][7].getInt()		// EXP
	);

	magic_list_.emplace_back(std::make_shared<HealMagic>());
	magic_list_.emplace_back(std::make_shared<FireMagic>());
	magic_list_.emplace_back(std::make_shared<MagnetismMagic>());

	looking_dir_ = eDir_8::DOWN;
	anim_dir_ = eDir_4::DOWN;
	act_state_ = eActState::IDLE;

}

Player::~Player() {
	tnl::DebugTrace("Playerのデストラクタが実行されました\n");
}

void Player::update(float delta_time) {

	sequence_.update(delta_time);
	magic_chanting_effect_->update(delta_time);
	atk_effect_->update(delta_time);
	
	if (is_use_magic_) {
		magic_list_[use_magic_index_]->update(delta_time);
	}

}

// ====================================================
// プレイヤー関連の描画
// ====================================================
void Player::draw(std::shared_ptr<Camera> camera) {

	// 描画位置調整
	tnl::Vector3 player_draw_pos = pos_ * GameManager::DRAW_CHIP_SIZE - camera->getPos() + tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);

	DrawExtendGraph(player_draw_pos.x, player_draw_pos.y, player_draw_pos.x + GameManager::DRAW_CHIP_SIZE, player_draw_pos.y + GameManager::DRAW_CHIP_SIZE,
		chara_gpc_hdl_[static_cast<int>(anim_dir_)][0], true);

	if (act_state_ != eActState::IDLE) {
		return;
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	// ---------------- 指定しているセルの描画 ----------------
	if (tnl::Input::IsKeyDown(eKeys::KB_LSHIFT)) {
		
		tnl::Vector3 select_cell_draw_pos = (pos_ + DIR_POS[static_cast<int>(looking_dir_)]) * GameManager::DRAW_CHIP_SIZE
			- camera->getPos() + tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);
		
		if (canActionToCell(looking_dir_)) {
			
			DrawExtendGraph(select_cell_draw_pos.x, select_cell_draw_pos.y, select_cell_draw_pos.x + GameManager::DRAW_CHIP_SIZE, select_cell_draw_pos.y + GameManager::DRAW_CHIP_SIZE,
				select_cell_blue_gpc_hdl_, true);
		}
		else {
			
			DrawExtendGraph(select_cell_draw_pos.x, select_cell_draw_pos.y, select_cell_draw_pos.x + GameManager::DRAW_CHIP_SIZE, select_cell_draw_pos.y + GameManager::DRAW_CHIP_SIZE,
				select_cell_red_gpc_hdl_, true);
		}
	}
	// ---------------- 斜め移動できる方向のセル描画 ----------------
	else if (tnl::Input::IsKeyDown(eKeys::KB_RSHIFT)) {
		for (int i = static_cast<int>(eDir_8::UP_LEFT); i < static_cast<int>(eDir_8::MAX); ++i) {
			eDir_8 dir = static_cast<eDir_8>(i);
			if (canActionToCell(dir) && !checkMapDataFromPos(pos_ + DIR_POS[static_cast<int>(dir)], eMapData::ENEMY)) {
				
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
// プレイヤー関連の描画
// ====================================================
void Player::drawEffect(std::shared_ptr<Camera> camera) {

	// ------------------ 魔法詠唱エフェクト --------------
	if (magic_chanting_effect_->isEnable()) {
		magic_chanting_effect_->draw(camera);
	}

	// ------------------ 攻撃エフェクト ------------------
	if (atk_effect_->isEnable()) {
		atk_effect_->draw(camera);
	}
	if (is_use_magic_) {
		magic_list_[use_magic_index_]->draw(camera);
	}
}

// ====================================================
// 行動を開始する
// ====================================================
void Player::beginAction() {
	act_state_ = eActState::IDLE;
}

// ====================================================
// 指定した目標位置にキャラクターを移動させる
// ====================================================
void Player::moveToTargetPos(const tnl::Vector3& target_pos) {

	std::shared_ptr<ScenePlay> scene_play = std::dynamic_pointer_cast<ScenePlay>(GameManager::GetInstance()->getSceneInstance());
	if (!scene_play) {
		return;
	}
	next_pos_ = target_pos;
	act_state_ = eActState::MOVE;
	sequence_.change(&Player::seqMoveToTarget);
	scene_play->setMapData(pos_, scene_play->getTerrainData(pos_));
	scene_play->setMapData(next_pos_, eMapData::PLAYER);
}

// ====================================================
// レベルアップできるか判定
// ====================================================
bool Player::canLevelUp() {

	// 最大レベルか判定
	if (status_.getLevel() >= MAX_LEVEL) {
		return false;
	}
	// 経験値がレベルアップの経験値より小さいか判定
	if (status_.getExp() < LEVEL_TABLE[status_.getLevel() - 1]) {
		return false;
	}
	return true;
}

// ====================================================
// 攻撃開始
// ====================================================
void Player::startAttack() {
	tnl::Vector3 attack_pos = pos_ + DIR_POS[std::underlying_type<eDir_8>::type(looking_dir_)];

	tnl::Vector3 effect_draw_pos = attack_pos * GameManager::DRAW_CHIP_SIZE
		+ tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);

	tnl::Vector3 effect_draw_size = tnl::Vector3(GameManager::DRAW_CHIP_SIZE, GameManager::DRAW_CHIP_SIZE, 0);

	atk_effect_->setDrawPos(tnl::Vector2i(static_cast<int>(effect_draw_pos.x), static_cast<int>(effect_draw_pos.y)));
	atk_effect_->setDrawSize(tnl::Vector2i(static_cast<int>(effect_draw_size.x), static_cast<int>(effect_draw_size.y)));

	sequence_.change(&Player::seqAttack);
}

// ====================================================
// 使用する魔法の準備
// ====================================================
void Player::setupMagic() {
	std::shared_ptr<ScenePlay> scene_play = std::dynamic_pointer_cast<ScenePlay>(GameManager::GetInstance()->getSceneInstance());

	if (!scene_play) {
		return;
	}

	magic_list_[use_magic_index_]->setupToUseMagic(scene_play->getPlayer());
}

// ====================================================
// 魔法使用開始
// ====================================================
void Player::startMagic() {

	std::shared_ptr<ScenePlay> scene_play = std::dynamic_pointer_cast<ScenePlay>(GameManager::GetInstance()->getSceneInstance());
	if (!scene_play) {
		return;
	}

	tnl::Vector2i effect_draw_pos = tnl::Vector2i(static_cast<int>(pos_.x), static_cast<int>(pos_.y)) * GameManager::DRAW_CHIP_SIZE
		+ tnl::Vector2i(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1);

	tnl::Vector2i effect_draw_size = tnl::Vector2i(GameManager::DRAW_CHIP_SIZE, GameManager::DRAW_CHIP_SIZE);

	std::string message = magic_list_[use_magic_index_]->getMagicName() + "の魔法を唱えた。";
	scene_play->setMessage(message);

	magic_chanting_effect_->setDrawPos(effect_draw_pos);
	magic_chanting_effect_->setDrawSize(effect_draw_size);
	magic_chanting_effect_->startAnimation();
	ResourceManager::getInstance()->playSound("sound/springin/magic_chanting.mp3", DX_PLAYTYPE_BACK);

	sequence_.change(&Player::seqMagicChanting);
}

// ====================================================
// レベルアップできるか判定
// ====================================================
void Player::startLevelUp() {
	status_.levelUP(4, 3, 3);
	sequence_.change(&Player::seqLevellUp);
	act_state_ = eActState::LEVEL_UP;
}

// ====================================================
// 魔法が使用できるか試す
// ====================================================
bool Player::tryUseMagic(int magic_index) {
	if (magic_index < 0 || magic_index >= magic_list_.size()) {
		return false;
	}
	// MPを確認する
	if ( !status_.tryConsumeMP(magic_list_[magic_index]->getConsumedMP() ) ) {
		return false;
	}
	
	use_magic_index_ = magic_index;
	act_state_ = eActState::USE_MAGIC;
	return true;
}

// ====================================================
// 指定した位置のセルが壁か判定
// ====================================================
bool Player::checkMapDataFromPos(const tnl::Vector3& pos, eMapData map_data) {

	std::shared_ptr<ScenePlay> scene_play = std::dynamic_pointer_cast<ScenePlay>(GameManager::GetInstance()->getSceneInstance());
	if (!scene_play) {
		return false;
	}

	return scene_play->getMapData(pos) == map_data;
}

// ====================================================
// 指定した方向の next_pos_、looking_dir_、dir_をセット
// ====================================================
void Player::setNextPosInDir(eDir_8 dir) {

	int index = static_cast<int>(dir);
	if (index < 0 || index >= static_cast<int>(eDir_8::MAX)) {
		return;
	}

	anim_dir_ = ANIM_DIR[index];
	looking_dir_ = dir;
	next_pos_ = pos_ + DIR_POS[index];
}

// ====================================================
// 指定した方向のセルが行動できるか判定
// ====================================================
bool Player::canActionToCell(eDir_8 dir) {

	if (dir == eDir_8::UP_LEFT) {
		if (checkMapDataFromPos(pos_ + DIR_POS[static_cast<int>(dir)], eMapData::WALL)) return false;
		if (checkMapDataFromPos(pos_ + DIR_POS[static_cast<int>(eDir_8::UP)], eMapData::WALL)) return false;
		if (checkMapDataFromPos(pos_ + DIR_POS[static_cast<int>(eDir_8::LEFT)], eMapData::WALL)) return false;
		return true;
	}
	else if (dir == eDir_8::UP_RIGHT) {
		if (checkMapDataFromPos(pos_ + DIR_POS[static_cast<int>(dir)], eMapData::WALL)) return false;
		if (checkMapDataFromPos(pos_ + DIR_POS[static_cast<int>(eDir_8::UP)], eMapData::WALL)) return false;
		if (checkMapDataFromPos(pos_ + DIR_POS[static_cast<int>(eDir_8::RIGHT)], eMapData::WALL)) return false;
		return true;
	}
	else if (dir == eDir_8::DOWN_LEFT) {
		if (checkMapDataFromPos(pos_ + DIR_POS[static_cast<int>(dir)], eMapData::WALL)) return false;
		if (checkMapDataFromPos(pos_ + DIR_POS[static_cast<int>(eDir_8::DOWN)], eMapData::WALL)) return false;
		if (checkMapDataFromPos(pos_ + DIR_POS[static_cast<int>(eDir_8::LEFT)], eMapData::WALL)) return false;
		return true;
	}
	else if (dir == eDir_8::DOWN_RIGHT) {
		if (checkMapDataFromPos(pos_ + DIR_POS[static_cast<int>(dir)], eMapData::WALL)) return false;
		if (checkMapDataFromPos(pos_ + DIR_POS[static_cast<int>(eDir_8::DOWN)], eMapData::WALL)) return false;
		if (checkMapDataFromPos(pos_ + DIR_POS[static_cast<int>(eDir_8::RIGHT)], eMapData::WALL)) return false;
		return true;
	}
	return !checkMapDataFromPos(pos_ + DIR_POS[static_cast<int>(dir)], eMapData::WALL);
}

// ====================================================
// 待機、入力待ちシーケンス
// ====================================================
bool Player::seqIdle(const float delta_time) {

	if (act_state_ != eActState::IDLE) return true;

	// ======== 方向転換 ========
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
	// ======== 斜め移動 ========
	else if (tnl::Input::IsKeyDown(eKeys::KB_RSHIFT)) {
		// 方向転換
		if (tnl::Input::IsKeyDown(eKeys::KB_W) && tnl::Input::IsKeyDown(eKeys::KB_A)) {
			if (canActionToCell(eDir_8::UP_LEFT)) {
				setNextPosInDir(eDir_8::UP_LEFT);
				act_state_ = eActState::MOVE;
				sequence_.change(&Player::seqMove);
				return true;
			}
		}
		else if (tnl::Input::IsKeyDown(eKeys::KB_W) && tnl::Input::IsKeyDown(eKeys::KB_D)) {
			if (canActionToCell(eDir_8::UP_RIGHT)) {
				setNextPosInDir(eDir_8::UP_RIGHT);
				act_state_ = eActState::MOVE;
				sequence_.change(&Player::seqMove);
				return true;
			}
		}
		else if (tnl::Input::IsKeyDown(eKeys::KB_S) && tnl::Input::IsKeyDown(eKeys::KB_A)) {
			if (canActionToCell(eDir_8::DOWN_LEFT)) {
				setNextPosInDir(eDir_8::DOWN_LEFT);
				act_state_ = eActState::MOVE;
				sequence_.change(&Player::seqMove);
				return true;
			}
		}
		else if (tnl::Input::IsKeyDown(eKeys::KB_S) && tnl::Input::IsKeyDown(eKeys::KB_D)) {
			if (canActionToCell(eDir_8::DOWN_RIGHT)) {
				setNextPosInDir(eDir_8::DOWN_RIGHT);
				act_state_ = eActState::MOVE;
				sequence_.change(&Player::seqMove);
				return true;
			}
		}
	}
	// ======== 上下左右移動　========
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

	// ======== ターンスキップ ========
	if (tnl::Input::IsKeyDown(eKeys::KB_Z)) {
		act_state_ = eActState::MOVE;
		sequence_.change(&Player::seqMove);
		return true;
	}

	// ======== 攻撃入力 ========
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		std::shared_ptr<ScenePlay> scene_play = std::dynamic_pointer_cast<ScenePlay>(GameManager::GetInstance()->getSceneInstance());
		if (!scene_play) {
			return true;
		}
		scene_play->addAttacker(scene_play->getPlayer());
		act_state_ = eActState::ATTACK;
	}

	return true;
}

// ====================================================
// 移動シーケンス
// ====================================================
bool Player::seqMove(const float delta_time) {

	if (abs(next_pos_.y - pos_.y) > 0.1f || abs(next_pos_.x - pos_.x) > 0.1f) {
		pos_ += (next_pos_ - pos_ ) * MOVE_SPEED;
	}
	else {
		status_.recoveryMP(regenerating_mp_);
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
// 目標に移動するシーケンス 
// seqMove関数を修正する時間がなかったために用意した関数なので、あまり良くない実装です。
// ====================================================
bool Player::seqMoveToTarget(const float delta_time) {

	if (abs(next_pos_.y - pos_.y) > 0.1f || abs(next_pos_.x - pos_.x) > 0.1f) {
		pos_ += DIR_POS[std::underlying_type<eDir_8>::type(looking_dir_)] * 10.0f * delta_time;
	}
	else {
		pos_ = next_pos_;
		act_state_ = eActState::END;
		sequence_.change(&Player::seqIdle);
	}

	return true;
}

// ====================================================
// 攻撃シーケンス
// ====================================================
bool Player::seqAttack(const float delta_time) {
	if (sequence_.isStart()) {
		atk_effect_->startAnimation();
		ResourceManager::getInstance()->playSound("sound/springin/fire.mp3", DX_PLAYTYPE_BACK);
	}

	// 攻撃エフェクト再生中の場合はここまで
	if (atk_effect_->isEnable()) {
		return true;
	}

	// 攻撃方向が有効だった場合
	if (canActionToCell(looking_dir_)) {

		std::shared_ptr<ScenePlay> scene_play = std::dynamic_pointer_cast<ScenePlay>(GameManager::GetInstance()->getSceneInstance());

		if (!scene_play) {
			return true;
		}

		tnl::Vector3 attack_pos = pos_ + DIR_POS[std::underlying_type<eDir_8>::type(looking_dir_)];
		scene_play->addAttackTarget(scene_play->findEnemy(attack_pos));

	}

	act_state_ = eActState::END;
	sequence_.change(&Player::seqIdle);
	return true;
}

// ====================================================
// 魔法詠唱シーケンス
// ====================================================
bool Player::seqMagicChanting(const float delta_time) {
	
	if (magic_chanting_effect_->isEnable()) {
		return true;
	}

	magic_list_[use_magic_index_]->startDrawEffect();
	is_use_magic_ = true;
	sequence_.change(&Player::seqUseMagic);
	return true;
}

// ====================================================
// 魔法使用シーケンス
// ====================================================
bool Player::seqUseMagic(const float delta_time) {

	std::shared_ptr<ScenePlay> scene_play = std::dynamic_pointer_cast<ScenePlay>(GameManager::GetInstance()->getSceneInstance());

	if (!scene_play) {
		return true;
	}

	if (magic_list_[use_magic_index_]->isDrawEffect()) {
		return true;
	}

	act_state_ = eActState::END;
	is_use_magic_ = false;
	sequence_.change(&Player::seqIdle);
	magic_list_[use_magic_index_]->useMagic(scene_play->getPlayer());
	return true;
}

// ====================================================
// レベルアップシーケンス
// レベルアップしたときのエフェクトを表示する。
// 現在は、用意していないので一定時間、待機。
// 今後実装、予定です。
// ====================================================
bool Player::seqLevellUp(const float delta_time) {

	/* ToDo:レベルアップしたときのエフェクトを今後出そうと思います。*/

	if (sequence_.getProgressTime() > SEQUENCE_WAIT_TIME) {
		act_state_ = eActState::END;
		sequence_.change(&Player::seqIdle);
		return true;
	}

	return true;
}