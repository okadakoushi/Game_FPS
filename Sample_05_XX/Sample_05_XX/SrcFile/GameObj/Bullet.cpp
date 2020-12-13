#include "stdafx.h"
#include "Bullet.h"
#include "GamePlayer.h"

void Bullet::OnDestroy()
{
    DeleteGO(m_render);
}

bool Bullet::Start()
{
    //������Btodo:FindGO�g�������Ȃ��Ȃ��B
    m_player = FindGO<GamePlayer>("Player");
    m_pos = m_player->GetPos();
    //NewGO���̒����v�Z���Ƃ��B
    m_LengthOnNew = m_pos.Length();
    m_render = NewGO<SkinModelRender>(EnPriority_3DRender);
    m_render->Init("Assets/modelData/levelTkm/box.tkm");
    //�x�N�g�����擾�B
    m_toTarget = GraphicsEngineObj()->GetCamera3D().GetCameraToTaget();
    m_toTarget.Normalize();
    return true;
}

void Bullet::Update()
{
    m_pos += m_toTarget * m_speed;

    m_render->SetPosition(m_pos);
    if (m_pos.Length() - m_LengthOnNew >= 2000.0f) {
        //500�ȏ�͔�񂾂�������B
        DeleteGO(this);
    }
}
