#include "../../dxlib_ext/dxlib_ext.h"
#include "../manager/gm_manager.h"
#include "../manager/resource_manager.h"
#include "../ui/select_window.h"
#include "../ui/message_window.h"
#include "../dungeon/dungeon_log.h"
#include "scene_play.h"
#include "scene_title.h"

namespace {
	// 背景の表示位置
	const tnl::Vector2i BACKGROUND_POS = { 0, 0 };
	
	// ゲームタイトルの表示位置
	const tnl::Vector2i TITLE_LOGO_POS = { 325, 100 };

	// メニューのフォントサイズ
	const int MENU_FONT_SIZE = 30;

	// ゲームメニューの表示位置
	const tnl::Vector2i TITLE_MENU_POS = { 550, 500 };
	// ゲームメニュー名
	const std::string TITLE_MENU_STR = "Start Enter";

	// メインメニューの位置
	const tnl::Vector2i TITLE_MAIN_MENU_POS = { 450, 500 };
	const tnl::Vector2i TITLE_MAIN_MENU_SIZE = { 400, 0 };

	// 
	const tnl::Vector2i DUNGEON_LOG_MENU_POS = { 200, 200 };
	const tnl::Vector2i DUNGEON_LOG_MENU_SIZE = { 300, 0 };

}


SceneTitle::SceneTitle() : sequence_(tnl::Sequence<SceneTitle>(this, &SceneTitle::seqSceneStart)), title_menu_(std::make_shared<SelectWindow>()),
	dungeon_log_menu_(std::make_shared<SelectWindow>()), selected_dungeon_log_list_index_(0), control_explanation_window_(std::make_shared<MessageWindow>()),
	back_ground_gpc_hdl_(0), sunlight_gpc_hdl_(0), title_bgm_hdl_path_("sound/bgm/title.ogg"), title_bgm_hdl_(0), title_logo_gpc_hdl_(0),
	bgm_end_freqency_(2722464), title_menu_alpha_(0), sunlight_alpha_(0), alpha_center_(176), sin_range_(32), scene_elapsed_time_(0.0f)
{

	tnl::DebugTrace("SceneTitleのコンストラクタが実行されました\n");
	SetBackgroundColor(32, 32, 32);
	back_ground_gpc_hdl_ = ResourceManager::getInstance()->loadGraph("graphics/title_background.jpg");
	sunlight_gpc_hdl_ = ResourceManager::getInstance()->loadGraph("graphics/sunlight.png");
	title_bgm_hdl_ = ResourceManager::getInstance()->loadSound(title_bgm_hdl_path_);

	alpha_center_ -= sin_range_;

	std::vector<std::string> cmd_names(2);

	cmd_names[0] = "森に入る";
	cmd_names[1] = "ダンジョン記録を見る";

	title_menu_->setWindowPos(TITLE_MAIN_MENU_POS);
	title_menu_->setWindowSize(tnl::Vector2i(400, 0));
	title_menu_->setFontSize(MENU_FONT_SIZE);
	title_menu_->setMessageSpace(5);
	title_menu_->setSelectCmdMax(cmd_names.size());
	title_menu_->setCommandNames(cmd_names);
	title_menu_->calculateWindowSize();

	// -------------------------- タイトルロゴの設定 -------------------------------------------------------------
	title_logo_gpc_hdl_ = ResourceManager::getInstance()->loadGraph("graphics/title_logo1.png");

	int width = 0;
	int height = 0;
	GetGraphSize(title_logo_gpc_hdl_, &width, &height);

	title_logo_size_ = tnl::Vector2i(width, height);
	// ------------------------------------------------------------------------------------------------------------

	dungeon_log_list_.resize(10);

	CsvData& control_explanation_message 
		= ResourceManager::getInstance()->loadCsvData("csv/control_explanation_data.csv");

	control_explanation_window_->setWindowPos(TITLE_MAIN_MENU_POS + TITLE_MAIN_MENU_SIZE + tnl::Vector2i(20, 0));
	control_explanation_window_->setWindowSize(tnl::Vector2i(365, 0));
	control_explanation_window_->setFontSize(25);
	control_explanation_window_->setMessageLine(control_explanation_message[GameManager::CSV_CELL_ROW_START + 2][0].getInt());
	control_explanation_window_->setMessageTopPos(tnl::Vector2i(10, 10));
	control_explanation_window_->setMessageSpace(0);
	control_explanation_window_->calculateWindowSize();

	for (int i = GameManager::CSV_CELL_ROW_START; i < control_explanation_message[GameManager::CSV_CELL_ROW_START + 2][0].getInt() + 1; ++i) {
		control_explanation_window_->setMessgae(control_explanation_message[GameManager::CSV_CELL_ROW_START + 2][i].getString());
	}

}

