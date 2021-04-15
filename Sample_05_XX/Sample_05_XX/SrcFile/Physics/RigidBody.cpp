#include "stdafx.h"
#include "RigidBody.h"
#include "ICollider.h"

RigidBody::~RigidBody()
{
	Release();
}

void RigidBody::Release()
{
}

void RigidBody::Create(RigidBodyInfo& rbInfo)
{
	//初期化。
	Release();
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(rbInfo.pos.x, rbInfo.pos.y, rbInfo.pos.z));
	transform.setRotation(btQuaternion(rbInfo.rot.x, rbInfo.rot.y, rbInfo.rot.z));
	//オフセット適用。
	m_myMotionState = make_unique<btDefaultMotionState>();
	m_myMotionState->setWorldTransform(transform);
	//ローカル。
	btVector3 btLocalInteria;
	rbInfo.localInteria.CopyTo(btLocalInteria);
	//作成。
	btRigidBody::btRigidBodyConstructionInfo btRbinfo(rbInfo.mass, m_myMotionState.get(), rbInfo.collider->GetBody(), btLocalInteria);
	m_rigidBody = make_unique<btRigidBody>(btRbinfo);
}