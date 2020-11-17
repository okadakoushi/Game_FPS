#pragma once

#include "tkFile/TkmFile.h"
#include "tkFile/TkaFile.h"
#include "MeshParts.h"
#include "Skeleton.h"
#include "srcFile/Animation/AnimationClip.h"
#include "srcFile/Animation/Animation.h"

class IShaderResource;

/// <summary>
/// ���f���̏������f�[�^
/// </summary>
struct ModelInitData {
	const char* m_tkmFilePath = nullptr;		//tkm�t�@�C���p�X�B
	const char* m_vsEntryPointFunc = "VSMain";	//���_�V�F�[�_�[�̃G���g���[�|�C���g�B
	const char* m_psEntryPointFunc = "PSMain";	//�s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g�B
	const char* m_fxFilePath = nullptr;			//.fx�t�@�C���̃t�@�C���p�X�B
	const char* m_tkaFilePath = nullptr;		//tka�t�@�C���p�X�B
	void* m_expandConstantBuffer = nullptr;		//���[�U�[�g���̒萔�o�b�t�@�B
	int m_expandConstantBufferSize = 0;			//���[�U�[�g���̒萔�o�b�t�@�̃T�C�Y�B
	IShaderResource* m_expandShaderResoruceView = nullptr;	//���[�U�[�g���̃V�F�[�_�[���\�[�X�B
};

/// <summary>
/// ���f���̏������X�e�b�v�B
/// </summary>
/// <remarks>
/// ������case�Ǘ����A���f���̏�������i�߂�B
/// </remarks>


/// <summary>
/// ���f���N���X�B
/// </summary>
class Model {

public:

	/// <summary>
	/// ���f���݂̂̏������B
	/// </summary>
	void InitModel(const char* filepath);
	/// <summary>
	/// ���[���h�s��̍X�V�B
	/// </summary>
	/// <param name="pos">���W</param>
	/// <param name="rot">��]</param>
	/// <param name="scale">�g�嗦</param>
	void UpdateWorldMatrix(Vector3 pos, Quaternion rot, Vector3 scale);
	/// <summary>
	/// �X�V�B
	/// </summary>
	/// <param name="pos">�ʒu�B</param>
	/// <param name="rot">��]�B</param>
	/// <param name="scale">�g��B</param>
	/// <param name="isForwardRender">�t�H���[�h�����_�[����H�B</param>
	void Update(Vector3 pos, Quaternion rot, Vector3 scale);

	/// <summary>
	/// �X�P���g�����֘A�t����B
	/// </summary>
	/// <param name="skeleton">�X�P���g��</param>
	void BindSkeleton(Skeleton& skeleton)
	{
		m_meshParts.BindSkeleton(skeleton);
	}
	/// <summary>
	/// �A�j���[�V�������Đ��B
	/// </summary>
	/// <param name="animNo">�N���b�v�ԍ��B</param>
	/// <param name="interpolateTime">��ԗ��B</param>
	void PlayAnim(int animNo, float interpolateTime) {
		m_animation.Play(animNo, interpolateTime);
	}
	/// <summary>
	/// 
	/// </summary>
	/// <param name="renderContext"></param>
	/// <param name="viewMat">�r���[�B</param>
	/// <param name="projMat">�v���W�F�N�V�����B</param>
	/// <param name="RenderMode">�����_�[���[�h�B</param>
	void Draw(RenderContext& renderContext, Matrix viewMat, Matrix projMat, int RenderMode = 0);
	/// <summary>
	/// ���[���h�s����擾�B
	/// </summary>
	/// <returns></returns>
	const Matrix& GetWorldMatrix() const
	{
		return m_world;
	}
	/// <summary>
	/// �V���h�E���V�[�o�[�Ƃ��ēo�^�B
	/// </summary>
	void SetShadowReciever()
	{
		isShadowReciever = true;
	}
	/// <summary>
	/// �V���h�E�L���X�^�[�Ƃ��ēo�^�B
	/// </summary>
	void SetShadwoCaster()
	{
		isShadowCaster = true;
	}
	/// <summary>
	/// �V�F�[�_�[��ݒ�B
	///<para>������Ǝ��p�����Ȃ����������Btodo:�C���B</para>
	/// </summary>
	/// <param name="fxPath">�V�F�[�_�[�̃t�@�C���p�X�B</param>
	void SetShader(const wchar_t* fxPath)
	{
		m_shaderFilePath = fxPath;
	}
private:
	using AnimationClipPtr = std:: unique_ptr<AnimationClip>;
	Matrix			m_world;		//���[���h�s��B
	TkmFile			m_tkmFile;		//tkm�t�@�C���B
	TkaFile			m_tkaFile;		//tka�t�@�C���B
	Skeleton		m_skeleton;		//�X�P���g���B
	MeshParts		m_meshParts;	//���b�V���p�[�c�B
	bool			isShadowCaster = false;		//�V���h�E�L���X�^�[�H
	bool			isShadowReciever = false;	//�V���h�E���V�[�o�[�H
	void*  m_expandConstantBuffer = nullptr;				//�g���萔�o�b�t�@�B
	int m_expandConstantBufferSize = 0;						//�g���萔�o�b�t�@�̃T�C�Y�B
	IShaderResource* m_expandShaderResoruceView = nullptr;	//�g��SRV
	std::vector<std::string>		m_tkaFilePaths;		//tka�t�@�C���̃t�@�C���p�X���X�g�B
	std::vector<AnimationClipPtr>	m_animationClips;	//�A�j���[�V�����N���b�v�B
	Animation						m_animation;		//�A�j���[�V����
	const wchar_t* m_shaderFilePath = L"Assets/shader/NoAnimModel_LambertSpecularAmbient.fx";			//�V�F�[�_�[�̃t�@�C���p�X�B
};
