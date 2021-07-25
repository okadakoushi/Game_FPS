#include "stdafx.h"
#include <random>
#include "RifleEnemy.h"
#include "SrcFile/GameObj/Rifle.h"
#include "SrcFile/GameObj/GamePlayer.h"
#include "SrcFile/RayTestCallBack.h"
#include "SrcFile/GameObj/Bullet.h"

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
	return true;
}

void RifleEnemy::Update()
{
    //��X�g���̂Ńw�b�h�̂������v�Z���Ă����B
    Quaternion headRot;
    m_head->CalcWorldTRS(m_headPos, headRot);

	//���}���u��swich-case�B���������Ȃ�����X�e�[�g�p�^�[�������ŉB�����悤�B
	switch (m_enemyState)
	{
	case EnEnemyState_Wandering:
        //if (m_isFindPlayer) {
        //    printf("�g���b�L���O��ԂɑJ��\n");
        //    m_enemyState = EnEnemyState_Tracking;
        //}
        //else {
        Move();
        //}
        //if (IsFindPlayer()) {
        //    printf("�G�𔭌��B\n");
        //    m_enemyState = EnEnemyState_Attack;
        //}
        //else {
        //    if (m_isFindPlayer) {
        //        m_enemyState = EnEnemyState_Tracking;
        //        //�g���b�L���O�A�j���[�V�����B
        //    }
        //    else {
        //        //���ʂɖ߂��B
        //        m_enemyState = EnEnemyState_Wandering;
        //        m_modelRender->PlayAnimation(0, 0.5f);
        //    }

        //}
		//�x����ԁB
		break;
	case EnEnemyState_Tracking:
		//�g���b�L���O��ԁB
        Tracking();
		break;
	case EnEnemyState_AttackWait:
		//�����[�h�Ƃ��̗\�肾��...
		break;
	case EnEnemyState_Attack:
        Attack();
		//�U���I
		break;
    case EnEnemyState_Damage:
        Damage();
        //�_���[�W�B
        break;
	case EnEnemyState_Death:
		//���S�B
        Death();
		break;
	}
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

void RifleEnemy::Attack()
{
    //printf("AttackMode\n");

    //�v���C���[���������̂Ŏ��̖ړI�n�̓v���C���[�̈ʒu�ɐݒ�B
    m_isFindPlayer = true;
    //m_nextTarget = m_player->GetPos();
    //�o�H�̍X�V���K�v�B
    //m_dirty = false;
    //Player�𔭌������B
    m_player->GetPlayerUIs()->AddFindList(this);
    if (m_modelRender->GetAnimLoop() || !m_modelRender->isPlayAnim()) {
        //�A�j���[�V���������[�v������̂܂��́A���[�v���Ȃ����̂ŃA�j���[�V����������Ă��Ȃ��B
        //�A�j���[�V������؂�ւ��B
        m_modelRender->PlayAnimation(2, 0.1f);
        //�v���C���[�̕����Ɍ�����B
        m_rot.SetRotation(Vector3::AxisY, atan2f(m_toPlayerDir.x * 1.0f, m_toPlayerDir.z * 1.0f));
        if (COOLDOWN < m_currentTime) {
            //���C�̒����𒲐��B
            Vector3 target = m_toPlayerDir;
            target *= m_VISION;
            //�G�l�~�[�̃G�C���̐��x�𒲐��B
            target = { target.x + rand() % m_currentRondomAIM, target.y + rand() % m_currentRondomAIM, target.z };
            //�e�۔��ˁB
            //Bullet* bullet = NewGO<Bullet>(EnPriority_3DModel);
            //bullet->SetPos(headPos);
            //bullet->SetRot(m_rot);
            //bullet->SetSpeed(110.0f);
            //���C���邽�т�Enemy��AIM�̃����_���l��������B
            m_currentRondomAIM--;
            //����l��ǉ������x�N�g���Ɍ������B
            //bullet->SetToTarget(target);
            //�e����Player�ɓ����邩���C�e�X�g�B
            RayTestCallBack::EnemyRayTestResult bulletRayCallBuck;
            PhysicObj().RayTest(m_headPos, target + m_headPos, bulletRayCallBuck);
            if (bulletRayCallBuck.hasHit() && bulletRayCallBuck.StaticObjectDist > bulletRayCallBuck.CharacterObjectDist) {
                //�v���C���[�ɖ����B
                m_player->DamageToPlayer(ATTACK);
                //printf("�����B");
                //���������̂ōĂ�AIM�̎����i���Ȃ��̂ɁB
                m_currentRondomAIM = MAX_RANDOM_AIM;
            }
            else {
                //printf("�O�����B");
            }
            m_currentTime = 0;
        }
    }
}

void RifleEnemy::Tracking()
{
    printf("�g���b�L���O���I�I\n");
    //�o�H��T�����Ĉړ��B
    //�o�H�ɉ����Đi�s���B
    Move();
    //if(m_dirty == true){
    //    //�w��n�_�ɓ����B
    //    m_enemyState = EnEnemyState_Wandering;
    //    m_isFindPlayer = false;
    //    //���Ƃ̌o�H�T���ɖ߂�B
    //    m_nextTarget = m_paths[m_pathIndex];
    //}

}

void RifleEnemy::Damage()
{
    if (m_hp <= 0 && this->IsActive()) {
        //printf("���S�I�I\n");
        m_enemyState = EnEnemyState_Death;
    }
    else {
        m_modelRender->PlayAnimation(1, 0.1f);
        m_enemyState = EnEnemyState_Attack;
        //�Ƃ肠�����Œ�l25�B
        m_hp -= 25;
    }
}

void RifleEnemy::Death()
{
    //printf("�V�j�}�[�X�B\n");
    m_modelRender->PlayAnimation(3, 1.0f);
    if (!m_modelRender->isPlayAnim()) {
        //�X�V�~�߁A�R���W��������B
        this->SetActive(false);
        m_collision.Release();
    }
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
        printf("FindPlayer!\n");
        return true;
    }
    //�����ĂȂ��B
    printf("CantFindPlayer\n");
    return false;
}



