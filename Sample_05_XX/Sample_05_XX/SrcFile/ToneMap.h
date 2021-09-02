#pragma once

/// <summary>
/// �g�[���}�b�v�B
/// �ǂ̖��邳�ł����ʂ����ɂ��āA�l�Ԃ̎���ɋ߂Â���
/// </summary>
class ToneMap
{
public:
	ToneMap();
	~ToneMap();
	/// <summary>
	/// �������B
	/// </summary>
	/// <param name="mainRT">�g�[���}�b�v���s���A�x�[�X�ƂȂ�Rt�B</param>
	void Init(RenderTarget& mainRT);
	/// <summary>
	/// �`��B
	/// </summary>
	/// <param name="rc">�����_�\�R���e�L�X�g�B</param>
	/// <param name="mainRenderTarget">���C�������_�\�^�[�Q�b�g�B</param>
	void OnRender(RenderContext& rc, RenderTarget& mainRT);
private:
	/// <summary>
	/// ���ϋP�x���v�Z����B
	/// </summary>
	/// <param name="">�����_�����O�R���e�L�X�g</param>
	void CalcLuminanceAvarage(RenderContext& rc);
private:
	static const int MAX_SAMPLES = 16;
	/// <summary>
	/// �g�[���}�b�v�p�����[�^�[�B
	/// </summary>
	struct STonemapParam {
		float deltaTime;
		float midddleGray;
		int currentAvgTexNo;
	};

	enum ClacAvgSprite {
		enCalcAvg_Log,					//�ΐ����ς����߂�B
		enCalcAvg_Start,
		enClacAvg_0 = enCalcAvg_Start,	//���ϋP�x���v�Z�B
		enClacAvg_1,					//���ϋP�x���v�Z�B
		enClacAvg_2,					//���ϋP�x���v�Z�B
		enClacAvg_3,					//���ϋP�x���v�Z�B
		enClacAvg_End,
		enCalcAvgExp = enClacAvg_End,	//
		enNumCalcAvgSprite
	};

	RenderTarget	m_calcAvgRt[enNumCalcAvgSprite];		//���ϋP�x�v�Z�pRT
	RenderTarget	m_avgRt[2];								//���ϋP�x���i�[�����RT�i2���ڂ̕��ɋP�x���߂Â���j
	int				m_currentAvgRt = 0;						//���݂̕���RT�B
	Sprite			m_calcAvgSprites[enNumCalcAvgSprite];	//�P�x
	Sprite			m_calcAdapteredLuminanceSprite;			//���Ï����p�̃X�v���C�g�B
	Sprite			m_calcAdapteredLuminanceFirstSprite;	//���Ï����p�̃X�v���C�g�i�V�[�����؂�ւ�������Ɏg�p�j
	Sprite			m_finalSprite;							//�ŏI�����p�X�v���C�g�B

	bool			m_isFirstWhenChangeScene = true;		//�V�[�����؂�ւ���ď���̕`�悩�ǂ����B
	Vector4			m_avSampleOffsets[MAX_SAMPLES];
	RenderTarget	m_finalRt;								//�ŏI���������_�����O�B
	STonemapParam	m_toneMapParam;							//�g�[���}�b�v�p�̃p�����[�^�[�B
	bool			m_isEnable = true;						//toneMap�L���H
	float			m_changeSceneTimer = 0.5f;				//�V�[���؂�ւ��̃^�C�}�[�B
};