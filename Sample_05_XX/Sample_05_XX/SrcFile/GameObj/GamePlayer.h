#pragma once

class GameCamera;
class Rifle;
class Bullet;
class SoundSource;

class IPlayerState;
class PlayerIdleState;
class PlayerAttackState;
class PlayerDashState;
class PlayerDeathState;
class PlayerReloadState;
class PlayerWalkState;

#include "SrcFile/RayTestCallBack.h"
#include "PlayerUIs.h"

/// <summary>
/// プレイヤークラス。
/// </summary>
class GamePlayer : public IGameObject
{
public:
	//定数値はpublicでも許されるか
	struct BASE_PARAM {
		static const float	PLAYER_MAX_HP;				//プレイヤーの最大体力。
		static const int	REGENE_VALUE_SECOND;				//秒間リジェネ量。
		static const float	REGENE_COOL_TIME;					//リジェネのクールタイム。
		static const float	JUMPFORSE;							//ジャンプ。
		static const float	GRAVITY;							//重力。
	};
	enum EnPlayerAnimation {
		EnPlayerAnimation_Idle,
		EnPlayerAnimation_Walk,
		EnPlayerAnimation_Shoot,
		EnPlayerAnimation_Death,
	};
private:
	~GamePlayer() override;
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
	/// 自己回復。
	/// </summary>
	void Regene();

	void ChangeState(IPlayerState* state);
public:
	void Init();
	/// <summary>
	/// 回転。
	/// <para>1フレームずれるため、カメラ側から呼び出し。</para>
	/// </summary>
	void Rotation();
	/// <summary>
	/// 移動。
	/// <para>移動はカメラを基準に行われる。</para>
	/// </summary>
	void Move();
	/// <summary>
	/// 位置を取得。
	/// </summary>
	/// <returns></returns>
	const Vector3& GetPos()const
	{
		return m_pos;
	}
	/// <summary>
	/// 頭の位置を取得。
	/// </summary>
	/// <returns></returns>
	const Vector3& GetHeadPos() const
	{
		return m_headPos;
	}
	/// <summary>
	/// レンダー取得。
	/// </summary>
	/// <returns></returns>
	SkinModelRender* GetRender()
	{
		return m_unityChan;
	}
	/// <summary>
	/// 位置を設定。
	/// </summary>
	/// <param name="pos"></param>
	void SetPos(const Vector3& pos)
	{
		m_pos = pos;
		m_cCon.SetPosition(m_pos);
	}
	/// <summary>
	/// 移動速度を取得。
	/// </summary>
	/// <returns></returns>
	const float& GetSpeed() const 
	{
		return m_speed;
	}
	/// <summary>
	/// 体力を取得。
	/// </summary>
	/// <returns></returns>
	const float& GetHP() const
	{
		return m_hp;
	}
	/// <summary>
	/// プレイヤーにダメージを与える。
	/// </summary>
	/// <param name="damage"></param>
	void DamageToPlayer(const int& damage);
	/// <summary>
	/// 武器取得。
	/// </summary>
	/// <returns></returns>
	Rifle* GetWepon() 
	{
		return m_wepon;
	}
	/// <summary>
	/// 頭の位置を計算。
	/// </summary>
	/// <returns></returns>
	Vector3& CalcHeadPos();
	/// <summary>
	/// プレイヤーUIを取得。
	/// </summary>
	/// <returns></returns>
	PlayerUIs* GetPlayerUIs()
	{
		return m_playerUIs;
	}
private:
	/// <summary>
	/// SkinModelRenderのアニメーション進行が終わったタイミングで呼ばれる処理。
	/// </summary>
	/// <remarks>
	/// アニメーションを進行して、骨のローカル行列を更新したあとで、
	/// 腰の骨をプログラム的に動かす必要があったので、本関数を追加。
	/// </remarks>
	void OnPostAnimationProgress();
private:
	SkinModelRender* m_unityChan = nullptr;		//ユニティーちゃん。
	CharacterController m_cCon;					//キャラコン。
	SpriteRender* m_reticle = nullptr;			//レティクル。
	Vector3 m_pos = g_vec3Zero;					//位置。
	Quaternion m_rot = g_quatIdentity;			//回転。
	Rifle* m_wepon = nullptr;					//武器。
	Bone* m_spineBone = nullptr;				//腰のボーン。
	Bone* m_rHandBone = nullptr;				//右手のボーン。
	Bone* m_headBone = nullptr;					//頭ボーン。
	Vector3 m_headPos = g_vec3Zero;				//頭の位置。
	Vector3 m_rHandBonePos = g_vec3Zero;
	Quaternion m_rHandBoneRot = g_quatIdentity;	
	Vector3 m_move = g_vec3Zero;				//移動。
	float m_speed = 100.0f;						//移動速度
	GameCamera* m_camera = nullptr;				//カメラ。
	float m_weponRaitTime = 0.0f;					
	float m_damageReactionTime = 0.0f;
	float m_damageEffectValue = 1.0f;

	SoundSource* m_footStepSE = nullptr;		//足音。
	SoundSource* m_beHitSE = nullptr;			//被弾。

	float m_hp = BASE_PARAM::PLAYER_MAX_HP;		//HP。
	PlayerUIs* m_playerUIs;						//UI。
	float m_currentRegeneTime = 0.0f;			//リジェネタイム。

	IPlayerState* m_currentState = nullptr;			//現在のステート。
	PlayerIdleState* m_playerIdleState = nullptr;	//アイドル。
	PlayerAttackState* m_attackState = nullptr;		//攻撃。
	PlayerDeathState* m_deathState = nullptr;		//死亡。
	PlayerReloadState* m_reloadState = nullptr;		//攻撃。
};

