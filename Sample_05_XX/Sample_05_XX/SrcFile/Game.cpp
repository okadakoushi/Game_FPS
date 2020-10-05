#include "stdafx.h"
#include "Game.h"

Game::Game()
{
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
		initState = m_animSampleModel.Init(InitDataModel);
		break;
	case enInitStep_LoadModel:
		//tks�t�@�C���̓ǂݍ��݁B
		initState = m_animSampleModel.LoadTks(InitDataModel);
		break;
	case enInitStep_LoadSkelton:
		//�{�[���̍s��v�Z�A�A�j���[�V�����N���b�v�̗L���̌����B
		initState = m_animSampleModel.initSkeleton();
		break;
	case enInitStep_LoadAnimationClips:
		//�A�j���[�V�����̏������B
		initState = m_animSampleModel.InitAnim();
		break;
	case enInitStep_Completed:
		//�������I���B
		m_inited = true;
		break;
	}
	return m_inited;
}

Game::~Game()
{
}

void Game::Update()
{
	m_animSampleModel.PlayAnim(0, 0.0f);
	//���[���h���W�A�A�j���[�V�����̍X�V�B
	m_animSampleModel.UpdateMatAndAnim();
	m_animSampleModel.Draw(g_graphicsEngine->GetRenderContext());
}