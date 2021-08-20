#pragma once

/// <summary>
/// �t�F�[�h�B
/// �X�v���C�g�̃��l�𒲐����čs���B
/// </summary>
class Fade : public IGameObject
{
public:
	Fade();
	~Fade();
	bool Start() override;
	void Update() override;
	/// <summary>
	/// �t�F�[�h�J�n�B
	/// </summary>
	void StartFade(const bool& FadeIn);
	/// <summary>
	/// �t�F�[�h���I����Ă邩�̃t���O���擾�B
	/// </summary>
	/// <returns></returns>
	bool IsFinishFade()
	{
		return m_isFinishFade;
	}
private:
	void Entry(float& time);
	void Leave(float& time);
private:
	SpriteRender* m_fade = nullptr;		//�t�F�[�h�p�̃X�v���C�g�B
	const float FADE_STRIDE = 3.0f;	//�t�F�[�h�Ԋu�B
	float m_currentTime = 0.0f;			//���݂̃^�C�}�[���ԁB
	float m_fadeColorAlpha = 0.0f;		//�t�F�[�h�J���[�̃��B
	bool m_isFinishFade = true;			//�t�F�[�h���I�����Ă邩�B
	bool m_isFadeIn = false;			//
};

