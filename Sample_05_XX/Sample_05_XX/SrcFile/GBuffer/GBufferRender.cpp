#include "stdafx.h"
#include "GBufferRender.h"

DefferdRender::~DefferdRender()
{
}

void DefferdRender::Init()
{
	//GBufferの初期化。
	//アルベド用RTを作成。
	m_GBuffers[GBuffer_albed].Create(
		FRAME_BUFFER_W, FRAME_BUFFER_H,
		1, 1,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_D32_FLOAT,
		CLEARCOLOR
	);
	//法線用RTを作成。
	m_GBuffers[GBuffer_normal].Create(
		FRAME_BUFFER_W, FRAME_BUFFER_H,
		1, 1,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_UNKNOWN,
		CLEARCOLOR
	);
	//スペキュラ用RTを作成。
	m_GBuffers[GBuffer_spec].Create(
		FRAME_BUFFER_W, FRAME_BUFFER_H,
		1, 1,
		DXGI_FORMAT_R32_FLOAT,			//αしかとらないので。
		DXGI_FORMAT_UNKNOWN,
		CLEARCOLOR
	);
	//ワールド座標用RT作成。
	m_GBuffers[GBuffer_worldPos].Create(
		FRAME_BUFFER_W, FRAME_BUFFER_H,
		1, 1,
		DXGI_FORMAT_R32G32B32A32_FLOAT,	//細かい値を保存。
		DXGI_FORMAT_UNKNOWN,
		CLEARCOLOR
	);
	//シャドウ用RT作成。
	m_GBuffers[GBuffer_Shadow].Create(
		FRAME_BUFFER_W, FRAME_BUFFER_H,
		1, 1,
		DXGI_FORMAT_R32_FLOAT,			//深度値しかとらない。
		DXGI_FORMAT_UNKNOWN,
		CLEARCOLOR
	);
}

void DefferdRender::SpriteInit()
{
	SpriteInitData initData;
	//全体レンダリング。
	initData.m_width = FRAME_BUFFER_W;
	initData.m_height = FRAME_BUFFER_H;
	for (int i = 0; i < Gbuffer_Num; i++) {
		//Gbufferの数だけ初期化。todo:static_cast
		initData.m_textures[i] = &GraphicsEngineObj()->GetDefferd().GetTexture((EnGBuffer)i);
	}
	//ディファード用のスプライトを設定。
	initData.m_fxFilePath = "Assets/shader/Defeardsprite.fx";
	//ライトの定数バッファ。
	g_light.eyePos = GraphicsEngineObj()->GetCamera3D().GetPosition();
	initData.m_expandConstantBuffer = &g_light;
	initData.m_expandConstantBufferSize = sizeof(g_light);
	//ポストエフェクトパラメーターを送る。
	m_defferdSprite.SetUserSetConstantBufferEntity(&m_postEffectEntity);
	//ディファード用のスプライトを初期化。
	m_defferdSprite.Init(initData);
}

void DefferdRender::Render(RenderContext& rc, const Matrix& view, const Matrix& proj, bool Clear)
{
	RenderTarget* rtv[]{
		&m_GBuffers[GBuffer_albed],
		&m_GBuffers[GBuffer_normal],
		&m_GBuffers[GBuffer_spec],
		&m_GBuffers[GBuffer_worldPos],
		&m_GBuffers[GBuffer_Shadow]
	};
	auto ge = GraphicsEngineObj();
	//使用可能までまつ。
	ge->GetRenderContext().WaitUntilToPossibleSetRenderTargets(Gbuffer_Num, rtv);
	//変更する。
	ge->GetRenderContext().SetRenderTargets(Gbuffer_Num, rtv);
	//クリア。
	ge->GetRenderContext().ClearRenderTargetViews(Gbuffer_Num, rtv);

#ifdef NAV_DEBUG
	//ナビメッシュのデバッグ表示。
	//EngineObj().GetNaviMesh()->Render();
#endif // NAV_DEBUG

	for (auto& model : m_models) {
		//登録されたオブジェクトはGbufferを描画。
		model->GetModel().Draw(rc, view, proj, model->GetRenderMode());
	}
	//描画終わったので、リストの中身をクリア。
	if (Clear) {
		m_models.clear();
	}
	//レンダーターゲットをもとに戻す。
	ge->ChangeRenderTargetToFrameBuffer(rc);
}

void DefferdRender::DeffardRender(RenderContext& rc, const Matrix& view, const Matrix& proj)
{
	if (!m_isInited) {
		//ユーザー設定用定数バッファが登録されてから初期化する。
		GraphicsEngineObj()->GetDefferd().SpriteInit();
		m_isInited = true;
	}
	//描画。
	m_defferdSprite.Draw(rc, view, proj);
}
