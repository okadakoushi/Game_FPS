#include "stdafx.h"
#include "Animation.h"

void Animation::Init(Skeleton& skeleton, const vector<unique_ptr<AnimationClip>>& animClips)
{
	m_skelton = &skeleton;
	for (auto& animClip : animClips) {
		//アニメメーションクリップ分回す。
		m_animationClips.push_back(animClip.get());
	}
	//footstepボーンの番号を調べる。
	int footstepBoneNo = -1;
	int numBone = m_skelton->GetNumBones();
	for (int boneNo = 0; boneNo < numBone; boneNo++) {
		auto bone = m_skelton->GetBone(boneNo);
		if (wcscmp(bone->GetName(), L"footstep") == 0) {
			//footstepボーンがあった。
			footstepBoneNo = boneNo;
			break;
		}
	}

	//アニメーションコントローラーの初期化。
	for (auto& ctr : m_animationController) {
		ctr.Init(m_skelton, footstepBoneNo);
	}

	//とりあえず最初のアニメションを補間なしで再生。
	Play(0);
	m_isInited = true;
}

void Animation::UpdateLocalPose(float deltaTime)
{
	/*					
		----再確認----
	*/				
	m_isterpolateTime += deltaTime;
	if (m_isterpolateTime >= 1.0f) {
		//補間完了。
		//補間が終了したのでアニメーションコントローラは一つしかいらなくなる。
		//最後の補間に作ったコントローラに設定する。
		m_startAnimationPlayController = GetLastAnimatitonControllerIndex();
		m_numAnimationPlayController = 1;
		m_isterpolateTime = 1.0f;
	}

	for (int i = 0; i < m_numAnimationPlayController - 1; i++) {
		//アニメーションコントローラ分。
		int index = GetAnimationControllerIndex(m_startAnimationPlayController, i);
		m_animationController[index].Update(deltaTime, this);
	}

	//最後のポーズだけ進める。
	int lastIndex = GetLastAnimatitonControllerIndex();
	m_animationController[lastIndex].Update(deltaTime, this);
}

void Animation::UpdateGlobalPose()
{
	//グローバルポーズ計算用のメモリをスタックから確保。
	int numBone = m_skelton->GetNumBones();
	//グローバルポーズ情報の集合体。
	Quaternion* qGlobalPose = (Quaternion*)alloca(sizeof(Quaternion) * numBone);
	Vector3* vGlobalPose = (Vector3*)alloca(sizeof(Vector3) * numBone);
	Vector3* vGlobalScale = (Vector3*)alloca(sizeof(Vector3) * numBone);
	//フットステップの移動量を初期化。
	m_footstepDeltaValue = g_vec3Zero;
	//ボーンを初期化。
	for (int i = 0; i < numBone; i++) {

		qGlobalPose[i] = Quaternion::Identity;
		vGlobalPose[i] = Vector3::Zero;
		vGlobalScale[i] = Vector3::Zero;
	}
	//グローバルポーズの計算
	int startIndex = m_startAnimationPlayController;

	for (int i = 0; i < m_numAnimationPlayController; i++) {
		//コントローラ。
		int index = GetAnimationControllerIndex(startIndex, i);
		//補間率。
		float intepolateRate = m_animationController[index].GetInterpolateRate();
		//ローカルボーン行列。
		const auto& localBoneMatrix = m_animationController[index].GetBoneLocalMatrix();
		//フットステップの移動量。
		auto deltaValueFootStep = m_animationController[index].GetFootstepDeltaValueOnUpdate();
		//footstepの補間。
		m_footstepDeltaValue.Lerp(intepolateRate, m_footstepDeltaValue, deltaValueFootStep);
		for (int boneNo = 0; boneNo < numBone; boneNo++) {
			//平行移動の補間。
			Matrix m = localBoneMatrix[boneNo];
			vGlobalPose[boneNo].Lerp(
				intepolateRate,
				vGlobalPose[boneNo],
				*(Vector3*)m.m[3]
			);
			//平行移動成分を削除。
			m.m[3][0] = 0.0f;
			m.m[3][1] = 0.0f;
			m.m[3][2] = 0.0f;

			//拡大成分の補間。
			Vector3 vBoneScale;
			vBoneScale.x = (*(Vector3*)m.m[0]).Length();
			vBoneScale.y = (*(Vector3*)m.m[1]).Length();
			vBoneScale.z = (*(Vector3*)m.m[2]).Length();

			vGlobalScale[boneNo].Lerp(
				intepolateRate,
				vGlobalScale[boneNo],
				vBoneScale
			);
			//拡大成分の除去。
			m.m[0][0] /= vBoneScale.x;
			m.m[0][1] /= vBoneScale.x;
			m.m[0][2] /= vBoneScale.x;

			m.m[1][0] /= vBoneScale.y;
			m.m[1][1] /= vBoneScale.y;
			m.m[1][2] /= vBoneScale.y;

			m.m[2][0] /= vBoneScale.z;
			m.m[2][1] /= vBoneScale.z;
			m.m[2][2] /= vBoneScale.z;

			//回転の補間。
			Quaternion qBone;
			qBone.SetRotation(m);
			qGlobalPose[boneNo].Slerp(intepolateRate, qGlobalPose[boneNo], qBone);
		}
	}
	//グローバルポーズをスケルトンに反映させていく。
	int numAnimationController = m_numAnimationPlayController;
	for (int i = 1; i < m_numAnimationPlayController; i++) {
		int index = GetAnimationControllerIndex(startIndex, i);
		if (m_animationController[index].GetInterpolateRate() > 0.99999f) {
			//補間終了。
			m_startAnimationPlayController = index;
			//アニメーションの開始位置を前にする。
			numAnimationController = m_numAnimationPlayController - i;
		}
	}
	m_numAnimationPlayController = numAnimationController;
}

void Animation::Progress(float deltaTime) {
	if (m_numAnimationPlayController == 0) {
		//アニメーションなし。
		return;
	}
	//更新時間。
	m_deltaTimeOnUpdate = deltaTime;
	//ローカルポーズの更新。
	UpdateLocalPose(deltaTime);
	//グローバルポーズを計算。
	UpdateGlobalPose();
}

//Vector3 Animation::CalcFootstepDeltaValueInWorldSpace(Quaternion rot, Vector3 scale) const
//{
//	auto footstepDeltaValueInWorldSpace = m_footstepDeltaValue;
//
//	Matrix mBias = Matrix::Identity;
//	//横に倒れてるのを直す？
//	mBias.MakeRotationX(Math::PI * -0.5f);
//	mBias.Apply(footstepDeltaValueInWorldSpace);
//
//	//フットステップの移動量を拡大する。
//	footstepDeltaValueInWorldSpace.x *= scale.x;
//	footstepDeltaValueInWorldSpace.y *= scale.y;
//	footstepDeltaValueInWorldSpace.z *= scale.z;
//	//フットステップの移動量を回す。
//	rot.Apply(footstepDeltaValueInWorldSpace);
//
//}