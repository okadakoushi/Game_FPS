#include "stdafx.h"
#include "Model.h"
#include "srcFile/Animation/AnimationClip.h"
#include "srcFile/SkinModelRender.h"
#include "SrcFile/Shape/Box.h"
#include <fstream>

Model::~Model()
{
	//���f�������_�[����������ShadowCaster�t���O��OFF�B
	isShadowCaster = false;
}
void Model::InitModel(const char* filepath)
{
	//�����̃V�F�[�_�[�����[�h���鏈�������߂Ă���̂�
	//wchar_t�^�̕�����Ȃ̂ŁA�����ŕϊ����Ă����B
	wchar_t wfxFilePath[256];
	if (filepath == nullptr) {
		MessageBoxA(nullptr, "fx�t�@�C���p�X���w�肳��Ă��܂���B", "�G���[", MB_OK);
		std::abort();
	}
	//�ϊ��B
	mbstowcs(wfxFilePath, filepath, 256);
	//���[�h�B
	m_tkmFile = EngineObj().GetModelDataManager().LoadTkm(filepath);
	//�l�[���B
	strcat(m_name, filepath);

	//�Ƃ�܋������C�g todo:Set�n�B
	//m_expandConstantBuffer = &g_light;
	//m_expandConstantBufferSize = sizeof(g_light);

	//���b�V���p�[�c�����������Ă����B
	m_meshParts.InitFromTkmFile(
		*m_tkmFile,
		m_expandConstantBuffer,
		m_expandConstantBufferSize,
		m_expandShaderResoruceView,
		m_shaderFilePath, m_vsEntry, m_psEntry
	);
	InitAABB();
	//������J�����O�������B
	m_frustomCulling.Init(&GraphicsEngineObj()->GetCamera3D());
}

void Model::Update(Vector3 pos, Quaternion rot, Vector3 scale, EnRenderMode& rm)
{
	//�V���h�E�L���X�^�[�Ƃ��ēo�^�B
	if (isShadowCaster == true) {
		GraphicsEngineObj()->GetShadowMap()->RegisterShadowCaster(this);
	}
	//�o�E���e�B���O�{�b�N�X�X�V�B
	m_aabb.Update(m_world);
	m_frustomCulling.Excute(m_aabb);
	//���[���h���W�X�V�B
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
		//�J�����O�s�\�I�u�W�F�N�g�B
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
	//�ő�A�ŏ����_��tkm�t�@�C������擾�B
	Vector3 vMax = m_tkmFile->GetMaxVertex();
	Vector3 vMin = m_tkmFile->GetMinVertex();
	//�n�[�t�T�C�Y�B
	Vector3 half = (vMax - vMin) * 0.5f;
	m_aabb.Init(half);
}
