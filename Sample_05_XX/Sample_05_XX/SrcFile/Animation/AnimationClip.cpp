#include "stdafx.h"
#include "AnimationClip.h"

AnimationClip::~AnimationClip()
{
}

void AnimationClip::Load(const char* filePath)
{
	//tkaファイルロード。
	m_tkaFile.Load(filePath);
	m_clipName = filePath;
	if (m_tkaFile.IsLoaded() == false) {
		//読み込み失敗。
		MessageBox(
			nullptr,
			L"AnimationClip::Load() アニメーションクリップのロードに失敗。",
			L"エラー",
			MB_OK
		);
		return;
	}
}

void AnimationClip::BuildKeyFrames()
{
	//リストのキャパシティーを合わせる。
	m_keyFrames.reserve(m_tkaFile.GetNumKeyFrame());
	//すべてのtkaファイルをクエリ。
	m_tkaFile.QueryKeyFrames([&](const TkaFile::KeyFrame& tkaKeyFrame) {
		//作成。
		auto keyFrame = make_unique<KeyFrame>();
		//キーフレームの情報をコピー。
		keyFrame->boneIndex = tkaKeyFrame.boneIndex;
		keyFrame->transform = Matrix::Identity;
		keyFrame->time = tkaKeyFrame.time;
		for (auto j = 0; j < 4; j++) {
			keyFrame->transform.m[j][0] = tkaKeyFrame.transform[j].x;
			keyFrame->transform.m[j][1] = tkaKeyFrame.transform[j].y;
			keyFrame->transform.m[j][2] = tkaKeyFrame.transform[j].z;
		}
		//キャストしてリストに積む。
		m_keyFrames.push_back(move(keyFrame));
		});
	//ボーンインデックスごとのキーフレームの連結リストを作成。
	m_keyFramePtrListArray.resize(MAX_BONE);
	for (auto& keyFrame : m_keyFrames) {
		m_keyFramePtrListArray[keyFrame->boneIndex].push_back(keyFrame.get());
		if (m_topBoneKeyFrameList == nullptr) {
			m_topBoneKeyFrameList = &m_keyFramePtrListArray[keyFrame->boneIndex];
		}
	}

}