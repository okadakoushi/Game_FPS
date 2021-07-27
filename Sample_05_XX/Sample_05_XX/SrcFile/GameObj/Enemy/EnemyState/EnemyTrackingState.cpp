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
    printf("�g���b�L���O���I�I\n");
    //�o�H��T�����Ĉړ��B
    //�o�H�ɉ����Đi�s���B
    m_enemy->Move();
    //if(m_dirty == true){
    //    //�w��n�_�ɓ����B
    //    m_enemyState = EnEnemyState_Wandering;
    //    m_isFindPlayer = false;
    //    //���Ƃ̌o�H�T���ɖ߂�B
    //    m_nextTarget = m_paths[m_pathIndex];
    //}

}
