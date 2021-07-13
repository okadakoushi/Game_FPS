#include "stdafx.h"
#include <random>
#include "RifleEnemy.h"
#include "SrcFile/GameObj/Rifle.h"
#include "SrcFile/GameObj/GamePlayer.h"
#include "SrcFile/RayTestCallBack.h"

bool RifleEnemy::Start()
{
	//�G�l�~�[���f���������B
	const char* tkaFile[]{
		"Assets/animData/Enemy/Rifle/Walk.tka",
		"Assets/animData/Enemy/Rifle/Damage.tka",
        "Assets/animData/soldier/singleShot.tka"
	};
	m_modelRender = NewGO<SkinModelRender>(EnPriority_3DModel);
    //�����V���b�g�Đ��Ƃ��ēo�^�B
    m_modelRender->SetAnimLoop(1, false);
	m_modelRender->Init("Assets/modelData/Chara/soldierMob.tkm", tkaFile);
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
        if (Search()) {
            m_modelRender->PlayAnimation(2, 0.1f);
        }
		//�x����ԁB
		break;
	case EnEnemyState_Tracking:
		//�g���b�L���O��ԁB
		break;
	case EnEnemyState_AttackWait:
		//�U���҂������B
		break;
	case EnEnemyState_Attack:
		//�U���I
		break;
    case EnEnemyState_Damage:
        Damage();
        //�_���[�W�B
        break;
	case EnEnemyState_Death:
		//���S�B
		break;
	}
	m_modelRender->SetPosition(m_pos);
    m_modelRender->SetRotation(m_rot);
	m_modelRender->SetScale(m_scale);
    m_collision.Update();
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
        //�����Ă�B
        //m_modelRender->PlayAnimation(0, 0.5f);
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

bool RifleEnemy::Search()
{
    //�܂���Enemy�̓��̃{�[���Ɖ�]���擾�B
    Vector3 headPos;
    Quaternion headRot;
    m_head->CalcWorldTRS(headPos, headRot);
    //Enemy�̒����_���ݒ肷��B
    Vector3 target = m_toNextCell;
    //m_rot.Apply(target);
    target *= m_VISION;
    target += headPos;

    //�R�[���o�b�N�֐��B
    RayTestCallBack::EnemyRayTestResult rayTestCB;
    //���C�e�X�g�B
    PhysicObj().RayTest(headPos, target, rayTestCB);
    if (rayTestCB.hasHit() && rayTestCB.StaticObjectDist > rayTestCB.CharacterObjectDist) {
        //��O�ɏ�Q���Ȃ��ł������Ă�B
        return true;
    }
    return false;
}



void RifleEnemy::Tracking()
{

}

void RifleEnemy::Damage()
{
    m_modelRender->PlayAnimation(1, 0.1f);
    m_enemyState = EnEnemyState_Wandering;
}

bool RifleEnemy::IsFindPlayer()
{
    //�ŏ���Z�����O�B
    Vector3 enemyForward = Vector3::Back;
    //��]���O�������񂷁B
    m_rot.Apply(enemyForward);
    //�v���C���[�Ƃ̋�������B
    Vector3 toPlayerDir = m_player->GetPos() - m_pos;
    //�����B
    float toPlayerLen = toPlayerDir.Length();
    //���ʃx�N�g���B
    toPlayerDir.Normalize();
    //�O�����ƃv���C���[�Ɍ������x�N�g���̓��ρB
    float dot = enemyForward.Dot(toPlayerDir);
    //�Ȃ��p���v�Z�B
    float angle = acos(dot);
    //����p���ɑ��݂��邩�̔���B
    if (fabsf(angle) < Math::DegToRad(m_FOV) && toPlayerLen < m_VISION) {
        //����p�͈͓̔��i-+)�Ŏ�����B
        //�������B
        return true;
    }
    //�����ĂȂ��B
    return false;
}



