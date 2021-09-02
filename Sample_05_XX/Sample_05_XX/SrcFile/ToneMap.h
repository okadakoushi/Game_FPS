#pragma once

/// <summary>
/// トーンマップ。
/// どの明るさでも光量を一定にして、人間の視野に近づける
/// </summary>
class ToneMap
{
public:
	ToneMap();
	~ToneMap();
	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="mainRT">トーンマップを行う、ベースとなるRt。</param>
	void Init(RenderTarget& mainRT);
	/// <summary>
	/// 描画。
	/// </summary>
	/// <param name="rc">レンダ―コンテキスト。</param>
	/// <param name="mainRenderTarget">メインレンダ―ターゲット。</param>
	void OnRender(RenderContext& rc, RenderTarget& mainRT);
private:
	/// <summary>
	/// 平均輝度を計算する。
	/// </summary>
	/// <param name="">レンダリングコンテキスト</param>
	void CalcLuminanceAvarage(RenderContext& rc);
private:
	static const int MAX_SAMPLES = 16;
	/// <summary>
	/// トーンマップパラメーター。
	/// </summary>
	struct STonemapParam {
		float deltaTime;
		float midddleGray;
		int currentAvgTexNo;
	};

	enum ClacAvgSprite {
		enCalcAvg_Log,					//対数平均を求める。
		enCalcAvg_Start,
		enClacAvg_0 = enCalcAvg_Start,	//平均輝度を計算。
		enClacAvg_1,					//平均輝度を計算。
		enClacAvg_2,					//平均輝度を計算。
		enClacAvg_3,					//平均輝度を計算。
		enClacAvg_End,
		enCalcAvgExp = enClacAvg_End,	//
		enNumCalcAvgSprite
	};

	RenderTarget	m_calcAvgRt[enNumCalcAvgSprite];		//平均輝度計算用RT
	RenderTarget	m_avgRt[2];								//平均輝度が格納されるRT（2枚目の方に輝度を近づける）
	int				m_currentAvgRt = 0;						//現在の平均RT。
	Sprite			m_calcAvgSprites[enNumCalcAvgSprite];	//輝度
	Sprite			m_calcAdapteredLuminanceSprite;			//明暗順応用のスプライト。
	Sprite			m_calcAdapteredLuminanceFirstSprite;	//明暗順応用のスプライト（シーンが切り替わった時に使用）
	Sprite			m_finalSprite;							//最終合成用スプライト。

	bool			m_isFirstWhenChangeScene = true;		//シーンが切り替わって初回の描画かどうか。
	Vector4			m_avSampleOffsets[MAX_SAMPLES];
	RenderTarget	m_finalRt;								//最終合成レンダリング。
	STonemapParam	m_toneMapParam;							//トーンマップ用のパラメーター。
	bool			m_isEnable = true;						//toneMap有効？
	float			m_changeSceneTimer = 0.5f;				//シーン切り替えのタイマー。
};