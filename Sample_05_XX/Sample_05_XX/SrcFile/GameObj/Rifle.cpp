#include "stdafx.h"
#include "Rifle.h"
#include "GamePlayer.h"

bool Rifle::Start()
{
	//�����_�[�������B
	m_render = NewGO<SkinModelRender>(EnPriority_3DRender);
	m_render->Init("Assets/modelData/Chara/AK103.tkm");

	//�v���C���[�̃f�[�^�[���Q�Ƃ��ďe�̈ʒu�����킹��B
	m_player = FindGO<GamePlayer>("Player");
	//�X�P���g���ȈՃA�N�Z�X�B
	Skeleton& skelton = m_player->GetRender()->GetSkelton();
	//�E��̃{�[��ID���擾�B
	int RhandBoneNo = skelton.FindBoneID(L"mixamorig:RightHand");
	//�E��{�[�����擾�B
	m_rHandBone = skelton.GetBone(RhandBoneNo);
	return true;
}

void Rifle::Update()
{
	m_rHandBone->CalcWorldTRS(m_pos, m_rot, m_scale);
	m_render->SetPosition(m_pos);
	m_render->SetRotation(m_rot);
	m_render->SetScale({1.5f, 1.5f, 1.5f});
}
