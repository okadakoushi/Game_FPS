#include "stdafx.h"
#include "Game.h"

Game::Game()
{
	//�A�j���[�V�����̏������X�e�b�v�̓s���ケ���ŏ������B
	//�V���h�E�}�b�v�̏������B
	g_graphicsEngine->GetShadowMap()->Init(2048, 2048, 1000.0f);
	//�V���h�E�L���X�^�[�Ƃ��ēo�^����B
	g_graphicsEngine->GetShadowMap()->RegisterShadowCaster(&m_unityChan);
	m_pos = { 0.0f,0.0f,0.0f };
}

bool Game::Start()
{
	////tka�t�@�C���p�X�B
	//const cha	* tkeFilePath[] = {
	//	"animData/unityChan/idle.tka",
	//	"animData/unityChan/run.tka",
	//	"animData/unityChan/walk.tka"
	//};

	//�������f�[�^�B
	ModelInitData InitDataModel;

	InitDataModel.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
	InitDataModel.m_tkaFilePath = "Assets/animData/unityChan/run.tka";
	InitDataModel.m_fxFilePath = "Assets/shader/NoAnimModel_LambertSpecularAmbient.fx";
	InitDataModel.m_expandConstantBuffer = &g_light;
	InitDataModel.m_expandConstantBufferSize = sizeof(g_light);

	ModelInitData InitStage;
	InitStage.m_tkmFilePath = "Assets/modelData/bgReo/bg.tkm";
	InitStage.m_fxFilePath = "Assets/shader/NoAnimModel_LambertSpecularAmbient.fx";
	InitStage.m_expandConstantBuffer = &g_light;
	InitStage.m_expandConstantBufferSize = sizeof(g_light);

	//�A�j���[�V�����n�̏������J�n�I
	switch (initState)
	{
	case enInitStep_None:
		//�t�@�C���p�X�̓ǂݍ��݁B
		initState = m_unityChan.Init(InitDataModel);
		m_bg.Init(InitStage);
		break;
	case enInitStep_LoadModel:
		//tks�t�@�C���̓ǂݍ��݁B
		initState = m_unityChan.LoadTks(InitDataModel);
		m_bg.LoadTks(InitDataModel);
		break;
	case enInitStep_LoadSkelton:
		//�{�[���̍s��v�Z�A�A�j���[�V�����N���b�v�̗L���̌����B
		initState = m_unityChan.initSkeleton(InitDataModel);
		m_bg.initSkeleton(InitDataModel);
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
	//�A�j���[�V�����̃v���C�B
	m_unityChan.PlayAnim(0, 0.0f);
	if (GetAsyncKeyState('A')) {
		//m_pos.x++;
	}
	else if (GetAsyncKeyState('D')) {
		//m_pos.x--;
	}
	else if (GetAsyncKeyState('W')) {
		m_pos.z++;
	}
	else if (GetAsyncKeyState('S')) {
		m_pos.z--;
	}
	m_unityChan.SetPosition(m_pos);
	//���[���h���W�A�A�j���[�V�����̍X�V�B
	m_unityChan.UpdateMatAndAnim();
	Quaternion qRot = Quaternion::Identity;
	qRot.SetRotationDeg(Vector3::AxisX, -90.0f);
	m_bg.SetRotation(qRot);
	m_bg.UpdateMatAndAnim();
	m_bg.SetShadowReciever();
	//�V���h�E�}�b�v�̍X�V�B
	g_graphicsEngine->GetShadowMap()->Update();
}

void Game::PreRender()
{
	//�`��Btodo : engineUpdate
	g_graphicsEngine->GetShadowMap()->RenderToShadowMap();
}

void Game::Draw()
{
	m_unityChan.Draw(g_graphicsEngine->GetRenderContext(), g_camera3D->GetViewMatrix(), g_camera3D->GetProjectionMatrix());
	//�X�L���Ȃ��`��B
	m_bg.Draw(g_graphicsEngine->GetRenderContext(), g_camera3D->GetViewMatrix(), g_camera3D->GetProjectionMatrix(), enRenderMode_NonSkin);
}