#include "stdafx.h"
#include "Animation.h"

void Animation::Init(Skeleton& skeleton, const vector<unique_ptr<AnimationClip>>& animClips)
{
	m_skelton = &skeleton;
	for (auto& animClip : animClips) {
		//�A�j�����[�V�����N���b�v���񂷁B
		m_animationClips.push_back(animClip.get());
	}
	//footstep�{�[���̔ԍ��𒲂ׂ�B
	int footstepBoneNo = -1;
	int numBone = m_skelton->GetNumBones();
	for (int boneNo = 0; boneNo < numBone; boneNo++) {
		auto bone = m_skelton->GetBone(boneNo);
		if (wcscmp(bone->GetName(), L"footstep") == 0) {
			//footstep�{�[�����������B
			footstepBoneNo = boneNo;
			break;
		}
	}

	//�A�j���[�V�����R���g���[���[�̏������B
	for (auto& ctr : m_animationController) {
		ctr.Init(m_skelton, footstepBoneNo);
	}

	//�Ƃ肠�����ŏ��̃A�j���V�������ԂȂ��ōĐ��B
	Play(0);
	m_isInited = true;
}

void Animation::UpdateLocalPose(float deltaTime)
{
	/*					
		----�Ċm�F----
	*/				
	m_isterpolateTime += deltaTime;
	if (m_isterpolateTime >= 1.0f) {
		//��Ԋ����B
		//��Ԃ��I�������̂ŃA�j���[�V�����R���g���[���͈��������Ȃ��Ȃ�B
		//�Ō�̕�Ԃɍ�����R���g���[���ɐݒ肷��B
		m_startAnimationPlayController = GetLastAnimatitonControllerIndex();
		m_numAnimationPlayController = 1;
		m_isterpolateTime = 1.0f;
	}

	for (int i = 0; i < m_numAnimationPlayController - 1; i++) {
		//�A�j���[�V�����R���g���[�����B
		int index = GetAnimationControllerIndex(m_startAnimationPlayController, i);
		m_animationController[index].Update(deltaTime, this);
	}

	//�Ō�̃|�[�Y�����i�߂�B
	int lastIndex = GetLastAnimatitonControllerIndex();
	m_animationController[lastIndex].Update(deltaTime, this);
}

void Animation::UpdateGlobalPose()
{
	//�O���[�o���|�[�Y�v�Z�p�̃��������X�^�b�N����m�ہB
	int numBone = m_skelton->GetNumBones();
	//�O���[�o���|�[�Y���̏W���́B
	Quaternion* qGlobalPose = (Quaternion*)alloca(sizeof(Quaternion) * numBone);
	Vector3* vGlobalPose = (Vector3*)alloca(sizeof(Vector3) * numBone);
	Vector3* vGlobalScale = (Vector3*)alloca(sizeof(Vector3) * numBone);
	//�t�b�g�X�e�b�v�̈ړ��ʂ��������B
	m_footstepDeltaValue = g_vec3Zero;
	//�{�[�����������B
	for (int i = 0; i < numBone; i++) {

		qGlobalPose[i] = Quaternion::Identity;
		vGlobalPose[i] = Vector3::Zero;
		vGlobalScale[i] = Vector3::Zero;
	}
	//�O���[�o���|�[�Y�̌v�Z
	int startIndex = m_startAnimationPlayController;

	for (int i = 0; i < m_numAnimationPlayController; i++) {
		//�R���g���[���B
		int index = GetAnimationControllerIndex(startIndex, i);
		//��ԗ��B
		float intepolateRate = m_animationController[index].GetInterpolateRate();
		//���[�J���{�[���s��B
		const auto& localBoneMatrix = m_animationController[index].GetBoneLocalMatrix();
		//�t�b�g�X�e�b�v�̈ړ��ʁB
		auto deltaValueFootStep = m_animationController[index].GetFootstepDeltaValueOnUpdate();
		//footstep�̕�ԁB
		m_footstepDeltaValue.Lerp(intepolateRate, m_footstepDeltaValue, deltaValueFootStep);
		for (int boneNo = 0; boneNo < numBone; boneNo++) {
			//���s�ړ��̕�ԁB
			Matrix m = localBoneMatrix[boneNo];
			vGlobalPose[boneNo].Lerp(
				intepolateRate,
				vGlobalPose[boneNo],
				*(Vector3*)m.m[3]
			);
			//���s�ړ��������폜�B
			m.m[3][0] = 0.0f;
			m.m[3][1] = 0.0f;
			m.m[3][2] = 0.0f;

			//�g�听���̕�ԁB
			Vector3 vBoneScale;
			vBoneScale.x = (*(Vector3*)m.m[0]).Length();
			vBoneScale.y = (*(Vector3*)m.m[1]).Length();
			vBoneScale.z = (*(Vector3*)m.m[2]).Length();

			vGlobalScale[boneNo].Lerp(
				intepolateRate,
				vGlobalScale[boneNo],
				vBoneScale
			);
			//�g�听���̏����B
			m.m[0][0] /= vBoneScale.x;
			m.m[0][1] /= vBoneScale.x;
			m.m[0][2] /= vBoneScale.x;

			m.m[1][0] /= vBoneScale.y;
			m.m[1][1] /= vBoneScale.y;
			m.m[1][2] /= vBoneScale.y;

			m.m[2][0] /= vBoneScale.z;
			m.m[2][1] /= vBoneScale.z;
			m.m[2][2] /= vBoneScale.z;

			//��]�̕�ԁB
			Quaternion qBone;
			qBone.SetRotation(m);
			qGlobalPose[boneNo].Slerp(intepolateRate, qGlobalPose[boneNo], qBone);
		}
	}
	//�O���[�o���|�[�Y���X�P���g���ɔ��f�����Ă����B
	int numAnimationController = m_numAnimationPlayController;
	for (int i = 1; i < m_numAnimationPlayController; i++) {
		int index = GetAnimationControllerIndex(startIndex, i);
		if (m_animationController[index].GetInterpolateRate() > 0.99999f) {
			//��ԏI���B
			m_startAnimationPlayController = index;
			//�A�j���[�V�����̊J�n�ʒu��O�ɂ���B
			numAnimationController = m_numAnimationPlayController - i;
		}
	}
	m_numAnimationPlayController = numAnimationController;
}

void Animation::Progress(float deltaTime) {
	if (m_numAnimationPlayController == 0) {
		//�A�j���[�V�����Ȃ��B
		return;
	}
	//�X�V���ԁB
	m_deltaTimeOnUpdate = deltaTime;
	//���[�J���|�[�Y�̍X�V�B
	UpdateLocalPose(deltaTime);
	//�O���[�o���|�[�Y���v�Z�B
	UpdateGlobalPose();
}

//Vector3 Animation::CalcFootstepDeltaValueInWorldSpace(Quaternion rot, Vector3 scale) const
//{
//	auto footstepDeltaValueInWorldSpace = m_footstepDeltaValue;
//
//	Matrix mBias = Matrix::Identity;
//	//���ɓ|��Ă�̂𒼂��H
//	mBias.MakeRotationX(Math::PI * -0.5f);
//	mBias.Apply(footstepDeltaValueInWorldSpace);
//
//	//�t�b�g�X�e�b�v�̈ړ��ʂ��g�傷��B
//	footstepDeltaValueInWorldSpace.x *= scale.x;
//	footstepDeltaValueInWorldSpace.y *= scale.y;
//	footstepDeltaValueInWorldSpace.z *= scale.z;
//	//�t�b�g�X�e�b�v�̈ړ��ʂ��񂷁B
//	rot.Apply(footstepDeltaValueInWorldSpace);
//
//}