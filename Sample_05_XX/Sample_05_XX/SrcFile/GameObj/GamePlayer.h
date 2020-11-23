#pragma once

class GameCamera;

/// <summary>
/// プレイヤークラス。
/// </summary>
class GamePlayer : public IGameObject
{
public:
	~GamePlayer();
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
	/// 移動。
	/// <para>移動はカメラを基準に行われる。</para>
	/// </summary>
	void Move();
private:
	SkinModelRender* m_unityChan = nullptr;		//ユニティーちゃん。
	Vector3 m_pos = g_vec3Zero;					//位置。
	Quaternion m_rot = g_quatIdentity;			//回転。
	GameCamera* m_camera = nullptr;				//ゲームカメラ。
	float m_speed = 1.0f;						//移動速度。
};

