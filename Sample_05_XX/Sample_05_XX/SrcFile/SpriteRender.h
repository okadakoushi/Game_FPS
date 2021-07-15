#pragma once

class Sprite;

/// <summary>
/// スプライトレンダー。
/// todo:isDraw3Dあるが機能してないから直す。
/// </summary>
class SpriteRender : public IGameObject
{
public:
	~SpriteRender();
	/// <summary>
	/// 初期化。
	/// <para>isDraw機能動いてないかも。</para>
	/// </summary>
	/// <param name="spriteData">初期化データー。</param>
	/// <param name="isDraw3D">3D描画する？</param>
	void Init(const SpriteInitData& spriteData, bool isDraw3D = false);
	/// <summary>
	/// スタート。
	/// </summary>
	/// <returns></returns>
	bool Start() override;
	/// <summary>
	/// 更新。
	/// </summary>
	void Update() override;
	/// <summary>
	/// 通常描画。
	/// </summary>
	void ForwardRender() override;
	/// <summary>
	/// HUD描画。
	/// </summary>
	void RenderHUD() override;
	/// <summary>
	/// 位置調整。
	/// </summary>
	/// <param name="v">位置。</param>
	void SetPos(const Vector3& v)
	{
		m_pos = v;
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
	/// スケール調整。
	/// </summary>
	/// <param name="scale">スケール。</param>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}
	/// <summary>
	/// ピボット調整。
	/// </summary>
	/// <param name="pivot">ピボット。</param>
	void SetPivot(const Vector2& pivot)
	{
		m_pivot = pivot;
	}
	/// <summary>
	/// 乗算カラーを設定。
	/// </summary>
	/// <param name="color"></param>
	void SetMulColor(const Vector4& color)
	{
		m_sprite.SetMulColor(color);
	}
	/// <summary>
	/// ビューを設定。
	/// </summary>
	/// <param name="view"></param>
	void SetView(const Matrix& view) 
	{
		m_view = view;
	}
	/// <summary>
	/// プロジェ。
	/// </summary>
	/// <param name="proj"></param>
	void SetProj(const Matrix& proj)
	{
		m_proj = proj;
	}
	/// <summary>
	/// 位置取得。
	/// </summary>
	/// <returns></returns>
	const Vector3& GetPos() const
	{
		return m_pos;
	}
	/// <summary>
	/// テクスチャを取得。
	/// </summary>
	/// <param name="index">添え字。</param>
	/// <returns></returns>
	Texture* GetTexture(int index) 
	{
		return m_sprite.GetTexture(index);
	}
private:
	Sprite m_sprite;			//スプライト。
	bool m_isInited = false;	//一応初期化フラグ。
	Vector3 m_pos = g_vec3Zero;				//位置。
	Quaternion m_rot = g_quatIdentity;		//回転。
	Vector3 m_scale = g_vec3One;			//拡大。
	Vector2 m_pivot = Sprite::DEFAULT_PIVOT;//ピボット。
	bool m_isDraw3D = false;					//3D描画？
	Matrix m_view;							//ビュー。
	Matrix m_proj;							//プロジェ。
};

