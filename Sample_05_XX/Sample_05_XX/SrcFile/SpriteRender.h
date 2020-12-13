#pragma once

class Sprite;

/// <summary>
/// スプライトレンダー。
/// </summary>
class SpriteRender : public IGameObject
{
public:
	~SpriteRender();
	/// <summary>
	/// 初期化。
	/// </summary>
	void Init(const SpriteInitData& spriteData);
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
private:
	Sprite m_sprite;			//スプライト。
	bool m_isInited = false;	//一応初期化フラグ。
	Vector3 m_pos = g_vec3Zero;				//位置。
	Quaternion m_rot = g_quatIdentity;		//回転。
	Vector3 m_scale = g_vec3One;			//拡大。
	Vector2 m_pivot = Sprite::DEFAULT_PIVOT;//ピボット。
};

