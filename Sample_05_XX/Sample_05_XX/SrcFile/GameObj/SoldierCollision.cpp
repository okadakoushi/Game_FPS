#include "stdafx.h"
#include "SoldierCollision.h"

void SoldierCollision::Init(SkinModelRender* render)
{
    //�R���W�������쐬�B
    for (int boneNum = 0; boneNum < COLLISION_COUNT; boneNum++) {
        int boneID = render->GetSkelton().FindBoneID(m_bonePaths[boneNum]);
        m_boneList[boneNum] = render->GetSkelton().GetBone(boneID);
        Vector3 pos;
        Quaternion rot;
        Vector3 scale;  //�̂āB
        m_boneList[boneNum]->CalcWorldTRS(pos, rot, scale);
        m_collision[boneNum].CreateBox(pos, rot, m_collisionScales[boneNum]);
    }
    m_isInited = true;
}

void SoldierCollision::Update()
{
    if (m_isInited) {
        //�R���W�������X�V�B
        for (int boneNum = 0; boneNum < COLLISION_COUNT; boneNum++) {
            Vector3 pos;
            Quaternion rot;
            Vector3 scale;  //�̂āB
            m_boneList[boneNum]->CalcWorldTRS(pos, rot, scale);
            auto& rb = m_collision[boneNum].GetRigidBody();
            rb.SetPositionAndRotation(pos, rot);
        }
    }
}
