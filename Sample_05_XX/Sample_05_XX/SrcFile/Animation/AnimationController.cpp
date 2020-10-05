#include "stdafx.h"
#include "AnimationController.h"
#include "Skeleton.h"

void AnimationController::Init(Skeleton* skelton, int footStepBoneNo)
{
	m_footstepBoneNo = footStepBoneNo;
	int numBones = skelton->GetNumBones();
	//ボーン行列を確保。
	m_boneMatrix.resize(numBones);
	m_skelton = skelton;
}

void AnimationController::StartLoop()
{
	//初期化。
	m_footstepPos = g_vec3Zero;
	m_currentKeyFrameNo = 0;
	m_time = 0.0f;
}

void AnimationController::ClacBoneMatrixInRootBoneSpace(Bone& bone, Matrix parentMatrix)
{
	//ワールド行列を計算。
	//ルートボーン空間での行列(初期化)
	auto& mBoneInRootSpace = m_boneMatrix[bone.GetNo()];
	//ローカル行列。
	Matrix localMatrix = m_boneMatrix[bone.GetNo()];
	//親の行列とローカル行列を乗算して、ワールド行列を計算。
	//**check WorldMat_boneA = WorldMat_model * LoacalMat_rootBone * LocalMat_boneA
	mBoneInRootSpace = localMatrix * parentMatrix;
	//子供のボーン行列も計算。
	for (auto& childBone : bone.GetChildren()) {
		//子供のボーン回
		ClacBoneMatrixInRootBoneSpace(*childBone, mBoneInRootSpace);
	}
}

void AnimationController::ClacBoneMatrixInRootBoneSpace()
{
	int numBone = m_skelton->GetNumBones();
	for (int boneNo = 0; boneNo < numBone; boneNo++) {
		//ルートの骨を検索する。
		auto bone = m_skelton->GetBone(boneNo);
		if (bone->GetParentBoneNo() != -1) {
			//見つかった
			//ルートボーン自身は変換する必要性がないのでスキップ。
			continue;
		}
		//アニメーション再生でモデルの位置は動かしたくないので、行列はIdentiye。
		ClacBoneMatrixInRootBoneSpace(*bone, g_matIdentity);
	}
}

void AnimationController::SamplingBoneMatrixFromAnimationClip()
{
	//骨の配列とキーフレームの二次元配列。
	const auto& keyFramePtrListArray = m_animationClip->GetKeyFramePtrListArray();
	for (const auto& keyFrameList : keyFramePtrListArray) {
		if (keyFrameList.size() == 0) {
			//キーフレームが積まれてない。
			continue;
		}
		//現在再生中のキーフレームを取得。
		KeyFrame* keyframe = keyFrameList.at(m_currentKeyFrameNo);
		//今再生しているキーフレームの位置情報の取得。
		m_boneMatrix[keyframe->boneIndex] = keyframe->transform;
	}
}

void AnimationController::SamplingDeltaValueFootstepBone()
{
	if (m_currentKeyFrameNoLastFrame == m_currentKeyFrameNo) {
		//キーフレームが進んでない。
		return;
	}
	if (m_footstepBoneNo == -1) {
		//フットステップがない。
		return;
	}
	//ボーンの数。
	int numBone = m_skelton->GetNumBones();

	for (int boneNo = 0; boneNo < numBone; boneNo++) {
		auto bone = m_skelton->GetBone(boneNo);
		if (m_footstepBoneNo == bone->GetNo()) {
			auto mat = m_boneMatrix[bone->GetNo()];
			Vector3 footstepBonePos;
			footstepBonePos.x = mat.m[3][0];
			footstepBonePos.y = mat.m[3][1];
			footstepBonePos.z = mat.m[3][2];
			//このフレームでのフットステップの移動量を計算。
			m_footstepDeltaValue = footstepBonePos - m_footstepPos;
			//このフレームでのfootstepの座標を更新する。
			m_footstepPos = footstepBonePos;
			break;
		}
	}
}

void AnimationController::SubtractFootstepbonePosFromAllBone()
{
	if (m_footstepBoneNo == -1) {
		//フットステップなし
		return;
	}
	int numBone = m_skelton->GetNumBones();
	//減算。
	for (int boneNo = 0; boneNo < numBone; boneNo++) {
		auto bone = m_skelton->GetBone(boneNo);
		m_boneMatrix[bone->GetNo()].m[3][0] -= m_footstepPos.x;
		m_boneMatrix[bone->GetNo()].m[3][1] -= m_footstepPos.y;
		m_boneMatrix[bone->GetNo()].m[3][2] -= m_footstepPos.z;
	}
}

void AnimationController::ProgressKeyFrameNo(float deltaTime)
{
	//1フレーム前のキーフレーム番号を記録しておく。
	m_currentKeyFrameNoLastFrame = m_currentKeyFrameNo;
	//キーフレームの先頭。
	const auto& topBoneKeyFrameList = m_animationClip->GetTopBoneKeyFrameList();

	//補間を進める
	//1.0になった時点で完全に次のアニメーションに切り替わる。
	m_interpolateTime = min(1.0f, m_interpolateTime + deltaTime);
	while (true) {
		if (m_currentKeyFrameNo >= (int)topBoneKeyFrameList.size()) {
			//キーフレームが終端まで行った
			if (m_animationClip->IsLoop()) {
				//ループするのでアニメーション情報を初期化。
				StartLoop();
			}
			else {
				//ワンショット再生。
				m_currentKeyFrameNo--;
				m_isPlaying = false;	//再生終了。
			}
			//すべてのキーフレームを流した。
			break;
		}
		if (topBoneKeyFrameList.at(m_currentKeyFrameNo)->time >= m_time) {
			//アニメーションを更新する時間まで到達したので抜ける。
			break;
		}
		//次へ。
		m_currentKeyFrameNo++;
	}
}

void AnimationController::Update(float deltaTime, Animation* animation)
{
	if (m_animationClip == nullptr) {
		MessageBox(
			nullptr,
			L"AnimationController::Update() アニメーションがありません",
			L"エラー",
			S_OK
		);
		return;
	}

	m_time += deltaTime;
	//キーフレーム番号を進めていく。
	ProgressKeyFrameNo(deltaTime);
	//ボーン行列をアニメーションクリップからサンプリングしていく。
	SamplingBoneMatrixFromAnimationClip();
	//親の骨座標系になっているボーン行列をルートのボーン空間に変換していく。
	//ClacBoneMatrixInRootBoneSpace();
	//footstepボーンの移動量を取得。
	SamplingDeltaValueFootstepBone();
	//footstepボーンの移動量を全体の骨から減算する。
	SubtractFootstepbonePosFromAllBone();
	//アニメーションを流した印をつける。
	m_skelton->SetMarkPlayAnimation();
}