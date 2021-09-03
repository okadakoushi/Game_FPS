#include "stdafx.h"
#include "SkyBox.h"
#include "Material.h"

SkyBox::SkyBox()
{
}

SkyBox::~SkyBox()
{
}

void SkyBox::OnDestroy()
{
	DeleteGO(m_modelRender);
	GraphicsEngineObj()->GetDefferd().GetDefferdSprite().UnRegistSRV(Gbuffer_Num);
}

void SkyBox::Init()
{
}

bool SkyBox::Start()
{
	//空のテクスチャ初期化。todo:確認。
	m_skyCube.InitFromDDSFile(m_skyCubeMapFilePath.c_str());

	m_modelRender = NewGO<SkinModelRender>(EnPriority_3DModel);
	//空は特殊なレンダリングなのでForwardレンダリング描画。
	m_modelRender->SetForwardRender();
	m_modelRender->GetModel().SetPSEntry("PSMain_SkyCube");
	m_modelRender->SetExpandConstantBufferParam(&m_skyParams, sizeof(m_skyParams));
	m_modelRender->Init(m_skyModelFilePath);
	//空用シェーダーロード。
	//m_psShader.LoadPS(L"Assets/shader/NoAnimModel_LambertSpecularAmbient.fx", "PSMain_SkyCube");

	//マテリアルのシェーダーとテクスチャを切り替え。
	//ラムダ使ったほうがいいと思いつつ、とりまforatuoでやっちゃお
	for (auto* mesh : m_modelRender->GetModel().GetMeshParts().GetMeshList()) {
		for (auto mat : mesh->m_materials) {
			mat->SetAlbedoMap(m_skyCube);
		}
	}

	GraphicsEngineObj()->GetDefferd().GetDefferdSprite().SetExternalTexture(m_skyCube, Gbuffer_Num);

	m_isDirty = true;
	return true;
}

void SkyBox::Update()
{
	if (m_isDirty) {
		m_modelRender->SetScale(m_scale);
		m_modelRender->SetPosition(m_position);
		m_isDirty = false;
	}
}