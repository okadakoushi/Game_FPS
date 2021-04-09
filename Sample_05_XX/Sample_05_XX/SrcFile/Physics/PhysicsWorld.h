#pragma once

/// <summary>
/// �t�B�W�b�N�X���[���h�B
/// </summary>
class PhysicsWorld
{
private:
	std::unique_ptr<btDefaultCollisionConfiguration>		m_collisionConfig;
	std::unique_ptr<btCollisionDispatcher>					m_collisionDispatcher;	//�Փˉ����B
	std::unique_ptr<btBroadphaseInterface>					m_overlappingPairCache;	//�u���[�h�t�F�[�Y�B�Փ˔���̎}�؁B
	std::unique_ptr<btSequentialImpulseConstraintSolver>	m_constrainSolver;		//�R���X�g���C���g�\���o�[�B�S�������̉��������B
	std::unique_ptr<btDiscreteDynamicsWorld>				m_dynamicWorld;			//���[���h�B

public:
	~PhysicsWorld();
	/// <summary>
	/// �������B
	/// </summary>
	void Init();
	/// <summary>
	/// �X�V�B
	/// </summary>
	void Update();
	/// <summary>
	/// ����B
	/// </summary>
	void Release();
	/// <summary>
	/// �d�͂�ݒ�B
	/// </summary>
	/// <param name="g"></param>
	void SetGravity(Vector3& g)
	{
		m_dynamicWorld->setGravity(btVector3(g.x, g.y, g.z));
	}
	/// <summary>
	/// �_�C�i�~�b�N���[���h���擾�B
	/// </summary>
	/// <returns></returns>
	btDiscreteDynamicsWorld* GetDynamicWorld()
	{
		return m_dynamicWorld.get();
	}
	/// <summary>
	/// �R���W�����I�u�W�F�N�g�����[���h�ɓo�^�B
	/// </summary>
	/// <param name="colObj">�R���W�����I�u�W�F�N�g�B</param>
	void AddCollisionObject(btCollisionObject& colObj)
	{
		m_dynamicWorld->addCollisionObject(&colObj);
	}
	/// <summary>
	/// �R���W�����I�u�W�F�N�g�����[���h����폜�B
	/// </summary>
	/// <param name="colObj"></param>
	void RemoveCollisionObject(btCollisionObject& colObj)
	{
		m_dynamicWorld->removeCollisionObject(&colObj);
	}
	/// <summary>
	/// �`��ɑ΂���Փ˃e�X�g�B
	/// </summary>
	/// <param name="castShape">�`��B</param>
	/// <param name="convexFromWorld">fromPos�B</param>
	/// <param name="convexToWorld">ToPos�B</param>
	/// <param name="resultCallBack">���ʁB</param>
	/// <param name="allowedCcdPanetration">�N���ł���͈́H</param>
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

