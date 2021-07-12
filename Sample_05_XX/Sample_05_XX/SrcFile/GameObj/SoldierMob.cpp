#include "stdafx.h"
#include "SoldierMob.h"
#include "GamePlayer.h"

void SoldierMob::OnDestroy()
{
    DeleteGO(m_modelRender);
}

bool SoldierMob::Start()
{
    //モデル初期化。
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
    
    //find。
    m_player = FindGO<GamePlayer>("Player");

    //コリジョン初期化。
    collision.Init(m_modelRender);

    return true;
}

void SoldierMob::Update()
{
    time += GameTime().GetFrameDeltaTime();

    //switch (m_currentState)
    //{
    //case En_Move://移動処理。
    //    Move();
    //    if (IsFindPlayer() && time > m_helloTime) {
    //        //プレイヤーいたのでけいれーい！！
    //        m_modelRender->PlayAnimation(2, 0.5f);
    //        m_currentState = En_Hello;
    //        time = 0;
    //    }
    //    break;
    //case En_Hello://敬礼。
    //    if (m_modelRender->isPlayAnim() == false) {
    //        //アニメーション終わったので移動に戻す。
    //        m_currentState = En_Move;
    //    }
    //    break;
    //}
    m_modelRender->PlayAnimation(1, 0.5f);
    //コリジョン更新。
    collision.Update();
}

void SoldierMob::Move()
{
    if (m_dirty == false) {
        //更新必要。
        m_nodeList = m_astar.Search(m_pos, m_targetPos, m_naviMesh->GetCellList());
        NaviMesh::Cell* parentCell = m_nodeCell;
        //NaviMeshObj()->AgentNodeRender(m_nodeList);
        //次の目的地は今の場所。
        m_nextTarget = m_pos;
        m_dirty = true;
    }

    //ここから移動処理。
    //次のセルに向かうベクトル。
    if (m_nodeList.size() != 0) {
        Vector3 toNextCell = m_nodeList.front()->m_CenterPos - m_pos;
        //方位化する前に、距離取っとく。
        float dist = toNextCell.Length();
        //方位ベクトル化。
        toNextCell.Normalize();
        //動かす。
        m_pos += toNextCell * m_spped;
        //回転計算。
        m_rot.SetRotation(Vector3::AxisY, atan2f(toNextCell.x * -1.0f, toNextCell.z * -1.0f));

        //ノードに到達したか判定。
        if (dist < 5) {
            m_nodeList.erase(m_nodeList.begin());
        }
        //動いてる。
        m_modelRender->PlayAnimation(0, 0.5f);
    }
    else {
        //動いてない。
        m_modelRender->PlayAnimation(1, 0.5f);
        //更新。
        m_targetPos = m_nextTarget;
        m_dirty = false;
    }
    m_modelRender->SetRotation(m_rot);
    m_modelRender->SetPosition(m_pos);
}

bool SoldierMob::IsFindPlayer()
{
    //最初はZ軸が前。
    Vector3 enemyForward = Vector3::Back;
    //回転分前方向を回す。
    m_rot.Apply(enemyForward);
    //プレイヤーとの距離測る。
    Vector3 toPlayerDir = m_player->GetPos() - m_pos;
    //距離。
    float toPlayerLen = toPlayerDir.Length();
    //方位ベクトル。
    toPlayerDir.Normalize();
    //前方向とプレイヤーに向かうベクトルの内積。
    float dot = enemyForward.Dot(toPlayerDir);
    //なす角を計算。
    float angle = acos(dot);
    //視野角内に存在するかの判定。
    if (fabsf(angle) < Math::DegToRad(m_FOV) && toPlayerLen < m_vision) {
        //視野角の範囲内（-+)で視野内。
        //見つけた。
        return true;
    }
    //見つけてない。
    return false;
}
