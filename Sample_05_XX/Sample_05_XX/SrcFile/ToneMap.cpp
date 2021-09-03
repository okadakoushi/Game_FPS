#include "stdafx.h"
#include "ToneMap.h"

namespace {
	//-----------------------------------------------------------------------------
	// Name: GetSampleOffsets_DownScale4x4
	// Desc: Get the texture coordinate offsets to be used inside the DownScale4x4
	//       pixel shader.
	//-----------------------------------------------------------------------------
	/// <summary>
	/// ����16�s�N�Z������T���v�����O�B
	/// </summary>
	/// <param name="dwWidth"></param>
	/// <param name="dwHeight"></param>
	/// <param name="avSampleOffsets"></param>
	/// <returns></returns>
	HRESULT GetSampleOffsets_DownScale4x4(DWORD dwWidth, DWORD dwHeight, Vector4 avSampleOffsets[])
	{
		if (NULL == avSampleOffsets)
			return E_INVALIDARG;

		float tU = 1.0f / dwWidth;
		float tV = 1.0f / dwHeight;

		// Sample from the 16 surrounding points. Since the center point will be in
		// the exact center of 16 texels, a 0.5f offset is needed to specify a texel
		// center.
		int index = 0;
		for (int y = 0; y < 4; y++)
		{
			for (int x = 0; x < 4; x++)
			{
				avSampleOffsets[index].x = (x - 1.5f) * tU;
				avSampleOffsets[index].y = (y - 1.5f) * tV;

				index++;
			}
		}

		return S_OK;
	}
}

ToneMap::ToneMap()
{
}

ToneMap::~ToneMap()
{
}

