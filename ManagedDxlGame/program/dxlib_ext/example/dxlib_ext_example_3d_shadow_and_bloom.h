#pragma once
/*

//-----------------------------------------------------------------------------------------------------------
//
//
// シャドウマップとブルームエフェクト適用サンプル
//
//
//-----------------------------------------------------------------------------------------------------------


#include <time.h>
#include <string>
#include <numbers>
#include <functional>
#include "../dxlib_ext/dxlib_ext.h"
#include "gm_main.h"

Shared<dxe::Camera> camera = nullptr;
Shared<dxe::Mesh> mesh = nullptr;
Shared<dxe::Mesh> ground = nullptr;
Shared<dxe::ScreenEffect> screen_efct = nullptr;
Shared<dxe::ShadowMap> shadow = nullptr;


//------------------------------------------------------------------------------------------------------------
// ゲーム起動時に１度だけ実行されます
void gameStart() {
	srand(time(0));

	camera = std::make_shared<dxe::Camera>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);

	mesh = dxe::Mesh::CreateSphereMV(100, 20, 20);
	mesh->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	mesh->pos_ = { 0, 100, 0 };

	ground = dxe::Mesh::CreatePlaneMV({ 2000, 2000, 0 }, 20, 20);
	ground->setTexture(dxe::Texture::CreateFromFile("graphics/lawn.png"));
	ground->rot_ = tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(90));

	// シャドウマップ作成
	shadow = std::make_shared<dxe::ShadowMap>(dxe::ShadowMap::eSize::S2048);

	// ブルームエフェクト作成
	screen_efct = std::make_shared<dxe::ScreenEffect>(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);


}


//------------------------------------------------------------------------------------------------------------
// 毎フレーム実行されます
void gameMain(float delta_time) {

	camera->update();

	//-------------------------------------------------------------------------------------
	//
	// シャドウマップ利用の準備
	//

	// シャドウマップへの描画開始
	shadow->reserveBegin();

	ground->reserveShadow();
	mesh->reserveShadow();

	// 終了
	shadow->reserveEnd();


	//-------------------------------------------------------------------------------------
	//
	// シャドウマップとブルームを適用した描画
	//

	// シャドウマップとブルーム適用開始
	shadow->renderBegin();
	screen_efct->renderBegin();

	// メッシュ描画
	mesh->render(camera);
	ground->render(camera);

	// シャドウマップとブルーム適用終了
	screen_efct->renderEnd();
	shadow->renderEnd();


	// デバッグ情報描画
	DrawDefaultLightGuiController();
	screen_efct->drawGuiController({ 0, 0 });

	DrawFpsIndicator({ 0, DXE_WINDOW_HEIGHT - 10, 0 }, delta_time);

}

//------------------------------------------------------------------------------------------------------------
// ゲーム終了時に１度だけ実行されます
void gameEnd() {

}

*/
