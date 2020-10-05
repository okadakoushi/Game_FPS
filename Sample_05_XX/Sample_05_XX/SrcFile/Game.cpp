#include "stdafx.h"
#include "Game.h"

Game::Game()
{
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
		initState = m_animSampleModel.Init(InitDataModel);
		break;
	case enInitStep_LoadModel:
		//tksファイルの読み込み。
		initState = m_animSampleModel.LoadTks(InitDataModel);
		break;
	case enInitStep_LoadSkelton:
		//ボーンの行列計算、アニメーションクリップの有無の検索。
		initState = m_animSampleModel.initSkeleton();
		break;
	case enInitStep_LoadAnimationClips:
		//アニメーションの初期化。
		initState = m_animSampleModel.InitAnim();
		break;
	case enInitStep_Completed:
		//初期化終了。
		m_inited = true;
		break;
	}
	return m_inited;
}

Game::~Game()
{
}

void Game::Update()
{
	m_animSampleModel.PlayAnim(0, 0.0f);
	//ワールド座標、アニメーションの更新。
	m_animSampleModel.UpdateMatAndAnim();
	m_animSampleModel.Draw(g_graphicsEngine->GetRenderContext());
}