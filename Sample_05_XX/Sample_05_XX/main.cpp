#include "stdafx.h"
#include "system/system.h"
#include "SrcFile/Game.h"

///////////////////////////////////////////////////////////////////
// ウィンドウプログラムのメイン関数。
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));

	//////////////////////////////////////
	// ここから初期化を行うコードを記述する。
	//////////////////////////////////////

	//3Dモデルを作成。
	
	Model robotPBRModel, robotModel;
	//Unitychanの初期化データー。
	ModelInitData initData;
	AnimInitData animData;

	//ファイルパス。
	initData.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
	animData.m_tkaFilePath = "Assets/modelData/unityChan/idle.tka";
	initData.m_fxFilePath = "Assets/shader/NoAnimModel_PBR.fx";
	initData.m_expandConstantBuffer = &g_light;
	initData.m_expandConstantBufferSize = sizeof(g_light);
	
	robotPBRModel.Init(initData);
	initData.m_fxFilePath = "Assets/shader/NoAnimModel_LambertSpecularAmbient.fx";
	robotModel.Init(initData);
	g_camera3D->SetPosition({ 0.0f, 50.0f, 100.0f });
	Vector3 pos, scale;
	

	scale.x = 1.0f;
	scale.y = 1.0f;
	scale.z = 1.0f;

	robotPBRModel.UpdateWorldMatrix(pos, g_quatIdentity, scale);
	robotModel.UpdateWorldMatrix(pos, g_quatIdentity, scale);

	//////////////////////////////////////
	// 初期化を行うコードを書くのはここまで！！！
	//////////////////////////////////////
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	bool isPBR = true;
	//ゲームのインスタンス作成。
	Game* mGame = new Game;
	// ここからゲームループ。
	while (DispatchWindowMessage())
	{
		//レンダリング開始。
		g_engine->BeginFrame();
		//ゲームの更新。
		mGame->Update();
		//////////////////////////////////////
		//ここから絵を描くコードを記述する。
		//////////////////////////////////////

		Quaternion qRot;
		if (g_pad[0]->IsPress(enButtonRight)) {
			qRot.SetRotationDegY(1.0f);
		}
		else if (g_pad[0]->IsPress(enButtonLeft)) {
			qRot.SetRotationDegY(-1.0f);
		}

		for (auto& lig : g_light.directionalLight) {
			qRot.Apply(lig.direction);
		}

		//カメラも回す。
		qRot.SetRotationDegY(g_pad[0]->GetLStickXF());
		auto camPos = g_camera3D->GetPosition();
		qRot.Apply(camPos);
		g_camera3D->SetPosition(camPos);
		g_light.eyePos = g_camera3D->GetPosition();
		if (g_pad[0]->IsTrigger(enButtonA)) {
			isPBR = !isPBR;
		}
		if (isPBR) {
			//3Dモデルを表示する。
			robotPBRModel.Draw(renderContext);
		}
		else {
			robotModel.Draw(renderContext);
		}
		//////////////////////////////////////
		//絵を描くコードを書くのはここまで！！！
		//////////////////////////////////////
		//レンダリング終了。
		g_engine->EndFrame();
	}
	return 0;
}


