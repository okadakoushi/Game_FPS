#include "stdafx.h"
#include "Rifle.h"
#include "GamePlayer.h"

bool Rifle::Start()
{
	//�����_�[�������B
	m_render = NewGO<SkinModelRender>(EnPriority_3DModel);
	m_render->Init("Assets/modelData/Chara/AK103.tkm");

	//�X�P���g���ȈՃA�N�Z�X�B
	Skeleton& skelton = m_refBoneRender->GetSkelton();
	//��̃{�[��ID���擾�B
	int RhandBoneNo = skelton.FindBoneID(L"mixamorig:RightHand");
	//��{�[�����擾�B
	m_rHandBone = skelton.GetBone(RhandBoneNo);
	return true;
}

void Rifle::Update()
{
	//m_currentRifleEvent = EnRifleEvent_None;

	if (m_currentAmo <= 0 || m_currentRifleEvent == EnRifleEvent_Reloading) {
		//�c�e�s���������[�h���B
		m_currentRifleEvent = EnRifleEvent_NoAmo;
		if (RELOADTIME > m_currentReloadTime) {
			//�����[�h���B
			m_currentReloadTime += GameTime().GetFrameDeltaTime();
			m_currentRifleEvent = EnRifleEvent_Reloading;
		}
		else {
			//�����[�h�I���B
			m_currentReloadTime = 0.0f;
			m_currentRifleEvent = EnRifleEvent_None;
			m_currentAmo = MAX_AMO;
		}
	}

	m_rHandBone->CalcWorldTRS(m_pos, m_rot, m_scale);
	m_render->SetPosition(m_pos);
	m_render->SetRotation(m_rot);
	m_render->SetScale({1.5f, 1.5f, 1.5f});
}
