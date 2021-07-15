#include "stdafx.h"
#include "CEffectEngine.h"

CEffectEngine::CEffectEngine()
{
}

CEffectEngine::~CEffectEngine()
{
	//解放
	Release();
}

void CEffectEngine::Release()
{
	//マネージャーとかはエフェクシア側でスマートポインタとして管理されてるから、
	//解放しかなくて大丈夫なはず。
}

void CEffectEngine::Init()
{
	auto format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//レンダラーを初期化。
	m_effekseerRenderer = EffekseerRendererDX12::Create(
		GraphicsEngineObj()->GetD3DDevice(),			//D3Dデバイス。
		GraphicsEngineObj()->GetCommandQueue(),			//コマンドキュー。
		3,												//スワップバッファーカウント。
		&format,										//フォーマットカラー。
		1,												//RTの数。
		DXGI_FORMAT_D32_FLOAT,							//デプスフォーマット。
		false,											//デプス反転？？
		2000											//板ポリの最大数。
	);
	//auto tex = EffekseerRendererDX12::CreateTexture(m_effekseerRenderer, GraphicsEngineObj()->GetDepthTextrue()
	////デプスを作成。
	//m_effekseerRenderer->SetDepth();
	// Create a memory pool
	// メモリプールの作成
	m_sfMemoryPoolEfk = EffekseerRendererDX12::CreateSingleFrameMemoryPool(m_effekseerRenderer);
	// Create a command list
	// コマンドリストの作成
	m_commandListEfk = EffekseerRendererDX12::CreateCommandList(m_effekseerRenderer, m_sfMemoryPoolEfk);
	
	//エフェクトマネージャを初期化。
	m_manager = Effekseer::Manager::Create(10000);

	// 描画用インスタンスから描画機能を設定
	m_manager->SetSpriteRenderer(m_effekseerRenderer->CreateSpriteRenderer());
	m_manager->SetRibbonRenderer(m_effekseerRenderer->CreateRibbonRenderer());
	m_manager->SetRingRenderer(m_effekseerRenderer->CreateRingRenderer());
	m_manager->SetTrackRenderer(m_effekseerRenderer->CreateTrackRenderer());
	m_manager->SetModelRenderer(m_effekseerRenderer->CreateModelRenderer());

	// 描画用インスタンスからテクスチャの読込機能を設定
	// 独自拡張可能、現在はファイルから読み込んでいる。
	m_manager->SetTextureLoader(m_effekseerRenderer->CreateTextureLoader());
	m_manager->SetModelLoader(m_effekseerRenderer->CreateModelLoader());
	m_manager->SetMaterialLoader(m_effekseerRenderer->CreateMaterialLoader());
}

void CEffectEngine::Update()
{
	//フレーム開始時に呼ぶ。
	m_sfMemoryPoolEfk->NewFrame();
	// コマンドリストを開始する。
	EffekseerRendererDX12::BeginCommandList(m_commandListEfk, GraphicsEngineObj()->GetCommandList());
	//コマンドリスト設定。
	m_effekseerRenderer->SetCommandList(m_commandListEfk);
	//エフェクサーのカメラ設定
	//m_effekseerRenderer->SetCameraMatrix(GraphicsEngineObj()->GetCamera3D().GetViewMatrix());
	//m_effekseerRenderer->SetProjectionMatrix(GraphicsEngineObj()->GetCamera3D().GetViewMatrix());
	//更新
	m_manager->Update();
}

void CEffectEngine::Render()
{
	//g_cameraからカメラ行列とプロジェクション行列をコピー
	Effekseer::Matrix44 efCameraMat;
	GraphicsEngineObj()->GetCamera3D().GetViewMatrix().CopyTo(efCameraMat);
	Effekseer::Matrix44 efProjMat;
	GraphicsEngineObj()->GetCamera3D().GetProjectionMatrix().CopyTo(efProjMat);

	//カメラ行列とプロジェクション行列を原点に設定。
	//tkEngine側はカメラの中央に出るようになってた希ガス
	m_effekseerRenderer->SetCameraMatrix(efCameraMat);
	m_effekseerRenderer->SetProjectionMatrix(efProjMat);

	//Effekseerを更新。
	m_manager->Update();

	//エフェクト描画用のレンダリング設定。
	m_effekseerRenderer->BeginRendering();
	//エフェクトの描画処理
	m_manager->Draw();
	//レンダリング設定をもとに戻す。
	m_effekseerRenderer->EndRendering();
}

Effekseer::EffectRef CEffectEngine::CreateEffect(const wchar_t* filepath)
{
	//作成
	return Effekseer::Effect::Create(m_manager, (const EFK_CHAR*)filepath);
}

Effekseer::Handle CEffectEngine::Play(Effekseer::EffectRef& effect)
{
	//EffectのUpdateでワールド座標は計算
	return m_manager->Play(effect, 0, 0, 0);
}