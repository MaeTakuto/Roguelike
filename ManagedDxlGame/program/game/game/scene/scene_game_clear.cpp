#include <queue>
#include "../../dxlib_ext/dxlib_ext.h"
#include "../manager/gm_manager.h"
#include "../manager/resource_manager.h"
#include "../ui/message_window.h"
#include "scene_title.h"
#include "scene_game_clear.h"

namespace {
	// �w�i�̕\���ʒu
	const tnl::Vector2i BACKGROUND_POS = { 0, 0 };

	// ���b�Z�[�W�E�B���h�E�̈ʒu
	const tnl::Vector2i	MESSAGE_WINDOW_POS = { 175, 500 };
	// ���b�Z�[�W�E�B���h�E�̃T�C�Y
	const tnl::Vector2i MESSAGE_WINDOW_SIZE = { 900, 160 };
}

// =====================================================================================
// �R���X�g���N�^
// =====================================================================================
SceneGameClear::SceneGameClear() : sequence_(tnl::Sequence<SceneGameClear>(this, &SceneGameClear::seqSceneStart)), 
	message_window_(std::make_shared<MessageWindow>()), now_page_(0), is_playing_se_(false), back_ground_gpc_hdl_(0)
{
	back_ground_gpc_hdl_ = ResourceManager::getInstance()->loadGraph("graphics/ClearScene_background.jpg");

	message_window_->setWindowPos(MESSAGE_WINDOW_POS);
	message_window_->setWindowSize(MESSAGE_WINDOW_SIZE);

	// �G���f�B���O�V�i���I��ǂݍ���
	CsvData& scenario_data = ResourceManager::getInstance()->loadCsvData("csv/ending_scenario_data.csv");

	event_messages_.resize(scenario_data.size() - 1);

	for (int y = GameManager::CSV_CELL_ROW_START; y < scenario_data.size(); ++y) {
		
		// ���b�Z�[�W�y�[�W�Ɍ��ʉ��p�X�������Ă�����ǉ�����
		if (scenario_data[y][0].getString() != "") {
			std::pair<int, std::string> se_pair = std::make_pair( y - 1, scenario_data[y][0].getString() );
			se_path_to_message_page_.push(se_pair);
		}

		event_messages_[y - 1].resize(scenario_data.size());
		
		for (int x = 0; x < scenario_data[y].size() - 1; ++x) {
			event_messages_[y - 1][x] = scenario_data[y][x + 1].getString();
		}
	}

	message_window_->setAllLineMessage(event_messages_[now_page_]);

}

// =====================================================================================
// �f�X�g���N�^
// =====================================================================================
SceneGameClear::~SceneGameClear() {

}

// =====================================================================================
// �V�[���̍X�V
// =====================================================================================
void SceneGameClear::update(float delta_time) {

	sequence_.update(delta_time);
}

// =====================================================================================
// �V�[���̕`��
// =====================================================================================
void SceneGameClear::draw() {
	
	DrawExtendGraph(BACKGROUND_POS.x, BACKGROUND_POS.y, BACKGROUND_POS.x + DXE_WINDOW_WIDTH, BACKGROUND_POS.y + DXE_WINDOW_HEIGHT, back_ground_gpc_hdl_, true);

	if (message_window_->isEnable()) {
		message_window_->draw();
	}
}

// =====================================================================================
// SE���Đ����邩�m�F
// =====================================================================================
void SceneGameClear::checkToPlaySE()
{
	if (se_path_to_message_page_.empty()) {
		return;
	}

	if (se_path_to_message_page_.front().first == now_page_) {
		ResourceManager::getInstance()->playSound(se_path_to_message_page_.front().second, DX_PLAYTYPE_BACK);
		is_playing_se_ = true;
	}

	return;
}

// =====================================================================================
// �V�[���X�^�[�g�V�[�P���X
// =====================================================================================
bool SceneGameClear::seqSceneStart(const float delta_time) {

	if (!GameManager::getInstance()->isTransition()) {
		message_window_->setEnable(true);
		sequence_.change(&SceneGameClear::seqMessageEvent);
	}

	return true;
}

// =====================================================================================
// ���b�Z�[�W�C�x���g�V�[�P���X
// =====================================================================================
bool SceneGameClear::seqMessageEvent(const float delta_time) {

	// SE���Đ����Ă��邩�m�F���A�Đ����I�������SE�p�X�� "pop" ����
	if (is_playing_se_) {
		if (!ResourceManager::getInstance()->checkSound(se_path_to_message_page_.front().second)) {
			is_playing_se_ = false;
			se_path_to_message_page_.pop();
		}
		return true;
	}

	if (now_page_ >= event_messages_.size() - 1) {
		sequence_.change(&SceneGameClear::seqSceneEnd);
		return true;
	}

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		++now_page_;
		message_window_->clearMessage();
		message_window_->setAllLineMessage(event_messages_[now_page_]);
		message_window_->setEnable(true);

		checkToPlaySE();
	}

	return true;
}

// =====================================================================================
// �V�[���I���V�[�P���X
// =====================================================================================
bool SceneGameClear::seqSceneEnd(const float delta_time) {

	if (GameManager::getInstance()->isTransition()) {
		return true;
	}

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		GameManager::getInstance()->changeScene(std::make_shared<SceneTitle>(), 2.0f);
	}

	return true;
}

