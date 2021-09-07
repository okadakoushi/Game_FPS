#include "stdafx.h"
#include "EnemyAttackState.h"
#include "IEnemyState.h"
#include "SrcFile/GameObj/GamePlayer.h"
#include "SrcFile/GameObj/Enemy/RifleEnemy.h"
#include "SrcFile/GameObj/Rifle.h"

void EnemyAttackState::Enter()
{
    m_enemy->GetRender()->PlayAnimation(EnEnemyAnimation_Shoot, 0.3f);
    m_lastUpdateState = this;
}

void EnemyAttackState::Leave()
{
}

void EnemyAttackState::Update()
{
    //エネミー頭からプレイヤー。
    Vector3 Head2PlayerDir = m_enemy->GetPlayerPtr()->GetPos() - m_enemy->GetHeadPos();
    Head2PlayerDir.Normalize();
   
    //タイマー加算。
    m_weponRaitTimer += GameTime().GetFrameDeltaTime();

    //プレイヤーを見つけたので次の目的地はプレイヤーの位置に設定。
    m_enemy->GetAgent().SetTargetPos(m_enemy->GetPlayerPtr()->GetPos());
    //Playerを発見した。
    m_enemy->GetPlayerPtr()->GetPlayerUIs()->AddFindList(m_enemy);
    if (m_enemy->GetRender()->GetAnimLoop() || !m_enemy->GetRender()->isPlayAnim()) {
        //アニメーションがループするものまたは、ループしないものでアニメーションが流れていない。
        //アニメーションを切り替え。
        m_enemy->GetRender()->PlayAnimation(EnEnemyAnimation_Shoot, 0.1f);
        //プレイヤーの方向に向ける。
        m_enemy->GetRotation().SetRotation(Vector3::AxisY, atan2f(Head2PlayerDir.x * 1.0f, Head2PlayerDir.z * 1.0f));
        if (m_enemy->GerRifle()->GetWeponRaito() * 1.5f < m_weponRaitTimer) {
            m_shootSE->SetPosition(m_enemy->GetPosition());
            m_shootSE->Stop();
            m_shootSE->Play();
            //レイの長さを調整。
            Vector3 target = Head2PlayerDir;
            target *= m_enemy->GerRifle()->GetWeponEnableRange();
            //エネミーのエイムの精度を調整。
            target = { target.x + rand() % m_currentRondomAIM, target.y + rand() % m_currentRondomAIM, target.z };
            //発砲するたびにEnemyのAIMのランダム値を下げる。
            if (m_currentRondomAIM > 1) {
                m_currentRondomAIM--;
            }
            //乱れ値を追加したベクトルに向かう。
            //bullet->SetToTarget(target);
            //弾道がPlayerに当たるかレイテスト。
            RayTestCallBack::EnemyRayTestResult bulletRayCallBuck;
            PhysicObj().RayTest(m_enemy->GetHeadPos(), target + m_enemy->GetHeadPos(), bulletRayCallBuck);
            if (bulletRayCallBuck.hasHit() && bulletRayCallBuck.StaticObjectDist > bulletRayCallBuck.CharacterObjectDist) {
                //プレイヤーに命中。
                m_enemy->GetPlayerPtr()->DamageToPlayer(ATTACK);
                printf("命中。");
                //命中したので再びAIMの質を低品質なものに。
                m_currentRondomAIM = MAX_RANDOM_AIM;
            }
            else {
                //printf("外した。");
            }
            //タイマーリセット。
            m_weponRaitTimer = 0;
        }
    }
}