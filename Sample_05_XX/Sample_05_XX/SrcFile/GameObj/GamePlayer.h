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
	/// <summary>
	/// 回転。
	/// <para>都合によりGameCameraより呼び出し。</para>
	/// </summary>
	void Rotation();
	/// <summary>
	/// 発射！！
	/// </summary>
	void Shot();
	/// <summary>
	/// 位置を取得。
	/// </summary>
	/// <returns></returns>
	const Vector3& GetPos()const
	{
		return m_pos;
	}
private:
	enum EnPlayerState {
		EnPlayerState_Idle,		//何もない。
		EnPlayerState_Walk,		//歩く。
		EnPlayerState_Run,		//走る。
		EnPlayerState_Shot,		//射撃
		EnPlayerState_Reload,	//リロード。
		EnPlayerState_Deth,		//死亡。
		EnPlayerState_Num		//数。
	};
	EnPlayerState m_playerState = EnPlayerState_Idle;
	SkinModelRender* m_unityChan = nullptr;		//ユニティーちゃん。
	Vector3 m_pos = g_vec3Zero;					//位置。
	Quaternion m_rot = g_quatIdentity;			//回転。
	GameCamera* m_camera = nullptr;				//ゲームカメラ。
	float m_speed = 1.0f;						//移動速度。
	int m_flame = 0;
};

