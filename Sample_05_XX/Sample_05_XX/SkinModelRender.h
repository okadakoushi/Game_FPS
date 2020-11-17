#pragma once

enum EnRenderMode {
	enRenderMode_Skin,				//スキンあり。
	enRenderMode_NonSkin,			//スキンなし。
	enRenderMode_DrawShadow,		//シャドウマップ描画。
	enRenderMode_NonSkinDrawShadow,	//スキンなしシャドウマップ描画。
	enRenderMode_Num				//いらんきもするが一応。
};


/// <summary>
/// スキンモデルレンダー。
/// </summary>
/// <code>
/// 1.NewGO();
/// 2.(if ForwardRender)SetShader() & SetForwardRender todo:wrap;
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
	~SkinModelRender();
	/// <summary>
	/// スタート。
	/// </summary>
	bool Start() override;
	/// <summary>
	/// 初期化。
	/// </summary>
	void Init(const char* tkmFile, const char* tkaFile = nullptr);
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
		m_model.PlayAnim(clipNo, interpolateTime);
	}
	/// <summary>
	/// 座標を設定。
	/// </summary>
	/// <param name="pos"></param>
	void SetPosition(Vector3& pos)
	{
		m_pos = pos;
	}
	/// <summary>
	/// 回転を設定。
	/// </summary>
	/// <param name="qRot"></param>
	void SetRotation(Quaternion& qRot)
	{
		m_rot = qRot;
	}
	/// <summary>
	/// 拡大率を設定。
	/// </summary>
	/// <param name="scale"></param>
	void SetScale(Vector3& scale)
	{	
		m_scale = scale;
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
		m_model.SetShadowReciever();
	}
	/// <summary>
	/// シャドウキャスターフラグを設定。
	/// </summary>
	/// <param name="flag">フラグ。</param>
	void SetShadwoCaster(bool flag)
	{
		ShadowCaster = flag;
	}
	/// <summary>
	/// フォワードレンダー描画にする？
	/// <para>特殊なレンダリングを行う場合はTrueにする。</para>
	/// <para>todo:Shaderも一緒にdeferred用に変更。</para>
	/// </summary>
	/// <param name="flag">フラグ。</param>
	void SetForwardRender(bool flag)
	{
		m_isForwardRender = flag;
	}
	/// <summary>
	/// シェーダーを設定。
	/// </summary>
	/// <param name="fxPath">シェーダーのパス。</param>
	void SetShader(const wchar_t* fxPath)
	{
		m_model.SetShader(fxPath);
	}
	/// <summary>
	/// モデルを取得。
	/// </summary>
	/// <returns></returns>
	Model& GetModel() 
	{
		return m_model;
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
	std::vector<std::string>		m_tkaFilePaths;		//tkaファイルのファイルパスリスト。
	std::vector<AnimationClipPtr>	m_animationClips;	//アニメーションクリップ。
	Animation						m_animation;		//アニメーション
	std::string						m_filePath;			//tkmファイルパス。
	bool ShadowCaster = false;							//シャドウキャスター。
	Vector3 m_pos = g_vec3Zero;			//座標。
	Quaternion m_rot = g_quatIdentity;	//回転。
	Vector3 m_scale = g_vec3One;		//拡大。
	EnRenderMode m_renderMode = enRenderMode_NonSkin;	//描画モード。
	EnModelInitStep m_initStep = enInitStep_None;		//初期化ステップ。
	bool m_isForwardRender = false;		//フォワードレンダリングする？
};

