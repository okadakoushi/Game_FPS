#include "stdafx.h"
#include "Model.h"
#include "srcFile/Animation/AnimationClip.h"
#include "srcFile/SkinModelRender.h"
#include "SrcFile/Shape/Box.h"
#include <fstream>

Model::~Model()
{
	//モデルレンダー消えたからShadowCasterフラグもOFF。
	isShadowCaster = false;
}
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
	m_tkmFile = EngineObj().GetModelDataManager().LoadTkm(filepath);
	//ネーム。
	strcat(m_name, filepath);

	//とりま強制ライト todo:Set系。
	//m_expandConstantBuffer = &g_light;
	//m_expandConstantBufferSize = sizeof(g_light);

	//メッシュパーツを初期化していく。
	m_meshParts.InitFromTkmFile(
		*m_tkmFile,
		m_expandConstantBuffer,
		m_expandConstantBufferSize,
		m_expandShaderResoruceView,
		m_shaderFilePath, m_vsEntry, m_psEntry
	);
	InitAABB();
	//視錐台カリング初期化。
	m_frustomCulling.Init(&GraphicsEngineObj()->GetCamera3D());
}

void Model::Update(Vector3 pos, Quaternion rot, Vector3 scale, EnRenderMode& rm)
{
	//シャドウキャスターとして登録。
	if (isShadowCaster == true) {
		GraphicsEngineObj()->GetShadowMap()->RegisterShadowCaster(this);
	}
	//バウンティングボックス更新。
	m_aabb.Update(m_world);
	m_frustomCulling.Excute(m_aabb);
	//ワールド座標更新。
	UpdateWorldMatrix(pos, rot, scale, rm);
}

void Model::UpdateWorldMatrix(Vector3 pos, Quaternion rot, Vector3 scale, EnRenderMode& rm)
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
	if (rm != enRenderMode_Skin) {
		m_world = mBias * mScale * mRot * mTrans;
	}
	else {
		m_isAnimation = true;
		m_world = mBias * mScale * mRot * mTrans;
	}

}
void Model::Draw(RenderContext& rc, Matrix viewMat, Matrix projMat, int RenderMode)
{
	//if(!m_frustomCulling.IsCulling()) {
		//カリング不可能オブジェクト。
		m_meshParts.Draw(
			rc,
			m_world,
			viewMat,
			projMat,
			isShadowReciever,
			RenderMode
		);
	//}
}

void Model::InitAABB()
{
	//最大、最小頂点をtkmファイルから取得。
	Vector3 vMax = m_tkmFile->GetMaxVertex();
	Vector3 vMin = m_tkmFile->GetMinVertex();
	//ハーフサイズ。
	Vector3 half = (vMax - vMin) * 0.5f;
	m_aabb.Init(half);
}
