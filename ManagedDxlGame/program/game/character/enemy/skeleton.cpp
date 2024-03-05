#include "../../../dxlib_ext/dxlib_ext.h"
#include "../../manager/gm_manager.h"
#include "../../manager/resource_manager.h"
#include "../../common/camera.h"
#include "../../scene/scene_play.h"
#include "skeleton.h"

namespace
{
	const std::string SKELETON_DATA_CSV_PATH = "csv/enemy_data/skeleton_data.csv";
};

// =====================================================================================
// コンストラクタ
// =====================================================================================
Skeleton::Skeleton() {
	std::vector<std::vector<tnl::CsvCell>> gpc_hdl_data = tnl::LoadCsv("csv/enemy_gpc_data.csv");

	chara_gpc_hdl_.resize(static_cast<int>(eDir_4::MAX));

	// キャラの各方向の画像データを "chara_gpc_hdl_" に保存。
	int gpc_index = std::underlying_type<eEnemyType>::type(eEnemyType::SKELETON) + 1;

	for (int i = 1; i < gpc_hdl_data[gpc_index].size(); i++) {

		chara_gpc_hdl_[i - 1].resize(CHARA_GPC_X_NUM);

		chara_gpc_hdl_[i - 1] = ResourceManager::getInstance()->loadAnimation
		(gpc_hdl_data[gpc_index][i].getString(),
			CHARA_GPC_MAX_NUM,
			CHARA_GPC_X_NUM,
			CHARA_GPC_Y_NUM,
			GameManager::CHIP_SIZE,
			GameManager::CHIP_SIZE
		);
	}

	// ステータスデータを CSV から取得
	std::vector<std::vector<tnl::CsvCell>> status_data = tnl::LoadCsv(SKELETON_DATA_CSV_PATH);

	// 敵の各ステータスをセット
	name_ = status_data[1][0].getString();
	status_.setStatus(
		status_data[1][1].getInt(),		// レベル 
		status_data[1][2].getInt(),		// HP
		status_data[1][3].getInt(),		// ATK
		status_data[1][4].getInt(),		// DEF
		status_data[1][5].getInt()		// EXP
	);

	anim_dir_ = eDir_4::DOWN;
	act_state_ = eActState::IDLE;

	is_alive_ = true;
}

// =====================================================================================
// デストラクタ
// =====================================================================================
Skeleton::~Skeleton() {
	tnl::DebugTrace("Skeletonのデストラクタが実行されました。\n");
}

// =====================================================================================
// アップデート
// =====================================================================================
void Skeleton::update(float delta_time) {

	sequence_.update(delta_time);
}

// =====================================================================================
// 描画
// =====================================================================================
void Skeleton::draw(const std::shared_ptr<Camera> camera) {

	// 描画位置調整
	tnl::Vector3 draw_pos = tnl::Vector3(pos_.x * GameManager::DRAW_CHIP_SIZE, pos_.y * GameManager::DRAW_CHIP_SIZE, 0)
		- camera->getPos() + tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);

	DrawExtendGraph(draw_pos.x, draw_pos.y, draw_pos.x + GameManager::DRAW_CHIP_SIZE, draw_pos.y + GameManager::DRAW_CHIP_SIZE,
		chara_gpc_hdl_[static_cast<int>(anim_dir_)][0], true);
}

// =====================================================================================
// クローンを生成する
// =====================================================================================
std::shared_ptr<EnemyBase> Skeleton::createClone() const {

	std::shared_ptr<Skeleton> clone = std::make_shared<Skeleton>();
	return clone;
}

