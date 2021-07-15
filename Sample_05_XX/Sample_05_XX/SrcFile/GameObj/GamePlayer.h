#pragma once

class GameCamera;
class Rifle;
class Bullet;

#include "SrcFile/RayTestCallBack.h"

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
	void PostUpdate() override;
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

	void GetRHandPos(Vector3& vec, Quaternion& qRot) const
	{
		vec = m_rHandBonePos;
		qRot = m_rHandBoneRot;
	}
	/// <summary>
	/// レンダー取得。
	/// </summary>
	/// <returns></returns>
	SkinModelRender* GetRender()
	{
		return m_unityChan;
	}
	void SetPos(Vector3& pos)
	{
		m_pos = pos;
		m_cCon.SetPosition(m_pos);
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
	Quaternion m_rot = g_quatIdentity;			//回転。
	Rifle* m_wepon = nullptr;					//武器。
	Bone* m_spineBone = nullptr;				//腰のボーン。
	Bone* m_rHandBone = nullptr;				//右手のボーン。
	Bone* m_headBone = nullptr;					//頭ボーン。
	Vector3 m_headPos = g_vec3Zero;				//頭の位置。
	Vector3 m_rHandBonePos = g_vec3Zero;
	Quaternion m_rHandBoneRot = g_quatIdentity;	
	Vector3 m_move = g_vec3Zero;				//移動。
	float m_speed = 100.0f;						//移動速度。
	float const m_JUMPFORSE = 520.0f;			//ジャンプ。
	float const m_GRAVITY = 20.0f;				//重力。
	const float fixYToEyePos = 110.0f;			//視点座標に変えるY軸修正。
	int m_flame = 0;
	GameCamera* m_camera = nullptr;				//カメラ。
	myEngine::Effect* m_effect = nullptr;		//エフェクト。
	const float RAY_RANGE = 2000.0f;
	int m_hp = 150;								//HP。
};

