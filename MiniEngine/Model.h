#pragma once

#include "tkFile/TkmFile.h"
#include "tkFile/TkaFile.h"
#include "MeshParts.h"
#include "Skeleton.h"
#include "srcFile/Animation/AnimationClip.h"
#include "srcFile/Animation/Animation.h"

class IShaderResource;

enum EnRenderMode {
	enRenderMode_Skin,				//�X�L������B
	enRenderMode_NonSkin,			//�X�L���Ȃ��B
	enRenderMode_DrawShadow,		//�V���h�E�}�b�v�`��B
	enRenderMode_NonSkinDrawShadow,	//�X�L���Ȃ��V���h�E�}�b�v�`��B
	enRenderMode_Num				//����񂫂����邪�ꉞ�B
};

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
	~Model();
	/// <summary>
	/// ���f���݂̂̏������B
	/// </summary>
	void InitModel(const char* filepath);
	/// <summary>
	/// ���[���h�s��̍X�V�B
	/// <para>�����_�[���[�h�Ŏ��ϊ����邩���߂Ă�Btodo:enZUP? enYUP?</para>
	/// </summary>
	/// <param name="pos">���W</param>
	/// <param name="rot">��]</param>
	/// <param name="scale">�g�嗦</param>
	/// <param name="rm">�����_�[���[�h�B</param>
	void UpdateWorldMatrix(Vector3 pos, Quaternion rot, Vector3 scale, EnRenderMode& rm);
	/// <summary>
	/// �X�V�B
	/// </summary>
	/// <param name="pos">�ʒu�B</param>
	/// <param name="rot">��]�B</param>
	/// <param name="scale">�g��B</param>
	/// <param name="isForwardRender">�t�H���[�h�����_�[����H�B</param>
	void Update(Vector3 pos, Quaternion rot, Vector3 scale, EnRenderMode& rm);

	/// <summary>
	/// �X�P���g�����֘A�t����B
	/// </summary>
	/// <param name="skeleton">�X�P���g��</param>
	void BindSkeleton(Skeleton& skeleton)
	{
		m_meshParts.BindSkeleton(skeleton);
	}
	/// <summary>
	/// �`��B
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

	const bool& GetAnimFlag() const
	{
		return m_isAnimation;
	}
	/// <summary>
	/// �V���h�E���V�[�o�[�Ƃ��ēo�^�B
	/// </summary>
	void SetShadowReciever(bool flag)
	{
		isShadowReciever = flag;
	}
	/// <summary>
	/// �V���h�E�L���X�^�[�Ƃ��ēo�^�B
	/// </summary>
	void SetShadwoCaster(bool flag)
	{
		isShadowCaster = flag;
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
	/// <summary>
	/// tkm�t�@�C�����擾�B
	/// </summary>
	/// <returns></returns>
	const TkmFile* GetTkmFile() const 
	{
		return m_tkmFile;
	}
private:
	using AnimationClipPtr = std:: unique_ptr<AnimationClip>;
	Matrix			m_world;					//���[���h�s��B
	TkmFile*		m_tkmFile = nullptr;		//tkm�t�@�C���B
	MeshParts		m_meshParts;	//���b�V���p�[�c�B
	bool			isShadowCaster = true;		//�V���h�E�L���X�^�[�H
	bool			isShadowReciever = true;	//�V���h�E���V�[�o�[�H
	bool			m_isAnimation = false;		//�A�j���[�V�������Ă�H todo:�������B
	void*  m_expandConstantBuffer = nullptr;				//�g���萔�o�b�t�@�B
	int m_expandConstantBufferSize = 0;						//�g���萔�o�b�t�@�̃T�C�Y�B
	IShaderResource* m_expandShaderResoruceView = nullptr;	//�g��SRV
	const wchar_t* m_shaderFilePath = L"Assets/shader/NoAnimModel_LambertSpecularAmbient.fx";			//�V�F�[�_�[�̃t�@�C���p�X�B
	char m_name[50];									//�m�F�p�̃l�[���B
};
