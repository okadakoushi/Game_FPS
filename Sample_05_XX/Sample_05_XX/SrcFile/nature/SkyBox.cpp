#include "stdafx.h"
#include "SkyBox.h"
#include "Material.h"

SkyBox::SkyBox()
{
}

SkyBox::~SkyBox()
{
}

bool SkyBox::Start()
{
	m_modelRender = NewGO<SkinModelRender>(EnPriority_3DModel);
	//��͓���ȃ����_�����O�Ȃ̂�Forward�����_�����O�`��B
	m_modelRender->SetForwardRender();
	m_modelRender->GetModel().SetPSEntry("PSMain_SkyCube");
	m_modelRender->Init(m_skyModelFilePath);
	//��̃e�N�X�`���������Btodo:�m�F�B
	m_skyCube.InitFromDDSFile(m_skyCubeMapFilePath.c_str());
	//��p�V�F�[�_�[���[�h�B
	//m_psShader.LoadPS(L"Assets/shader/NoAnimModel_LambertSpecularAmbient.fx", "PSMain_SkyCube");

	//�}�e���A���̃V�F�[�_�[�ƃe�N�X�`����؂�ւ��B
	for (auto* mesh : m_modelRender->GetModel().GetMeshParts().GetMeshList()) {
		for (auto mat : mesh->m_materials) {
			mat->SetAlbedoMap(m_skyCube);
		}
	}
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
