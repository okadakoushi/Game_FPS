#include "stdafx.h"
#include "Rifle.h"
#include "GamePlayer.h"

bool Rifle::Start()
{
	//レンダー初期化。
	m_render = NewGO<SkinModelRender>(EnPriority_3DModel);
	m_render->Init("Assets/modelData/Chara/AK103.tkm");

	//プレイヤーのデーターを参照して銃の位置を合わせる。
	m_player = FindGO<GamePlayer>("Player");
	//スケルトン簡易アクセス。
	Skeleton& skelton = m_player->GetRender()->GetSkelton();
	//手のボーンIDを取得。
	int RhandBoneNo = skelton.FindBoneID(L"RHW");
	int LhandBoneNo = skelton.FindBoneID(L"LHW");
	//手ボーンを取得。
	m_rHandBone = skelton.GetBone(RhandBoneNo);
	m_lHandBone = skelton.GetBone(LhandBoneNo);
	return true;
}

void Rifle::Update()
{
	//手の位置を計算。
	m_lHandBone->CalcWorldTRS(m_Lpos, m_Rrot, m_scale);
	m_rHandBone->CalcWorldTRS(m_Rpos, m_Rrot, m_scale);
	//右手から左手伸びるベクトル。
	Vector3 RHtoLH = m_Rpos - m_Lpos;
	m_rot.SetRotation(Vector3::AxisY, atan2f(RHtoLH.x, RHtoLH.z));
	m_render->SetPosition(m_Rpos);
	m_render->SetRotation(m_rot);
	m_render->SetScale({1.5f, 1.5f, 1.5f});
}
