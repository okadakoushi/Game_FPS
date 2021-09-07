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
	//���C�̃R�[���o�b�N�B
	RayTestCallBack::PlayerRayTestResult rayCallBack;
	//���̈ʒu�B
	Vector3 headPos = m_player->GetHeadPos();
	//���C�e�X�g���s���B
	Vector3 toDir = GraphicsEngineObj()->GetCamera3D().GetTarget() - headPos;
	toDir.Normalize();
	toDir *= RAY_RANGE;
	PhysicObj().RayTest(headPos, toDir + headPos, rayCallBack);

	//���C���B
	m_shootSE->Stop();
	m_shootSE->Play(false);
	m_shootSE->SetPosition(m_player->GetPos());
	//�e�����炷�B
	m_player->GetWepon()->ReduseAmo();

	if (rayCallBack.hasHit() && rayCallBack.StaticObjectDist * RAY_RANGE > rayCallBack.CharacterObjectDist * RAY_RANGE) {
		//�G�Ƀ��C�������B
		//���|�C���^����G�ɋ����L���X�g�B
		RifleEnemy* enemy = reinterpret_cast<RifleEnemy*>(rayCallBack.m_collisionObject->getUserPointer());
		if (enemy != nullptr) {
			enemy->GetDamage();
		}
	}
	else if (rayCallBack.hasHit() && rayCallBack.StaticObjectDist < rayCallBack.CharacterObjectDist) {
		//�Ȃ񂩂̃��b�V���ɖ����B
		//���������n�_����G�t�F�N�g���Đ��B
		Vector3 effectPos = headPos + toDir * rayCallBack.StaticObjectDist;
		Quaternion effectRot;
		effectRot.SetRotation(g_vec3AxisY, atan2f(g_vec3Front.z, toDir.x));
		m_effect->SetPosition(effectPos);
		m_effect->SetRotation(effectRot);
		//Y�����͂���Ȃ��B
		m_effect->Play(L"Assets/effect/aaaa.efk");
	}
}

void PlayerAttackState::Leave()
{
}
