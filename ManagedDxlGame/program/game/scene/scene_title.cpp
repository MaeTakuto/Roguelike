#include "../../dxlib_ext/dxlib_ext.h"
#include "../manager/gm_manager.h"
#include "../manager/resource_manager.h"
#include "scene_play.h"
#include "scene_title.h"

namespace {
	// 背景の表示位置
	const tnl::Vector2i BACKGROUND_POS = { 0, 0 };
	
	// ゲームタイトルの表示位置
	const tnl::Vector2i TITLE_POS = { 400, 200 };

	// ゲームタイトル名
	const std::string TITLE = "不思議な森";

	// ゲームメニューの表示位置
	const tnl::Vector2i TITLE_MENU_POS = { 500, 500 };

	// ゲームメニュー名
	const std::string TITLE_MENU_STR = "Start Enter";
}


SceneTitle::SceneTitle() : sequence_(tnl::Sequence<SceneTitle>(this, &SceneTitle::seqSceneTransition)), back_ground_gpc_hdl_(0), sunlight_gpc_hdl_(0), 
	title_bgm_hdl_path_("sound/title.ogg"), title_bgm_hdl_(0), bgm_end_freqency_(2722464), title_menu_alpha_(0), sunlight_alpha_(0), alpha_center_(176), sin_range_(32), scene_elapsed_time_(0.0f)
{

	tnl::DebugTrace("SceneTitleのコンストラクタが実行されました\n");
	SetBackgroundColor(32, 32, 32);
	back_ground_gpc_hdl_ = ResourceManager::getInstance()->loadGraph("graphics/title_background.jpg");
	sunlight_gpc_hdl_ = ResourceManager::getInstance()->loadGraph("graphics/sunlight.png");
	title_bgm_hdl_ = ResourceManager::getInstance()->loadSound(title_bgm_hdl_path_);

	alpha_center_ -= sin_range_;

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

	SetFontSize(80);
	DrawExtendGraph(BACKGROUND_POS.x, BACKGROUND_POS.y, BACKGROUND_POS.x + DXE_WINDOW_WIDTH, BACKGROUND_POS.y + DXE_WINDOW_HEIGHT, back_ground_gpc_hdl_, true);

	SetDrawBlendMode( DX_BLENDMODE_ALPHA, sunlight_alpha_ );
	DrawExtendGraph(BACKGROUND_POS.x, BACKGROUND_POS.y, BACKGROUND_POS.x + DXE_WINDOW_WIDTH, BACKGROUND_POS.y + DXE_WINDOW_HEIGHT, sunlight_gpc_hdl_, true);
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 255 );

	DrawStringEx(TITLE_POS.x, TITLE_POS.y, -1, TITLE.c_str());
	SetFontSize(30);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, title_menu_alpha_);
	DrawStringEx(TITLE_MENU_POS.x, TITLE_MENU_POS.y, -1, TITLE_MENU_STR.c_str());
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

}

bool SceneTitle::seqSceneTransition(const float delta_time) {

	if (!GameManager::GetInstance()->isTransition()) {
		sequence_.change(&SceneTitle::seqRun);
		PlaySoundMem(title_bgm_hdl_, DX_PLAYTYPE_LOOP, true);
	}

	return true;
}

bool SceneTitle::seqRun(const float delta_time) {

	title_menu_alpha_ = 255 * abs(sin(2 * tnl::PI * sequence_.getProgressTime() / 5.0f));

	if (GameManager::GetInstance()->isTransition()) {
		return true;
	}

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		GameManager::GetInstance()->changeScene(std::make_shared<ScenePlay>());
	}

	return true;
}