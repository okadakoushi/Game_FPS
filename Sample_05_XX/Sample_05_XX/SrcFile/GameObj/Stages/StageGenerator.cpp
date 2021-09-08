#include "stdafx.h"
#include "StageGenerator.h"
#include "Stage.h"
#include "BattleStage1.h"
#include "SrcFile/Fade.h"
#include "SrcFile/GameObj/GamePlayer.h"
#include "SrcFile/nature/SkyBox.h"

StageGenerator::StageGenerator()
{
	//�e�X�e�[�W�̃��C�e�B���O�p�����[�^�[��ݒ�B
	//Start�ł���Ă������ȁB

	//s1
	{
		m_ligParams[EnStageNumber_BattleStage1].DirectionLightColor = { 1.4f, 1.4f, 1.4f, 1.0f };
		m_ligParams[EnStageNumber_BattleStage1].DirectionLightDir = { 1.0f, -1.0f, 0.3f };
		m_ligParams[EnStageNumber_BattleStage1].SkySelfLuminous = 1.0f;
		m_ligParams[EnStageNumber_BattleStage1].IBLItensity = 4.0f;
		m_ligParams[EnStageNumber_BattleStage1].MiddleGray = 0.1f;
	}

	//s2
	{
		m_ligParams[EnStageNumber_BattleStage2].DirectionLightColor = { 1.4f, 1.4f, 1.4f, 1.0f };
		m_ligParams[EnStageNumber_BattleStage2].DirectionLightDir = { 1.0f, -1.0f, 0.3f };
		m_ligParams[EnStageNumber_BattleStage2].SkySelfLuminous = 1.0f;
		m_ligParams[EnStageNumber_BattleStage2].IBLItensity = 1.0f;
		m_ligParams[EnStageNumber_BattleStage2].MiddleGray = 0.15f;
	}

	//s3
	{
		m_ligParams[EnStageNumber_BattleStage3].DirectionLightColor = { 15.4f, 15.4f, 15.4f, 1.0f };
		m_ligParams[EnStageNumber_BattleStage3].DirectionLightDir = { 1.0f, -1.0f, 0.3f };
		m_ligParams[EnStageNumber_BattleStage3].SkySelfLuminous = 10.0f;
		m_ligParams[EnStageNumber_BattleStage3].IBLItensity = 1.0f;
		m_ligParams[EnStageNumber_BattleStage3].MiddleGray = 0.18f;
	}

	//s4 �v�\��B
	{
		m_ligParams[EnStageNumber_BattleStage4].DirectionLightColor = { 1.4f, 1.4f, 1.4f, 1.0f };
		m_ligParams[EnStageNumber_BattleStage4].DirectionLightDir = { 1.0f, -1.0f, 0.3f };
		m_ligParams[EnStageNumber_BattleStage4].SkySelfLuminous = 3.0f;
		m_ligParams[EnStageNumber_BattleStage4].IBLItensity = 2.0f;
		m_ligParams[EnStageNumber_BattleStage4].MiddleGray = 0.23f;
	}
}

void StageGenerator::OnDestroy()
{
	DeleteCurrentStage();
	DeleteGO(m_windSE);
}

bool StageGenerator::Start()
{
	//����SE�B
	m_windSE = NewGO<SoundSource>(0);
	m_windSE->Init(L"Assets/Audio/wind.wav");
	m_windSE->SetVolume(0.1f);

	//�t�F�[�h�B
	m_fade = NewGO<Fade>(EnPriority_UI);

	m_currentStageNum = EnStageNumber_BattleStage1;

	return true;
}

void StageGenerator::Update()
{
	m_windSE->Play(true);

	if (!m_isStageDeleteCall) {
		//�X�e�[�W�폜������łȂ��B
		return;
	}
	if (m_currentStageNum == EnStageNumber_BattleStage1 || m_currentStageNum == EnStageNumber_BattleStage3) {
		if (m_stage->IsStart()) {
			//���x���̏��������I���B
			FadeProcess(false);
			m_isStageDeleteCall = false;
		}
	}
	else if (m_currentStageNum == EnStageNumber_BattleStage2 || m_currentStageNum == EnStageNumber_BattleStage4) {
		if (m_stage->IsStart()) {
			//���x���̏��������I���B
			FadeProcess(false);
			m_isStageDeleteCall = false;
		}
	}
}

bool StageGenerator::FadeProcess(const bool& fadeIn)
{
	if (m_firstFadeCall) {
		m_fade->StartFade(fadeIn);
		m_firstFadeCall = false;
	}

	if (m_fade->IsFinishFade()) {
		//�t�F�[�h�I��
		m_firstFadeCall = true;
		return true;
	}

	if (!fadeIn) {
		//�t�F�[�h�A�E�g�̏ꍇ�͑��������B
		m_firstFadeCall = true;
	}

	return false;
}

void StageGenerator::CreateStage(const StageNumber& stageNum)
{
	//�����œn���ꂽ�X�e�[�W�ԍ��ɉ����ăX�e�[�W�𐶐�����B
	if (stageNum == EnStageNumber_BattleStage1) {
		m_stage = NewGO<Stage>(EnPriority_3DModel);
	}
	else if (stageNum == EnStageNumber_BattleStage2) {
		m_stage = NewGO<BattleStage1>(EnPriority_3DModel);
	}
	else if (stageNum == EnStageNumber_BattleStage3) {
		m_stage = NewGO<Stage>(EnPriority_3DModel);
	}
	else if (stageNum == EnStageNumber_BattleStage4) {
		m_stage = NewGO<BattleStage1>(EnPriority_3DModel);
	}
	else {
		//printf("StageNum = %d", stageNum);
		//�G���[�B
		MessageBoxA(
			nullptr, 
			"�X�e�[�W���폜�ς݂܂��́A�����œn���ꂽ�X�e�[�W�͑��݂��܂���B",
			"StageGenerator", 
			MB_OK
		);
	}


	//player�ݒ�B
	m_stage->SetPlayer(m_player);
	//�W�F�l���[�^�[�ݒ�B
	m_stage->SetStageGenerator(this);

	//�X�e�[�W�ԍ��ɍ��킹�ăp�����[�^�[��ݒ肵�Ă����B
	//Dir���C�g�̃p�����[�^�[��ݒ�B
	g_light.directionalLight[0].color = m_ligParams[stageNum].DirectionLightColor;
	g_light.directionalLight[0].direction = m_ligParams[stageNum].DirectionLightDir;
	//SkyBox�̉���B
	if (m_sky != nullptr) {
		DeleteGO(m_sky);
	}
	//SkyBox���C���X�^���X���B
	m_sky = NewGO<SkyBox>(EnPriority_3DModel, "SkyBox");
	//Sky�p�̃p�����[�^�[��ݒ�B
	m_sky->SetSkyCubeTexturePath(m_skyBoxFilePaths[stageNum]);
	m_sky->SetSelfLuminous(m_ligParams[stageNum].SkySelfLuminous);
	//IBL�p�̋P�x��ݒ�B
	GraphicsEngineObj()->GetDefferd().GetDefferdSprite().SetIBLItensity(m_ligParams[stageNum].IBLItensity);
	//�g�[���}�b�v�p�~�h���O���C��ݒ�B
	GraphicsEngineObj()->GetRenderingEngine().GetToneMap().SetMiddleGray(m_ligParams[stageNum].MiddleGray);
	//���݂̃X�e�[�W�ԍ����X�V����B
	m_currentStageNum = stageNum;
}

void StageGenerator::DeleteCurrentStage()
{
	DeleteGO(m_stage);
	m_player->Init();
	m_isStageDeleteCall = true;
	return;
}
