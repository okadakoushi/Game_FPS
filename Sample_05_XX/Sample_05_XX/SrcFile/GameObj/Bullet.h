#pragma once

class GamePlayer;

/// <summary>
/// 弾丸クラス。
/// 色々粗削りだから修正必要。
/// </summary>
class Bullet : public IGameObject
{
public:
	/// <summary>
	/// デストロイ。
	/// </summary>
	void OnDestroy() override;
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
	/// 位置を設定。
	/// <para>NewGOする場合、絶対呼び出すこと。</para>
	/// </summary>
	/// <param name="v">位置。</param>
	void SetPos(const Vector3& v)
	{
		m_pos = v;
	}
private:
	GamePlayer* m_player = nullptr;
	Vector3 m_pos = g_vec3Zero;		//位置。
	Vector3 m_toTarget = g_vec3Zero;//注視点に向かうベクトル。
	float m_LengthOnNew = 0.0f;		//NewGO時の長さ。
	const float m_speed = 5.0f;		//速さ。
	SkinModelRender* m_render = nullptr;
};

