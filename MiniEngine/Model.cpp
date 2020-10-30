#include "stdafx.h"
#include "Model.h"
#include "srcFile/Animation/AnimationClip.h"
#include <fstream>


void Model::InitModel(const char* filepath)
{
	//内部のシェーダーをロードする処理が求めているのが
	//wchar_t型の文字列なので、ここで変換しておく。
	wchar_t wfxFilePath[256];
	if (filepath == nullptr) {
		MessageBoxA(nullptr, "fxファイルパスが指定されていません。", "エラー", MB_OK);
		std::abort();
	}
	//変換。
	mbstowcs(wfxFilePath, filepath, 256);
	//ロード。
	m_tkmFile.Load(filepath);

	//とりま強制ライト todo:Set系。
	m_expandConstantBuffer = &g_light;
	m_expandConstantBufferSize = sizeof(g_light);

	//メッシュパーツを初期化していく。
	m_meshParts.InitFromTkmFile(
		m_tkmFile,
		m_expandConstantBuffer,
		m_expandConstantBufferSize,
		m_expandShaderResoruceView
	);
}

void Model::UpdateMatAndAnim()
{
	//アニメーション更新。
	m_animation.Progress(DELTA_TIME);
	//めんどいからまだSet作ってないよぉ！
	UpdateWorldMatrix(m_pos, m_rot, m_scale);
	//スケルトンも更新。
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
	m_world =/* mBias **/mScale * mRot * mTrans;
}
void Model::Draw(RenderContext& rc, Matrix viewMat, Matrix projMat, int RenderMode)
{
	m_meshParts.Draw(
		rc, 
		m_world, 
		viewMat,
		projMat,
		isShadowReciever,
		RenderMode
	);
}
