#pragma once

#include "PhysicsObjectBase.h"

class PhysicsStaticObject : public PhysicsObjectBase
{
public:
	~PhysicsStaticObject()
	{
		Release();
	}
	/// <summary>
	/// 解放。
	/// </summary>
	void Release() override final;
	/// <summary>
	/// 座標と回転を設定。
	/// </summary>
	/// <param name="pos">位置。</param>
	/// <param name="rot">回転。</param>
	void SetPositionAndRotation(const Vector3& pos, const Quaternion& rot)
	{
		m_rigidBody.SetPositionAndRotation(pos, rot);
	}
	/// <summary>
	/// 剛体を取得。
	/// </summary>
	/// <returns></returns>
	RigidBody& GetRigidBody()
	{
		return m_rigidBody;
	}
private:
	/// <summary>
	/// 静的物理オブジェクト作成処理の共通処理。
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="rot"></param>
	void CreateCommon(Vector3 pos, Quaternion rot) override final;
private:
	RigidBody m_rigidBody;		//剛体。
};

