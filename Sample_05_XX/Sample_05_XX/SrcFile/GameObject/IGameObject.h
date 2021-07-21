#pragma once

/// <summary>
/// ゲームオブジェクトの基底クラス
/// オブジェクトマネージャーの関数を使う場合は継承してね！！
/// </summary>
/// <remarks>
/// /*---各Renderで使用が望まれるもの---*/
/// PreRender→シャドウマップ、シルエット
/// PostRender→2D、エフェクト、フォント
/// Draw→上記以外
/// /*----------------------------------*/
/// </remarks>


class GameObjectManager;
//名前変更ししてるだけ
typedef unsigned char	GameObjPrio;
class IGameObject : Noncopyable
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	IGameObject() :
		m_priority(0),
		m_isStart(false),
		m_isDead(false),
		m_isActive(true),
		m_isUpdate(true)
	{
	};
	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <remarks>
	/// virtualは定義してるだけです、処理は派生クラスに書いてください
	/// </remarks>
	virtual ~IGameObject()
	{
	};
	/// <summary>
	/// <para>Updateの直前に呼ばれる開始処理</para>
	/// <para>trueが返されると更新処理(Update)が開始されるよ</para>
	/// </summary>
	/// <returns>
	/// オブジェクトの準備が1フレームでできるならtrue、できていないならfalse
	/// </returns>
	virtual bool Start() { return true; }
	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() {};
	/// <summary>
	/// 遅延更新。
	/// </summary>
	virtual void PostUpdate() {};
	/// <summary>
	/// オンデストロイ。
	/// <para>DeleteGOすると呼び出し。</para>
	/// </summary>
	virtual void OnDestroy() {};
	/// <summary>
	/// フォワードレンダリング。
	/// <para>半透明オブジェクトを描画する際に使用してください。</para>
	/// <para>todo:奥からオブジェクトを書くようにソートする？</para>
	/// </summary>
	virtual void ForwardRender() {};
	/// <summary>
	/// HUD(最終的に表示される画面)を描画。
	/// </summary>
	virtual void RenderHUD()
	{
	}
	/// <summary>
	/// 優先度取得
	/// </summary>
	/// <returns>優先度</returns>
	GameObjPrio GetPriority() const
	{
		return m_priority;
	}


	virtual bool IsStart() const
	{
		return m_isStart;
	}
	/*!
	*@brief アクティブかどうかを判定。
	*/
	bool IsActive() const
	{
		return m_isActive;
	}
	void SetActive(bool act) {
		m_isActive = act;
	}

	void SetUpdate(bool Up) {
		m_isUpdate = Up;
	}
	bool IsUpdate() {
		return m_isUpdate;
	}
public:
	/// <summary>
	/// 関数をラップしてるだけだよ
	/// 各オブジェクトの関数の処理を開始させるよ
	/// </summary>
	void StartWrapper()
	{
		if (m_isActive && !m_isStart)
		{
			if (Start()) {
				//初期化終了
				m_isStart = true;
			}
		}
	}
	void UpdateWrapper()
	{
		if (m_isActive && m_isStart && m_isUpdate && !m_isDead)
		{
			Update();
		}
	}
	void PostUpdateWrapper()
	{
		if (m_isActive && m_isStart && m_isUpdate && !m_isDead)
		{
			PostUpdate();
		}
	}
	void ForwardRenderWrapper()
	{
		if (m_isActive && m_isStart && !m_isDead)
		{
			ForwardRender();
		}
	}
	void RenderHUDWrapper()
	{
		if (m_isActive && m_isStart && !m_isDead)
		{
			RenderHUD();
		}
	}
	friend class GameObjectManager;
protected:
	GameObjPrio		m_priority;		//優先度
	bool			m_isStart;		//Startフラグ
	bool			m_isDead;		//死亡フラグ
	bool			m_isUpdate;		//更新フラグ
	bool			m_isActive;		//Activeフラグ
	const char*		m_tags = 0;		//タグ
	unsigned int	m_nameKey = 0;	//名前キー
};


