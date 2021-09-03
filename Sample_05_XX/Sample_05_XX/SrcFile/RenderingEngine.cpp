#include "stdafx.h"
#include "RenderingEngine.h"

void RenderingEngine::Init()
{
    ////ZPrePass�������B
    //m_zPrePassRT.Create(
    //    FRAME_BUFFER_W,
    //    FRAME_BUFFER_H,
    //    1,1,
    //    DXGI_FORMAT_R32G32B32A32_FLOAT,
    //    DXGI_FORMAT_D32_FLOAT,
    //    CLEARCOLOR
    //);

    //main�������B
    m_mainRenderTarget.Create(
        FRAME_BUFFER_W,
        FRAME_BUFFER_H,
        1, 1,
        DXGI_FORMAT_R16G16B16A16_FLOAT,
        DXGI_FORMAT_UNKNOWN,
        CLEARCOLOR
    );

    //�R�s�[�p���C��RT�X�v���g�𐶐��B
    SpriteInitData initData;
    initData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
    initData.m_width = FRAME_BUFFER_W;
    initData.m_height = FRAME_BUFFER_H;
    initData.m_fxFilePath = "Assets/shader/sprite.fx";
    //initData.m_colorBufferFormat[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    //�������B
    m_copyMainRtToFrameBufferSprite.Init(initData);

    //�g�[���}�b�v�������B
    m_toneMap.Init(m_mainRenderTarget);
    //�u���[���������B
    m_bloom.Init(m_mainRenderTarget);
}

void RenderingEngine::ZPrePass(RenderContext& rc)
{
    // �܂��A�����_�����O�^�[�Q�b�g�Ƃ��Đݒ�ł���悤�ɂȂ�܂ő҂�
    rc.WaitUntilToPossibleSetRenderTarget(m_zPrePassRT);
    // �����_�����O�^�[�Q�b�g��ݒ�
    rc.SetRenderTargetAndViewport(m_zPrePassRT);
    //�N���A�B
    rc.ClearRenderTargetView(m_zPrePassRT.GetRTVCpuDescriptorHandle(), CLEARCOLOR);

    //�o�^����Ă���RenderObj��S�ĕ`��B
    for (auto* render : m_renders) {
        render->GetModel().Draw(rc, GraphicsEngineObj()->GetCamera3D().GetViewMatrix(), GraphicsEngineObj()->GetCamera3D().GetProjectionMatrix(), render->GetRenderMode());
    }
    
    //�`��I���܂ł܂B
    rc.WaitUntilFinishDrawingToRenderTarget(m_zPrePassRT);
}

void RenderingEngine::CopyMainRenderTargetToFrameBuffer(RenderContext& rc)
{
    // ���C�������_�����O�^�[�Q�b�g�̊G���t���[���o�b�t�@�[�ɃR�s�[
    rc.SetRenderTarget(
        GraphicsEngineObj()->GetCurrentFrameBuffuerRTV(),
        GraphicsEngineObj()->GetCurrentFrameBufferDSV()
    );

    //// �r���[�|�[�g���w�肷��
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


