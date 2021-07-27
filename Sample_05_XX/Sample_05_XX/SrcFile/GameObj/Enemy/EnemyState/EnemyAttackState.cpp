#include "stdafx.h"
#include "EnemyAttackState.h"
#include "IEnemyState.h"
#include "SrcFile/GameObj/GamePlayer.h"
#include "SrcFile/GameObj/Enemy/RifleEnemy.h"

void EnemyAttackState::Enter()
{
    m_enemy->m_modelRender->PlayAnimation(EnEnemyAnimation_Shoot, 0.3f);
}

void EnemyAttackState::Leave()
{
}

void EnemyAttackState::Update()
{
    //プレイヤーを見つけたので次の目的地はプレイヤーの位置に設定。
    m_enemy->m_agent.SetTargetPos(m_enemy->m_player->GetPos());
    //Playerを発見した。
    m_enemy->m_player->GetPlayerUIs()->AddFindList(m_enemy);
    if (m_enemy->m_modelRender->GetAnimLoop() || !m_enemy->m_modelRender->isPlayAnim()) {
        //アニメーションがループするものまたは、ループしないものでアニメーションが流れていない。
        //アニメーションを切り替え。
        m_enemy->m_modelRender->PlayAnimation(2, 0.1f);
        //プレイヤーの方向に向ける。
        m_enemy->m_rot.SetRotation(Vector3::AxisY, atan2f(m_enemy->m_toPlayerDir.x * 1.0f, m_enemy->m_toPlayerDir.z * 1.0f));
        if (m_enemy->COOLDOWN < m_enemy->m_currentTime) {
            //レイの長さを調整。
            Vector3 target = m_enemy->m_toPlayerDir;
            target *= m_enemy->m_VISION;
            //エネミーのエイムの精度を調整。
            target = { target.x + rand() % m_enemy->m_currentRondomAIM, target.y + rand() % m_enemy->m_currentRondomAIM, target.z };
            //弾丸発射。
            //Bullet* bullet = NewGO<Bullet>(EnPriority_3DModel);
            //bullet->SetPos(headPos);
            //bullet->SetRot(m_rot);
            //bullet->SetSpeed(110.0f);
            //発砲するたびにEnemyのAIMのランダム値を下げる。
            m_enemy->m_currentRondomAIM--;
            //乱れ値を追加したベクトルに向かう。
            //bullet->SetToTarget(target);
            //弾道がPlayerに当たるかレイテスト。
            RayTestCallBack::EnemyRayTestResult bulletRayCallBuck;
            PhysicObj().RayTest(m_enemy->m_headPos, target + m_enemy->m_headPos, bulletRayCallBuck);
            if (bulletRayCallBuck.hasHit() && bulletRayCallBuck.StaticObjectDist > bulletRayCallBuck.CharacterObjectDist) {
                //プレイヤーに命中。
                m_enemy->m_player->DamageToPlayer(m_enemy->ATTACK);
                //printf("命中。");
                //命中したので再びAIMの質を低品質なものに。
                m_enemy->m_currentRondomAIM = m_enemy->MAX_RANDOM_AIM;
            }
            else {
                //printf("外した。");
            }
            m_enemy->m_currentTime = 0;
        }
    }
}