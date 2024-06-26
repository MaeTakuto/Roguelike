#include "../dxlib_ext/dxlib_ext.h"
#include "../ui/message_window.h"
#include "../my_library/my_library.h"
#include  "dungeon_log.h"

namespace {
	// �_���W�����L�^�̃��b�Z�[�W�s
	const int LOG_MESSAGE_LINE = 9;

	// ��
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

void DungeonLog::setWindowPos(const tnl::Vector2i& pos) {
	dungeon_log_window_->setWindowPos(pos);
}

void DungeonLog::setDrawing(bool is_drawing) {
	is_drawing_ = is_drawing;
}

void DungeonLog::setEndStatus(const CharaStatus& status) {
	end_status_ = status;
}

// �_���W�����L�^�̃f�[�^�X�V
void DungeonLog::updateDungeonLogData() {

	// �����X�R�A���v�Z
	calculateOverallScore();

	log_messages_[0] = "�E�`���̌���";
	log_messages_[1] = "";
	log_messages_[2] = "���B�t���A�@�@�@�F";
	log_messages_[3] = "���x���@�@�@�@�@�F" + mtl::rightAlign(std::to_string(end_status_.getLevel()), LOG_MESSAGE_SPACE );
	log_messages_[4] = "HP�@�@�@�@�@�@�@�F" + mtl::rightAlign(std::to_string(end_status_.getMaxHP()), LOG_MESSAGE_SPACE );
	log_messages_[5] = "�G���ސ��@�@�@�@�F" + mtl::rightAlign(std::to_string(repelling_enemy_count_) + "��", LOG_MESSAGE_SPACE);
	log_messages_[6] = "";
	log_messages_[7] = "�E�I�������b�Z�[�W";
	log_messages_[8] = end_message_;

	std::string message = is_dungeon_clear_ ? "�Ő[��" : std::to_string(end_floor_) + "F";

	log_messages_[2] += mtl::rightAlign(message, LOG_MESSAGE_SPACE);

	dungeon_log_window_->clearMessage();
	dungeon_log_window_->setEnable(true);

	for (int i = 0; i < log_messages_.size(); ++i) {
		dungeon_log_window_->setMessgae(log_messages_[i]);
	}

}

// �����X�R�A���v�Z����
void DungeonLog::calculateOverallScore() {

	overall_score_ += is_dungeon_clear_ ? 2000 : 0;
	overall_score_ += end_floor_ * 2000;
	overall_score_ += end_status_.getLevel() * 150;
	overall_score_ += repelling_enemy_count_ * 10;
}

