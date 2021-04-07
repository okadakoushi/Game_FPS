#include "stdafx.h"
#include "Rifle.h"
#include "GamePlayer.h"

bool Rifle::Start()
{
	//�����_�[�������B
	m_render = NewGO<SkinModelRender>(EnPriority_3DModel);
	m_render->Init("Assets/modelData/Chara/AK103.tkm");

	//�v���C���[�̃f�[�^�[���Q�Ƃ��ďe�̈ʒu�����킹��B
	m_player = FindGO<GamePlayer>("Player");
	//�X�P���g���ȈՃA�N�Z�X�B
	Skeleton& skelton = m_player->GetRender()->GetSkelton();
	//��̃{�[��ID���擾�B
	int RhandBoneNo = skelton.FindBoneID(L"RHW");
	int LhandBoneNo = skelton.FindBoneID(L"LHW");
	//��{�[�����擾�B
	m_rHandBone = skelton.GetBone(RhandBoneNo);
	m_lHandBone = skelton.GetBone(LhandBoneNo);
	return true;
}

void Rifle::Update()
{
	//��̈ʒu���v�Z�B
	m_lHandBone->CalcWorldTRS(m_Lpos, m_Rrot, m_scale);
	m_rHandBone->CalcWorldTRS(m_Rpos, m_Rrot, m_scale);
	//�E�肩�獶��L�т�x�N�g���B
	Vector3 RHtoLH = m_Rpos - m_Lpos;
	m_rot.SetRotation(Vector3::AxisY, atan2f(RHtoLH.x, RHtoLH.z));
	m_render->SetPosition(m_Rpos);
	m_render->SetRotation(m_rot);
	m_render->SetScale({1.5f, 1.5f, 1.5f});
}
