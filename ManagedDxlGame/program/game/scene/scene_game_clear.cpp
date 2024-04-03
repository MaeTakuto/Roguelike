#include "../../dxlib_ext/dxlib_ext.h"
#include "../manager/gm_manager.h"
#include "../manager/resource_manager.h"
#include "../ui/message_window.h"
#include "scene_title.h"
#include "scene_game_clear.h"

namespace {
	// 背景の表示位置
	const tnl::Vector2i BACKGROUND_POS = { 0, 0 };

	// メッセージウィンドウの位置
	const tnl::Vector2i	MESSAGE_WINDOW_POS = { 175, 500 };
	// メッセージウィンドウのサイズ
	const tnl::Vector2i MESSAGE_WINDOW_SIZE = { 900, 200 };
}

SceneGameClear::SceneGameClear() : sequence_(tnl::Sequence<SceneGameClear>(this, &SceneGameClear::seqSceneTransition)), 
	message_window_(std::make_shared<MessageWindow>()), now_page_(0), back_ground_gpc_hdl_(0)
{
	back_ground_gpc_hdl_ = ResourceManager::getInstance()->loadGraph("graphics/ClearScene_background.jpg");

	message_window_->setWindowPos(MESSAGE_WINDOW_POS);
	message_window_->setWindowSize(MESSAGE_WINDOW_SIZE);

	// エンディングシナリオを読み込み
	CsvData& scenario_data = ResourceManager::getInstance()->loadCsvData("csv/ending_scenario_data.csv");

	event_messages_.resize(scenario_data.size() - 1);

	for (int y = GameManager::CSV_CELL_ROW_START; y < scenario_data.size(); ++y) {
		
		event_messages_[y - 1].resize(scenario_data.size());
		
		for (int x = 0; x < scenario_data[y].size(); ++x) {
			event_messages_[y - 1][x] = scenario_data[y][x].getString();
		}
	}

	message_window_->setAllLineMessage(event_messages_[now_page_]);

}

SceneGameClear::~SceneGameClear() {

}

void SceneGameClear::update(float delta_time) {

	sequence_.update(delta_time);
}

void SceneGameClear::draw() {
	
	DrawExtendGraph(BACKGROUND_POS.x, BACKGROUND_POS.y, BACKGROUND_POS.x + DXE_WINDOW_WIDTH, BACKGROUND_POS.y + DXE_WINDOW_HEIGHT, back_ground_gpc_hdl_, true);

	if (message_window_->isEnable()) {
		message_window_->draw();
	}
}

bool SceneGameClear::seqSceneTransition(const float delta_time) {

	if (!GameManager::GetInstance()->isTransition()) {
		message_window_->setEnable(true);
		sequence_.change(&SceneGameClear::seqMessageEvent);
	}

	return true;
}

bool SceneGameClear::seqMessageEvent(const float delta_time) {

	if (now_page_ >= event_messages_.size() - 1) {
		sequence_.change(&SceneGameClear::seqEnd);
		return true;
	}

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		++now_page_;
		message_window_->clearMessage();
		message_window_->setAllLineMessage(event_messages_[now_page_]);
		message_window_->setEnable(true);
	}

	return true;
}

bool SceneGameClear::seqEnd(const float delta_time) {

	if (GameManager::GetInstance()->isTransition()) {
		return true;
	}

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		GameManager::GetInstance()->changeScene(std::make_shared<SceneTitle>(), 2.0f);
	}

	return true;
}

