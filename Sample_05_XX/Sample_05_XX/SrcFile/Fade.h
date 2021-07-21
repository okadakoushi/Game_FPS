#pragma once
class Fade : public IGameObject
{
public:
	Fade();
	~Fade();
	void Entry();
	void Leave();
private:
	SpriteRender* m_fade = nullptr;
	const float FADE_STRIDE = 1.5f;
	float m_currentTime = 0.0f;
};

