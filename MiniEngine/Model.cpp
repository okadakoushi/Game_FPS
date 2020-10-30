#include "stdafx.h"
#include "Model.h"
#include "srcFile/Animation/AnimationClip.h"
#include <fstream>


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
	m_tkmFile.Load(filepath);

	//�Ƃ�܋������C�g todo:Set�n�B
	m_expandConstantBuffer = &g_light;
	m_expandConstantBufferSize = sizeof(g_light);

	//���b�V���p�[�c�����������Ă����B
	m_meshParts.InitFromTkmFile(
		m_tkmFile,
		m_expandConstantBuffer,
		m_expandConstantBufferSize,
		m_expandShaderResoruceView
	);
}

void Model::UpdateMatAndAnim()
{
	//�A�j���[�V�����X�V�B
	m_animation.Progress(DELTA_TIME);
	//�߂�ǂ�����܂�Set����ĂȂ��悧�I
	UpdateWorldMatrix(m_pos, m_rot, m_scale);
	//�X�P���g�����X�V�B
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
