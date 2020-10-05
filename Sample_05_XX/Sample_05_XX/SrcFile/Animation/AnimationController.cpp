#include "stdafx.h"
#include "AnimationController.h"
#include "Skeleton.h"

void AnimationController::Init(Skeleton* skelton, int footStepBoneNo)
{
	m_footstepBoneNo = footStepBoneNo;
	int numBones = skelton->GetNumBones();
	//�{�[���s����m�ہB
	m_boneMatrix.resize(numBones);
	m_skelton = skelton;
}

void AnimationController::StartLoop()
{
	//�������B
	m_footstepPos = g_vec3Zero;
	m_currentKeyFrameNo = 0;
	m_time = 0.0f;
}

void AnimationController::ClacBoneMatrixInRootBoneSpace(Bone& bone, Matrix parentMatrix)
{
	//���[���h�s����v�Z�B
	//���[�g�{�[����Ԃł̍s��(������)
	auto& mBoneInRootSpace = m_boneMatrix[bone.GetNo()];
	//���[�J���s��B
	Matrix localMatrix = m_boneMatrix[bone.GetNo()];
	//�e�̍s��ƃ��[�J���s�����Z���āA���[���h�s����v�Z�B
	//**check WorldMat_boneA = WorldMat_model * LoacalMat_rootBone * LocalMat_boneA
	mBoneInRootSpace = localMatrix * parentMatrix;
	//�q���̃{�[���s����v�Z�B
	for (auto& childBone : bone.GetChildren()) {
		//�q���̃{�[����
		ClacBoneMatrixInRootBoneSpace(*childBone, mBoneInRootSpace);
	}
}

void AnimationController::ClacBoneMatrixInRootBoneSpace()
{
	int numBone = m_skelton->GetNumBones();
	for (int boneNo = 0; boneNo < numBone; boneNo++) {
		//���[�g�̍�����������B
		auto bone = m_skelton->GetBone(boneNo);
		if (bone->GetParentBoneNo() != -1) {
			//��������
			//���[�g�{�[�����g�͕ϊ�����K�v�����Ȃ��̂ŃX�L�b�v�B
			continue;
		}
		//�A�j���[�V�����Đ��Ń��f���̈ʒu�͓����������Ȃ��̂ŁA�s���Identiye�B
		ClacBoneMatrixInRootBoneSpace(*bone, g_matIdentity);
	}
}

void AnimationController::SamplingBoneMatrixFromAnimationClip()
{
	//���̔z��ƃL�[�t���[���̓񎟌��z��B
	const auto& keyFramePtrListArray = m_animationClip->GetKeyFramePtrListArray();
	for (const auto& keyFrameList : keyFramePtrListArray) {
		if (keyFrameList.size() == 0) {
			//�L�[�t���[�����ς܂�ĂȂ��B
			continue;
		}
		//���ݍĐ����̃L�[�t���[�����擾�B
		KeyFrame* keyframe = keyFrameList.at(m_currentKeyFrameNo);
		//���Đ����Ă���L�[�t���[���̈ʒu���̎擾�B
		m_boneMatrix[keyframe->boneIndex] = keyframe->transform;
	}
}

void AnimationController::SamplingDeltaValueFootstepBone()
{
	if (m_currentKeyFrameNoLastFrame == m_currentKeyFrameNo) {
		//�L�[�t���[�����i��łȂ��B
		return;
	}
	if (m_footstepBoneNo == -1) {
		//�t�b�g�X�e�b�v���Ȃ��B
		return;
	}
	//�{�[���̐��B
	int numBone = m_skelton->GetNumBones();

	for (int boneNo = 0; boneNo < numBone; boneNo++) {
		auto bone = m_skelton->GetBone(boneNo);
		if (m_footstepBoneNo == bone->GetNo()) {
			auto mat = m_boneMatrix[bone->GetNo()];
			Vector3 footstepBonePos;
			footstepBonePos.x = mat.m[3][0];
			footstepBonePos.y = mat.m[3][1];
			footstepBonePos.z = mat.m[3][2];
			//���̃t���[���ł̃t�b�g�X�e�b�v�̈ړ��ʂ��v�Z�B
			m_footstepDeltaValue = footstepBonePos - m_footstepPos;
			//���̃t���[���ł�footstep�̍��W���X�V����B
			m_footstepPos = footstepBonePos;
			break;
		}
	}
}

void AnimationController::SubtractFootstepbonePosFromAllBone()
{
	if (m_footstepBoneNo == -1) {
		//�t�b�g�X�e�b�v�Ȃ�
		return;
	}
	int numBone = m_skelton->GetNumBones();
	//���Z�B
	for (int boneNo = 0; boneNo < numBone; boneNo++) {
		auto bone = m_skelton->GetBone(boneNo);
		m_boneMatrix[bone->GetNo()].m[3][0] -= m_footstepPos.x;
		m_boneMatrix[bone->GetNo()].m[3][1] -= m_footstepPos.y;
		m_boneMatrix[bone->GetNo()].m[3][2] -= m_footstepPos.z;
	}
}

void AnimationController::ProgressKeyFrameNo(float deltaTime)
{
	//1�t���[���O�̃L�[�t���[���ԍ����L�^���Ă����B
	m_currentKeyFrameNoLastFrame = m_currentKeyFrameNo;
	//�L�[�t���[���̐擪�B
	const auto& topBoneKeyFrameList = m_animationClip->GetTopBoneKeyFrameList();

	//��Ԃ�i�߂�
	//1.0�ɂȂ������_�Ŋ��S�Ɏ��̃A�j���[�V�����ɐ؂�ւ��B
	m_interpolateTime = min(1.0f, m_interpolateTime + deltaTime);
	while (true) {
		if (m_currentKeyFrameNo >= (int)topBoneKeyFrameList.size()) {
			//�L�[�t���[�����I�[�܂ōs����
			if (m_animationClip->IsLoop()) {
				//���[�v����̂ŃA�j���[�V���������������B
				StartLoop();
			}
			else {
				//�����V���b�g�Đ��B
				m_currentKeyFrameNo--;
				m_isPlaying = false;	//�Đ��I���B
			}
			//���ׂẴL�[�t���[���𗬂����B
			break;
		}
		if (topBoneKeyFrameList.at(m_currentKeyFrameNo)->time >= m_time) {
			//�A�j���[�V�������X�V���鎞�Ԃ܂œ��B�����̂Ŕ�����B
			break;
		}
		//���ցB
		m_currentKeyFrameNo++;
	}
}

void AnimationController::Update(float deltaTime, Animation* animation)
{
	if (m_animationClip == nullptr) {
		MessageBox(
			nullptr,
			L"AnimationController::Update() �A�j���[�V����������܂���",
			L"�G���[",
			S_OK
		);
		return;
	}

	m_time += deltaTime;
	//�L�[�t���[���ԍ���i�߂Ă����B
	ProgressKeyFrameNo(deltaTime);
	//�{�[���s����A�j���[�V�����N���b�v����T���v�����O���Ă����B
	SamplingBoneMatrixFromAnimationClip();
	//�e�̍����W�n�ɂȂ��Ă���{�[���s������[�g�̃{�[����Ԃɕϊ����Ă����B
	//ClacBoneMatrixInRootBoneSpace();
	//footstep�{�[���̈ړ��ʂ��擾�B
	SamplingDeltaValueFootstepBone();
	//footstep�{�[���̈ړ��ʂ�S�̂̍����猸�Z����B
	SubtractFootstepbonePosFromAllBone();
	//�A�j���[�V�����𗬂����������B
	m_skelton->SetMarkPlayAnimation();
}