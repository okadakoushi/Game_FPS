#include "stdafx.h"
#include "Rifle.h"
#include "GamePlayer.h"

bool Rifle::Start()
{
	//レンダー初期化。
	m_render = NewGO<SkinModelRender>(EnPriority_3DRender);
	m_render->Init("Assets/modelData/Chara/AK103.tkm");

	//プレイヤーのデーターを参照して銃の位置を合わせる。
	m_player = FindGO<GamePlayer>("Player");
	//スケルトン簡易アクセス。
	Skeleton& skelton = m_player->GetRender()->GetSkelton();
	//右手のボーンIDを取得。
	int RhandBoneNo = skelton.FindBoneID(L"mixamorig:RightHand");
	//右手ボーンを取得。
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
