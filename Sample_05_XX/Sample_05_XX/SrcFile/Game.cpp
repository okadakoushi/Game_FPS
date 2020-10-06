#include "stdafx.h"
#include "Game.h"

Game::Game()
{
	//アニメーションの初期化ステップの都合上ここで初期化。
	//シャドウマップの初期化。
	g_graphicsEngine->GetShadowMap()->Init(1024, 1024, 2000.0f);
	//シャドウキャスターとして登録する。
	g_graphicsEngine->GetShadowMap()->RegisterShadowCaster(&m_unityChan);
}

bool Game::Start()
{
	//tkaファイルパス。
	const char* tkeFilePath[] = {
		"animData/unityChan/idle.tka",
		"animData/unityChan/run.tka",
		"animData/unityChan/walk.tka"
	};

	//初期化データ。
	ModelInitData InitDataModel;

	InitDataModel.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
	InitDataModel.m_tkaFilePath = "Assets/animData/unityChan/run.tka";
	InitDataModel.m_fxFilePath = "Assets/shader/NoAnimModel_LambertSpecularAmbient.fx";
	InitDataModel.m_expandConstantBuffer = &g_light;
	InitDataModel.m_expandConstantBufferSize = sizeof(g_light);

	//アニメーション系の初期化開始！
	switch (initState)
	{
	case enInitStep_None:
		//ファイルパスの読み込み。
		initState = m_unityChan.Init(InitDataModel);
		break;
	case enInitStep_LoadModel:
		//tksファイルの読み込み。
		initState = m_unityChan.LoadTks(InitDataModel);
		break;
	case enInitStep_LoadSkelton:
		//ボーンの行列計算、アニメーションクリップの有無の検索。
		initState = m_unityChan.initSkeleton();
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
	//シャドウマップの更新。
	g_graphicsEngine->GetShadowMap()->Update();
	//アニメーションのプレイ。
	m_unityChan.PlayAnim(0, 0.0f);
	//ワールド座標、アニメーションの更新。
	m_unityChan.UpdateMatAndAnim();
}

void Game::PreRender()
{
	//描画。
	g_graphicsEngine->GetShadowMap()->RenderToShadowMap();
}

void Game::Draw()
{
	m_unityChan.Draw(g_graphicsEngine->GetRenderContext(), g_camera3D->GetViewMatrix(), g_camera3D->GetProjectionMatrix());
}