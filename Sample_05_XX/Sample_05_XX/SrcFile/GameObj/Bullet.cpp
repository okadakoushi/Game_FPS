#include "stdafx.h"
#include "Bullet.h"
#include "GamePlayer.h"

void Bullet::OnDestroy()
{
    DeleteGO(m_render);
}

bool Bullet::Start()
{
    //見つける。todo:FindGO使いたくないなぁ。
    m_player = FindGO<GamePlayer>("Player");
    m_pos = m_player->GetPos();
    //NewGO時の長さ計算しとく。
    m_LengthOnNew = m_pos.Length();
    m_render = NewGO<SkinModelRender>(EnPriority_3DRender);
    m_render->Init("Assets/modelData/levelTkm/box.tkm");
    //ベクトルを取得。
    m_toTarget = GraphicsEngineObj()->GetCamera3D().GetCameraToTaget();
    m_toTarget.Normalize();
    return true;
}

void Bullet::Update()
{
    m_pos += m_toTarget * m_speed;

    m_render->SetPosition(m_pos);
    if (m_pos.Length() - m_LengthOnNew >= 2000.0f) {
        //500以上は飛んだから消す。
        DeleteGO(this);
    }
}
