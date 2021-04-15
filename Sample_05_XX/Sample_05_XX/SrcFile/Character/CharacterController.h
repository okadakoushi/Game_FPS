#pragma once

#include "SrcFile/Physics/CapsuleCollider.h"
#include "SrcFile/Physics/RigidBody.h"

/// <summary>
/// キャラクターコントローラー。
/// </summary>
class CharacterController
{
public:
	CharacterController() {};
	~CharacterController() {
		
	};
	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="radius">カプセルコライダーの半径。</param>
	/// <param name="height">カプセルコライダーの高さ。</param>
	/// <param name="position">初期位置。</param>
	void Init(float radius, float height, const Vector3& position);
	/// <summary>
	/// 実行。
	/// </summary>
	/// <param name="moveSpeed">移動速度。</param>
	/// <param name="deltaTime">経過時間。</param>
	/// <returns></returns>
	const Vector3& Execute(Vector3& moveSpeed, float deltaTime = GameTime().GetFrameDeltaTime());
	/// <summary>
	/// 位置を取得。
	/// </summary>
	/// <returns></returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}
	/// <summary>
	/// 位置を設定。
	/// </summary>
	/// <param name="pos"></param>
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}
	/// <summary>
	/// ジャンプ中か判定。
	/// </summary>
	/// <returns></returns>
	bool IsJump() const
	{
		return m_isJump;
	}
	/// <summary>
	/// 地面上にいるか判定。
	/// </summary>
	/// <returns></returns>
	bool IsOnGround() const
	{
		return m_isOnGround;
	}
	/// <summary>
	/// コライダーを取得。
	/// </summary>
	/// <returns></returns>
	CapsuleCollider* GetCollider()
	{
		return &m_collider;
	}
	/// <summary>
	/// 剛体を取得。
	/// </summary>
	/// <returns></returns>
	RigidBody* GetRigidBody()
	{
		return &m_rigidBody;
	}
	/// <summary>
	/// 剛体を物理エンジンから削除。
	/// </summary>
	void RemoveRigidBody();
private:
	bool			m_isInited = false;			//初期化済みフラグ。
	Vector3			m_position = g_vec3Zero;	//座標。
	bool			m_isJump = false;			//ジャンプ。
	bool			m_isOnGround = true;		//地面にいる？
	CapsuleCollider	m_collider;					//コライダー。
	float			m_radius = 0.0f;			//カプセルの半径。
	float			m_height = 0.0f;			//カプセルの高さ。
	RigidBody		m_rigidBody;				//剛体。
};

