#include "stdafx.h"
#include "Bullet.h"
#include "GamePlayer.h"

void Bullet::OnDestroy()
{
    DeleteGO(m_render);
}

bool Bullet::Start()
{
    //NewGO時の長さ計算しとく。
    m_LengthOnNew = m_pos.Length();
    m_render = NewGO<SkinModelRender>(EnPriority_3DModel);
    m_render->Init("Assets/modelData/chara/bullet_7mm.tkm");
    m_render->SetShadwoCaster(true);
    //ベクトルを取得。
    //m_toTarget = GraphicsEngineObj()->GetCamera3D().GetCameraToTaget();
    //m_toTarget.Normalize();
    return true;
}

void Bullet::Update()
{
    m_toTarget.Normalize();
    m_pos += m_toTarget * m_speed;

    m_render->SetPosition({m_pos.x, m_pos.y, m_pos.z});
    m_render->SetRotation(m_rot);
    if (m_pos.Length() - m_LengthOnNew >= EffectRange) {
        //500以上は飛んだから消す。
        DeleteGO(this);
    }
}
