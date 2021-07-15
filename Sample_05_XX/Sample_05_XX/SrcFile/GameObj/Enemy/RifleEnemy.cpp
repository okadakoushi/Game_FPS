#include "stdafx.h"
#include <random>
#include "RifleEnemy.h"
#include "SrcFile/GameObj/Rifle.h"
#include "SrcFile/GameObj/GamePlayer.h"
#include "SrcFile/RayTestCallBack.h"
#include "SrcFile/GameObj/Bullet.h"

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
	m_rifle = NewGO<Rifle>(EnPriority_3DModel);
	m_rifle->SetRefBoneRender(m_modelRender);
    
    //�o�H�T���̖ړI�n��ݒ�B
    m_targetPos = { 1000.0f, 0.0f, 500.0f };
    m_targetPos = { -1000.0f, 0.0f, -500.0f };
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
	//���}���u��swich-case�����������Ȃ�����X�e�[�g�p�^�[�������ŉB�����悤�B
	switch (m_enemyState)
	{
	case EnEnemyState_Wandering:
        Move();
        if (IsFindPlayer()) {
            m_enemyState = EnEnemyState_Attack;
            //m_enemyState = EnEnemyState_Tracking;
        }
		//�x����ԁB
		break;
	case EnEnemyState_Tracking:
		//�g���b�L���O��ԁB
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
    if (m_dirty == false) {
        //�X�V�K�v�B
        m_nodeList = m_astar.Search(m_pos, m_targetPos, m_naviMesh->GetCellList());
        NaviMesh::Cell* parentCell = m_nodeCell;
        //NaviMeshObj()->AgentNodeRender(m_nodeList);
        if (m_pathIndex != m_paths.size() - 1) {
            m_pathIndex++;
        }
        else {
            m_pathIndex = 0;
        }
        //���̖ړI�n�̓p�X���烉���_���őI�ԁB
        m_nextTarget = m_paths[/*rand() % m_paths.size()*/m_pathIndex];
        m_dirty = true;
    }

    //��������ړ������B
    //���̃Z���Ɍ������x�N�g���B
    if (m_nodeList.size() != 0) {
         m_toNextCell = m_nodeList.front()->m_CenterPos - m_pos;
        //���ʉ�����O�ɁA��������Ƃ��B
        float dist = m_toNextCell.Length();
        //���ʃx�N�g�����B
        m_toNextCell.Normalize();
        //�������B
        m_pos += m_toNextCell * m_spped;
        //��]�v�Z�B
        m_rot.SetRotation(Vector3::AxisY, atan2f(m_toNextCell.x * 1.0f, m_toNextCell.z * 1.0f));

        //�m�[�h�ɓ��B����������B
        if (dist < 5) {
            m_nodeList.erase(m_nodeList.begin());
        }
    }
    else {
        //�����ĂȂ��B
        //m_modelRender->PlayAnimation(1, 0.5f);
        //�X�V�B
        m_targetPos = m_nextTarget;
        m_dirty = false;
    }

    if (!m_modelRender->isPlayAnim()) {
        m_modelRender->PlayAnimation(0, 0.1f);
    }
}

