#pragma once

/// <summary>
/// スキンモデルレンダー。
/// </summary>
/// <code>
/// 1.NewGO();
/// 2.(if ForwardRender)SetForwardRender;
/// 3.Init();
/// 4.(if Needed)Set~~();
/// 5.(if Shadow)SetShadow~~();
/// 6.(if Anim)PlayAnimation();
/// 7.(if ChangeRM)SetRenderMode();
/// </code>
class SkinModelRender : public IGameObject
{
public:
	SkinModelRender();
	/// <summary>
	/// スタート。
	/// </summary>
	bool Start() { 		//アニメーション初期化。
		return true;
	};
	/// <summary>
	/// 初期化。
	/// <para>todo:T黒魔術使わない修正。</para>
	/// </summary>
	/// <param name="tkmFile">tkmファイルパス。</param>
	/// <param name="tkaFilePaths">tkaファイルパス。</param>
	template<typename TKA_FILE_ARRAY, std::size_t NUM_TKA_FILE>
	void Init(const char* tkmFile, TKA_FILE_ARRAY(&tkaFilePaths)[NUM_TKA_FILE])
	{
		if (!m_isForwardRender) {
			//毎回初期化するので面倒なのでDefferdの場合はここで初期化しちゃう。
			SetDefferedRender();
		}
		//モデル初期化。
		m_model.InitModel(tkmFile);
		//todo : 複数pushBuck
		for (auto i = 0; i < NUM_TKA_FILE; i++) {
			m_tkaFilePaths.push_back(tkaFilePaths[i]);
		}
		//モデルの読み込み終了。
		m_initStep = enInitStep_LoadSkelton;
		m_filePath = tkmFile;
		//スキン描画。
		m_renderMode = enRenderMode_Skin;
		//アニメーション初期化。
		AnimInit();
	}
	void Init(const char* tkmFile) {
		if (!m_isForwardRender) {
			//毎回初期化するので面倒なのでDefferdの場合はここで初期化しちゃう。
			SetDefferedRender();
		}
		//モデル初期化。
		m_model.InitModel(tkmFile);
		//モデルの読み込み終了。
		m_initStep = enInitStep_LoadSkelton;
		m_filePath = tkmFile;
		//ノンスキン描画。
		m_renderMode = enRenderMode_NonSkin;
		//アニメーション初期化。
		AnimInit();
	}