SceneTitle::~SceneTitle() {
	tnl::DebugTrace("SceneTitleのデストラクタが実行されました\n");
	StopSoundMem(title_bgm_hdl_);
	ResourceManager::getInstance()->deleteSound(title_bgm_hdl_path_);
}

void SceneTitle::update(float delta_time) {

	sequence_.update(delta_time);

	scene_elapsed_time_ += delta_time;

	sunlight_alpha_ = alpha_center_ + sin_range_ * sin(2 * tnl::PI * scene_elapsed_time_ / 5.0f);

	int freqency = GetCurrentPositionSoundMem(title_bgm_hdl_);
	if (freqency > bgm_end_freqency_)
	{
		StopSoundMem(title_bgm_hdl_);
		SetCurrentPositionSoundMem(0, title_bgm_hdl_);
		PlaySoundMem(title_bgm_hdl_, DX_PLAYTYPE_LOOP, true);
	}
}

void SceneTitle::draw() {

	DrawExtendGraph(BACKGROUND_POS.x, BACKGROUND_POS.y, BACKGROUND_POS.x + DXE_WINDOW_WIDTH, BACKGROUND_POS.y + DXE_WINDOW_HEIGHT, back_ground_gpc_hdl_, true);

	// DrawGraph(TITLE_LOGO_POS.x, TITLE_LOGO_POS.y, title_logo_gpc_hdl_, true);

	int width = static_cast<int>(title_logo_size_.x * 2.5f);
	int height = static_cast<int>(title_logo_size_.y * 2.5f);

	DrawExtendGraph(TITLE_LOGO_POS.x, TITLE_LOGO_POS.y, TITLE_LOGO_POS.x + width, TITLE_LOGO_POS.y + height, title_logo_gpc_hdl_, true);

	SetDrawBlendMode( DX_BLENDMODE_ALPHA, sunlight_alpha_ );
	DrawExtendGraph(BACKGROUND_POS.x, BACKGROUND_POS.y, BACKGROUND_POS.x + DXE_WINDOW_WIDTH, BACKGROUND_POS.y + DXE_WINDOW_HEIGHT, sunlight_gpc_hdl_, true);
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 255 );

	SetFontSize(MENU_FONT_SIZE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, title_menu_alpha_);
	DrawStringEx(TITLE_MENU_POS.x, TITLE_MENU_POS.y, -1, TITLE_MENU_STR.c_str());
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	title_menu_->draw();
	dungeon_log_menu_->draw();

	control_explanation_window_->draw();

	if (dungeon_log_list_[selected_dungeon_log_list_index_]) {
		dungeon_log_list_[selected_dungeon_log_list_index_]->draw();
	}

}

void SceneTitle::setupDugeonLogMenu() {
	// ダンジョン記録リストを取得
	dungeon_log_list_ = GameManager::getInstance()->getDungeonLogList();

	// 記録の数をカウント
	int log_count = 0;

	tnl::Vector2i window_pos = DUNGEON_LOG_MENU_POS + DUNGEON_LOG_MENU_SIZE + tnl::Vector2i(30, -100);

	for ( int i = 0; i < dungeon_log_list_.size(); ++i ) {
		if (!dungeon_log_list_[i]) {
			break;
		}
		dungeon_log_list_[i]->setDrawing(false);
		dungeon_log_list_[i]->setWindowPos(window_pos);
		++log_count;
	}

	dungeon_log_menu_->setWindowPos(DUNGEON_LOG_MENU_POS);
	dungeon_log_menu_->setWindowSize(DUNGEON_LOG_MENU_SIZE);
	dungeon_log_menu_->setFontSize(MENU_FONT_SIZE);

	if (log_count <= 0) {
		std::vector<std::string> cmd_names = {"記録がありません"};

		dungeon_log_menu_->setSelectCmdMax(1);
		dungeon_log_menu_->setCommandNames(cmd_names);
		dungeon_log_menu_->calculateWindowSize();
		return;
	}

	std::vector<std::string> cmd_names(log_count);

	for (int i = 0; i < cmd_names.size(); ++i) {
		cmd_names[i] = std::to_string(i + 1) + "位：" + std::to_string(dungeon_log_list_[i]->getOverallScore()) + "点";
	}
	dungeon_log_menu_->setSelectCmdMax(cmd_names.size());
	dungeon_log_menu_->setCommandNames(cmd_names);
	dungeon_log_menu_->calculateWindowSize();

}

