#include "stdafx.h"
#include "Bloom.h"

void Bloom::Init(RenderTarget& mainRT)
{
	m_luminanceRenderTarget.Create(
		mainRT.GetWidth(),
		mainRT.GetHeight(),
		1, 1,
		mainRT.GetColorBufferFormat(),
		DXGI_FORMAT_UNKNOWN
	);

	//�P�x���o�p�̃X�v���C�g���������B
	{
        SpriteInitData spriteInitData;
        // �P�x���o�p�̃V�F�[�_�[�̃t�@�C���p�X���w�肷��
        spriteInitData.m_fxFilePath = "Assets/shader/bloom.fx";
        // ���_�V�F�[�_�[�̃G���g���[�|�C���g���w�肷��
        spriteInitData.m_vsEntryPointFunc = "VSMain";
        // �s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g���w�肷��
        spriteInitData.m_psEntryPoinFunc = "PSSamplingLuminance";
        // �X�v���C�g�̕��ƍ�����luminnceRenderTarget�Ɠ���
        spriteInitData.m_width = mainRT.GetWidth();
        spriteInitData.m_height = mainRT.GetHeight();
        // �e�N�X�`���̓��C�������_�����O�^�[�Q�b�g�̃J���[�o�b�t�@�[
        spriteInitData.m_textures[0] = &mainRT.GetRenderTargetTexture();
        // �`�����ރ����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g���w�肷��
        spriteInitData.m_colorBufferFormat[0] = mainRT.GetColorBufferFormat();

        m_luminanceSprite.Init(spriteInitData);
	}

    //�K�E�V�A���u���[��������
    // gaussianBlur[0]�͋P�x�e�N�X�`���ɃK�E�V�A���u���[��������
    m_gaussianBlur[0].Init(&m_luminanceRenderTarget.GetRenderTargetTexture());
    // gaussianBlur[1]��gaussianBlur[0]�̃e�N�X�`���ɃK�E�V�A���u���[��������
    m_gaussianBlur[1].Init(&m_gaussianBlur[0].GetBokeTexture());
    // gaussianBlur[2]��gaussianBlur[1]�̃e�N�X�`���ɃK�E�V�A���u���[��������
    m_gaussianBlur[2].Init(&m_gaussianBlur[1].GetBokeTexture());
    // gaussianBlur[3]��gaussianBlur[2]�̃e�N�X�`���ɃK�E�V�A���u���[��������
    m_gaussianBlur[3].Init(&m_gaussianBlur[2].GetBokeTexture());

    //�ŏI�����p�̃X�v���C�g���������B
    {
        SpriteInitData spriteInitData;
        // �{�P�e�N�X�`����4���w�肷��
        spriteInitData.m_textures[0] = &m_gaussianBlur[0].GetBokeTexture();
        spriteInitData.m_textures[1] = &m_gaussianBlur[1].GetBokeTexture();
        spriteInitData.m_textures[2] = &m_gaussianBlur[2].GetBokeTexture();
        spriteInitData.m_textures[3] = &m_gaussianBlur[3].GetBokeTexture();
        // �𑜓x��mainRenderTarget�̕��ƍ���
        spriteInitData.m_width = mainRT.GetWidth();
        spriteInitData.m_height = mainRT.GetHeight();
        // �ڂ������摜���A�ʏ��2D�Ƃ��ă��C�������_�����O�^�[�Q�b�g�ɕ`�悷��̂ŁA
        // 2D�p�̃V�F�[�_�[���g�p����
        spriteInitData.m_fxFilePath = "Assets/shader/bloom.fx";
        spriteInitData.m_psEntryPoinFunc = "PSBloomFinal";

        // �������A���Z�����ŕ`�悷��̂ŁA�A���t�@�u�����f�B���O���[�h�����Z�ɂ���
        spriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;
        spriteInitData.m_colorBufferFormat[0] = mainRT.GetColorBufferFormat();

        m_finalSprite.Init(spriteInitData);
    }
}

void Bloom::Render(RenderContext& rc, RenderTarget& mainRT)
{
    // �P�x���o
    // �P�x���o�p�̃����_�����O�^�[�Q�b�g�ɕύX
    rc.WaitUntilToPossibleSetRenderTarget(m_luminanceRenderTarget);
    // �����_�����O�^�[�Q�b�g��ݒ�
    rc.SetRenderTargetAndViewport(m_luminanceRenderTarget);
    // �����_�����O�^�[�Q�b�g���N���A
    rc.ClearRenderTargetView(m_luminanceRenderTarget.GetRTVCpuDescriptorHandle(), m_clearColor);
    // �P�x���o���s��
    m_luminanceSprite.Draw(rc, GraphicsEngineObj()->GetCamera2D().GetViewMatrix(), GraphicsEngineObj()->GetCamera2D().GetProjectionMatrix());
    // �����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
    rc.WaitUntilFinishDrawingToRenderTarget(m_luminanceRenderTarget);

    // �K�E�V�A���u���[��4����s����
    m_gaussianBlur[0].ExecuteOnGPU(rc, 5);
    m_gaussianBlur[1].ExecuteOnGPU(rc, 5);
    m_gaussianBlur[2].ExecuteOnGPU(rc, 5);
    m_gaussianBlur[3].ExecuteOnGPU(rc, 5);

    // 4���̃{�P�摜���������ă��C�������_�����O�^�[�Q�b�g�ɉ��Z����
    // �����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���܂ő҂�
    rc.WaitUntilToPossibleSetRenderTarget(mainRT);
    // �����_�����O�^�[�Q�b�g��ݒ�
    rc.SetRenderTargetAndViewport(mainRT);
    // �ŏI����
    m_finalSprite.Draw(rc, GraphicsEngineObj()->GetCamera2D().GetViewMatrix(), GraphicsEngineObj()->GetCamera2D().GetProjectionMatrix());
    // �����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
    rc.WaitUntilFinishDrawingToRenderTarget(mainRT);
}
