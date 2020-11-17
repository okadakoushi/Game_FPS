#include "stdafx.h"
#include "GBufferRender.h"

GBufferRender::~GBufferRender()
{
}

void GBufferRender::Init()
{
	//GBufferの初期化。
	//クリアカラー。
	float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//アルベド用RTを作成。
	m_GBuffers[GBuffer_albed].Create(
		FRAME_BUFFER_W, FRAME_BUFFER_H,
		1, 1,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_D32_FLOAT,
		clearColor
	);
	//法線用RTを作成。
	m_GBuffers[GBuffer_normal].Create(
		FRAME_BUFFER_W, FRAME_BUFFER_H,
		1, 1,
		DXGI_FORMAT_R16G16B16A16_UNORM,
		DXGI_FORMAT_UNKNOWN,
		clearColor
	);
	//スペキュラ用RTを作成。
	m_GBuffers[GBuffer_spec].Create(
		FRAME_BUFFER_W, FRAME_BUFFER_H,
		1, 1,
		DXGI_FORMAT_R32_FLOAT,			//αしかとらないので。
		DXGI_FORMAT_UNKNOWN,
		clearColor
	);
	//ワールド座標用RT作成。
	m_GBuffers[GBuffer_worldPos].Create(
		FRAME_BUFFER_W, FRAME_BUFFER_H,
		1, 1,
		DXGI_FORMAT_R32G32B32A32_FLOAT,	//細かい値を保存。
		DXGI_FORMAT_UNKNOWN,
		clearColor
	);
	//シャドウ用RT作成。
	m_GBuffers[GBuffer_Shadow].Create(
		FRAME_BUFFER_W, FRAME_BUFFER_H,
		1, 1,
		DXGI_FORMAT_R32_FLOAT,			//深度値しかとらない。
		DXGI_FORMAT_UNKNOWN,
		clearColor
	);
}

void GBufferRender::Render(RenderContext& rc)
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

	for (auto& model : m_models) {
		//登録されたオブジェクトはGbufferを描画。
		model->GetModel().Draw(rc, ge->GetCamera3D().GetViewMatrix(), ge->GetCamera3D().GetProjectionMatrix(), model->GetRenderMode());
	}
	//描画終わったので、リストの中身をクリア。
	m_models.clear();
	//レンダーターゲットをもとに戻す。
	ge->ChangeRenderTargetToFrameBuffer(rc);
}