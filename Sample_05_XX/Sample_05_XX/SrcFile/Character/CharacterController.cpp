#include "stdafx.h"
#include "CharacterController.h"
#include "SrcFile/Physics/CollisionAttr.h"

/// <summary>
/// �Փ˂����Ƃ��ɌĂ΂��֐��I�u�W�F�N�g(�Ǘp)
/// </summary>
struct SweepResultWall : public btCollisionWorld::ConvexResultCallback{
	bool isHit = false;					//�Փ˃t���O�B
	Vector3 hitPos = g_vec3Zero;		//�Փ˓_�B
	Vector3 startPos = g_vec3Zero;		//���C�̎n�_�B
	float dist = FLT_MAX;				//�Փ˓_�܂ł̋����B
	Vector3 hitNormal = g_vec3Zero;		//�Փ˓X�̖@���B
	btCollisionObject* me = nullptr;	//�������g�B

	virtual btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		if (convexResult.m_hitCollisionObject == me
			|| convexResult.m_hitCollisionObject->getInternalType() == btCollisionObject::CO_GHOST_OBJECT
			) {
			//�����ɏՓ˂��� or �n�ʂɏՓˁB
			return 0.0f;
		}
		//�X�Δ���B
		//�Փ˓_�̖@�������������Ă���B
		Vector3 hitNormalTmp;
		hitNormalTmp.Set(convexResult.m_hitNormalLocal);
		//������ƏՓ˓X�̖@���̂Ȃ��p�����߂�B
		float angle = fabsf(acosf(hitNormalTmp.Dot(Vector3::Up)));
		if (angle >= Math::PI * 0.3f
			|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Character)
		{
			//�n�ʌX��54�x�ȏ�܂��́A�R���W�����������L�����N�^�[
			//�������̂ŕǂƂ݂Ȃ��B
			isHit = true;
			Vector3 hitPosTmp;
			hitPosTmp.Set(convexResult.m_hitPointLocal);
			//��_�Ƃ̋����𒲂ׂ�B
			Vector3 vDist;
			vDist.Subtract(hitPosTmp, startPos);
			vDist.y = 0.0f;
			float distTmp = vDist.Length();
			if (distTmp < dist) {
				//���̏Փ˓X�̕����߂��̂ŁA�X�V�B
				hitPos = hitPosTmp;
				dist = distTmp;
				hitNormal = hitNormalTmp;
			}
		}//�ǂ��ǂ����̔���B
		return 0.0f;
	}//function;
};

/// <summary>
/// �Փ˂������ɌĂ΂��֐��I�u�W�F�N�g(�n�ʗp)
/// </summary>
struct SweepResultGround : public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;								//�Փ˃t���O�B
	Vector3 hitPos = Vector3(0.0f, -FLT_MAX, 0.0f);	//�Փ˓_�B
	Vector3 startPos = g_vec3Zero;					//���C�̎n�_�B
	Vector3 hitNormal = g_vec3Zero;					//�Փ˓_�̖@���B
	btCollisionObject* me = nullptr;				//�������g�B
	float dist = FLT_MAX;							//�Փ˓_�܂ł̋����B

	virtual btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		if (convexResult.m_hitCollisionObject == me
			|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Character
			|| convexResult.m_hitCollisionObject->getInternalType() == btCollisionObject::CO_GHOST_OBJECT)
		{
			//�����ɏՓ˂��� or �L�����N�^�[�����̃R���W�����ƏՓˁB
			return 0.0f;
		}
		//�Փ˓_�̖@�������������Ă���B
		Vector3 hitNormalTmp = *(Vector3*)&convexResult.m_hitNormalLocal;
		//������Ɩ@���̂Ȃ��p�x�����߂�B
		float angle = hitNormalTmp.Dot(Vector3::Up);
		angle = fabsf(acosf(angle));
		if (angle < Math::PI * 0.3f
			|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Ground)
		{
			//�n�ʂ̌X�΂�54�x��菬�����A�R���W�����������n�ʂɎw��B
			//�Փ˂��Ă���B
			isHit = true;
			Vector3 hitPosTmp = *(Vector3*)&convexResult.m_hitPointLocal;
			//�Փ˓X�̋��������߂�B
			Vector3 vDist;
			vDist.Subtract(hitPosTmp, startPos);
			float distTmp = vDist.Length();
			if (dist > distTmp) {
				//�Փ˓_���߂��̂ōX�V�B
				hitPos = hitPosTmp;
				hitNormal = *(Vector3*)&convexResult.m_hitNormalLocal;
				dist = distTmp;
			}
		}//�Փ˔���I���B
		return 0.0f;
	}//function
};

