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
		float StaticObjectDist = 10000.0f;
		float CharacterObjectDist = 10000.0f;

		//衝突時呼ばれるコールバック。
		virtual btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace)
		{
			isHit = false;
			if (rayResult.m_collisionObject->getUserIndex() == enCollisionAttr_StaticObject) {
				//静的物理だった。
				StaticObjectDist = rayResult.m_hitFraction;
				return 0.0f;
			}
			else if (rayResult.m_collisionObject->getUserIndex() == enCollisionAttr_Enemy) {
				//エネミーにヒット。
				isHit = true;
				m_collisionObject = rayResult.m_collisionObject;
				CharacterObjectDist = rayResult.m_hitFraction;
				return rayResult.m_hitFraction;
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
				return 0.0f;
			}
			else if (rayResult.m_collisionObject->getUserIndex() == enCollisionAttr_Character) {
				//プレイヤーにヒット。
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

