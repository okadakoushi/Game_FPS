#include "stdafx.h"
#include "Game.h"

Game::Game()
{
	//�A�j���[�V�����̏������X�e�b�v�̓s���ケ���ŏ������B
	//�V���h�E�}�b�v�̏������B
	g_graphicsEngine->GetShadowMap()->Init(1024, 1024, 2000.0f);
	//�V���h�E�L���X�^�[�Ƃ��ēo�^����B
	g_graphicsEngine->GetShadowMap()->RegisterShadowCaster(&m_unityChan);
}

bool Game::Start()
{
	//tka�t�@�C���p�X�B
	const char* tkeFilePath[] = {
		"animData/unityChan/idle.tka",
		"animData/unityChan/run.tka",
		"animData/unityChan/walk.tka"
	};

	//�������f�[�^�B
	ModelInitData InitDataModel;

	InitDataModel.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
	InitDataModel.m_tkaFilePath = "Assets/animData/unityChan/run.tka";
	InitDataModel.m_fxFilePath = "Assets/shader/NoAnimModel_LambertSpecularAmbient.fx";
	InitDataModel.m_expandConstantBuffer = &g_light;
	InitDataModel.m_expandConstantBufferSize = sizeof(g_light);

	//�A�j���[�V�����n�̏������J�n�I
	switch (initState)
	{
	case enInitStep_None:
		//�t�@�C���p�X�̓ǂݍ��݁B
		initState = m_unityChan.Init(InitDataModel);
		break;
	case enInitStep_LoadModel:
		//tks�t�@�C���̓ǂݍ��݁B
		initState = m_unityChan.LoadTks(InitDataModel);
		break;
	case enInitStep_LoadSkelton:
		//�{�[���̍s��v�Z�A�A�j���[�V�����N���b�v�̗L���̌����B
		initState = m_unityChan.initSkeleton();
		break;
	case enInitStep_LoadAnimationClips:
		//�A�j���[�V�����̏������B
		initState = m_unityChan.InitAnim();
		break;
	case enInitStep_Completed:
		//�������I���B
		m_inited = true;
		break;
	}

	//�������I���`�B
	return m_inited;
}

Game::~Game()
{
}

void Game::Update()
{
	//�V���h�E�}�b�v�̍X�V�B
	g_graphicsEngine->GetShadowMap()->Update();
	//�A�j���[�V�����̃v���C�B
	m_unityChan.PlayAnim(0, 0.0f);
	//���[���h���W�A�A�j���[�V�����̍X�V�B
	m_unityChan.UpdateMatAndAnim();
}

void Game::PreRender()
{
	//�`��B
	g_graphicsEngine->GetShadowMap()->RenderToShadowMap();
}

void Game::Draw()
{
	m_unityChan.Draw(g_graphicsEngine->GetRenderContext(), g_camera3D->GetViewMatrix(), g_camera3D->GetProjectionMatrix());
}