// =====================================================================================
// 敵の強さを設定する
// =====================================================================================
void Skeleton::setEnemyLevel(int lv) {

	if (lv <= 0) {
		tnl::DebugTrace("lv は 1以上の値をセットしてください. 入力値：%d\n", lv);
		return;
	}

	// ステータスデータを CSV から取得
	std::vector<std::vector<tnl::CsvCell>> status_data = tnl::LoadCsv(SKELETON_DATA_CSV_PATH);

	if (lv >= status_data.size()) {
		tnl::DebugTrace("lv の値が CSVデータのサイズを超えています. 入力値：%d\n", lv);
		return;
	}

	// 敵の各ステータスをセット
	name_ = status_data[lv][0].getString();
	status_.setStatus(
		status_data[lv][1].getInt(),		// レベル 
		status_data[lv][2].getInt(),		// HP
		status_data[lv][3].getInt(),		// ATK
		status_data[lv][4].getInt(),		// DEF
		status_data[lv][5].getInt()			// EXP
	);
}

// =====================================================================================
// 行動を決める
// =====================================================================================
void Skeleton::decideAction() {

	auto scene_play = scene_play_.lock();
	if (scene_play == nullptr) {
		return;
	}

	// 周囲 8マスにプレイヤーがいるか確認
	eDir_8 player_dir = findPlayerDir_8();
	if (player_dir != eDir_8::NONE) {

		is_find_player_ = true;

		// プレイヤーのいる方向が攻撃可能か判定
		if (canAttackInDir(player_dir)) {
			changeToAttackAction(player_dir);
			return;
		}
	}

	// プレイヤーを見つけた場合、追跡を行う
	if (is_find_player_) {
		trackingPlayer();
		return;
	}

	// 通路にいる場合、通路での行動処理を行う
	if (scene_play->getPlace(pos_) == ePlace::CORRIDOR) {
		actionInCorridor();
	}
	else {
		actionInRoom();
	}
}

// =====================================================================================
// 行動を開始する
// =====================================================================================
void Skeleton::beginAction() {

	if (!sequence_.isComparable(&Skeleton::seqIdle)) {
		tnl::DebugTrace("sequence_ がIdleになっていません\n");
		return;
	}
	if (act_state_ == eActState::MOVE) {
		sequence_.immediatelyChange(&Skeleton::seqMove);
		return;
	}
	if (act_state_ == eActState::ATTACK) {
		sequence_.immediatelyChange(&Skeleton::seqAttack);
	}
}

// =====================================================================================
// 自信をデスさせる
// =====================================================================================
void Skeleton::death() {
	is_alive_ = false;
}

// =====================================================================================
// 待機シーケンス
// =====================================================================================
bool Skeleton::seqIdle(const float delta_time) {

	return true;
}

// =====================================================================================
// next_pos_ に移動させる。
// =====================================================================================
bool Skeleton::seqMove(const float delta_time) {

	if (abs(next_pos_.y - pos_.y) > 0.1f || abs(next_pos_.x - pos_.x) > 0.1f) {
		pos_ += (next_pos_ - pos_) * MOVE_SPEED;
	}
	else {
		pos_ = next_pos_;
		act_state_ = eActState::END;
		sequence_.change(&Skeleton::seqIdle);
	}

	return true;
}

// ====================================================
// 攻撃シーケンス
// ====================================================
bool Skeleton::seqAttack(const float delta_time) {

	attack_time_ += delta_time;

	if (attack_time_ > ATTACK_TIME_MAX) {
		attack_time_ = 0.0f;
		act_state_ = eActState::END;
		sequence_.change(&Skeleton::seqIdle);
	}
	return true;
}

// =====================================================================================
// "next_pos_" を目標の位置に向かって 1マス移動する
// =====================================================================================
void Skeleton::setNextPosToTarget() {

	// セルの更新
	updateCellsCost();
	// 各セルのコストをもとに次の位置を決める
	decideNextPosFromCellCost();
	// 移動行動に切り替え
	changeToMoveAction();
}

