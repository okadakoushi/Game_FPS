#include "stdafx.h"
#include <random>
#include "RifleEnemy.h"
#include "SrcFile/GameObj/Rifle.h"
#include "SrcFile/GameObj/GamePlayer.h"
#include "SrcFile/RayTestCallBack.h"
#include "SrcFile/GameObj/Bullet.h"

#include "EnemyState/IEnemyState.h"
#include "EnemyState/EnemyAttackState.h"
#include "EnemyState/EnemyDamageState.h"
#include "EnemyState/EnemyDethState.h"
#include "EnemyState/EnemyTrackingState.h"
#include "EnemyState/EnemyWanderingState.h"

RifleEnemy::~RifleEnemy()
{
    DeleteGO(m_modelRender);
    DeleteGO(m_rifle);
}

bool RifleEnemy::Start()
{
	//�G�l�~�[���f���������B
	const char* tkaFile[]{
		"Assets/animData/Enemy/Rifle/Walk.tka",
		"Assets/animData/Enemy/Rifle/Damage.tka",
        "Assets/animData/Enemy/Rifle/Shoot.tka",
        "Assets/animData/Enemy/Rifle/Death.tka"
	};
	m_modelRender = NewGO<SkinModelRender>(EnPriority_3DModel);
    //�����V���b�g�Đ��Ƃ��ēo�^�B����todo:
    m_modelRender->SetAnimLoop(0, true);
    m_modelRender->SetAnimLoop(1, false);
    m_modelRender->SetAnimLoop(2, true);
    m_modelRender->SetAnimLoop(3, false);
	m_modelRender->Init("Assets/modelData/Chara/Enemy.tkm", tkaFile);
	m_modelRender->SetMulColor({ 1.0f, 0.5f, 0.5f, 1.0f });
	
    //�e�C���X�^���X���B
	m_rifle = NewGO<Rifle>(EnPriority_3DModel, "Rifle");
	m_rifle->SetRefBoneRender(m_modelRender);
    
    //�G�[�W�F���g�������B
    m_agent.Init(m_modelRender, m_naviMesh, true);

    m_collision.SetPointer(this);
    m_collision.Init(m_modelRender);
    
    //�v���C���[��Find�B
    m_player = GetPlayerForUseFind();

    //���̃{�[�����擾�B
    int headID = m_modelRender->GetSkelton().FindBoneID(L"mixamorig:Head");
    m_head = m_modelRender->GetSkelton().GetBone(headID);

    //�X�e�[�g�𐶐��B
    m_attackState = new EnemyAttackState(this);
    m_damageState = new EnemyDamageState(this);
    m_deathState = new EnemyDethState(this);
    m_trackingState = new EnemyTrackingState(this);
    m_wanderingState = new EnemyWanderingState(this);

	return true;
}

void RifleEnemy::Update()
{
    if (m_enemyState == m_deathState) {
        //��������ł�̂ōX�V�͍s��Ȃ��B
        return;
    }

    //��X�g���̂Ńw�b�h�̂������v�Z���Ă����B
    Quaternion headRot;
    m_head->CalcWorldTRS(m_headPos, headRot);

    ChangeState(m_wanderingState);
    if (IsFindPlayer()) {
        //�U���B
        ChangeState(m_attackState);
        //�v���C���[�����������B
        m_isMissingPlayer = true;
    }
    else if (m_isMissingPlayer) {
        //���������̂ŁA�v���C���[�������ł��낤�n�_�܂ŒǐՂ���B
        ChangeState(m_trackingState);
    }

    if (m_hp <= 0) {
        //����ł�B
        ChangeState(m_deathState);
    }

    m_enemyState->Update();

	m_modelRender->SetPosition(m_pos);
    m_modelRender->SetRotation(m_rot);
	m_modelRender->SetScale(m_scale);
    m_collision.Update();
    m_currentTime += GameTime().GetFrameDeltaTime();
}

void RifleEnemy::Move()
{
    m_modelRender->SetPosition(m_pos);
    m_agent.Move();
    m_pos = m_agent.GetAgentPos();
    m_rot = m_agent.GetAgentRot();
}

bool RifleEnemy::IsFindPlayer()
{
    //�v���C���[�Ƃ̋�������B
    m_toPlayerDir = m_player->GetPos() - m_pos;
    //�����B
    float toPlayerLen = m_toPlayerDir.Length();
    //���ʃx�N�g���B
    m_toPlayerDir.Normalize();
    //�O�����ƃv���C���[�Ɍ������x�N�g���̓��ρB
    float dot = m_agent.MoveDirection().Dot(m_toPlayerDir);
    //�Ȃ��p���v�Z�B
    float angle = acos(dot);
    //����p���ɑ��݂��邩�̔���B
    if (fabsf(angle) < Math::DegToRad(m_FOV) && toPlayerLen < m_VISION) {
        //����p�͈͓̔��i-+)�Ŏ�����B
    }
    else {
        //�����ĂȂ��B
        return false;
    }

    //��O�ɃI�u�W�F�N�g�����݂��Ȃ����𔻒�B
    //���씻��p�R�[���o�b�N�֐��B
    RayTestCallBack::EnemyRayTestResult visionCallBuck;

    //�v���C���[�֐L�т���ʃx�N�g���B
    m_toPlayerDir = m_player->GetPos() - m_pos;
    //�v���C���[�Ɍ������G�l�~�[�̃G�C���B
    //Vector3 EnemyAIM_to_Player = { toPlayerDir.x + rand() % m_currentRondomAIM, toPlayerDir.y + rand() % m_currentRondomAIM, toPlayerDir.z };
    m_toPlayerDir.Normalize();
    //EnemyAIM_to_Player.Normalize();
    //����̃^�[�Q�b�g�̓v���C���[�ɐL�т�x�N�g���B
    Vector3 target = m_toPlayerDir;
    target *= m_VISION;
    //���C�e�X�g�B
    PhysicObj().RayTest(m_headPos, target + m_headPos, visionCallBuck);
    if (visionCallBuck.hasHit() && visionCallBuck.StaticObjectDist > visionCallBuck.CharacterObjectDist) {
        //�������Ă���B
        //printf("FindPlayer!\n");
        return true;
    }
    //�����ĂȂ��B
    //printf("CantFindPlayer\n");
    return false;
}

void RifleEnemy::ChangeState(IEnemyState* state)
{
    if (m_modelRender->GetAnimLoop() || !m_modelRender->isPlayAnim()) {
        //���[�v���Ȃ��A�j���[�V�����͂������藬���Ă���ς�������B
        if (m_enemyState != nullptr) {
            m_enemyState->Leave();
        }
        m_enemyState = state;
        m_enemyState->Enter();
    }
}

void RifleEnemy::GetDamage()
{
    ChangeState(m_damageState);
}

GamePlayer* RifleEnemy::GetPlayerForUseFind() const
{
    return FindGO<GamePlayer>("Player");
}



