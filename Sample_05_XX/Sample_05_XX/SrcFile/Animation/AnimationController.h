#pragma once

#include "AnimationClip.h"
class Skeleton;
class Animation;

class AnimationController
{
public:
	AnimationController()
	{
	}
	~AnimationController()
	{
	}
	/// <summary>
	/// �������B
	/// </summary>
	/// <param name="skelton">�X�P���g���B</param>
	/// <param name="footStepBoneNo">footstep�{�[���ԍ��B</param>
	void Init(Skeleton* skelton, int footStepBoneNo);
	/// <summary>
	/// �A�j���[�V�����N���b�v�̕ύX�B
	/// </summary>
	/// <param name="clip">�A�j���[�V�����N���b�v�B</param>
	void ChangeAnimationClip(AnimationClip* clip)
	{
		//�ύX������̂ŏ������B
		m_animationClip = clip;
		m_currentKeyFrameNo = 0;
		m_time = 0.0f;
		m_isPlaying = true;
		m_footstepPos = g_vec3Zero;
		m_footstepDeltaValue = g_vec3Zero;
	}
	/// <summary>
	/// ��Ԏ��Ԃ�ݒ�B
	/// </summary>
	/// <param name="interpolateTime">��Ԏ��ԁB</param>
	void SetInterpolateTime(float interpolateTime)
	{
		if (interpolateTime < 0.0f) {
			MessageBox(
				nullptr,
				L"SetInterpolateTime(value) ��Ԏ��Ԃ��s���ł��B",
				L"�G���[",
				S_OK
			);
			m_interpolateEndTime = interpolateTime;
			m_interpolateTime = 0.0f;
		}
	}
	/// <summary>
	/// ��ԗ����擾�B
	/// </summary>
	/// <returns></returns>
	float GetInterpolateRate() const
	{
		if (m_interpolateEndTime <= 0.0f) {
			//�l���s���A��Ԃ��Ȃ��B
			return 1.0f;
		}
		return min(1.0f, m_interpolateTime / m_interpolateEndTime);
	}
	/// <summary>
	/// �A�j���[�V������i�߂�B
	/// </summary>
	/// <param name="deltaTime">�i�߂鎞�ԁB�B</param>
	/// <param name="animtion">�A�j���[�V�����B</param>
	void Update(float deltaTime, Animation* animtion);
	/// <summary>
	/// ���[�J���{�[���s����擾�B
	/// </summary>
	/// <returns></returns>
	const std::vector<Matrix>& GetBoneLocalMatrix() const
	{
		return m_boneMatrix;
	}
	/// <summary>
	/// �A�j���[�V�����N���b�v���擾�B
	/// </summary>
	/// <returns>�A�j���[�V�����N���b�v�B</returns>
	AnimationClip* GetAnimClip() const
	{
		return m_animationClip;
	}
	/// <summary>
	/// �Đ����H
	/// </summary>
	/// <returns></returns>
	bool IsPlaying() const
	{
		return m_isPlaying;
	}
	/// <summary>
	/// Update�֐����Ăяo�������́AfootStep�{�[���̈ړ��ʂ��擾�B
	/// </summary>
	/// <returns></returns>
	Vector3 GetFootstepDeltaValueOnUpdate() const
	{
		return m_footstepDeltaValue;
	}
private:
	/// <summary>
	/// ���[�v�Đ����J�n����Ƃ��̏����B
	/// </summary>
	void StartLoop();
	/// <summary>
	/// ���[�g�{�[����Ԃł̃{�[���s����v�Z�B
	/// </summary>
	/// <param name="bone">�v�Z���鍜</param>
	/// <param name="parentMarix">�e�̍s��B</param>
	void ClacBoneMatrixInRootBoneSpace(Bone& bone, Matrix parentMatrix);
	/// <summary>
	/// �A�j���[�V�����N���b�v����{�[���s����T���v�����O�B
	/// </summary>
	void SamplingBoneMatrixFromAnimationClip();
	/// <summary>
	/// ���[�g�{�[����Ԃł̍s����v�Z�B
	/// </summary>
	void ClacBoneMatrixInRootBoneSpace();
	/// <summary>
	/// footstep�{�[���̕ω��ʂ��T���v�����O�B
	/// </summary>
	void SamplingDeltaValueFootstepBone();
	/// <summary>
	/// footstep�{�[���̍��W��S�̂̍����猸�Z����B
	/// </summary>
	void SubtractFootstepbonePosFromAllBone();
	/// <summary>
	/// �L�[�t���[���ԍ���i�߂�B
	/// </summary>
	/// <param name="deltaTime">�i�߂鎞�ԁB</param>
	void ProgressKeyFrameNo(float deltaTime);

private:
	AnimationClip*		m_animationClip = nullptr;			//�A�j���[�V�����N���b�v�B
	int					m_currentKeyFrameNoLastFrame = 0;	//1�t���[���O�̃L�[�t���[���B
	int					m_currentKeyFrameNo = 0;			//���݂̃t���[���ԍ��B
	float				m_time = 0.0f;						//���ԁB
	std::vector<Matrix>	m_boneMatrix;						//�{�[���s��B
	float				m_interpolateTime;					//��Ԏ��ԁB
	float				m_interpolateEndTime;				//��ԏI�����ԁB
	bool				m_isPlaying = false;				//�Đ������ǂ����B
	Skeleton*			m_skelton = nullptr;				//�X�P���g���B
	Vector3				m_footstepDeltaValue = g_vec3Zero;	//�t�b�g�X�e�b�v�̈ړ��x�N�g���B
	Vector3				m_footstepPos = g_vec3Zero;			//�t�b�g�X�e�b�v�{�[���̍��W�B
	int					m_footstepBoneNo = -1;				//�t�b�g�X�e�b�v�̃{�[���̔ԍ��B
};

