#include "stdafx.h"
#include "AnimationClip.h"

AnimationClip::~AnimationClip()
{
}

void AnimationClip::Load(const char* filePath)
{
	//tka�t�@�C�����[�h�B
	m_tkaFile.Load(filePath);
	m_clipName = filePath;
	if (m_tkaFile.IsLoaded() == false) {
		//�ǂݍ��ݎ��s�B
		MessageBox(
			nullptr,
			L"AnimationClip::Load() �A�j���[�V�����N���b�v�̃��[�h�Ɏ��s�B",
			L"�G���[",
			MB_OK
		);
		return;
	}
}

void AnimationClip::BuildKeyFrames()
{
	//���X�g�̃L���p�V�e�B�[�����킹��B
	m_keyFrames.reserve(m_tkaFile.GetNumKeyFrame());
	//���ׂĂ�tka�t�@�C�����N�G���B
	m_tkaFile.QueryKeyFrames([&](const TkaFile::KeyFrame& tkaKeyFrame) {
		//�쐬�B
		auto keyFrame = make_unique<KeyFrame>();
		//�L�[�t���[���̏����R�s�[�B
		keyFrame->boneIndex = tkaKeyFrame.boneIndex;
		keyFrame->transform = Matrix::Identity;
		keyFrame->time = tkaKeyFrame.time;
		for (auto j = 0; j < 4; j++) {
			keyFrame->transform.m[j][0] = tkaKeyFrame.transform[j].x;
			keyFrame->transform.m[j][1] = tkaKeyFrame.transform[j].y;
			keyFrame->transform.m[j][2] = tkaKeyFrame.transform[j].z;
		}
		//�L���X�g���ă��X�g�ɐςށB
		m_keyFrames.push_back(move(keyFrame));
		});
	//�{�[���C���f�b�N�X���Ƃ̃L�[�t���[���̘A�����X�g���쐬�B
	m_keyFramePtrListArray.resize(MAX_BONE);
	for (auto& keyFrame : m_keyFrames) {
		m_keyFramePtrListArray[keyFrame->boneIndex].push_back(keyFrame.get());
		if (m_topBoneKeyFrameList == nullptr) {
			m_topBoneKeyFrameList = &m_keyFramePtrListArray[keyFrame->boneIndex];
		}
	}

}