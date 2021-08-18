#include "stdafx.h"
#include "CharacterController.h"
#include "SrcFile/Physics/CollisionAttr.h"

/// <summary>
/// 衝突したときに呼ばれる関数オブジェクト(壁用)
/// </summary>
struct SweepResultWall : public btCollisionWorld::ConvexResultCallback{
	bool isHit = false;					//衝突フラグ。
	Vector3 hitPos = g_vec3Zero;		//衝突点。
	Vector3 startPos = g_vec3Zero;		//レイの始点。
	float dist = FLT_MAX;				//衝突点までの距離。
	Vector3 hitNormal = g_vec3Zero;		//衝突店の法線。
	btCollisionObject* me = nullptr;	//自分自身。

	virtual btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		if (convexResult.m_hitCollisionObject == me
			|| convexResult.m_hitCollisionObject->getInternalType() == btCollisionObject::CO_GHOST_OBJECT
			) {
			//自分に衝突した or 地面に衝突。
			return 0.0f;
		}
		//傾斜判定。
		//衝突点の法線を引っ張ってくる。
		Vector3 hitNormalTmp;
		hitNormalTmp.Set(convexResult.m_hitNormalLocal);
		//上方向と衝突店の法線のなす角を求める。
		float angle = fabsf(acosf(hitNormalTmp.Dot(Vector3::Up)));
		if (angle >= Math::PI * 0.3f
			|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Character)
		{
			//地面傾斜54度以上または、コリジョン属性がキャラクター
			//だったので壁とみなす。
			isHit = true;
			Vector3 hitPosTmp;
			hitPosTmp.Set(convexResult.m_hitPointLocal);
			//交点との距離を調べる。
			Vector3 vDist;
			vDist.Subtract(hitPosTmp, startPos);
			vDist.y = 0.0f;
			float distTmp = vDist.Length();
			if (distTmp < dist) {
				//この衝突店の方が近いので、更新。
				hitPos = hitPosTmp;
				dist = distTmp;
				hitNormal = hitNormalTmp;
			}
		}//壁かどうかの判定。
		return 0.0f;
	}//function;
};

/// <summary>
/// 衝突した時に呼ばれる関数オブジェクト(地面用)
/// </summary>
struct SweepResultGround : public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;								//衝突フラグ。
	Vector3 hitPos = Vector3(0.0f, -FLT_MAX, 0.0f);	//衝突点。
	Vector3 startPos = g_vec3Zero;					//レイの始点。
	Vector3 hitNormal = g_vec3Zero;					//衝突点の法線。
	btCollisionObject* me = nullptr;				//自分自身。
	float dist = FLT_MAX;							//衝突点までの距離。

	virtual btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		if (convexResult.m_hitCollisionObject == me
			|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Character
			|| convexResult.m_hitCollisionObject->getInternalType() == btCollisionObject::CO_GHOST_OBJECT)
		{
			//自分に衝突した or キャラクター属性のコリジョンと衝突。
			return 0.0f;
		}
		//衝突点の法線を引っ張ってくる。
		Vector3 hitNormalTmp = *(Vector3*)&convexResult.m_hitNormalLocal;
		//上方向と法線のなす角度を求める。
		float angle = hitNormalTmp.Dot(Vector3::Up);
		angle = fabsf(acosf(angle));
		if (angle < Math::PI * 0.3f
			|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Ground)
		{
			//地面の傾斜が54度より小さい、コリジョン属性が地面に指定。
			//衝突している。
			isHit = true;
			Vector3 hitPosTmp = *(Vector3*)&convexResult.m_hitPointLocal;
			//衝突店の距離を求める。
			Vector3 vDist;
			vDist.Subtract(hitPosTmp, startPos);
			float distTmp = vDist.Length();
			if (dist > distTmp) {
				//衝突点が近いので更新。
				hitPos = hitPosTmp;
				hitNormal = *(Vector3*)&convexResult.m_hitNormalLocal;
				dist = distTmp;
			}
		}//衝突判定終わり。
		return 0.0f;
	}//function
};