void ToneMap::Init(RenderTarget& mainRT)
{
	for (int i = 0; i < enNumCalcAvgSprite; i++) {
		//���ϋP�x�v�Z�p��RT���쐬�B
		//RT�̉𑜓x�B
		int rtSize = 1 << (2 * (enNumCalcAvgSprite - i - 1));
		m_calcAvgRt[i].Create(
			rtSize,
			rtSize,
			1, 1,
			DXGI_FORMAT_R16_FLOAT,	//�P�x���o���邾���Ȃ̂�R16�B
			DXGI_FORMAT_UNKNOWN		//�f�v�X�͕K�v�Ȃ��B
		);
	}

	for (auto& rt : m_avgRt) {
		rt.Create(
			1, 1, 1, 1,
			DXGI_FORMAT_R16_FLOAT,
			DXGI_FORMAT_UNKNOWN
		);
	}

	m_finalRt.Create(
		mainRT.GetWidth(),
		mainRT.GetHeight(),
		1, 1,
		mainRT.GetColorBufferFormat(),
		DXGI_FORMAT_UNKNOWN
	);

	m_toneMapParam.midddleGray = 0.10f;
	m_toneMapParam.deltaTime = DELTA_TIME;

	// �ΐ����ς��Ƃ�X�v���C�g���������B
	int curRtNo = 0;
	{
		SpriteInitData initData;
		initData.m_width = m_calcAvgRt[curRtNo].GetWidth();
		initData.m_height = m_calcAvgRt[curRtNo].GetHeight();
		initData.m_colorBufferFormat[0] = m_calcAvgRt[curRtNo].GetColorBufferFormat();
		initData.m_fxFilePath = "Assets/shader/tonemap.fx";
		initData.m_psEntryPoinFunc = "PSCalcLuminanceLogAvarage";
		initData.m_expandConstantBuffer = m_avSampleOffsets;
		initData.m_expandConstantBufferSize = sizeof(m_avSampleOffsets);
		initData.m_textures[0] = &mainRT.GetRenderTargetTexture();
		m_calcAvgSprites[enCalcAvg_Log].Init(initData);
	}
	// ���ς��Ƃ�X�v���C�g���������B
	curRtNo++;
	int calsAvgSpriteNo = enCalcAvg_Start;
	while (curRtNo < enClacAvg_End) {
		SpriteInitData initData;
		initData.m_width = m_calcAvgRt[curRtNo].GetWidth();
		initData.m_height = m_calcAvgRt[curRtNo].GetHeight();
		initData.m_colorBufferFormat[0] = m_calcAvgRt[curRtNo].GetColorBufferFormat();
		initData.m_fxFilePath = "Assets/shader/tonemap.fx";
		initData.m_psEntryPoinFunc = "PSCalcLuminanceAvarage";
		initData.m_expandConstantBuffer = m_avSampleOffsets;
		initData.m_expandConstantBufferSize = sizeof(m_avSampleOffsets);
		initData.m_textures[0] = &m_calcAvgRt[curRtNo - 1].GetRenderTargetTexture();
		m_calcAvgSprites[calsAvgSpriteNo].Init(initData);
		calsAvgSpriteNo++;
		curRtNo++;
	}
	// exp�֐���p���čŏI���ς����߂�B
	{
		SpriteInitData initData;
		initData.m_width = m_calcAvgRt[curRtNo].GetWidth();
		initData.m_height = m_calcAvgRt[curRtNo].GetHeight();
		initData.m_colorBufferFormat[0] = m_calcAvgRt[curRtNo].GetColorBufferFormat();
		initData.m_fxFilePath = "Assets/shader/tonemap.fx";
		initData.m_psEntryPoinFunc = "PSCalcLuminanceExpAvarage";
		initData.m_expandConstantBuffer = m_avSampleOffsets;
		initData.m_expandConstantBufferSize = sizeof(m_avSampleOffsets);
		initData.m_textures[0] = &m_calcAvgRt[curRtNo - 1].GetRenderTargetTexture();
		m_calcAvgSprites[curRtNo].Init(initData);
	}
	// ���Ï���
	{
		SpriteInitData initData;
		initData.m_width = mainRT.GetWidth();
		initData.m_height = mainRT.GetHeight();
		initData.m_colorBufferFormat[0] = m_calcAvgRt[enCalcAvgExp].GetColorBufferFormat();
		initData.m_fxFilePath = "Assets/shader/tonemap.fx";
		initData.m_psEntryPoinFunc = "PSCalcAdaptedLuminance";
		initData.m_expandConstantBuffer = &m_toneMapParam;
		initData.m_expandConstantBufferSize = sizeof(m_toneMapParam);
		initData.m_textures[0] = &m_calcAvgRt[enCalcAvgExp].GetRenderTargetTexture();
		initData.m_textures[1] = &m_avgRt[0].GetRenderTargetTexture();
		initData.m_textures[2] = &m_avgRt[1].GetRenderTargetTexture();

		m_calcAdapteredLuminanceSprite.Init(initData);
	}
	{
		SpriteInitData initData;
		initData.m_width = mainRT.GetWidth();
		initData.m_height = mainRT.GetHeight();
		initData.m_colorBufferFormat[0] = m_calcAvgRt[enCalcAvgExp].GetColorBufferFormat();
		initData.m_fxFilePath = "Assets/shader/tonemap.fx";
		initData.m_psEntryPoinFunc = "PSCalcAdaptedLuminanceFirst";
		initData.m_expandConstantBuffer = &m_toneMapParam;
		initData.m_expandConstantBufferSize = sizeof(m_toneMapParam);
		initData.m_textures[0] = &m_calcAvgRt[enCalcAvgExp].GetRenderTargetTexture();

		m_calcAdapteredLuminanceFirstSprite.Init(initData);
	}
	// �ŏI����
	{
		SpriteInitData initData;
		initData.m_width = mainRT.GetWidth();
		initData.m_height = mainRT.GetHeight();
		initData.m_colorBufferFormat[0] = mainRT.GetColorBufferFormat();
		initData.m_fxFilePath = "Assets/shader/tonemap.fx";
		initData.m_psEntryPoinFunc = "PSFinal";
		initData.m_expandConstantBuffer = &m_toneMapParam;
		initData.m_expandConstantBufferSize = sizeof(m_toneMapParam);
		initData.m_textures[0] = &mainRT.GetRenderTargetTexture();
		initData.m_textures[1] = &m_avgRt[0].GetRenderTargetTexture();
		initData.m_textures[2] = &m_avgRt[1].GetRenderTargetTexture();
		m_finalSprite.Init(initData);
	}
}

