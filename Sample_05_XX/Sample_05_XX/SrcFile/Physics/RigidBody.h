#pragma once

class ICollider;
/// <summary>
/// ���̏��B
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
	Vector3 pos;			//���W�B
	Quaternion rot;			//��]�B
	Vector3 localInteria;	//�����e���\���B
	ICollider* collider;	//�R���C�_�[�B
	float mass;				//���ʁB
};

/// <summary>
/// ���́B
/// </summary>
class RigidBody
{
	std::unique_ptr<btRigidBody>			m_rigidBody;		//���́B
	std::unique_ptr<btDefaultMotionState>	m_myMotionState;	//���[�V�����X�e�[�g�B
	bool m_isAddPhysicsWorld = false;					//�������[���h�ɒǉ�����Ă��邩�B
public:
	~RigidBody();
	/// <summary>
	/// ����B
	/// </summary>
	void Release();
	/// <summary>
	/// �쐬�B
	/// </summary>
	/// <param name="rbInfo">���̏��B</param>
	void Create(RigidBodyInfo& rbInfo);
	/// <summary>
	/// ���̎擾�B
	/// </summary>
	/// <returns></returns>
	btRigidBody* GetBody()
	{
		return m_rigidBody.get();
	}
	/// <summary>
	/// �������[���h�o�^���̈������B
	/// </summary>
	void SetMarkAddPhysicsWorld()
	{
		m_isAddPhysicsWorld = true;
	}
	/// <summary>
	/// �������[���h�o�^���̈���O���B
	/// </summary>
	void SetUnmarkAddPhysicsWorld()
	{
		m_isAddPhysicsWorld = false;
	}
	/// <summary>
	/// �������[���h�ɓo�^����Ă�H
	/// </summary>
	/// <returns></returns>
	bool IsAddPhysicsWorld() const
	{
		return m_isAddPhysicsWorld;
	}
	/// <summary>
	/// �����I�u�W�F�N�g�̍��W�Ɖ�]���擾�B
	/// </summary>
	/// <param name="pos">�ʒu�B</param>
	/// <param name="rot">��]�B</param>
	void GetPositionAndRotation(Vector3& pos, Quaternion& rot) const
	{
		btTransform trans;
		m_myMotionState->getWorldTransform(trans);
		pos.Set(trans.getOrigin());
		rot.Set(trans.getRotation());
	}
	/// <summary>
	/// �ʒu�Ɖ�]��ݒ�B
	/// </summary>
	/// <param name="pos">�ʒu�B</param>
	/// <param name="rot">��]�B</param>
	void SetPositionAndRotation(const Vector3& pos, const Quaternion& rot)
	{
		btTransform trans;
		btVector3 btPos;
		//�ʒu�A��]���R�s�[�B
		pos.CopyTo(btPos);
		trans.setOrigin(btPos);
		btQuaternion btRot;
		rot.CopyTo(btRot);
		trans.setRotation(btRot);
		m_rigidBody->setWorldTransform(trans);
	}
	/// <summary>
	/// �w������ɗ͂�������B
	/// </summary>
	/// <param name="force">�́B</param>
	/// <param name="relPos">�͂���������W�B</param>
	void AddForce(const Vector3& force, const Vector3& relPos)
	{
		btVector3 btForce;
		btVector3 btRelPos;
		force.CopyTo(btForce);
		relPos.CopyTo(btRelPos);
		m_rigidBody->applyForce(btForce, btRelPos);
	}
};