void SceneTitle::openDungeonLogMenu() {

	selected_dungeon_log_list_index_ = 0;

	dungeon_log_menu_->setDrawing(true);

	if (dungeon_log_menu_->getSelectCmdMax() > 1) {
		dungeon_log_menu_->setOperate(true);
	}

	for ( int i = 0; i < dungeon_log_menu_->getSelectCmdMax(); ++i ) {
		if (!dungeon_log_list_[i]) {
			return;
		}
		dungeon_log_list_[i]->setDrawing(true);
	}

	int draw_pos_y 
		= dungeon_log_list_[selected_dungeon_log_list_index_]->getWindowPos().y + dungeon_log_list_[selected_dungeon_log_list_index_]->getWindowSize().y + 20;

	control_explanation_window_->setWindowPos( TITLE_MAIN_MENU_SIZE 
		+ tnl::Vector2i(TITLE_MAIN_MENU_POS.x + 20, draw_pos_y) );

}

void SceneTitle::closeDungeonLogMenu() {

	dungeon_log_menu_->setDrawing(false);
	dungeon_log_menu_->setOperate(false);

	for (int i = 0; i < dungeon_log_menu_->getSelectCmdMax(); ++i) {
		if (!dungeon_log_list_[i]) {
			return;
		}
		dungeon_log_list_[i]->setDrawing(false);
	}

	control_explanation_window_->setWindowPos(TITLE_MAIN_MENU_POS + TITLE_MAIN_MENU_SIZE + tnl::Vector2i(20, 0));
}

bool SceneTitle::seqSceneStart(const float delta_time) {

	if (!GameManager::getInstance()->isTransition()) {
		setupDugeonLogMenu();
		sequence_.change(&SceneTitle::seqRun);
		PlaySoundMem(title_bgm_hdl_, DX_PLAYTYPE_LOOP, true);
	}

	return true;
}

bool SceneTitle::seqRun(const float delta_time) {

	title_menu_alpha_ = 255 * abs(sin(2 * tnl::PI * sequence_.getProgressTime() / 5.0f));

	if (GameManager::getInstance()->isTransition()) {
		return true;
	}

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		sequence_.change(&SceneTitle::seqSelectMainMenu);
		title_menu_alpha_ = 0;
		title_menu_->setDrawing(true);
		title_menu_->setOperate(true);
		control_explanation_window_->setEnable(true);
		ResourceManager::getInstance()->playSound("sound/springin/open_window.mp3", DX_PLAYTYPE_BACK);
	}

	return true;
}

bool SceneTitle::seqSelectMainMenu(const float delta_time) {

	title_menu_->update(delta_time);

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

		ResourceManager::getInstance()->playSound("sound/se/button_enter.mp3", DX_PLAYTYPE_BACK);

		if (title_menu_->getSelectedCmdIndex() == 0) {
			ResourceManager::getInstance()->playSound("sound/se/walk.mp3", DX_PLAYTYPE_BACK);
			GameManager::getInstance()->changeScene(std::make_shared<ScenePlay>());
			sequence_.change(&SceneTitle::seqTransitionNextScene);
		}
		else {
			title_menu_->setDrawing(false);
			title_menu_->setOperate(false);
			openDungeonLogMenu();
			sequence_.change(&SceneTitle::seqSelectDungeonLogList);
		}
	}

	return false;
}

bool SceneTitle::seqSelectDungeonLogList(const float delta_time) {

	dungeon_log_menu_->update(delta_time);

	selected_dungeon_log_list_index_ = dungeon_log_menu_->getSelectedCmdIndex();

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_BACK)) {
		ResourceManager::getInstance()->playSound("sound/se/cancel.mp3", DX_PLAYTYPE_BACK);
		title_menu_->setDrawing(true);
		title_menu_->setOperate(true);
		closeDungeonLogMenu();
		sequence_.change(&SceneTitle::seqSelectMainMenu);
	}

	return true;
}

bool SceneTitle::seqTransitionNextScene(const float delta_time) {

	return true;
}
