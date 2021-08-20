#pragma once

/// <summary>
/// フェード。
/// スプライトのα値を調整して行う。
/// </summary>
class Fade : public IGameObject
{
public:
	Fade();
	~Fade();
	bool Start() override;
	void Update() override;
	/// <summary>
	/// フェード開始。
	/// </summary>
	void StartFade(const bool& FadeIn);
	/// <summary>
	/// フェードが終わってるかのフラグを取得。
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
	SpriteRender* m_fade = nullptr;		//フェード用のスプライト。
	const float FADE_STRIDE = 3.0f;	//フェード間隔。
	float m_currentTime = 0.0f;			//現在のタイマー時間。
	float m_fadeColorAlpha = 0.0f;		//フェードカラーのα。
	bool m_isFinishFade = true;			//フェードが終了してるか。
	bool m_isFadeIn = false;			//
};

