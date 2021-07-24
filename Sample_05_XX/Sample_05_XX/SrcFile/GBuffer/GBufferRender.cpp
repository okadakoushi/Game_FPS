#include "stdafx.h"
#include "GBufferRender.h"

DefferdRender::~DefferdRender()
{
}

void DefferdRender::Init()
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

void DefferdRender::SpriteInit()
{
	SpriteInitData initData;
	//�S�̃����_�����O�B
	initData.m_width = FRAME_BUFFER_W;
	initData.m_height = FRAME_BUFFER_H;
	for (int i = 0; i < Gbuffer_Num; i++) {
		//Gbuffer�̐������������Btodo:static_cast
		initData.m_textures[i] = &GraphicsEngineObj()->GetDefferd().GetTexture((EnGBuffer)i);
	}
	//�f�B�t�@�[�h�p�̃X�v���C�g��ݒ�B
	initData.m_fxFilePath = "Assets/shader/Defeardsprite.fx";
	//���C�g�̒萔�o�b�t�@�B
	g_light.eyePos = GraphicsEngineObj()->GetCamera3D().GetPosition();
	initData.m_expandConstantBuffer = &g_light;
	initData.m_expandConstantBufferSize = sizeof(g_light);
	//�|�X�g�G�t�F�N�g�p�����[�^�[�𑗂�B
	m_defferdSprite.SetUserSetConstantBufferEntity(&m_postEffectEntity);
	//�f�B�t�@�[�h�p�̃X�v���C�g���������B
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
	//�g�p�\�܂ł܂B
	ge->GetRenderContext().WaitUntilToPossibleSetRenderTargets(Gbuffer_Num, rtv);
	//�ύX����B
	ge->GetRenderContext().SetRenderTargets(Gbuffer_Num, rtv);
	//�N���A�B
	ge->GetRenderContext().ClearRenderTargetViews(Gbuffer_Num, rtv);

#ifdef NAV_DEBUG
	//�i�r���b�V���̃f�o�b�O�\���B
	//EngineObj().GetNaviMesh()->Render();
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

void DefferdRender::DeffardRender(RenderContext& rc, const Matrix& view, const Matrix& proj)
{
	if (!m_isInited) {
		//���[�U�[�ݒ�p�萔�o�b�t�@���o�^����Ă��珉��������B
		GraphicsEngineObj()->GetDefferd().SpriteInit();
		m_isInited = true;
	}
	//�`��B
	m_defferdSprite.Draw(rc, view, proj);
}
