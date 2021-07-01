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
	//��̈ʒu���v�Z�B
	//m_lHandBone->CalcWorldTRS(m_Lpos, m_Rrot, m_scale);
	m_rHandBone->CalcWorldTRS(m_pos, m_rot, m_scale);
	//�E�肩�獶��L�т�x�N�g���B
	//Vector3 RHtoLH = m_Rpos - m_Lpos;
	//RHtoLH.Normalize();
	//RHtoLH.z = 0;
	//m_rot.SetRotation(g_vec3Front, RHtoLH);
	//Y�����̊p�x�B
	//m_rot.SetRotation(Vector3::AxisY, atan2f(RHtoLH.x, RHtoLH.z));
	//X�����̊p�x�����߂Ă����B
	//float angle = atan2f(RHtoLH.z, RHtoLH.y);
	//RHtoLH.Normalize();
	//m_Lpos.Normalize();
	//Vector3 Axis = Cross(RHtoLH, { 0 ,m_Lpos.y, 0 });
	//Quaternion qRot;
	//qRot.SetRotation(Axis, angle);
	m_render->SetPosition(m_pos);
	m_render->SetRotation(m_rot);
	m_render->SetScale({1.5f, 1.5f, 1.5f});
}
