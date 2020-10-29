#include "stdafx.h"
#include "Game.h"

Game::Game()
{
	//アニメーションの初期化ステップの都合上ここで初期化。
	//シャドウマップの初期化。
	EngineObj().GetGraphicsEngine()->GetShadowMap()->Init(2048, 2048, 1000.0f);
	//シャドウキャスターとして登録する。
	EngineObj().GetGraphicsEngine()->GetShadowMap()->RegisterShadowCaster(&m_unityChan);
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

	//ステージのインスタンス作成。
	m_bg = NewGO<SkinModelRender>(0, "bg");
	//初期化。
	m_bg->Init("Assets/modelData/bgReo/bg.tkm");

	//アニメーション系の初期化開始！
	switch (initState)
	{
	case enInitStep_None:
		//ファイルパスの読み込み。
		m_unityChan.InitModel("Assets/modelData/unityChan.tkm");

		return true;
		break;
	case enInitStep_LoadModel:
		//tksファイルの読み込み。
		initState = m_unityChan.LoadTks(InitDataModel);
		//m_bg.LoadTks(InitStage);
		break;
	case enInitStep_LoadSkelton:
		//ボーンの行列計算、アニメーションクリップの有無の検索。
		initState = m_unityChan.initSkeleton(InitDataModel);
		//m_bg.initSkeleton(InitStage);
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
	//回転を作成。
	Quaternion qRot = Quaternion::Identity;
	qRot.SetRotationDeg(Vector3::AxisX, -90.0f);
	m_bg->SetRotation(qRot);
	m_bg->Update();
	m_bg->SetShadowReciever();
	//シャドウマップの更新。
	EngineObj().GetGraphicsEngine()->GetShadowMap()->Update();
}

void Game::PreRender()
{
	//描画。todo : engineUpdate
	EngineObj().GetGraphicsEngine()->GetShadowMap()->RenderToShadowMap();
}

void Game::Draw()
{
	m_unityChan.Draw(GraphicsEngineObj()->GetRenderContext(), GraphicsEngineObj()->GetCamera3D().GetViewMatrix(), GraphicsEngineObj()->GetCamera3D().GetProjectionMatrix(), enRenderMode_NonSkin);
	//スキンなし描画。
	m_bg->SetRenderMode(enRenderMode_NonSkin);
	m_bg->Draw();
}