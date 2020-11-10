#include "stdafx.h"
#include "GBufferRender.h"

GBufferRender::~GBufferRender()
{
}

void GBufferRender::Init()
{
	//GBuffer�̏������B
	//�N���A�J���[�B
	float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//�A���x�h�pRT���쐬�B
	m_GBuffers[GBuffer_albed].Create(
		FRAME_BUFFER_W, FRAME_BUFFER_H,
		1, 1,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_D32_FLOAT,
		clearColor
	);
	//�@���pRT���쐬�B
	m_GBuffers[GBuffer_normal].Create(
		FRAME_BUFFER_W, FRAME_BUFFER_H,
		1, 1,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_UNKNOWN,
		clearColor
	);
	//���[���h���W�pRT�쐬�B
	m_GBuffers[GBuffer_worldPos].Create(
		FRAME_BUFFER_W, FRAME_BUFFER_H,
		1, 1,
		DXGI_FORMAT_R32G32B32A32_FLOAT,	//�ׂ����l��ۑ��B
		DXGI_FORMAT_UNKNOWN,
		clearColor
	);

}

void GBufferRender::Render(RenderContext& rc)
{
	RenderTarget* rtv[]{
		&m_GBuffers[GBuffer_albed],
		&m_GBuffers[GBuffer_normal],
		&m_GBuffers[GBuffer_worldPos]
	};

	auto ge = GraphicsEngineObj();
	//�g�p�\�܂ł܂B
	ge->GetRenderContext().WaitUntilToPossibleSetRenderTargets(3, rtv);
	//�ύX����B
	ge->GetRenderContext().SetRenderTargets(3, rtv);
	//�N���A�B
	ge->GetRenderContext().ClearRenderTargetViews(3, rtv);

	for (auto& model : m_models) {
		//�o�^���ꂽ�I�u�W�F�N�g��Gbuffer��`��B
		model->GetModel().Draw(rc, ge->GetCamera3D().GetViewMatrix(), ge->GetCamera3D().GetProjectionMatrix(), model->GetRenderMode());
	}
	//�`��I������̂ŁA���X�g�̒��g���N���A�B
	m_models.clear();
	//�����_�[�^�[�Q�b�g�����Ƃɖ߂��B
	ge->ChangeRenderTargetToFrameBuffer(rc);
}