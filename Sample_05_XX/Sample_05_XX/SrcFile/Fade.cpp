#include "stdafx.h"
#include "Fade.h"

Fade::Fade()
{
}

Fade::~Fade()
{
}

bool Fade::Start()
{
	//初期化データ。
	SpriteInitData initData;
	initData.m_width = FRAME_BUFFER_W;
	initData.m_height = FRAME_BUFFER_H;
	initData.m_fxFilePath = "Assets/shader/sprite.fx";
	initData.m_ddsFilePath[0] = "Assets/sprite/fade.dds";
	//初期化。
	m_fade = NewGO<SpriteRender>(EnPriority_Fade);
	m_fade->Init(initData);
	m_fade->SetMulColor({ 1.0f, 1.0f, 1.0f, m_fadeColorAlpha });
	return true;
}

void Fade::Update()
{
	if (!m_isFinishFade) {
		//フェード中。
		m_currentTime += GameTime().GetFrameDeltaTime();

		if (m_isFadeIn) {
			//fedeIn
			Entry(m_currentTime);
		}
		else {
			//fadeOut
			Leave(m_currentTime);
		}

		m_fade->SetMulColor({ 1.0f, 1.0f, 1.0f, m_fadeColorAlpha });

		if (m_currentTime >= FADE_STRIDE) {
			//フェード終了！
			m_isFinishFade = true;
			this->SetActive(false);
			m_currentTime = 0.0f;
		}
	}
}

void Fade::StartFade(const bool& FadeIn)
{
	if (m_isFadeIn == FadeIn) {
		//同じなので特にフェードを行わない。
		return;
	}

	m_isFadeIn = FadeIn;

	m_isFinishFade = false;
	this->SetActive(true);
}

void Fade::Entry(float& time)
{
	//fadeIn。
	m_fadeColorAlpha = m_currentTime / FADE_STRIDE;
}

void Fade::Leave(float& time)
{
	//fadeOut。
	m_fadeColorAlpha = 1.0f - m_currentTime / FADE_STRIDE;
}

