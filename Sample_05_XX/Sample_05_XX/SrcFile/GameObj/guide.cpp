#include "stdafx.h"
#include "guide.h"
#include "GamePlayer.h"
#include "StageGenerator.h"

void Guide::OnDestroy()
{
	DeleteGO(m_skinModelRender);
}

bool Guide::Start()
{
	m_stageGenerator = FindGO<StageGenerator>("StageGenerator");
	//���f��
	const char* animFilePaths[] = {
		"Assets/animData/guide/idle.tka"
	};
	m_skinModelRender = NewGO<SkinModelRender>(EnPriority_3DModel);
	m_skinModelRender->Init("Assets/modelData/Chara/guide.tkm", animFilePaths);
	m_skinModelRender->SetScale({ 1.2f, 1.2f, 1.2f});
	m_player = FindGO<GamePlayer>("Player");
	m_collision.SetEnemyFlag(false);
	m_collision.Init(m_skinModelRender);
	return true;
}

void Guide::Update()
{
	m_skinModelRender->SetPosition(m_pos);

	//�X�e�[�W�J�ڏ����B
	if (ClacPossibleTrans()) {
		//�o�ߎ��Ԃ����Z�B
		m_changeStageTimer += GameTime().GetFrameDeltaTime();
		if (m_changeStageTimer > m_CHANGESCENETIME) {
			//�X�e�[�W�ԍ�����Ƃ��B
			int stageNum = m_stageGenerator->GetCurrentStageNumber();
			m_stageGenerator->DeleteCurrentStage();
			if (stageNum == StageGenerator::EnStageNumber_BattleStage
				|| stageNum == StageGenerator::EnStageNumber_BattleStage2
				|| stageNum == StageGenerator::EnStageNumber_BattleStage3) 
			{
				//�o�g���X�e�[�W�̏ꍇ�͈�U�X�^���o�C�ɖ߂��B
				m_stageGenerator->CreateStage(StageGenerator::EnStageNumber_StanbyStage);
			}
			//test
			else if (stageNum == StageGenerator::EnStageNumber_StanbyStage) {
				m_stageGenerator->CreateStage(StageGenerator::EnStageNumber_BattleStage);
			}
		}
	}
	else {
		//���͈͂���ł��Ⴄ�ƃ^�C�}�[���Z�b�g�B
		m_changeStageTimer = 0.0f;
	}
	m_collision.Update();
}

bool Guide::ClacPossibleTrans()
{
	Vector3 toGuide = m_pos - m_player->GetPos();
	float dist = toGuide.Length();
	if (dist < 150.0f) {
		return true;
	}
	else {
		return false;
	}
}
