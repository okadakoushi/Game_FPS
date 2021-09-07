#include "stdafx.h"
#include "PlayerAttackState.h"
#include "SrcFile/RayTestCallBack.h"
#include "SrcFile/GameObj/GamePlayer.h"
#include "SrcFile/GameObj/Rifle.h"
#include "SrcFile/GameObj/Enemy/RifleEnemy.h"

void PlayerAttackState::Enter()
{
	m_player->GetRender()->PlayAnimation(GamePlayer::EnPlayerAnimation_Shoot, 0.1f);
}

void PlayerAttackState::Update()
{
	//レイのコールバック。
	RayTestCallBack::PlayerRayTestResult rayCallBack;
	//頭の位置。
	Vector3 headPos = m_player->GetHeadPos();
	//レイテストを行う。
	Vector3 toDir = GraphicsEngineObj()->GetCamera3D().GetTarget() - headPos;
	toDir.Normalize();
	toDir *= RAY_RANGE;
	PhysicObj().RayTest(headPos, toDir + headPos, rayCallBack);

	//発砲音。
	m_shootSE->Stop();
	m_shootSE->Play(false);
	m_shootSE->SetPosition(m_player->GetPos());
	//弾を減らす。
	m_player->GetWepon()->ReduseAmo();

	if (rayCallBack.hasHit() && rayCallBack.StaticObjectDist * RAY_RANGE > rayCallBack.CharacterObjectDist * RAY_RANGE) {
		//敵にレイが命中。
		//生ポインタから敵に強制キャスト。
		RifleEnemy* enemy = reinterpret_cast<RifleEnemy*>(rayCallBack.m_collisionObject->getUserPointer());
		if (enemy != nullptr) {
			enemy->GetDamage();
		}
	}
	else if (rayCallBack.hasHit() && rayCallBack.StaticObjectDist < rayCallBack.CharacterObjectDist) {
		//なんかのメッシュに命中。
		//命中した地点からエフェクトを再生。
		Vector3 effectPos = headPos + toDir * rayCallBack.StaticObjectDist;
		Quaternion effectRot;
		effectRot.SetRotation(g_vec3AxisY, atan2f(g_vec3Front.z, toDir.x));
		m_effect->SetPosition(effectPos);
		m_effect->SetRotation(effectRot);
		//Y成分はいらない。
		m_effect->Play(L"Assets/effect/aaaa.efk");
	}
}

void PlayerAttackState::Leave()
{
}
