#pragma once

/// <summary>
/// スキンモデルレンダー。
/// </summary>
/// <code>
/// NewGO();
/// Init();
/// (if Needed)Set~~();
/// Update();
/// Draw();
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
	void Init(const char* filepath);
	/// <summary>
	/// 更新。
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画。
	/// </summary>
	void Draw() override;
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
	/// <param name="rm"></param>
	void SetRenderMode(const RenderMode& rm)
	{
		m_renderMode = rm;
	}
	/// <summary>
	/// シャドウレシーバーとして登録。
	/// </summary>
	void SetShadowReciever()
	{
		m_model.SetShadowReciever();
	}
private:
	Model m_model;						//モデル。
	Vector3 m_pos = g_vec3Zero;			//座標。
	Quaternion m_rot = g_quatIdentity;	//回転。
	Vector3 m_scale = g_vec3One;		//拡大。
	RenderMode m_renderMode = enRenderMode_NonSkin;	//描画モード。
};

