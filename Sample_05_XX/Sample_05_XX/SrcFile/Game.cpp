#include "stdafx.h"
#include "Game.h"

Game::Game()
{
	//アニメーションの初期化ステップの都合上ここで初期化。
	//シャドウマップの初期化。
	g_graphicsEngine->GetShadowMap()->Init(2048, 2048, 1000.0f);
	//シャドウキャスターとして登録する。
	g_graphicsEngine->GetShadowMap()->RegisterShadowCaster(&m_unityChan);
	m_pos = { 0.0f,0.0f,0.0f };
}

bool Game::Start()
{
	////tkaファイルパス。
	//const cha	* tkeFilePath[] = {
	//	"animData/unityChan/idle.tka",
	//	"animData/unityChan/run.tka",
	//	"animData/unityChan/walk.tka"
	//};

	//初期化データ。
	ModelInitData InitDataModel;

	InitDataModel.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
	InitDataModel.m_tkaFilePath = "Assets/animData/unityChan/run.tka";
	InitDataModel.m_fxFilePath = "Assets/shader/NoAnimModel_LambertSpecularAmbient.fx";
	InitDataModel.m_expandConstantBuffer = &g_light;
	InitDataModel.m_expandConstantBufferSize = sizeof(g_light);

	ModelInitData InitStage;
	InitStage.m_tkmFilePath = "Assets/modelData/bgReo/bg.tkm";
	InitStage.m_fxFilePath = "Assets/shader/NoAnimModel_LambertSpecularAmbient.fx";
	InitStage.m_expandConstantBuffer = &g_light;
	InitStage.m_expandConstantBufferSize = sizeof(g_light);

	//アニメーション系の初期化開始！
	switch (initState)
	{
	case enInitStep_None:
		//ファイルパスの読み込み。
		initState = m_unityChan.Init(InitDataModel);
		m_bg.Init(InitStage);
		break;
	case enInitStep_LoadModel:
		//tksファイルの読み込み。
		initState = m_unityChan.LoadTks(InitDataModel);
		m_bg.LoadTks(InitDataModel);
		break;
	case enInitStep_LoadSkelton:
		//ボーンの行列計算、アニメーションクリップの有無の検索。
		initState = m_unityChan.initSkeleton(InitDataModel);
		m_bg.initSkeleton(InitDataModel);
		break;
	case enInitStep_LoadAnimationClips:
		//アニメーションの初期化。
		initState = m_unityChan.InitAnim();
		break;
	case enInitStep_Completed:
		//初期化終了。
		m_inited = true;
		break;
	}

	//初期化終わり～。
	return m_inited;
}

Game::~Game()
{
}

void Game::Update()
{
	//アニメーションのプレイ。
	m_unityChan.PlayAnim(0, 0.0f);
	if (GetAsyncKeyState('A')) {
		//m_pos.x++;
	}
	else if (GetAsyncKeyState('D')) {
		//m_pos.x--;
	}
	else if (GetAsyncKeyState('W')) {
		m_pos.z++;
	}
	else if (GetAsyncKeyState('S')) {
		m_pos.z--;
	}
	m_unityChan.SetPosition(m_pos);
	//ワールド座標、アニメーションの更新。
	m_unityChan.UpdateMatAndAnim();
	Quaternion qRot = Quaternion::Identity;
	qRot.SetRotationDeg(Vector3::AxisX, -90.0f);
	m_bg.SetRotation(qRot);
	m_bg.UpdateMatAndAnim();
	m_bg.SetShadowReciever();
	//シャドウマップの更新。
	g_graphicsEngine->GetShadowMap()->Update();
}

void Game::PreRender()
{
	//描画。todo : engineUpdate
	g_graphicsEngine->GetShadowMap()->RenderToShadowMap();
}

void Game::Draw()
{
	m_unityChan.Draw(g_graphicsEngine->GetRenderContext(), g_camera3D->GetViewMatrix(), g_camera3D->GetProjectionMatrix());
	//スキンなし描画。
	m_bg.Draw(g_graphicsEngine->GetRenderContext(), g_camera3D->GetViewMatrix(), g_camera3D->GetProjectionMatrix(), enRenderMode_NonSkin);
}