void RifleEnemy::Attack()
{
    //printf("AttackMode\n");

    //�܂���Enemy�̓��̃{�[���Ɖ�]���擾�B
    Vector3 headPos;
    Quaternion headRot;
    m_head->CalcWorldTRS(headPos, headRot);

    //�v���C���[�֐L�т���ʃx�N�g���B
    Vector3 toPlayerDir = m_player->GetPos() - m_pos;
    //�v���C���[�Ɍ������G�l�~�[�̃G�C���B
    //Vector3 EnemyAIM_to_Player = { toPlayerDir.x + rand() % m_currentRondomAIM, toPlayerDir.y + rand() % m_currentRondomAIM, toPlayerDir.z };
    toPlayerDir.Normalize();
    //EnemyAIM_to_Player.Normalize();
    //����̃^�[�Q�b�g�̓v���C���[�ɐL�т�x�N�g���B
    Vector3 target = toPlayerDir;
    target *= m_VISION;

    
    //���씻��p�R�[���o�b�N�֐��B
    RayTestCallBack::EnemyRayTestResult visionCallBuck;
    //���C�e�X�g�B
    PhysicObj().RayTest(headPos, target + headPos, visionCallBuck);
    if (visionCallBuck.hasHit() && visionCallBuck.StaticObjectDist > visionCallBuck.CharacterObjectDist) {
        //��O�ɏ�Q���Ȃ��I�������Ă�B
        if (m_modelRender->GetAnimLoop() || !m_modelRender->isPlayAnim()) {
            //�A�j���[�V������؂�ւ��B
            m_modelRender->PlayAnimation(2, 0.1f);
            //�v���C���[�̕����Ɍ�����B
            m_rot.SetRotation(Vector3::AxisY, atan2f(toPlayerDir.x * 1.0f, toPlayerDir.z * 1.0f));
            if (COOLDOWN < m_currentTime) {
                //�G�l�~�[�̃G�C���̐��x�𒲐��B
                target = { target.x + rand() % m_currentRondomAIM, target.y + rand() % m_currentRondomAIM, target.z };
                //�e�۔��ˁB
                Bullet* bullet = NewGO<Bullet>(EnPriority_3DModel);
                bullet->SetPos(headPos);
                bullet->SetRot(m_rot);
                bullet->SetSpeed(110.0f);
                //���C���邽�т�Enemy��AIM�̃����_���l��������B
                m_currentRondomAIM--;
                //����l��ǉ������x�N�g���Ɍ������B
                bullet->SetToTarget(target);
                //�e����Player�ɓ����邩���C�e�X�g�B
                RayTestCallBack::EnemyRayTestResult bulletRayCallBuck;
                PhysicObj().RayTest(headPos, target + headPos, bulletRayCallBuck);
                if (bulletRayCallBuck.hasHit() && bulletRayCallBuck.StaticObjectDist > bulletRayCallBuck.CharacterObjectDist) {
                    //�v���C���[�ɖ����B
                    printf("�����B");
                    //���������̂ōĂ�AIM�̎����i���Ȃ��̂ɁB
                    m_currentRondomAIM = MAX_RANDOM_AIM;
                }
                else {
                    printf("�O�����B");
                }
                m_currentTime = 0;
            }
        }
    }
    else {
        //��O�ɃI�u�W�F�N�g������ or ���ɂ��������Ă��Ȃ��B
        m_enemyState = EnEnemyState_Wandering;
        m_modelRender->PlayAnimation(0, 0.5f);
    }
}

void RifleEnemy::Tracking()
{
    //�܂���Enemy�̓��̃{�[���Ɖ�]���擾�B
    Vector3 headPos;
    Quaternion headRot;
    m_head->CalcWorldTRS(headPos, headRot);
    //Enemy�̒����_���v�Z����B
    Vector3 target = m_toNextCell;
    //m_rot.Apply(target);
    target *= m_VISION;
    target += headPos;

    ////�R�[���o�b�N�֐��B
    //RayTestCallBack::EnemyRayTestResult rayTestCB;
    ////���C�e�X�g�B
    //PhysicObj().RayTest(headPos, target, rayTestCB);
    //if (rayTestCB.hasHit() && rayTestCB.StaticObjectDist > rayTestCB.CharacterObjectDist) {
    //    //��O�ɏ�Q���Ȃ��ł������Ă�B
    //    return true;
    //}
}

void RifleEnemy::Damage()
{
    //printf("hp = %d", m_hp);
    if (m_hp <= 0 && this->IsActive()) {
        printf("���S�I�I\n");
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
    printf("�V�j�}�[�X�B\n");
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
    Vector3 toPlayerDir = m_player->GetPos() - m_pos;
    //�����B
    float toPlayerLen = toPlayerDir.Length();
    //���ʃx�N�g���B
    toPlayerDir.Normalize();
    //�O�����ƃv���C���[�Ɍ������x�N�g���̓��ρB
    float dot = m_toNextCell.Dot(toPlayerDir);
    //�Ȃ��p���v�Z�B
    float angle = acos(dot);
    //����p���ɑ��݂��邩�̔���B
    if (fabsf(angle) < Math::DegToRad(m_FOV) && toPlayerLen < m_VISION) {
        //����p�͈͓̔��i-+)�Ŏ�����B
        return true;
    }
    //�����ĂȂ��B
    return false;
}



