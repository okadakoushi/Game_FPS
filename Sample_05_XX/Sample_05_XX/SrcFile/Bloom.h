#pragma once

#include "SrcFile/GaussianBlur.h"

class Bloom : Noncopyable
{
public:
	/// <summary>
	/// �������B
	/// </summary>
	/// <param name="mainRT"></param>
	void Init(RenderTarget& mainRT);
	/// <summary>
	/// �`��B
	/// </summary>
	/// <param name="rc"></param>
	/// <param name="mainRT"></param>
	void Render(RenderContext& rc, RenderTarget& mainRT);
	/// <summary>
	/// �|�X�g�G�t�F�N�g�����s�������ʂƂȂ�e�N�X�`���B
	/// </summary>
	/// <returns></returns>
	Texture& GetResultTexture()
	{
		return m_luminanceRenderTarget.GetRenderTargetTexture();
	}

private:
	RenderTarget m_luminanceRenderTarget;	//�P�x���o�pRT�B
	Sprite m_luminanceSprite;				//�P�x���o�pSprite�B
	GaussianBlur m_gaussianBlur[4];			//�K�E�V�A���u���[�B
	Sprite m_finalSprite;					//�ŏI�����X�v���C�g�B
	float m_clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
};

