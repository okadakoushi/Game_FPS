#pragma once

#include <utility>
#include "AnimationClip.h"
#include "AnimationController.h"

/// <summary>
/// �A�j���[�V�����N���X�B
/// </summary>
/// <remarks>
/// �V���v���ȃA�j���[�V�����Đ��B
/// �A�j���[�V������ԁB
/// </remarks>


class Animation
{
public:
	/// <summary>
	/// �������ς݂��̔���B
	/// </summary>
	/// <returns>�����A���s�B</returns>
	bool IsInited() const
	{
	}
	/// <summary>
	/// �������B
	/// </summary>
	/// <param name="skelton">�X�P���g���B</param>
	/// <param name="animClip">�A�j���[�V�����N���b�v�B</param>
	void Init(Skeleton& skelton, const vector<unique_ptr<AnimationClip>>& animClip);
	
	void Play(int clipNo, float interpolateTime = 0.0f)
	{
		if (clipNo < m_animationClips.size()) {
			PlayCommon(m_animationClips[clipNo], interpolateTime);
		}
	}
	/// <summary>
	/// �A�j���[�V�����N���b�v�̃��[�v�t���O��ݒ�B
	/// </summary>
	/// <param name="clipName">�A�j���[�V�����N���b�v���B</param>
	/// <param name="flag">�t���O�B</param>
	void SetAnimationClipLoopFlag(const wchar_t* clipName, bool flag)
	{
		//�N���b�v�l�[���ŃC�e���[�^�[�B
		auto it = find_if(
			m_animationClips.begin(),
			m_animationClips.end(),
			[clipName](auto& clip) {return clip->GetName() == clipName; }
		);
		if (it == m_animationClips.end()) {
			//������Ȃ������B
			return;
		}
		(*it)->SetLoopFlag(flag);
	}

	bool IsPlaying() const
	{
		int lastIndex = GetLastAnimatitonControllerIndex();
		return m_animationController[lastIndex].IsPlaying();
	}

	/// <summary>
	/// �A�j���[�V������i�߂�B
	/// <para>�C���^�[�t�F�C�X�������B</para>
	/// </summary>
	/// <param name="deltaTime"></param>
	void Progress(float deltaTime);

	//Vector3 CalcFootstepDeltaValueInWorldSpace(Quaternion rot, Vector3 scale) const;

private:
	//�����������ł݂̂����g��Ȃ��̂ŉB���B

	void PlayCommon(AnimationClip* nextClip, float interpolateTime)
	{
		if (nextClip->IsLoaded() == false) {
			//�A�j���[�V���������[�h����ĂȂ��B
			MessageBox(
				nullptr,
				L"PlayCommon(value) �A�j���[�V�����N���b�v�����[�h����Ă��܂���B",
				L"�G���[",
				S_OK
			);
			return;
		}
		int index = GetLastAnimatitonControllerIndex();
		if (m_animationController[index].GetAnimClip() == nextClip) {
			//����ȏ�A�j���[�V�����R���g���[���[���Ȃ��B
			return;
		}
		if (interpolateTime == 0.0f) {
			//��ԂȂ��̂ŕ����̃R���g���[���[���K�v�Ȃ��B
			m_numAnimationPlayController = 1;
		}
		else {
			//��Ԃ���̂ŃR���g���[���[�𑝂₷�B
			m_numAnimationPlayController++;
		}
		index = GetLastAnimatitonControllerIndex();
		m_animationController[index].ChangeAnimationClip(nextClip);
		m_animationController[index].SetInterpolateTime(interpolateTime);
		interpolateTime = 0.0f;
		m_isterpolateTimeEnd = interpolateTime;
	}
	/// <summary>
	/// ���[�J���|�[�Y�̍X�V�B
	/// </summary>
	/// <param name="deltaTime">�A�j���[�V������i�߂鎞�ԁB</param>
	void UpdateLocalPose(float deltaTime);
	/// <summary>
	/// �O���[�o���|�[�Y�̍X�V�B
	/// </summary>
	void UpdateGlobalPose();
	/// <summary>
	/// �ŏI�|�[�Y�ɂȂ�A�j���[�V�����̃����O�o�b�t�@��ł̃C���f�b�N�X���擾�B
	/// </summary>
	/// <returns>�ŏI�|�[�Y�A�j���[�V�����̃C���f�b�N�X�B</returns>
	int GetLastAnimatitonControllerIndex() const
	{
		return GetAnimationControllerIndex(m_startAnimationPlayController, m_numAnimationPlayController - 1);
	}
	/// <summary>
	/// �A�j���[�V�����R���g���[���̃����O�o�b�t�@��ł̃C���f�b�N�X���擾�B
	/// </summary>
	/// <param name="startIndex">�J�n�C���f�b�N�X�B</param>
	/// <param name="localIndex">���[�J���C���f�b�N�X�B</param>
	/// <returns></returns>
	int GetAnimationControllerIndex(int startIndex, int localIndex) const
	{
		return (startIndex + localIndex) % ANIMATION_PLAY_CONTROLLER_NUM;
	}
private:
	static const int ANIMATION_PLAY_CONTROLLER_NUM = 32;	//�A�j���[�V�����R���g���[���̐��B
	vector<AnimationClip*> m_animationClips;				//�A�j���[�V�����N���b�v�̃��X�g�B
	Skeleton* m_skelton = nullptr;							//�A�j���[�V������K�p����{�[���B
	AnimationController m_animationController[ANIMATION_PLAY_CONTROLLER_NUM];	//�A�j���[�V�����R���g���[���[�B
	int m_numAnimationPlayController = 0;					//�g�p���̃A�j���[�V�����R���g���[���[�B
	int m_startAnimationPlayController = 0;					//�A�j���[�V�����R���g���[���J�n�C���f�b�N�X�B
	float m_isterpolateTime = 0.0f;							//��Ԏ��ԁH						
	float m_isterpolateTimeEnd = 0.0f;						//��ԏI�����ԁH
	bool m_isInterpolate = false;							//��Ԓ����̃t���O�B
	Vector3 m_footstepDeltaValue = g_vec3Zero;				//footstep�{�[���̈ړ��ʁB
	bool m_isInited = false;								//�������t���O�B
	float m_deltaTimeOnUpdate = 0.0f;						//Update����Ƃ���DeltaTime�B

};


