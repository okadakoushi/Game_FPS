#pragma once

#include "SrcFile/Physics/CollisionAttr.h"

/// <summary>
/// ���C�e�X�g�����ۂ̃R�[���o�b�N�֐��B
/// </summary>
class RayTestCallBack
{
public:
	/// <summary>
	/// �v���C���[����̃��C�e�X�g�B
	/// </summary>
	struct PlayerRayTestResult : public btCollisionWorld::RayResultCallback
	{
		bool isHit = false;
		float StaticObjectDist = 10000.0f;
		float CharacterObjectDist = 10000.0f;

		//�Փˎ��Ă΂��R�[���o�b�N�B
		virtual btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace)
		{
			isHit = false;
			if (rayResult.m_collisionObject->getUserIndex() == enCollisionAttr_StaticObject) {
				//�ÓI�����������B
				StaticObjectDist = rayResult.m_hitFraction;
				return 0.0f;
			}
			else if (rayResult.m_collisionObject->getUserIndex() == enCollisionAttr_Enemy) {
				//�G�l�~�[�Ƀq�b�g�B
				isHit = true;
				m_collisionObject = rayResult.m_collisionObject;
				CharacterObjectDist = rayResult.m_hitFraction;
				return rayResult.m_hitFraction;
			}
			return rayResult.m_hitFraction;
		}
	};

	/// <summary>
	/// �G�l�~�[����̃��C�e�X�g�B
	/// </summary>
	struct EnemyRayTestResult : public btCollisionWorld::RayResultCallback
	{
		bool isHit = false;
		float StaticObjectDist = 10000.0f;
		float CharacterObjectDist = 10000.0f;

		virtual btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace)
		{
			isHit = false;
			if (rayResult.m_collisionObject->getUserIndex() == enCollisionAttr_StaticObject) {
				//�ÓI�����������B
				StaticObjectDist = rayResult.m_hitFraction;
				return 0.0f;
			}
			else if (rayResult.m_collisionObject->getUserIndex() == enCollisionAttr_Character) {
				//�v���C���[�Ƀq�b�g�B
				if (rayResult.m_hitFraction < StaticObjectDist) {
					isHit = true;
					m_collisionObject = rayResult.m_collisionObject;
					CharacterObjectDist = rayResult.m_hitFraction;
					return rayResult.m_hitFraction;
				}
			}
			return rayResult.m_hitFraction;
		}
	};
};

