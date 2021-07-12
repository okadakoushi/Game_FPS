#include "stdafx.h"
#include "SoldierMob.h"
#include "GamePlayer.h"

void SoldierMob::OnDestroy()
{
    DeleteGO(m_modelRender);
}

bool SoldierMob::Start()
{
    //���f���������B
    m_modelRender = NewGO<SkinModelRender>(EnPriority_3DModel);
    const char* tkaFilePaths[]{
        "Assets/animData/soldierMob/walk.tka",
        "Assets/animData/soldierMob/idle.tka",
        "Assets/animData/soldierMob/hello.tka"
    };
    m_modelRender->SetAnimLoop(2, false);
    m_modelRender->Init("Assets/modelData/Chara/soldierMob.tkm", tkaFilePaths);
    m_modelRender->SetRenderMode(enRenderMode_Skin);
    m_modelRender->SetShadwoCaster(true);
    m_modelRender->SetShadowReciever(true);
    Vector3 scale = { 1.5f, 1.5f, 1.5f };
    m_modelRender->SetScale(scale);
    m_modelRender->SetPosition({ 100.0f, 0,0 });
    
    //find�B
    m_player = FindGO<GamePlayer>("Player");

    //�R���W�����������B
    collision.Init(m_modelRender);

    return true;
}

void SoldierMob::Update()
{
    time += GameTime().GetFrameDeltaTime();

    //switch (m_currentState)
    //{
    //case En_Move://�ړ������B
    //    Move();
    //    if (IsFindPlayer() && time > m_helloTime) {
    //        //�v���C���[�����̂ł�����[���I�I
    //        m_modelRender->PlayAnimation(2, 0.5f);
    //        m_currentState = En_Hello;
    //        time = 0;
    //    }
    //    break;
    //case En_Hello://�h��B
    //    if (m_modelRender->isPlayAnim() == false) {
    //        //�A�j���[�V�����I������̂ňړ��ɖ߂��B
    //        m_currentState = En_Move;
    //    }
    //    break;
    //}
    m_modelRender->PlayAnimation(1, 0.5f);
    //�R���W�����X�V�B
    collision.Update();
}

void SoldierMob::Move()
{
    if (m_dirty == false) {
        //�X�V�K�v�B
        m_nodeList = m_astar.Search(m_pos, m_targetPos, m_naviMesh->GetCellList());
        NaviMesh::Cell* parentCell = m_nodeCell;
        //NaviMeshObj()->AgentNodeRender(m_nodeList);
        //���̖ړI�n�͍��̏ꏊ�B
        m_nextTarget = m_pos;
        m_dirty = true;
    }

    //��������ړ������B
    //���̃Z���Ɍ������x�N�g���B
    if (m_nodeList.size() != 0) {
        Vector3 toNextCell = m_nodeList.front()->m_CenterPos - m_pos;
        //���ʉ�����O�ɁA��������Ƃ��B
        float dist = toNextCell.Length();
        //���ʃx�N�g�����B
        toNextCell.Normalize();
        //�������B
        m_pos += toNextCell * m_spped;
        //��]�v�Z�B
        m_rot.SetRotation(Vector3::AxisY, atan2f(toNextCell.x * -1.0f, toNextCell.z * -1.0f));

        //�m�[�h�ɓ��B����������B
        if (dist < 5) {
            m_nodeList.erase(m_nodeList.begin());
        }
        //�����Ă�B
        m_modelRender->PlayAnimation(0, 0.5f);
    }
    else {
        //�����ĂȂ��B
        m_modelRender->PlayAnimation(1, 0.5f);
        //�X�V�B
        m_targetPos = m_nextTarget;
        m_dirty = false;
    }
    m_modelRender->SetRotation(m_rot);
    m_modelRender->SetPosition(m_pos);
}

bool SoldierMob::IsFindPlayer()
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
    if (fabsf(angle) < Math::DegToRad(m_FOV) && toPlayerLen < m_vision) {
        //����p�͈͓̔��i-+)�Ŏ�����B
        //�������B
        return true;
    }
    //�����ĂȂ��B
    return false;
}
