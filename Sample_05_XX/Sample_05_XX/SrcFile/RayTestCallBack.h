#pragma once

#include "SrcFile/Physics/CollisionAttr.h"

/// <summary>
/// レイテストした際のコールバック関数。
/// </summary>
class RayTestCallBack
{
public:
	/// <summary>
	/// プレイヤーからのレイテスト。
	/// </summary>
	struct PlayerRayTestResult : public btCollisionWorld::RayResultCallback
	{
		bool isHit = false;
		float StaticObjectDist = 100.0f;
		float CharacterObjectDist = 100.0f;
		void* characterHit = nullptr;
		Vector3 Normal = g_vec3Zero;

		//衝突時呼ばれるコールバック。
		virtual btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace)
		{
			isHit = false;
			if (rayResult.m_collisionObject->getUserIndex() == enCollisionAttr_Enemy) {
				//エネミーにヒット。
				isHit = true;
				CharacterObjectDist = rayResult.m_hitFraction;
				m_collisionObject = rayResult.m_collisionObject;
				//return rayResult.m_hitFraction;
			}
			if (rayResult.m_collisionObject->getUserIndex() == enCollisionAttr_StaticObject && StaticObjectDist > rayResult.m_hitFraction) {
				//静的物理だったかつ、この静的オブジェクトの方が手前にある。
				StaticObjectDist = rayResult.m_hitFraction;
				if (StaticObjectDist < CharacterObjectDist) {
					//静的物理が手前。
					m_collisionObject = rayResult.m_collisionObject;
					Normal.Set(rayResult.m_hitNormalLocal);
				}

				//return rayResult.m_hitFraction;
			}
			return rayResult.m_hitFraction;
		}
	};

	/// <summary>
	/// エネミーからのレイテスト。
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
				//静的物理だった。
				StaticObjectDist = rayResult.m_hitFraction;
				if (StaticObjectDist < CharacterObjectDist) {
					//静的物理が手前。
					m_collisionObject = rayResult.m_collisionObject;
				}
				//return rayResult.m_hitFraction;
			}
			if (rayResult.m_collisionObject->getUserIndex() == enCollisionAttr_Character) {
				//プレイヤーにヒット。
				CharacterObjectDist = rayResult.m_hitFraction;
				m_collisionObject = rayResult.m_collisionObject;
			}
			return rayResult.m_hitFraction;
		}
	};
};

