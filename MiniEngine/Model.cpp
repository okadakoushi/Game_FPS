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
		m_shaderFilePath
	);
	//InitAABB();
	//視錐台カリング初期化。
	//m_frustomCulling.Init();
}

void Model::Update(Vector3 pos, Quaternion rot, Vector3 scale, EnRenderMode& rm)
{
	//シャドウキャスターとして登録。
	if (isShadowCaster == true) {
		GraphicsEngineObj()->GetShadowMap()->RegisterShadowCaster(this);
	}
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
	m_meshParts.Draw(
		rc, 
		m_world, 
		viewMat,
		projMat,
		isShadowReciever,
		RenderMode
	);
}

void Model::InitAABB()
{
	//m_tkmFile->QueryMeshParts([&](const TkmFile::SMesh& mesh) {
	Vector3 vMax = Vector3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	Vector3 vMin = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
	auto dv = GraphicsEngineObj()->GetD3DDevice();
	{
		for (auto mesh : m_meshParts.GetMeshList()) {
			//ビュー。Desc?
			auto view = mesh->m_vertexBuffer.GetView();
			//cpuVB
			void* vertexBufferCPU;
			CD3DX12_RANGE readRange(0, 0);
			//map
			HRESULT hr = mesh->m_vertexBuffer.GetResource()->Map(0, &readRange, reinterpret_cast<void**>(&vertexBufferCPU));
			if (FAILED(hr)) {
				MessageBoxA(nullptr, "マッピングに失敗しました。", "Model::InitAABB", MB_OK);
			}
			memcpy(vertexBufferCPU, &mesh->m_vertexBuffer, sizeof(mesh->m_vertexBuffer));
			//頂点数。
			auto vertexCount = (int)(view.SizeInBytes / view.StrideInBytes);
			//開始アドレス。
			auto pData = reinterpret_cast<char*>(vertexBufferCPU);
			for (int i = 0; i < vertexCount; i++) {
				auto pos = *reinterpret_cast<Vector3*>(pData);
				vMax.Max(pos);
				vMin.Min(pos);
				//次の頂点。
				pData += view.StrideInBytes;
			}
			mesh->m_vertexBuffer.GetResource()->Unmap(0, &readRange);
		}
		Vector3 halfSize = (vMax - vMin) * 0.5f;
		m_aabb.Init(halfSize);
	}
	//});
}