	void AnimInit();
	/// <summary>
	/// デストロイ。
	/// </summary>
	void OnDestroy();
	/// <summary>
	/// 更新。
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画。
	/// </summary>
	void ForwardRender() override;
	/// <summary>
	/// アニメーションを再生。
	/// </summary>
	void PlayAnimation(int clipNo, float interpolateTime)
	{
		m_animation.Play(clipNo, interpolateTime);
	}
	/// <summary>
	/// アニメーションフラグ。
	/// </summary>
	/// <returns></returns>
	const bool& isPlayAnim() const 
	{
		return m_animation.IsPlaying();
	}
	/// <summary>
	/// ループフラグを設定。
	/// </summary>
	/// <param name="animName"></param>
	/// <param name="flag"></param>
	void SetAnimLoop(const int& animNo, const bool& flag)
	{
		m_loopMap[animNo] = flag;
	}
	/// <summary>
	/// 座標を設定。
	/// </summary>
	/// <param name="pos"></param>
	void SetPosition(const Vector3& pos)
	{
		m_pos = pos;
	}
	/// <summary>
	/// アニメーションのウェイトタイムを設定（フレーム）。
	/// </summary>
	/// <param name="wait"></param>
	void SetWaitTimeForAnim(int wait)
	{
		m_waitTime = wait;
	}
	/// <summary>
	/// 座標取得。
	/// </summary>
	/// <returns></returns>
	Vector3& GetPosition()
	{
		return m_pos;
	}
	/// <summary>
	/// 回転を設定。
	/// </summary>
	/// <param name="qRot"></param>
	void SetRotation(const Quaternion& qRot)
	{
		m_rot = qRot;
	}
	/// <summary>
	/// 回転を取得。
	/// </summary>
	/// <returns></returns>
	Quaternion& GetRotation() 
	{
		return m_rot;
	}
	/// <summary>
	/// 拡大率を設定。
	/// </summary>
	/// <param name="scale"></param>
	void SetScale(const Vector3& scale)
	{	
		m_scale = scale;
	}
	/// <summary>
	/// 拡大を取得。
	/// </summary>
	/// <returns></returns>
	Vector3& GetScale()
	{
		return m_scale;
	}
	/// <summary>
	/// レンダーモードの設定。
	/// <para>特に設定しない場合、スキンなし描画。</para>
	/// </summary>
	/// <param name="rm">RenderMode</param>
	void SetRenderMode(const EnRenderMode& rm)
	{
		m_renderMode = rm;
	}
	/// <summary>
	/// レンダーモードを取得。
	/// </summary>
	/// <returns></returns>
	EnRenderMode& GetRenderMode() 
	{
		return m_renderMode;
	}
	/// <summary>
	/// シャドウレシーバーとして登録。
	/// </summary>
	/// <param name="flag">フラグ。</param>
	void SetShadowReciever(bool flag)
	{
		m_model.SetShadowReciever(flag);
	}
	/// <summary>
	/// シャドウキャスターフラグを設定。
	/// </summary>
	/// <param name="flag">フラグ。</param>
	void SetShadwoCaster(bool flag)
	{
		m_model.SetShadwoCaster(flag);
	}
	/// <summary>
	/// フォワードレンダー描画にする。
	/// <para>todo:ZBuffer周りにバグあり。</para>
	/// </summary>
	void SetForwardRender()
	{
		m_isForwardRender = true;
		//フォワードレンダー設定するのでシェーダーも一緒に変更する。
		m_model.SetShader(L"Assets/shader/NoAnimModel_LambertSpecularAmbient.fx");
	}
	/// <summary>
	/// ディファードレンダー描画にする。
	/// <para>通常の描画に戻す際に使用。</para>
	/// </summary>
	void SetDefferedRender()
	{
		m_isForwardRender = false;
		//ディファードレンダー設定するのでシェーダーも一緒に変更する。
		m_model.SetShader(L"Assets/shader/DefeardModel.fx");
	}
	/// <summary>
	/// モデルを取得。
	/// </summary>
	/// <returns></returns>
	Model& GetModel() 
	{
		return m_model;
	}
	/// <summary>
	/// スケルトンを取得。
	/// </summary>
	/// <returns></returns>
	Skeleton& GetSkelton()
	{
		return m_skeleton;
	}
private:
	/// <summary>
	/// モデルの初期化ステップ。
	/// </summary>
	enum EnModelInitStep {
		enInitStep_None,
		enInitStep_LoadModel,
		enInitStep_LoadSkelton,
		enInitStep_LoadAnimation,
		enInitStep_InitAnimationClip,
		enInitStep_Completed,
		initStep,
	};
	Model m_model;						//モデル。
	Skeleton m_skeleton;				//スケルトン。
	std::map<int, bool>	m_loopMap;		//ループフラグ管理マップ。
	std::vector<std::string>		m_tkaFilePaths;		//tkaファイルのファイルパスリスト。
	std::vector<AnimationClipPtr>	m_animationClips;	//アニメーションクリップ。
	std::vector<const char*>		m_noLoopAnimations;	//ループしないアニメーション。
	Animation						m_animation;		//アニメーション
	int m_waitTime = 0;									//アニメーションのウェイトタイム。
	std::string						m_filePath;			//tkmファイルパス。
	Vector3 m_pos = g_vec3Zero;			//座標。
	Quaternion m_rot = g_quatIdentity;	//回転。
	Vector3 m_scale = g_vec3One;		//拡大。
	EnRenderMode m_renderMode = enRenderMode_NonSkin;	//描画モード。
	EnModelInitStep m_initStep = enInitStep_None;		//初期化ステップ。
	bool m_isForwardRender = false;		//フォワードレンダリングする？
};

