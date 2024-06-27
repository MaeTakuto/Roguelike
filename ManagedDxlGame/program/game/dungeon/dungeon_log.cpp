#include "../dxlib_ext/dxlib_ext.h"
#include "../ui/message_window.h"
#include "../my_library/my_library.h"
#include  "dungeon_log.h"

namespace {
	// ダンジョン記録のメッセージ行
	const int LOG_MESSAGE_LINE = 11;

	// 空白
	const int LOG_MESSAGE_SPACE = 18;
}

DungeonLog::DungeonLog() : dungeon_log_window_(std::make_shared<MessageWindow>()), is_drawing_(false), is_dungeon_clear_(false),
	end_floor_(0), end_status_(CharaStatus()), repelling_enemy_count_(0), end_message_(""), overall_score_(0)
{
	dungeon_log_window_->setWindowPos(tnl::Vector2i(340, 150));
	dungeon_log_window_->setMessageLine(LOG_MESSAGE_LINE);
	dungeon_log_window_->setWindowSize(tnl::Vector2i(600, 40 * LOG_MESSAGE_LINE));

	log_messages_.resize(LOG_MESSAGE_LINE);

	updateDungeonLogData();

}

DungeonLog::~DungeonLog() {

}

void DungeonLog::draw() {

	if (!is_drawing_) {
		return;
	}

	dungeon_log_window_->draw();

}

const tnl::Vector2i& DungeonLog::getWindowPos() const {
	return dungeon_log_window_->getWindowPos();
}

const tnl::Vector2i& DungeonLog::getWindowSize() const {
	return dungeon_log_window_->getWindowSize();
}

void DungeonLog::setWindowPos(const tnl::Vector2i& pos) {
	dungeon_log_window_->setWindowPos(pos);
}

void DungeonLog::setDrawing(bool is_drawing) {
	is_drawing_ = is_drawing;
}

void DungeonLog::setEndStatus(const CharaStatus& status) {
	end_status_ = status;
}

// ダンジョン記録のデータ更新
void DungeonLog::updateDungeonLogData() {

	// 総合スコアを計算
	calculateOverallScore();

	log_messages_[0] = "・冒険の結果";
	log_messages_[1] = "";
	log_messages_[2] = "総合スコア　　　：" + mtl::rightAlign(std::to_string(overall_score_), LOG_MESSAGE_SPACE);
	log_messages_[3] = "到達フロア　　　：";
	log_messages_[4] = "レベル　　　　　：" + mtl::rightAlign(std::to_string(end_status_.getLevel()), LOG_MESSAGE_SPACE );
	log_messages_[5] = "HP　　　　　　　：" + mtl::rightAlign(std::to_string(end_status_.getMaxHP()), LOG_MESSAGE_SPACE );
	log_messages_[6] = "MP　　　　　　　：" + mtl::rightAlign(std::to_string(end_status_.getMaxMP()), LOG_MESSAGE_SPACE );
	log_messages_[7] = "敵撃退数　　　　：" + mtl::rightAlign(std::to_string(repelling_enemy_count_) + "体", LOG_MESSAGE_SPACE);
	log_messages_[8] = "";
	log_messages_[9] = "・終了時メッセージ";
	log_messages_[10] = end_message_;

	std::string message = is_dungeon_clear_ ? "最深部" : std::to_string(end_floor_) + "F";

	log_messages_[3] += mtl::rightAlign(message, LOG_MESSAGE_SPACE);

	dungeon_log_window_->clearMessage();
	dungeon_log_window_->setEnable(true);

	for (int i = 0; i < log_messages_.size(); ++i) {
		dungeon_log_window_->setMessgae(log_messages_[i]);
	}

}

// 総合スコアを計算する
void DungeonLog::calculateOverallScore() {

	overall_score_ += is_dungeon_clear_ ? 2000 : 0;
	overall_score_ += end_floor_ * 2000;
	overall_score_ += end_status_.getLevel() * 150;
	overall_score_ += repelling_enemy_count_ * 10;
}

