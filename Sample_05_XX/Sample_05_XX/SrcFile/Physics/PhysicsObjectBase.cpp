#include "stdafx.h"
#include "PhysicsObjectBase.h"
#include "BoxCollider.h"
#include "MeshCollider.h"

void PhysicsObjectBase::CreateBox(Vector3 pos, Quaternion rot, Vector3 size)
{
	Release();
	auto boxCollider = make_unique<BoxCollider>();
	boxCollider->Create(size);
	m_collider = move(boxCollider);
	CreateCommon(pos, rot);
}

void PhysicsObjectBase::CreateMesh(Vector3 pos, Quaternion rot, const TkmFile& tkmfile)
{
	Release();
	auto meshCollider = make_unique<MeshCollider>();
	meshCollider->CreateFromTkmFile(tkmfile, nullptr);
	m_collider = move(meshCollider);
	CreateCommon(pos, rot);
}

void PhysicsObjectBase::CreateMesh(Vector3 pos, Quaternion rot, Vector3 scale, SkinModelRender* modelRender)
{
	CreateMesh(pos, rot, scale, modelRender->GetModel());
}

void PhysicsObjectBase::CreateMesh(Vector3 pos, Quaternion rot, Vector3 scale, const Model& model)
{
	Release();
	Matrix mScale;
	mScale.MakeScaling(scale);
	auto meshCollider = make_unique<MeshCollider>();
	meshCollider->CreateFromModel(model, &mScale);
	m_collider = move(meshCollider);
	CreateCommon(
		pos,
		rot
	);
}
