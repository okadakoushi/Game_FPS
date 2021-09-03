#include "stdafx.h"
#include "RenderingEngine.h"

void RenderingEngine::Init()
{
    ////ZPrePass初期化。
    //m_zPrePassRT.Create(
    //    FRAME_BUFFER_W,
    //    FRAME_BUFFER_H,
    //    1,1,
    //    DXGI_FORMAT_R32G32B32A32_FLOAT,
    //    DXGI_FORMAT_D32_FLOAT,
    //    CLEARCOLOR
    //);

    //main初期化。
    m_mainRenderTarget.Create(
        FRAME_BUFFER_W,
        FRAME_BUFFER_H,
        1, 1,
        DXGI_FORMAT_R16G16B16A16_FLOAT,
        DXGI_FORMAT_UNKNOWN,
        CLEARCOLOR
    );

    //コピー用メインRTスプラトを生成。
    SpriteInitData initData;
    initData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
    initData.m_width = FRAME_BUFFER_W;
    initData.m_height = FRAME_BUFFER_H;
    initData.m_fxFilePath = "Assets/shader/sprite.fx";
    //initData.m_colorBufferFormat[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    //初期化。
    m_copyMainRtToFrameBufferSprite.Init(initData);

    //トーンマップ初期化。
    m_toneMap.Init(m_mainRenderTarget);
    //ブルーム初期化。
    m_bloom.Init(m_mainRenderTarget);
}

void RenderingEngine::ZPrePass(RenderContext& rc)
{
    // まず、レンダリングターゲットとして設定できるようになるまで待つ
    rc.WaitUntilToPossibleSetRenderTarget(m_zPrePassRT);
    // レンダリングターゲットを設定
    rc.SetRenderTargetAndViewport(m_zPrePassRT);
    //クリア。
    rc.ClearRenderTargetView(m_zPrePassRT.GetRTVCpuDescriptorHandle(), CLEARCOLOR);

    //登録されているRenderObjを全て描画。
    for (auto* render : m_renders) {
        render->GetModel().Draw(rc, GraphicsEngineObj()->GetCamera3D().GetViewMatrix(), GraphicsEngineObj()->GetCamera3D().GetProjectionMatrix(), render->GetRenderMode());
    }
    
    //描画終わりまでまつ。
    rc.WaitUntilFinishDrawingToRenderTarget(m_zPrePassRT);
}

void RenderingEngine::CopyMainRenderTargetToFrameBuffer(RenderContext& rc)
{
    // メインレンダリングターゲットの絵をフレームバッファーにコピー
    rc.SetRenderTarget(
        GraphicsEngineObj()->GetCurrentFrameBuffuerRTV(),
        GraphicsEngineObj()->GetCurrentFrameBufferDSV()
    );

    //// ビューポートを指定する
    D3D12_VIEWPORT viewport;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = static_cast<FLOAT>(FRAME_BUFFER_W);
    viewport.Height = static_cast<FLOAT>(FRAME_BUFFER_H);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    rc.SetViewport(viewport);

    m_copyMainRtToFrameBufferSprite.Draw(rc, GraphicsEngineObj()->GetCamera2D().GetViewMatrix(), GraphicsEngineObj()->GetCamera2D().GetProjectionMatrix());
}

void RenderingEngine::ToneMapRender()
{
    m_toneMap.OnRender(GraphicsEngineObj()->GetRenderContext(), m_mainRenderTarget);
}

void RenderingEngine::BloomRender()
{
    m_bloom.Render(GraphicsEngineObj()->GetRenderContext(),m_mainRenderTarget);
}


