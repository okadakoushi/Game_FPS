#include "stdafx.h"
#include "Model.h"
#include "srcFile/Animation/AnimationClip.h"
#include <fstream>


int Model::Init(const ModelInitData& initData)
{
	//内部のシェーダーをロードする処理が求めているのが
	//wchar_t型の文字列なので、ここで変換しておく。
	wchar_t wfxFilePath[256];
	if (initData.m_fxFilePath == nullptr) {
		MessageBoxA(nullptr, "fxファイルパスが指定されていません。", "エラー", MB_OK);
		std::abort();
	}
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
	//モデルの初期化、メッシュ作成が終わっている。
	//スケルトンデータの読み込み。
	std::string skeltonFilePath = initData.m_tkmFilePath;
	int pos = (int)skeltonFilePath.find(".tkm");
	//.tkmを.tksに置き換え。
	skeltonFilePath.replace(pos, 4, ".tks");
	//ファイルストリーム。
	std::ifstream ifs(skeltonFilePath);
	if (ifs.is_open()) {
		//tksファイルがあるので読み込む。
		m_skeleton.Init(skeltonFilePath.c_str());
		return enInitStep_LoadSkelton;
	}
	else {
		//なかった。
		return enInitStep_Completed;
	}
}

int Model::initSkeleton()
{
	//バインドポーズとかの計算。
	m_skeleton.BuildBoneMatrices();
	//スケルトンとモデルの関連付け。
	BindSkeleton(m_skeleton);
	//アニメーションクリップ。
	if (m_tkaFilePaths.empty() == false) {
		//アニメーションクリップがあった。
		for (auto& m_tkaFilePath : m_tkaFilePaths) {
			//アニメーションのファイルパス分回す。
			auto animClip = make_unique<AnimationClip>();
			//ファイルパスをロード。
			animClip->Load(m_tkaFilePath.c_str());
			m_animationClips.push_back(move(animClip));
		}
		//アニメーションクリップのロード終了。
		return enInitStep_LoadAnimationClips;
	}
	else {
		//全initステップ終了。
		return enInitStep_Completed;
	}
}

void Model::InitAnim(const AnimInitData& initData)
{
	bool isLoaded = true;
	for (auto& animClip : m_animationClips) {
		if (animClip->IsLoaded() == false) {
			isLoaded = false;
			break;
		}
	}
	if (isLoaded == true) {
		//全アニメーションクリップのロード完了。
		//ロードが完了したので、キーフレームの構築。
		for (auto& animClip : m_animationClips) {
			animClip->SetLoopFlag(true);
			animClip->BuildKeyFrames();
		}
		//アニメーション初期化。
	}

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
	m_world = mBias * mScale * mRot * mTrans;
}
void Model::Draw(RenderContext& rc)
{
	m_meshParts.Draw(
		rc, 
		m_world, 
		g_camera3D->GetViewMatrix(), 
		g_camera3D->GetProjectionMatrix()
	);
}
