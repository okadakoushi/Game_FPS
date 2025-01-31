#include "stdafx.h"
#include "SoldierCollision.h"
#include "SrcFile/Physics/CollisionAttr.h"

SoldierCollision::~SoldierCollision()
{
    Release();
}

void SoldierCollision::Release()
{
    //解放かもー。
    for (auto& col : m_collision) {
        col.Release();
    }
}

void SoldierCollision::Init(SkinModelRender* render)
{
    //コリジョンを作成。
    for (int boneNum = 0; boneNum < COLLISION_COUNT; boneNum++) {
        int boneID = render->GetSkelton().FindBoneID(m_bonePaths[boneNum]);
        m_boneList[boneNum] = render->GetSkelton().GetBone(boneID);
        Vector3 pos;
        Quaternion rot;
        Vector3 scale;  //捨て。
        m_boneList[boneNum]->CalcWorldTRS(pos, rot, scale);
        m_collision[boneNum].CreateBox(pos, rot, m_collisionScales[boneNum]);
        if(m_isEnemy) {
            m_collision[boneNum].GetRigidBody().GetBody()->setUserIndex(enCollisionAttr_Enemy);
            m_collision[boneNum].GetRigidBody().GetBody()->setUserPointer(m_pointer);
        }

    }
    m_isInited = true;
}

void SoldierCollision::Update()
{
    if (m_isInited) {
        //コリジョンを更新。
        for (int boneNum = 0; boneNum < COLLISION_COUNT; boneNum++) {
            Vector3 pos;
            Quaternion rot;
            Vector3 scale;  //捨て。
            m_boneList[boneNum]->CalcWorldTRS(pos, rot, scale);
            auto& rb = m_collision[boneNum].GetRigidBody();
            rb.SetPositionAndRotation(pos, rot);
        }
    }
}