void CharacterController::Init(float radius, float height, const Vector3& position)
{
	m_position = position;
	//�R���W�����쐬�B
	m_radius = radius;
	m_height = height;
	m_collider.Create(radius, height);

	//���̏������B
	RigidBodyInfo rbInfo;
	rbInfo.collider = &m_collider;
	rbInfo.mass = 0.0f;
	m_rigidBody.Create(rbInfo);
	btTransform& trans = m_rigidBody.GetBody()->getWorldTransform();
	//���̂̈ʒu���X�V�B
	trans.setOrigin(btVector3(position.x, position.y + m_height * 0.5f + m_radius, position.z));
	//todo trans.setRotation;
	m_rigidBody.GetBody()->setUserIndex(enCollisionAttr_Character);
	m_rigidBody.GetBody()->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
	PhysicObj().AddRigidBody(m_rigidBody);
	m_isInited = true;
}

const Vector3& CharacterController::Execute(Vector3& moveSpeed, float deltaTime)
{
	if (!m_isInited) {
		//���������G���[//
		MessageBoxA(nullptr, "�G���[_CCon::Excute", "CharacterController�����������ł��B", MB_OK);
		return g_vec3Zero;
	}
	if (moveSpeed.y > 0.0f) {
		//���V���B
		m_isJump = true;
		m_isOnGround = false;
	}
	//���̈ړ���ƂȂ���W���v�Z�B
	Vector3 nextPosition = m_position;
	//���x���炱�̃t���[���ł̈ړ��ʂ��v�Z�B�I�C���[�ϕ��B
	Vector3 addPos = moveSpeed;
	addPos.Scale(deltaTime);
	nextPosition.Add(addPos);
	Vector3 originalXZDir = addPos;
	originalXZDir.y = 0.0f;
	originalXZDir.Normalize();
	//XZ���ʂł̏Փˌ��o�ƏՓˉ������s���B
	{
		int loopCount = 0;
		while (true) {
			//���݂̍��W���玟�̈ړ���֌������x�N�g�������߂�B
			Vector3 addPos;
			addPos.Subtract(nextPosition, m_position);
			Vector3 addPosXZ = addPos;
			addPosXZ.y = 0.0f;
			if (addPosXZ.Length() < FLT_EPSILON) {
				//float�덷���l������XZ���ʂł̓��������邩�̔���B
				//XZ���ʓ����Ȃ��B
				break;
			}
			//�J�v�Z���R���C�_�[�̒��S���W + ���� * 0.1 �̍��W��posTmp�ɋ��߂�B
			Vector3 posTmp = m_position;
			posTmp.y += m_height * 0.5f + m_radius + m_height * 0.1f;
			//���C���쐬�B
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			//�n�_�̓J�v�Z���R���C�_�[�̒��S���W + 0.2�̍��W��posTmp�ɋ��߂�B
			start.setOrigin(btVector3(posTmp.x, posTmp.y, posTmp.z));
			//�I�_�͎��̈ړ���BXZ���ʂł̏Փ˂𒲂ׂ邽�߁Ay��posTmp���g�p�B
			end.setOrigin(btVector3(nextPosition.x, posTmp.y, nextPosition.z));

			//�X�Δ���p�̊֐��I�u�W�F�N�g��p�ӁB
			SweepResultWall callBack;
			callBack.me = m_rigidBody.GetBody();
			callBack.startPos = posTmp;
			//callBack�֐����g�p���āA�Փˌ��o�B
			if (fabsf(addPos.x) > 0.01f || fabsf(addPos.z) > 0.01f) {
				PhysicObj().ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), start, end, callBack);
			}
			if (callBack.isHit) {
				//�Փ˂����B
				//�ǁB
				Vector3 vT0, vT1;
				//XZ���ʏ�ł̈ړ���̍��W��vT0�ɁA��_�̍��W��vT1�ɐݒ�B
				vT0.Set(nextPosition.x, 0.0f, nextPosition.z);
				vT1.Set(callBack.hitPos.x, 0.0f, callBack.hitPos.z);
				//�߂荞�݂��������Ă���ړ��x�N�g�����v�Z�B
				Vector3 vMerikomi;
				vMerikomi.Subtract(vT0, vT1);
				//XZ���ʂł̏Փ˂Ƃ����ǂ̖@�������߂�B
				Vector3 hitNormalXZ = callBack.hitNormal;
				hitNormalXZ.y = 0.0f;
				hitNormalXZ.Normalize();
				//�߂荞�݃x�N�g����ǂ̖@���Ɏˉe����B
				float ft0 = hitNormalXZ.Dot(vMerikomi);
				//�����Ԃ��x�N�g���͕ǂ̖@���Ɏˉe���ꂽ�߂荞�݃x�N�g�� + ���a�B
				Vector3 vOffset;
				vOffset = hitNormalXZ;
				//�����߂��̂�-����B
				vOffset.Scale(-ft0 + m_radius);
				nextPosition.Add(vOffset);
				Vector3 currentDir;
				currentDir.Subtract(nextPosition, m_position);
				currentDir.y = 0.0f;
				currentDir.Normalize();
				if (currentDir.Dot(originalXZDir) < 0.0f) {
					//�p�ɓ������L�����N�^�[�̐U����h�~���邽��
					//�ړ��悪�t�����ɂȂ�����ړ����L�����Z���B
					nextPosition.x = m_position.x;
					nextPosition.z = m_position.z;
					break;
				}
			}//isHit==true.
			else {
				//�ǂ��Ƃ�������Ȃ��̂ŏI���B
				break;
			}//NoHit.
			loopCount++;
			if (loopCount == 5) {
				break;
			}
		}//while
	}//XZ���ʂ̏Փˉ����I���B
	//XZ�̈ړ��͊m��B
	m_position.x = nextPosition.x;
	m_position.z = nextPosition.z;
	//�������𒲂ׂ�B
	{
		Vector3 addPos;
		addPos.Subtract(nextPosition, m_position);

		m_position = nextPosition;	//�ړ������m��B

		//���C���쐬����B
		btTransform start, end;
		start.setIdentity();
		end.setIdentity();
		//�n�_�̓J�v�Z���R���C�_�[�̒��S�B
		start.setOrigin(btVector3(m_position.x, m_position.y + m_height * 0.5f + m_radius, m_position.z));
		//�I�_�͒n�ʏ�ɂ��Ȃ��ꍇ��1m��������B
		//�n�ʏ�ɂ��Ȃ��Ă��W�����v�ŏ㏸���̏ꍇ�͏㏸�ʂ�0.01�{��������B
		//�n�ʏ�ɂ��Ȃ��č~�����̏ꍇ�͂��̂܂ܗ�����𒲂ׂ�B
		Vector3 endPos;
		endPos.Set(start.getOrigin());
		if (m_isOnGround == false) {
			if (addPos.y > 0.0f) {
				//�㏸���B
				//XZ���ʂňړ����Ă��錋�ʁA�߂荞��ł���\��������̂ŉ��𒲂ׂ�B
				endPos.y -= addPos.y * 0.01f;
			}
			else {
				//�����̏ꍇ�͂��̂܂ܒ��ׂ�B
				endPos.y += addPos.y;
			}
		}//���V�B
		else {
			//�n�ʏア�Ȃ��ꍇ��1M�����݂�B
			endPos.y -= 1.0f;
		}
		end.setOrigin(btVector3(endPos.x, endPos.y, endPos.z));
		SweepResultGround callBack;
		callBack.me = m_rigidBody.GetBody();
		callBack.startPos.Set(start.getOrigin());
		//�Փˌ��o�B
		if (fabsf(endPos.y - callBack.startPos.y) > FLT_EPSILON) {
			PhysicObj().ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), start, end, callBack);
			if (callBack.isHit) {
				//���������B
				moveSpeed.y = 0.0f;
				m_isJump = false;
				m_isOnGround = true;
				nextPosition.y = callBack.hitPos.y;
			}
			else {
				//�n�ʏ�ɂ��Ȃ��B
				m_isOnGround = false;
			}
		}//�Փˌ��o�I���B
		//�ړ����m��B
		m_position = nextPosition;
		btRigidBody* btBody = m_rigidBody.GetBody();
		//���̂𓮂����B
		btBody->setActivationState(DISABLE_DEACTIVATION);
		btTransform& trans = btBody->getWorldTransform();
		//���̂̈ʒu���X�V�B
		trans.setOrigin(btVector3(m_position.x, m_position.y + m_height * 0.5f + m_radius, m_position.z));
		//todo trans.setRotation
		return m_position;
	}//�����B
}

void CharacterController::RemoveRigidBody()
{
	PhysicObj().RemoveRigidBody(m_rigidBody);
}