void ToneMap::OnRender(RenderContext& rc, RenderTarget& mainRT)
{
	if (!m_isEnable) {
		return;
	}
	CalcLuminanceAvarage(rc);

	// ���Ï���
	m_toneMapParam.currentAvgTexNo = m_currentAvgRt;
	m_toneMapParam.deltaTime = GameTime().GetFrameDeltaTime();
	rc.WaitUntilToPossibleSetRenderTarget(m_avgRt[m_currentAvgRt]);
	// �����_�����O�^�[�Q�b�g��ݒ�
	rc.SetRenderTargetAndViewport(m_avgRt[m_currentAvgRt]);

	if (m_isFirstWhenChangeScene) {
		m_changeSceneTimer -= GameTime().GetFrameDeltaTime();
		if (m_changeSceneTimer < 0.0f) {
			// �V�[���؂�ւ��I���B
			m_isFirstWhenChangeScene = false;
		}
		m_calcAdapteredLuminanceFirstSprite.Draw(rc, GraphicsEngineObj()->GetCamera2D().GetViewMatrix(), GraphicsEngineObj()->GetCamera2D().GetProjectionMatrix());
	}
	else {
		m_calcAdapteredLuminanceSprite.Draw(rc, GraphicsEngineObj()->GetCamera2D().GetViewMatrix(), GraphicsEngineObj()->GetCamera2D().GetProjectionMatrix());
	}

	// �����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
	rc.WaitUntilFinishDrawingToRenderTarget(m_avgRt[m_currentAvgRt]);


	//// �ŏI�����B
	//// �����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���܂ő҂�
	//rc.WaitUntilToPossibleSetRenderTarget(m_finalRt);
	//// �����_�����O�^�[�Q�b�g��ݒ�
	//rc.SetRenderTargetAndViewport(m_finalRt);

    // �����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���܂ő҂�
    rc.WaitUntilToPossibleSetRenderTarget(mainRT);
    // �����_�����O�^�[�Q�b�g��ݒ�
    rc.SetRenderTargetAndViewport(mainRT);

	m_finalSprite.Draw(rc, GraphicsEngineObj()->GetCamera2D().GetViewMatrix(), GraphicsEngineObj()->GetCamera2D().GetProjectionMatrix());

	// �����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
	rc.WaitUntilFinishDrawingToRenderTarget(mainRT);

	m_currentAvgRt = 1 ^ m_currentAvgRt;
}

void ToneMap::CalcLuminanceAvarage(RenderContext& rc)
{
	// �V�[���̋P�x�̕��ς��v�Z���Ă����B
	for (int spriteNo = 0; spriteNo < enNumCalcAvgSprite; spriteNo++) {

		// �����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���܂ő҂�
		rc.WaitUntilToPossibleSetRenderTarget(m_calcAvgRt[spriteNo]);
		// �����_�����O�^�[�Q�b�g��ݒ�
		rc.SetRenderTargetAndViewport(m_calcAvgRt[spriteNo]);
		rc.ClearRenderTargetView(m_calcAvgRt[spriteNo].GetRTVCpuDescriptorHandle(), CLEARCOLOR);

		GetSampleOffsets_DownScale4x4(
			m_calcAvgSprites[spriteNo].GetTextureWidth(0),
			m_calcAvgSprites[spriteNo].GetTextureHeight(0),
			m_avSampleOffsets
		);

		m_calcAvgSprites[spriteNo].Draw(rc, GraphicsEngineObj()->GetCamera2D().GetViewMatrix(), GraphicsEngineObj()->GetCamera2D().GetProjectionMatrix());

		// �����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
		rc.WaitUntilFinishDrawingToRenderTarget(m_calcAvgRt[spriteNo]);
	}
}
