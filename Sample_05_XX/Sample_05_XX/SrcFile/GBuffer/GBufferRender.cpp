#include "stdafx.h"
#include "GBufferRender.h"

GBufferRender::~GBufferRender()
{
}

void GBufferRender::Init()
{
	//GBuffer�̏������B
	//�A���x�h�pRT���쐬�B
	m_GBuffers[GBuffer_albed].Create(
		FRAME_BUFFER_W, FRAME_BUFFER_H,
		1, 1,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_D32_FLOAT,
		CLEARCOLOR
	);
	//�@���pRT���쐬�B
	m_GBuffers[GBuffer_normal].Create(
		FRAME_BUFFER_W, FRAME_BUFFER_H,
		1, 1,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_UNKNOWN,
		CLEARCOLOR
	);
	//�X�y�L�����pRT���쐬�B
	m_GBuffers[GBuffer_spec].Create(
		FRAME_BUFFER_W, FRAME_BUFFER_H,
		1, 1,
		DXGI_FORMAT_R32_FLOAT,			//�������Ƃ�Ȃ��̂ŁB
		DXGI_FORMAT_UNKNOWN,
		CLEARCOLOR
	);
	//���[���h���W�pRT�쐬�B
	m_GBuffers[GBuffer_worldPos].Create(
		FRAME_BUFFER_W, FRAME_BUFFER_H,
		1, 1,
		DXGI_FORMAT_R32G32B32A32_FLOAT,	//�ׂ����l��ۑ��B
		DXGI_FORMAT_UNKNOWN,
		CLEARCOLOR
	);
	//�V���h�E�pRT�쐬�B
	m_GBuffers[GBuffer_Shadow].Create(
		FRAME_BUFFER_W, FRAME_BUFFER_H,
		1, 1,
		DXGI_FORMAT_R32_FLOAT,			//�[�x�l�����Ƃ�Ȃ��B
		DXGI_FORMAT_UNKNOWN,
		CLEARCOLOR
	);
}

void GBufferRender::Render(RenderContext& rc, const Matrix& view, const Matrix& proj, bool Clear)
{
	RenderTarget* rtv[]{
		&m_GBuffers[GBuffer_albed],
		&m_GBuffers[GBuffer_normal],
		&m_GBuffers[GBuffer_spec],
		&m_GBuffers[GBuffer_worldPos],
		&m_GBuffers[GBuffer_Shadow]
	};

	auto ge = GraphicsEngineObj();
	//�g�p�\�܂ł܂B
	ge->GetRenderContext().WaitUntilToPossibleSetRenderTargets(Gbuffer_Num, rtv);
	//�ύX����B
	ge->GetRenderContext().SetRenderTargets(Gbuffer_Num, rtv);
	//�N���A�B
	ge->GetRenderContext().ClearRenderTargetViews(Gbuffer_Num, rtv);

#ifdef NAV_DEBUG
	//�i�r���b�V���̃f�o�b�O�\���B
	EngineObj().GetNaviMesh()->Render();
#endif // NAV_DEBUG

	for (auto& model : m_models) {
		//�o�^���ꂽ�I�u�W�F�N�g��Gbuffer��`��B
		model->GetModel().Draw(rc, view, proj, model->GetRenderMode());
	}
	//�`��I������̂ŁA���X�g�̒��g���N���A�B
	if (Clear) {
		m_models.clear();
	}
	//�����_�[�^�[�Q�b�g�����Ƃɖ߂��B
	ge->ChangeRenderTargetToFrameBuffer(rc);
}