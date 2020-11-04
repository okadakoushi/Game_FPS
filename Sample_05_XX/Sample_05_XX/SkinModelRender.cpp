#include "stdafx.h"
#include "SkinModelRender.h"
#include <fstream>

SkinModelRender::SkinModelRender()
{
}

SkinModelRender::~SkinModelRender()
{
}

bool SkinModelRender::Start()
{
	//�������I������H
	bool inited = false;
	switch (m_initStep)
	{
	case enInitStep_None:
		//���f���̏������܂��B
		break;
	case enInitStep_LoadSkelton:
		//swich case�ɂ��R���X�g���N�^���X�L�b�v����邱�Ƃ�����̂�
		//{...}�ň͂ށB
	{
		//�X�P���g�������[�h����B
		//���f���̏������A���b�V���쐬���I����Ă���B
		//�X�P���g���f�[�^�̓ǂݍ��݁B
		std::string skeltonFilePath = m_filePath;
		int pos = (int)skeltonFilePath.find(".tkm");
		//.tkm��.tks�ɒu�������B
		skeltonFilePath.replace(pos, 4, ".tks");
		//�t�@�C���X�g���[���B
		std::ifstream ifs(skeltonFilePath);
		if (ifs.is_open()) {
			//tks�t�@�C��������̂œǂݍ��ށB
			m_skeleton.Init(skeltonFilePath.c_str());
			m_initStep = enInitStep_LoadAnimation;
		}
		else {
			//�Ȃ������B
			m_initStep = enInitStep_Completed;
		}
	}
		break;
	case enInitStep_LoadAnimation:
		//�o�C���h�|�[�Y�Ƃ��̌v�Z�B
		m_skeleton.BuildBoneMatrices();
		//�X�P���g���ƃ��f���̊֘A�t���B
		if (m_skeleton.IsInited()) {
			//�X�P���g��������������Ă��B
			//���f���ƃX�P���g�������ѕt���B
			m_model.BindSkeleton(m_skeleton);
		}

		//�A�j���[�V�����N���b�v�̃��[�h�B
		if (m_tkaFilePaths.empty() == false) {
			//�A�j���[�V�����N���b�v���������B
			for (auto& m_tkaFilePath : m_tkaFilePaths) {
				//�A�j���[�V�����̃t�@�C���p�X���񂷁B
				auto animClip = make_unique<AnimationClip>();
				//�t�@�C���p�X�����[�h�B
				animClip->Load(m_tkaFilePath.c_str());
				//�A�j���[�V�����N���b�v�Ƀ��������ڏ��B
				m_animationClips.push_back(move(animClip));
			}
		}

	case enInitStep_InitAnimationClip: {
		bool isLoaded = true;
		for (auto& animClip : m_animationClips) {
			if (animClip->IsLoaded() == false) {
				isLoaded = false;
				break;
			}
		}
		if (isLoaded == true) {
			//�S�A�j���[�V�����N���b�v�̃��[�h�I���B
			//�L�[�t���[���ƃA�j���[�V�����N���b�v�̍\�z�B
			for (auto& animClip : m_animationClips) {
				animClip->SetLoopFlag(true);
				animClip->BuildKeyFrames();
			}
			//�A�j���[�V�������������B
			m_animation.Init(m_skeleton, m_animationClips);
			//�A�j���[�V�����N���b�v�̃��[�h�I���B
			m_initStep = enInitStep_Completed;
		}
	}
		break;
	case enInitStep_Completed:
		//�������I���I
		inited = true;
		break;
	}
	return inited;
}

void SkinModelRender::Init(const char* tkmFile, const char* tkaFile)
{
	m_model.InitModel(tkmFile);
	//todo : ����pushBuck
	if (tkaFile != nullptr) {
		m_tkaFilePaths.push_back(tkaFile);
	}
	//���f���̓ǂݍ��ݏI���B
	m_initStep = enInitStep_LoadSkelton;
	m_filePath = tkmFile;
}

void SkinModelRender::Update()
{
	//unitychan���V���h�E�L���X�^�[�Ƃ��ēo�^�B
	if (ShadowCaster == true) {
		GraphicsEngineObj()->GetShadowMap()->RegisterShadowCaster(&m_model);
	}

	if (m_animation.IsInited()) {
		//�A�j���[�V��������������Ă��B
		m_animation.Progress(DELTA_TIME);
	}
	//���[���h���W�X�V�B
	m_model.UpdateWorldMatrix(m_pos, m_rot, m_scale);

	if (m_skeleton.IsInited()) {
		//�X�P���g������������Ă��B
		m_skeleton.Update(m_model.GetWorldMatrix());
	}
}

void SkinModelRender::ForwardRender()
{
	auto& rc = GraphicsEngineObj()->GetRenderContext();
	auto& camera3D = GraphicsEngineObj()->GetCamera3D();
	//���f����Draw
	m_model.Draw(rc, camera3D.GetViewMatrix(), camera3D.GetProjectionMatrix(), m_renderMode);
}

