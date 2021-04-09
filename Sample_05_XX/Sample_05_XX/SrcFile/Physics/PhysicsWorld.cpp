#include "stdafx.h"
#include "PhysicsWorld.h"

PhysicsWorld::~PhysicsWorld()
{
	Release();
}

void PhysicsWorld::Release()
{
	m_dynamicWorld.reset();
	m_constrainSolver.reset();
	m_overlappingPairCache.reset();
	m_collisionDispatcher.reset();
	m_collisionConfig.reset();
}

void PhysicsWorld::Init()
{
	Release();

	//�����G���W���������B
	//...�Փˍ\���ɂ́A�������̃f�t�H���g�ݒ�A�Փːݒ肪�܂܂�܂��B �㋉���[�U�[�́A�Ǝ��̍\�����쐬�ł��܂��B
	m_collisionConfig = make_unique<btDefaultCollisionConfiguration>();
	//�f�t�H���g�̏Փ˃f�B�X�p�b�`�����g�p���܂��B ���񏈗��̏ꍇ�́A�ʂ̃f�B�X�p�b�`���[���g�p�ł��܂��iExtras / Bullet MultiThreaded���Q�Ɓj�B
	m_collisionDispatcher = make_unique<btCollisionDispatcher>(m_collisionConfig.get());
	//...btDbvtBroadphase�́A�D�ꂽ�ėp�u���[�h�t�F�[�Y�ł��B btAxis3Sweep���������Ƃ��ł��܂��B
	//�u���[�h�t�F�C�Y��܂��ȓ����蔻��B�S�I�u�W�F�N�g��AABB�Ƃ��ē����蔻�肷��݂����Ȋ������낤���B
	//�u���[�h�t�F�C�Y��A�i���[�t�F�C�Y�Ƃ��������ȓ����蔻�������B
	m_overlappingPairCache = make_unique<btDbvtBroadphase>();
	//...�f�t�H���g�̐���\���o�[�B ���񏈗��̏ꍇ�́A�ʂ̃\���o�[���g�p�ł��܂��iExtras / BulletMultiThreaded���Q�Ɓj�B
	m_constrainSolver = make_unique<btSequentialImpulseConstraintSolver>();

	m_dynamicWorld = make_unique<btDiscreteDynamicsWorld>(
		m_collisionDispatcher.get(),
		m_overlappingPairCache.get(),
		m_constrainSolver.get(),
		m_collisionConfig.get()
		);

	//�d�͐ݒ�B
	m_dynamicWorld->setGravity(btVector3(0.0f, -10.0f, 0.0f));
}

void PhysicsWorld::Update()
{
	m_dynamicWorld->stepSimulation(GameTime().GetFrameDeltaTime());
}

void PhysicsWorld::ContactTest(btCollisionObject* colObj, std::function<void(const btCollisionObject& contactCollisionObject)> cb)
{
	
}