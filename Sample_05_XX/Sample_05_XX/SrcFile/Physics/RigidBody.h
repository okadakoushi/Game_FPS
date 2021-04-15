#pragma once

class ICollider;
/// <summary>
/// 剛体情報。
/// </summary>
struct RigidBodyInfo {
	RigidBodyInfo() :
		collider(NULL),
		mass(0.0f)
	{
		pos = g_vec3Zero;
		rot = g_quatIdentity;
		localInteria = g_vec3Zero;
	}
	Vector3 pos;			//座標。
	Quaternion rot;			//回転。
	Vector3 localInteria;	//慣性テンソル。
	ICollider* collider;	//コライダー。
	float mass;				//質量。
};

/// <summary>
/// 剛体。
/// </summary>
class RigidBody
{
	std::unique_ptr<btRigidBody>			m_rigidBody;		//剛体。
	std::unique_ptr<btDefaultMotionState>	m_myMotionState;	//モーションステート。
	bool m_isAddPhysicsWorld = false;					//物理ワールドに追加されているか。
public:
	~RigidBody();
	/// <summary>
	/// 解放。
	/// </summary>
	void Release();
	/// <summary>
	/// 作成。
	/// </summary>
	/// <param name="rbInfo">剛体情報。</param>
	void Create(RigidBodyInfo& rbInfo);
	/// <summary>
	/// 剛体取得。
	/// </summary>
	/// <returns></returns>
	btRigidBody* GetBody()
	{
		return m_rigidBody.get();
	}
	/// <summary>
	/// 物理ワールド登録中の印をつける。
	/// </summary>
	void SetMarkAddPhysicsWorld()
	{
		m_isAddPhysicsWorld = true;
	}
	/// <summary>
	/// 物理ワールド登録中の印を外す。
	/// </summary>
	void SetUnmarkAddPhysicsWorld()
	{
		m_isAddPhysicsWorld = false;
	}
	/// <summary>
	/// 物理ワールドに登録されてる？
	/// </summary>
	/// <returns></returns>
	bool IsAddPhysicsWorld() const
	{
		return m_isAddPhysicsWorld;
	}
	/// <summary>
	/// 物理オブジェクトの座標と回転を取得。
	/// </summary>
	/// <param name="pos">位置。</param>
	/// <param name="rot">回転。</param>
	void GetPositionAndRotation(Vector3& pos, Quaternion& rot) const
	{
		btTransform trans;
		m_myMotionState->getWorldTransform(trans);
		pos.Set(trans.getOrigin());
		rot.Set(trans.getRotation());
	}
	/// <summary>
	/// 位置と回転を設定。
	/// </summary>
	/// <param name="pos">位置。</param>
	/// <param name="rot">回転。</param>
	void SetPositionAndRotation(const Vector3& pos, const Quaternion& rot)
	{
		btTransform trans;
		btVector3 btPos;
		//位置、回転をコピー。
		pos.CopyTo(btPos);
		trans.setOrigin(btPos);
		btQuaternion btRot;
		rot.CopyTo(btRot);
		trans.setRotation(btRot);
		m_rigidBody->setWorldTransform(trans);
	}
	/// <summary>
	/// 指定方向に力を加える。
	/// </summary>
	/// <param name="force">力。</param>
	/// <param name="relPos">力を加える座標。</param>
	void AddForce(const Vector3& force, const Vector3& relPos)
	{
		btVector3 btForce;
		btVector3 btRelPos;
		force.CopyTo(btForce);
		relPos.CopyTo(btRelPos);
		m_rigidBody->applyForce(btForce, btRelPos);
	}
};