void CharacterController::Init(float radius, float height, const Vector3& position)
{
	m_position = position;
	//コリジョン作成。
	m_radius = radius;
	m_height = height;
	m_collider.Create(radius, height);

	//剛体初期化。
	RigidBodyInfo rbInfo;
	rbInfo.collider = &m_collider;
	rbInfo.mass = 0.0f;
	m_rigidBody.Create(rbInfo);
	btTransform& trans = m_rigidBody.GetBody()->getWorldTransform();
	//剛体の位置を更新。
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
		//未初期化エラー//
		MessageBoxA(nullptr, "エラー_CCon::Excute", "CharacterControllerが未初期化です。", MB_OK);
		return g_vec3Zero;
	}
	if (moveSpeed.y > 0.0f) {
		//浮遊中。
		m_isJump = true;
		m_isOnGround = false;
	}
	//次の移動先となる座標を計算。
	Vector3 nextPosition = m_position;
	//速度からこのフレームでの移動量を計算。オイラー積分。
	Vector3 addPos = moveSpeed;
	addPos.Scale(deltaTime);
	nextPosition.Add(addPos);
	Vector3 originalXZDir = addPos;
	originalXZDir.y = 0.0f;
	originalXZDir.Normalize();
	//XZ平面での衝突検出と衝突解決を行う。
	{
		int loopCount = 0;
		while (true) {
			//現在の座標から次の移動先へ向かうベクトルを求める。
			Vector3 addPos;
			addPos.Subtract(nextPosition, m_position);
			Vector3 addPosXZ = addPos;
			addPosXZ.y = 0.0f;
			if (addPosXZ.Length() < FLT_EPSILON) {
				//float誤差を考慮したXZ平面での動きがあるかの判定。
				//XZ平面動きなし。
				break;
			}
			//カプセルコライダーの中心座標 + 高さ * 0.1 の座標をposTmpに求める。
			Vector3 posTmp = m_position;
			posTmp.y += m_height * 0.5f + m_radius + m_height * 0.1f;
			//レイを作成。
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			//始点はカプセルコライダーの中心座標 + 0.2の座標をposTmpに求める。
			start.setOrigin(btVector3(posTmp.x, posTmp.y, posTmp.z));
			//終点は次の移動先。XZ平面での衝突を調べるため、yはposTmpを使用。
			end.setOrigin(btVector3(nextPosition.x, posTmp.y, nextPosition.z));

			//傾斜判定用の関数オブジェクトを用意。
			SweepResultWall callBack;
			callBack.me = m_rigidBody.GetBody();
			callBack.startPos = posTmp;
			//callBack関数を使用して、衝突検出。
			if (fabsf(addPos.x) > 0.01f || fabsf(addPos.z) > 0.01f) {
				PhysicObj().ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), start, end, callBack);
			}
			if (callBack.isHit) {
				//衝突した。
				//壁。
				Vector3 vT0, vT1;
				//XZ平面上での移動後の座標をvT0に、交点の座標をvT1に設定。
				vT0.Set(nextPosition.x, 0.0f, nextPosition.z);
				vT1.Set(callBack.hitPos.x, 0.0f, callBack.hitPos.z);
				//めり込みが発生している移動ベクトルを計算。
				Vector3 vMerikomi;
				vMerikomi.Subtract(vT0, vT1);
				//XZ平面での衝突とした壁の法線を求める。
				Vector3 hitNormalXZ = callBack.hitNormal;
				hitNormalXZ.y = 0.0f;
				hitNormalXZ.Normalize();
				//めり込みベクトルを壁の法線に射影する。
				float ft0 = hitNormalXZ.Dot(vMerikomi);
				//押し返すベクトルは壁の法線に射影されためり込みベクトル + 半径。
				Vector3 vOffset;
				vOffset = hitNormalXZ;
				//押し戻すので-する。
				vOffset.Scale(-ft0 + m_radius);
				nextPosition.Add(vOffset);
				Vector3 currentDir;
				currentDir.Subtract(nextPosition, m_position);
				currentDir.y = 0.0f;
				currentDir.Normalize();
				if (currentDir.Dot(originalXZDir) < 0.0f) {
					//角に入ったキャラクターの振動を防止するため
					//移動先が逆向きになったら移動をキャンセル。
					nextPosition.x = m_position.x;
					nextPosition.z = m_position.z;
					break;
				}
			}//isHit==true.
			else {
				//どこともあたらないので終わり。
				break;
			}//NoHit.
			loopCount++;
			if (loopCount == 5) {
				break;
			}
		}//while
	}//XZ平面の衝突解決終わり。
	//XZの移動は確定。
	m_position.x = nextPosition.x;
	m_position.z = nextPosition.z;
	//下方向を調べる。
	{
		Vector3 addPos;
		addPos.Subtract(nextPosition, m_position);

		m_position = nextPosition;	//移動を仮確定。

		//レイを作成する。
		btTransform start, end;
		start.setIdentity();
		end.setIdentity();
		//始点はカプセルコライダーの中心。
		start.setOrigin(btVector3(m_position.x, m_position.y + m_height * 0.5f + m_radius, m_position.z));
		//終点は地面上にいない場合は1m下を見る。
		//地面上にいなくてもジャンプで上昇中の場合は上昇量の0.01倍下を見る。
		//地面上にいなくて降下中の場合はそのまま落下先を調べる。
		Vector3 endPos;
		endPos.Set(start.getOrigin());
		if (m_isOnGround == false) {
			if (addPos.y > 0.0f) {
				//上昇中。
				//XZ平面で移動している結果、めり込んでいる可能性があるので下を調べる。
				endPos.y -= addPos.y * 0.01f;
			}
			else {
				//落下の場合はそのまま調べる。
				endPos.y += addPos.y;
			}
		}//浮遊。
		else {
			//地面上いない場合は1M下をみる。
			endPos.y -= 1.0f;
		}
		end.setOrigin(btVector3(endPos.x, endPos.y, endPos.z));
		SweepResultGround callBack;
		callBack.me = m_rigidBody.GetBody();
		callBack.startPos.Set(start.getOrigin());
		//衝突検出。
		if (fabsf(endPos.y - callBack.startPos.y) > FLT_EPSILON) {
			PhysicObj().ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), start, end, callBack);
			if (callBack.isHit) {
				//当たった。
				moveSpeed.y = 0.0f;
				m_isJump = false;
				m_isOnGround = true;
				nextPosition.y = callBack.hitPos.y;
			}
			else {
				//地面上にいない。
				m_isOnGround = false;
			}
		}//衝突検出終わり。
		//移動を確定。
		m_position = nextPosition;
		btRigidBody* btBody = m_rigidBody.GetBody();
		//剛体を動かす。
		btBody->setActivationState(DISABLE_DEACTIVATION);
		btTransform& trans = btBody->getWorldTransform();
		//剛体の位置を更新。
		trans.setOrigin(btVector3(m_position.x, m_position.y + m_height * 0.5f + m_radius, m_position.z));
		//todo trans.setRotation
		return m_position;
	}//方向。
}

void CharacterController::RemoveRigidBody()
{
	PhysicObj().RemoveRigidBody(m_rigidBody);
}
