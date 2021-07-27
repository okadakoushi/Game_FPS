#include "stdafx.h"
#include "EnemyTrackingState.h"
#include "IEnemyState.h"
#include "SrcFile/GameObj/Enemy/RifleEnemy.h"

void EnemyTrackingState::Enter()
{
	m_enemy->m_modelRender->PlayAnimation(EnEnemyAnimation_Walk, 0.3f);
}

void EnemyTrackingState::Leave()
{

}

void EnemyTrackingState::Update()
{
    printf("トラッキング中！！\n");
    //経路を探査して移動。
    //経路に沿って進行中。
    m_enemy->Move();
    //if(m_dirty == true){
    //    //指定地点に到着。
    //    m_enemyState = EnEnemyState_Wandering;
    //    m_isFindPlayer = false;
    //    //もとの経路探査に戻る。
    //    m_nextTarget = m_paths[m_pathIndex];
    //}

}
