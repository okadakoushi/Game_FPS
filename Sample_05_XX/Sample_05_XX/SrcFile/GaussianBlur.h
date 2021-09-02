#pragma once
class GaussianBlur : public Noncopyable
{
public:
	/// <summary>
	/// �������B
	/// </summary>
	/// <param name="originalTexure">�K�E�V�A���u���[���|����I���W�i���e�N�X�`���B</param>
	/// <param name="isBlurAlpha"></param>
	/// <param name="isDownSample"></param>
	void Init(Texture* originalTexure, bool isBlurAlpha = false, bool isDownSample = true);
	/// <summary>
	/// �K�E�V�A���u���[��GPU��Ŏ��s�B
	/// <para>�{�֐��Ăяo���́ADirectX12�𗘗p�����`��R�}���h�������ɌĂяo���B</para>
	/// </summary>
	/// <param name="rc"></param>
	/// <param name="blurPower"></param>
	void ExecuteOnGPU(RenderContext& rc, float blurPower);
	/// <summary>
	/// �{�P�e�N�X�`�����擾�B
	/// </summary>
	/// <returns></returns>
	Texture& GetBokeTexture()
	{
		return m_yBlurRenderTarget.GetRenderTargetTexture();
	}
private:
	/// <summary>
	/// RT�������B
	/// </summary>
	void InitRenderTarget(bool isDownSample);
	/// <summary>
	/// �X�v���C�g�������B
	/// </summary>
	void InitSprite(bool isBlurAlpha);
	/// <summary>
	/// �d�݂��X�V�B
	/// </summary>
	/// <param name="blurPower"></param>
	void UpdateWeightsTable(float blurPower);
private:
	enum { NUM_WEIGHTS = 8 };				//�d�݂̐��B
	float m_weights[NUM_WEIGHTS];			//�d�݂̃e�[�u���B
	Texture* m_originalTexture = nullptr;	//�I���W�i���e�N�X�`���B
	RenderTarget m_xBlurRenderTarget;		//���{�P�B
	Sprite m_xBlurSprite;
	RenderTarget m_yBlurRenderTarget;		//�c�{�P�B
	Sprite m_yBlurSprite;
};

