#pragma once

class GameCamera;
class Rifle;

/// <summary>
/// プレイヤークラス。
/// </summary>
class GamePlayer : public IGameObject
{
public:
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
	/// <summary>
	/// レンダー取得。
	/// </summary>
	/// <returns></returns>
	SkinModelRender* GetRender()
	{
		return m_unityChan;
	}

	void SetCamera(GameCamera* camera)
	{
		m_camera = camera;
	}
private:
	enum EnPlayerState {
		EnPlayerState_Idle,		//何もない。
		EnPlayerState_Walk,		//歩く。
		EnPlayerState_Run,		//走る。
		EnPlayerState_Buck,		//後退。
		EnPlayerState_Shot,		//射撃
		EnPlayerState_Reload,	//リロード。
		EnPlayerState_Deth,		//死亡。
		EnPlayerState_Num		//数。
	};
	EnPlayerState m_playerState = EnPlayerState_Idle;
	SkinModelRender* m_unityChan = nullptr;		//ユニティーちゃん。
	CharacterController m_cCon;					//キャラコン。
	SpriteRender* m_reticle = nullptr;			//レティクル。
	Vector3 m_pos = g_vec3Zero;					//位置。
	Vector3 m_cameraPos = g_vec3Zero;			//カメラの位置。
	Quaternion m_rot = g_quatIdentity;			//回転。
	GameCamera* m_camera = nullptr;				//ゲームカメラ。
	Rifle* m_wepon = nullptr;					//武器。
	Vector3 m_move = g_vec3Zero;				//移動。
	float m_speed = 100.0f;						//移動速度。
	float const m_JUMPFORSE = 800.0f;			//ジャンプ。
	float const m_GRAVITY = 30.0f;				//重力。
	const float fixYToEyePos = 110.0f;			//視点座標に変えるY軸修正。
	int m_flame = 0;
};

