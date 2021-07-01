#include "stdafx.h"
#include "Rifle.h"
#include "GamePlayer.h"

bool Rifle::Start()
{
	//レンダー初期化。
	m_render = NewGO<SkinModelRender>(EnPriority_3DModel);
	m_render->Init("Assets/modelData/Chara/AK103.tkm");

	//スケルトン簡易アクセス。
	Skeleton& skelton = m_refBoneRender->GetSkelton();
	//手のボーンIDを取得。
	int RhandBoneNo = skelton.FindBoneID(L"mixamorig:RightHand");
	//手ボーンを取得。
	m_rHandBone = skelton.GetBone(RhandBoneNo);
	return true;
}

void Rifle::Update()
{
	//手の位置を計算。
	//m_lHandBone->CalcWorldTRS(m_Lpos, m_Rrot, m_scale);
	m_rHandBone->CalcWorldTRS(m_pos, m_rot, m_scale);
	//右手から左手伸びるベクトル。
	//Vector3 RHtoLH = m_Rpos - m_Lpos;
	//RHtoLH.Normalize();
	//RHtoLH.z = 0;
	//m_rot.SetRotation(g_vec3Front, RHtoLH);
	//Y軸側の角度。
	//m_rot.SetRotation(Vector3::AxisY, atan2f(RHtoLH.x, RHtoLH.z));
	//X軸側の角度を求めていく。
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
