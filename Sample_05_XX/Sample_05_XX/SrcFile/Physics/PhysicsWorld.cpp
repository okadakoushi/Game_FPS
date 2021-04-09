#include "stdafx.h"
#include "PhysicsWorld.h"

PhysicsWorld::~PhysicsWorld()
{
	Release();
}

void PhysicsWorld::Release()
{
	m_dynamicWorld.reset();
	m_constrainSolver.reset();
	m_overlappingPairCache.reset();
	m_collisionDispatcher.reset();
	m_collisionConfig.reset();
}

void PhysicsWorld::Init()
{
	Release();

	//物理エンジン初期化。
	//...衝突構成には、メモリのデフォルト設定、衝突設定が含まれます。 上級ユーザーは、独自の構成を作成できます。
	m_collisionConfig = make_unique<btDefaultCollisionConfiguration>();
	//デフォルトの衝突ディスパッチャを使用します。 並列処理の場合は、別のディスパッチャーを使用できます（Extras / Bullet MultiThreadedを参照）。
	m_collisionDispatcher = make_unique<btCollisionDispatcher>(m_collisionConfig.get());
	//...btDbvtBroadphaseは、優れた汎用ブロードフェーズです。 btAxis3Sweepを試すこともできます。
	//ブロードフェイズ大まかな当たり判定。全オブジェクトをAABBとして当たり判定するみたいな感じだろうか。
	//ブロードフェイズ後、ナローフェイズという精密な当たり判定もある。
	m_overlappingPairCache = make_unique<btDbvtBroadphase>();
	//...デフォルトの制約ソルバー。 並列処理の場合は、別のソルバーを使用できます（Extras / BulletMultiThreadedを参照）。
	m_constrainSolver = make_unique<btSequentialImpulseConstraintSolver>();

	m_dynamicWorld = make_unique<btDiscreteDynamicsWorld>(
		m_collisionDispatcher.get(),
		m_overlappingPairCache.get(),
		m_constrainSolver.get(),
		m_collisionConfig.get()
		);

	//重力設定。
	m_dynamicWorld->setGravity(btVector3(0.0f, -10.0f, 0.0f));
}

void PhysicsWorld::Update()
{
	m_dynamicWorld->stepSimulation(GameTime().GetFrameDeltaTime());
}

void PhysicsWorld::ContactTest(btCollisionObject* colObj, std::function<void(const btCollisionObject& contactCollisionObject)> cb)
{
	
}