// =====================================================================================
// プレイヤーを追跡する
// =====================================================================================
void Skeleton::trackingPlayer() {

	auto scene_play = scene_play_.lock();
	if (!scene_play) {
		tnl::DebugTrace("trackingPlayer関数が正常に動作できませんでした\n");
		return;
	}

	tnl::Vector3 player_pos = scene_play->getPlayer()->getNextPos();

	// 部屋にいる場合いて、プレイヤーが通常にいる場合
	if (scene_play->getPlace(pos_) == ePlace::ROOM && scene_play->getPlace(player_pos) == ePlace::CORRIDOR) {

		tnl::Vector3 entrance_pos = getEntrancePosToNearestPlayer(scene_play->getAreaId(pos_));

		// 部屋の入口から、プレイヤーが一定以上距離が離れていたら見失い、ランダムに入口を目指す。
		if (abs((entrance_pos - player_pos).length()) > 3.0f) {
			is_find_player_ = false;
			target_pos_ = getRandomEntranceToNowRoom();
			is_find_target_ = true;
			setNextPosToTarget();
			return;
		}
	}

	target_pos_ = scene_play->getPlayer()->getNextPos();
	setNextPosToTarget();
}

// =====================================================================================
// 各セルの移動コスト、有効なセルか更新を行う
// =====================================================================================
void Skeleton::updateCellsCost() {

	auto scene_play = scene_play_.lock();
	if (!scene_play) {
		tnl::DebugTrace("updateCellCost関数が正常に動作できませんでした\n");
		return;
	}

	// 各セルの位置、移動コストを計算し、その位置が移動できるか判定をセットする
	for (int i = 0; i < static_cast<int>(eDir_8::MAX); ++i) {
		cells_[i].pos_ = tnl::Vector2i(pos_.x + DIR_POS[i].x, pos_.y + DIR_POS[i].y);
		cells_[i].cost_ = abs(target_pos_.x - cells_[i].pos_.x) + abs(target_pos_.y - cells_[i].pos_.y);
		cells_[i].is_enable_ = false;

		// 斜め方向を確認するとき
		if (i >= static_cast<int>(eDir_8::UP_LEFT)) {
			if (scene_play->getMapData(tnl::Vector3(cells_[i].pos_.x, cells_[i].pos_.y, 0)) != eMapData::GROUND) continue;;
			if (!canActionToCell(tnl::Vector2i((pos_.x + DIR_POS[i].x), pos_.y))) continue;
			if (!canActionToCell(tnl::Vector2i(pos_.x, (pos_.y + DIR_POS[i].y)))) continue;
			cells_[i].is_enable_ = true;
			continue;
		}

		if (scene_play->getMapData(tnl::Vector3(cells_[i].pos_.x, cells_[i].pos_.y, 0)) == eMapData::GROUND) {
			cells_[i].is_enable_ = true;
		}
	}
}

// =====================================================================================
// 各セルの移動コストから次の位置を決める
// =====================================================================================
void Skeleton::decideNextPosFromCellCost() {

	int index = getMinimunScoreCellIndex();

	// 有効なセルが存在していた場合
	if (index != -1) {
		next_pos_ = tnl::Vector3(cells_[index].pos_.x, cells_[index].pos_.y, 0);
		anim_dir_ = ANIM_DIR[index];
	}
	else {
		next_pos_ = pos_;
	}
}

// =====================================================================================
// 指定した方向のセルが行動できるか判定
// =====================================================================================
bool Skeleton::canActionToCell(const tnl::Vector2i& pos) {

	auto scene_play = scene_play_.lock();
	if (!scene_play) {
		tnl::DebugTrace("canActionToCell関数が正常に動作できませんでした\n");
		return false;
	}

	if (!isInField(tnl::Vector3(pos.x, pos.y, 0))) {
		return false;
	}
	if (scene_play->getMapData(tnl::Vector3(pos.x, pos.y, 0)) == eMapData::WALL) {
		return false;
	}
	return true;
}

