#include "../../dxlib_ext/dxlib_ext.h"
#include "../manager/gm_manager.h"
#include "../manager/resource_manager.h"
#include "scene_play.h"
#include "scene_title.h"

namespace {
	// �w�i�̕\���ʒu
	const tnl::Vector2i BACKGROUND_POS = { 0, 0 };
	
	// �Q�[���^�C�g���̕\���ʒu
	const tnl::Vector2i TITLE_POS = { 400, 200 };

	// �Q�[���^�C�g����
	const std::string TITLE = "�s�v�c�ȐX";
}


SceneTitle::SceneTitle() {

	tnl::DebugTrace("SceneTitle�̃R���X�g���N�^�����s����܂���\n");
	SetBackgroundColor(32, 32, 32);
	is_transition_process_ = false;
	back_ground_gpc_hdl_ = ResourceManager::getInstance()->loadGraph("graphics/title_background.jpg");
}

SceneTitle::~SceneTitle() {
	tnl::DebugTrace("SceneTitle�̃f�X�g���N�^�����s����܂���\n");
}

void SceneTitle::update(float delta_time) {

	if (GameManager::GetInstance()->isTransition()) return;
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		is_transition_process_ = true;
		GameManager::GetInstance()->changeScene(  std::make_shared<ScenePlay>() );
	}
}

void SceneTitle::draw() {

	SetFontSize(80);
	DrawExtendGraph(BACKGROUND_POS.x, BACKGROUND_POS.y, BACKGROUND_POS.x + DXE_WINDOW_WIDTH, BACKGROUND_POS.y + DXE_WINDOW_HEIGHT, back_ground_gpc_hdl_, true);
	DrawStringEx(TITLE_POS.x, TITLE_POS.y, -1, TITLE.c_str());
}
