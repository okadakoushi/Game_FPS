#include "stdafx.h"
#include "Model.h"
#include "srcFile/Animation/AnimationClip.h"
#include <fstream>


int Model::Init(const ModelInitData& initData)
{
	//�����̃V�F�[�_�[�����[�h���鏈�������߂Ă���̂�
	//wchar_t�^�̕�����Ȃ̂ŁA�����ŕϊ����Ă����B
	wchar_t wfxFilePath[256];
	if (initData.m_fxFilePath == nullptr) {
		MessageBoxA(nullptr, "fx�t�@�C���p�X���w�肳��Ă��܂���B", "�G���[", MB_OK);
		std::abort();
	}

	//�A�j���[�V���������[�h���ĐςށB
	m_tkaFile.Load(initData.m_tkaFilePath);
	m_tkaFilePaths.push_back(initData.m_tkaFilePath);

	mbstowcs(wfxFilePath, initData.m_fxFilePath, 256);

	m_tkmFile.Load(initData.m_tkmFilePath);
	m_meshParts.InitFromTkmFile(
		m_tkmFile, 
		wfxFilePath, 
		initData.m_vsEntryPointFunc,
		initData.m_psEntryPointFunc,
		initData.m_expandConstantBuffer,
		initData.m_expandConstantBufferSize,
		initData.m_expandShaderResoruceView
	);

	UpdateWorldMatrix(g_vec3Zero, g_quatIdentity, g_vec3One);
	return enInitStep_LoadModel;
}

int Model::LoadTks(const ModelInitData& initData)
{
	//���f���̏������A���b�V���쐬���I����Ă���B
	//�X�P���g���f�[�^�̓ǂݍ��݁B
	std::string skeltonFilePath = initData.m_tkmFilePath;
	int pos = (int)skeltonFilePath.find(".tkm");
	//.tkm��.tks�ɒu�������B
	skeltonFilePath.replace(pos, 4, ".tks");
	//�t�@�C���X�g���[���B
	std::ifstream ifs(skeltonFilePath);
	if (ifs.is_open()) {
		//tks�t�@�C��������̂œǂݍ��ށB
		m_skeleton.Init(skeltonFilePath.c_str());
		return enInitStep_LoadSkelton;
	}
	else {
		//�Ȃ������B
		return enInitStep_Completed;
	}
}

int Model::initSkeleton()
{
	//�o�C���h�|�[�Y�Ƃ��̌v�Z�B
	m_skeleton.BuildBoneMatrices();
	//�X�P���g���ƃ��f���̊֘A�t���B
	BindSkeleton(m_skeleton);
	//�A�j���[�V�����N���b�v�B
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
		//�A�j���[�V�����N���b�v�̃��[�h�I���B
		return enInitStep_LoadAnimationClips;
	}
	else {
		//�Sinit�X�e�b�v�I���B
		return enInitStep_Completed;
	}
}

int Model::InitAnim()
{
	bool isLoaded = true;
	for (auto& animClip : m_animationClips) {
		if (animClip->IsLoaded() == false) {
			isLoaded = false;
			break;
		}
	}
	if (isLoaded == true) {
		//�S�A�j���[�V�����N���b�v�̃��[�h�����B
		//���[�h�����������̂ŁA�L�[�t���[���̍\�z�B
		for (auto& animClip : m_animationClips) {
			animClip->SetLoopFlag(true);
			animClip->BuildKeyFrames();
		}
		//�A�j���[�V�����������B
		m_animation.Init(m_skeleton, m_animationClips);

		//�Sinit�����I���B
		return enInitStep_Completed;
	}
}

void Model::UpdateMatAndAnim()
{
	//�A�j���[�V�����X�V�B
	m_animation.Progress(DELTA_TIME);
	//�߂�ǂ�����܂�Set����ĂȂ��悧�I
	UpdateWorldMatrix(m_pos, m_rot, m_scale);
	//�X�P���g�����X�V�B
	m_skeleton.Update(m_world);
}

void Model::UpdateWorldMatrix(Vector3 pos, Quaternion rot, Vector3 scale)
{
	Matrix mBias;
//todo	if (enUpdateAxis == enFbxUpAxisZ) {
		//Z-up
		mBias.MakeRotationX(Math::PI * -0.5f);
//	}
	Matrix mTrans, mRot, mScale;
	mTrans.MakeTranslation(pos);
	mRot.MakeRotationFromQuaternion(rot);
	mScale.MakeScaling(scale);
	m_world = /*mBias * */mScale * mRot * mTrans;
}
void Model::Draw(RenderContext& rc, Matrix viewMat, Matrix projMat, int RenderMode)
{
	m_meshParts.Draw(
		rc, 
		m_world, 
		viewMat,
		projMat,
		RenderMode
	);
}
