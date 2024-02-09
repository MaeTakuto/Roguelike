
#include <time.h>
#include <string>
#include <numbers>
#include <functional>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_main.h"
#include "manager/gm_manager.h"
#include "manager/resource_manager.h"
#include "scene/scene_title.h"


//------------------------------------------------------------------------------------------------------------
// �Q�[���N�����ɂP�x�������s����܂�
void gameStart() {
	srand(time(0));
	SetBackgroundColor(32, 32, 32);

	GameManager::GetInstance(std::make_shared<SceneTitle>());
}


//------------------------------------------------------------------------------------------------------------
// ���t���[�����s����܂�
void gameMain(float delta_time) {

	GameManager::GetInstance()->update(delta_time);

	DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10, 0 }, delta_time);

}

//------------------------------------------------------------------------------------------------------------
// �Q�[���I�����ɂP�x�������s����܂�
void gameEnd() {

	ResourceManager::destroy();
	GameManager::Destroy();
}
