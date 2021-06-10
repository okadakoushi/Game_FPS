/// <summary>
/// ���b�V���p�[�c�N���X�B
/// </summary>

#pragma once

#include "tkFile/TkmFile.h"
#include "StructuredBuffer.h"

class RenderContext;
class Skeleton;
class Material;
class IShaderResource;


/// <summary>
/// ���b�V��
/// </summary>
struct SMesh {
	VertexBuffer m_vertexBuffer;						//���_�o�b�t�@�B
	std::vector< IndexBuffer* >		m_indexBufferArray;	//�C���f�b�N�X�o�b�t�@�B
	std::vector< Material* >		m_materials;			//�}�e���A���B
	std::vector<int>				skinFlags;				//�X�L���������Ă��邩�ǂ����̃t���O�B
};

/// <summary>
/// ���b�V���p�[�c�B
/// </summary>
class MeshParts {
public:
	/// <summary>
	/// �f�X�g���N�^�B
	/// </summary>
	~MeshParts();

	void Release();
	/// <summary>
	/// tkm�t�@�C�����珉����
	/// </summary>
	/// <param name="tkmFile">tkm�t�@�C���B</param>
	/// /// <param name="fxFilePath">fx�t�@�C���̃t�@�C���p�X</param>
	/// <param name="vsEntryPointFunc">���_�V�F�[�_�[�̃G���g���[�|�C���g�̊֐���</param>
	/// <param name="psEntryPointFunc">�s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g�̊֐���</param>
	void InitFromTkmFile(
		const TkmFile& tkmFile,
		void* expandData,
		int expandDataSize,
		IShaderResource* expandShaderResourceView,
		const wchar_t* filePath, 
		const char* VSEntry, 
		const char* PSEntry
	) ;
	/// <summary>
	/// �`��B
	/// </summary>
	/// <param name="rc">�����_�����O�R���e�L�X�g</param>
	/// <param name="mWorld">���[���h�s��</param>
	/// <param name="mView">�r���[�s��</param>
	/// <param name="mProj">�v���W�F�N�V�����s��</param>
	/// <param name="light">���C�g</param>
	void Draw(RenderContext& rc, const Matrix& mWorld, const Matrix& mView, const Matrix& mProj, const bool isShadowReciever = false, const int renderMode = 0);
	/// <summary>
	/// �X�P���g�����֘A�t����B
	/// </summary>
	/// <param name="skeleton">�X�P���g��</param>
	void BindSkeleton(Skeleton& skeleton) ;
	/// <summary>
	/// ��Z�J���[��ݒ�B
	/// </summary>
	void SetMulColor(const Vector4& color)
	{
		m_mulColor = color;
	}
	/// <summary>
	/// ���b�V�����X�g���擾�B
	/// </summary>
	/// <returns></returns>
	std::vector< SMesh* >& GetMeshList()
	{
		return m_meshs;
	}

	std::vector<DescriptorHeap>& GetDescriptorHeap()
	{
		return m_descriptorHeap;
	}
private:
	/// <summary>
	/// tkm���b�V�����烁�b�V�����쐬�B
	/// </summary>
	/// <param name="mesh">���b�V��</param>
	/// <param name="meshNo">���b�V���ԍ�</param>
	/// <param name="fxFilePath">fx�t�@�C���̃t�@�C���p�X</param>
	/// <param name="vsEntryPointFunc">���_�V�F�[�_�[�̃G���g���[�|�C���g�̊֐���</param>
	/// <param name="psEntryPointFunc">�s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g�̊֐���</param>
	void CreateMeshFromTkmMesh(
		const TkmFile::SMesh& mesh, 
		int meshNo,
		const wchar_t* filePath, 
		const char* VSEntry, 
		const char* PSEntry
	);
	/// <summary>
	/// �f�B�X�N���v�^�q�[�v���쐬�B
	/// </summary>
	void CreateDescriptorHeaps();
private:
	//�g��SRV���ݒ肳��郌�W�X�^�̊J�n�ԍ��B
	const int EXPAND_SRV_REG__START_NO = 10;
	/// <summary>
	/// �萔�o�b�t�@�B
	/// </summary>
	/// <remarks>
	/// ���̍\���̂�ύX������ASimpleModel.fx��CB���ύX����悤�ɁB
	/// </remarks>
	struct SConstantBuffer {
		Matrix mWorld;			//���[���h�s��B
		Matrix mView;			//�r���[�s��B
		Matrix mProj;			//�v���W�F�N�V�����s��B
		Vector4 mulColor;			//��Z�J���[�B
		int isShadowReciever;	//�V���h�E���V�[�o�[�H
	};
	ConstantBuffer m_commonConstantBuffer;					//���b�V�����ʂ̒萔�o�b�t�@�B
	ConstantBuffer	m_expandConstantBuffer;					//���[�U�[�g���p�̒萔�o�b�t�@
	IShaderResource* m_expandShaderResourceView = nullptr;	//���[�U�[�g���V�F�[�_�[���\�[�X�r���[�B
	StructuredBuffer m_boneMatricesStructureBuffer;			//�{�[���s��̍\�����o�b�t�@�B
	std::vector< SMesh* > m_meshs;							//���b�V���B
	std::vector< DescriptorHeap > m_descriptorHeap;			//�f�B�X�N���v�^�q�[�v�B
	Skeleton* m_skeleton = nullptr;							//�X�P���g���B
	void* m_expandData = nullptr;							//���[�U�[�g���f�[�^�B
	bool m_isCreateDescriptorHeap = false;					//�f�B�X�N���v�^�q�[�v���쐬�ς݁H
	Vector4 m_mulColor = Vector4::White;					//��Z�J���[�B
};