// =====================================================================================
// 指定した方向が攻撃可能か
// =====================================================================================
bool Skeleton::canAttackInDir(eDir_8 dir) {

	auto scene_play = scene_play_.lock();
	if (!scene_play) {
		tnl::DebugTrace("canAttackInDir関数が正常に動作できませんでした\n");
		return false;
	}

	// dir が斜め方向の場合 
	if (dir == eDir_8::UP_LEFT) {
		if (scene_play->getMapData(pos_ + DIR_POS[static_cast<int>(eDir_8::UP)]) == eMapData::WALL) return false;
		if (scene_play->getMapData(pos_ + DIR_POS[static_cast<int>(eDir_8::LEFT)]) == eMapData::WALL) return false;
	}
	else if (dir == eDir_8::UP_RIGHT) {
		if (scene_play->getMapData(pos_ + DIR_POS[static_cast<int>(eDir_8::UP)]) == eMapData::WALL) return false;
		if (scene_play->getMapData(pos_ + DIR_POS[static_cast<int>(eDir_8::RIGHT)]) == eMapData::WALL) return false;
	}
	else if (dir == eDir_8::DOWN_LEFT) {
		if (scene_play->getMapData(pos_ + DIR_POS[static_cast<int>(eDir_8::DOWN)]) == eMapData::WALL) return false;
		if (scene_play->getMapData(pos_ + DIR_POS[static_cast<int>(eDir_8::LEFT)]) == eMapData::WALL) return false;
	}
	else if (dir == eDir_8::DOWN_RIGHT) {
		if (scene_play->getMapData(pos_ + DIR_POS[static_cast<int>(eDir_8::DOWN)]) == eMapData::WALL) return false;
		if (scene_play->getMapData(pos_ + DIR_POS[static_cast<int>(eDir_8::RIGHT)]) == eMapData::WALL) return false;
	}
	return true;
}

// =====================================================================================
// 通路での行動
// =====================================================================================
void Skeleton::actionInCorridor() {

	// 行動エラーのカウント回数が3回以上の場合、来た道を戻る
	if (action_error_ >= ACTION_ERROR_MAX) {
		eDir_8 reverce_dir = REVERCE_DIRECTION[std::underlying_type<eDir_4>::type(anim_dir_)];
		anim_dir_ = ANIM_DIR[std::underlying_type<eDir_8>::type(reverce_dir)];
		action_error_ = 0;
	}

	// 地面がある方向を取得する
	std::vector<eDir_4> directions = getMapDataDir_4(eMapData::GROUND);

	// 来た方向の要素だけ削除する
	eDir_8 reverce_dir = REVERCE_DIRECTION[std::underlying_type<eDir_4>::type(anim_dir_)];
	auto it = std::find(directions.begin(), directions.end(), ANIM_DIR[std::underlying_type<eDir_8>::type(reverce_dir)]);
	if (it != directions.end()) directions.erase(it);

	// 移動できる方向がない場合、行動エラーをカウントする
	if (directions.size() == 0) {
		action_error_++;
	}
	// 移動できる方向が 1つの場合
	else if (directions.size() == 1) {
		next_pos_ = pos_ + DIR_POS[std::underlying_type<eDir_4>::type(directions[0])];
		anim_dir_ = directions[0];
	}
	// 移動できる方向が複数ある場合、ランダムに選ぶ
	else {
		int index = rand() % directions.size();
		next_pos_ = pos_ + DIR_POS[std::underlying_type<eDir_4>::type(directions[index])];
		anim_dir_ = directions[index];
	}

	// 移動行動に切り替え
	changeToMoveAction();
}

// =====================================================================================
// 部屋での行動
// =====================================================================================
void Skeleton::actionInRoom() {

	auto scene_play = scene_play_.lock();
	if (!scene_play) {
		tnl::DebugTrace("actionInRoom関数が正常に動作できませんでした\n");
		return;
	}

	// プレイヤーが同じ部屋いた場合、プレイヤーを追跡する
	if (isSameRoomToPlayer()) {
		is_find_player_ = true;
		target_pos_ = scene_play->getPlayer()->getNextPos();
		setNextPosToTarget();
		return;
	}

	// 目的地を見つけていない場合、入口を探す
	if (!is_find_target_) {
		target_pos_ = getRandomEntranceToNowRoom();
		is_find_target_ = true;
	}

	// 目標位置に向かう
	setNextPosToTarget();

	// 通路の入口に着いたら、targetフラグを切る
	if ((next_pos_ - target_pos_).length() < FLT_EPSILON) {
		is_find_target_ = false;
	}
}