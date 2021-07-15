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
	//m_currentRifleEvent = EnRifleEvent_None;

	if (m_currentAmo <= 0 || m_currentRifleEvent == EnRifleEvent_Reloading) {
		//残弾不足かリロード中。
		m_currentRifleEvent = EnRifleEvent_NoAmo;
		if (RELOADTIME > m_currentReloadTime) {
			//リロード中。
			m_currentReloadTime += GameTime().GetFrameDeltaTime();
			m_currentRifleEvent = EnRifleEvent_Reloading;
		}
		else {
			//リロード終わり。
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
