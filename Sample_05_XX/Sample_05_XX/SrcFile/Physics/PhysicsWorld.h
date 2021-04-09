#pragma once

/// <summary>
/// フィジックスワールド。
/// </summary>
class PhysicsWorld
{
private:
	std::unique_ptr<btDefaultCollisionConfiguration>		m_collisionConfig;
	std::unique_ptr<btCollisionDispatcher>					m_collisionDispatcher;	//衝突解決。
	std::unique_ptr<btBroadphaseInterface>					m_overlappingPairCache;	//ブロードフェーズ。衝突判定の枝切。
	std::unique_ptr<btSequentialImpulseConstraintSolver>	m_constrainSolver;		//コンストレイントソルバー。拘束条件の解決処理。
	std::unique_ptr<btDiscreteDynamicsWorld>				m_dynamicWorld;			//ワールド。

public:
	~PhysicsWorld();
	/// <summary>
	/// 初期化。
	/// </summary>
	void Init();
	/// <summary>
	/// 更新。
	/// </summary>
	void Update();
	/// <summary>
	/// 解放。
	/// </summary>
	void Release();
	/// <summary>
	/// 重力を設定。
	/// </summary>
	/// <param name="g"></param>
	void SetGravity(Vector3& g)
	{
		m_dynamicWorld->setGravity(btVector3(g.x, g.y, g.z));
	}
	/// <summary>
	/// ダイナミックワールドを取得。
	/// </summary>
	/// <returns></returns>
	btDiscreteDynamicsWorld* GetDynamicWorld()
	{
		return m_dynamicWorld.get();
	}
	/// <summary>
	/// コリジョンオブジェクトをワールドに登録。
	/// </summary>
	/// <param name="colObj">コリジョンオブジェクト。</param>
	void AddCollisionObject(btCollisionObject& colObj)
	{
		m_dynamicWorld->addCollisionObject(&colObj);
	}
	/// <summary>
	/// コリジョンオブジェクトをワールドから削除。
	/// </summary>
	/// <param name="colObj"></param>
	void RemoveCollisionObject(btCollisionObject& colObj)
	{
		m_dynamicWorld->removeCollisionObject(&colObj);
	}
	/// <summary>
	/// 形状に対する衝突テスト。
	/// </summary>
	/// <param name="castShape">形状。</param>
	/// <param name="convexFromWorld">fromPos。</param>
	/// <param name="convexToWorld">ToPos。</param>
	/// <param name="resultCallBack">結果。</param>
	/// <param name="allowedCcdPanetration">侵入できる範囲？</param>
	void ConvexSweepTest(
		const btConvexShape* castShape,
		const btTransform& convexFromWorld,
		const btTransform& convexToWorld,
		btCollisionWorld::ConvexResultCallback& resultCallBack,
		btScalar allowedCcdPanetration = 0.0f
	)
	{
		m_dynamicWorld->convexSweepTest(castShape, convexFromWorld, convexToWorld, resultCallBack, allowedCcdPanetration);
	}

	void ContactTest(
		btCollisionObject* colObj,
		std::function<void(const btCollisionObject& contactCollisionObject)> cb
	);
};

