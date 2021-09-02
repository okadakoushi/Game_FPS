#pragma once

//#include "Indexbuffer.h"
//#include "VertexBuffer.h"
//#include "ConstantBuffer.h"
//#include "DescriptorHeap.h"

class Texture;

//�X�v���C�g�ɐݒ�ł���ő�e�N�X�`�����B
const int MAX_TEXTURE = 16;	
//�g��SRV���ݒ肳��郌�W�X�^�̊J�n�ԍ��B
const int EXPAND_SRV_REG__START_NO = 10;
//���[�U�[�g���R���X�^���g�o�b�t�@�[�ݒ�p�̃��W�X�^�̊J�n�ԍ��B
const int EXPAND_SET_CONSTANT_BUFFER_START_NO = 10;

class IShaderResource;

/// <summary>
/// �A���t�@�u�����f�B���O���[�h
/// </summary>
enum AlphaBlendMode {
	AlphaBlendMode_None,	//�A���t�@�u�����f�B���O�Ȃ�(�㏑��)�B
	AlphaBlendMode_Trans,	//����������
	AlphaBlendMode_Add,		//���Z����
	AlphaBlendMode_Multiply	//��Z����
};

template< class TExpandData > struct SpriteExpandDataInfo {
	TExpandData* m_expandData = nullptr;
	int m_expandDataSize = 0;
};
/// <summary>
/// �X�v���C�g�̏������f�[�^�B
/// </summary>
struct SpriteInitData {
	const char* m_ddsFilePath[MAX_TEXTURE]= {nullptr};		//DDS�t�@�C���̃t�@�C���p�X�B
	Texture* m_textures[MAX_TEXTURE] = { nullptr };			//�g�p����e�N�X�`���BDDS�t�@�C���̃p�X���w�肳��Ă���ꍇ�́A���̃p�����[�^�͖�������܂��B
	const char* m_vsEntryPointFunc = "VSMain";				//���_�V�F�[�_�[�̃G���g���[�|�C���g�B
	const char* m_psEntryPoinFunc = "PSMain";				//�s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g�B
	const char* m_fxFilePath = nullptr;						//.fx�t�@�C���̃t�@�C���p�X�B
	UINT m_width = 0;										//�X�v���C�g�̕��B
	UINT m_height = 0;										//�X�v���C�g�̍����B
	void* m_expandConstantBuffer = nullptr;					//���[�U�[�g���̒萔�o�b�t�@
	int m_expandConstantBufferSize = 0;						//���[�U�[�g���̒萔�o�b�t�@�̃T�C�Y�B
	IShaderResource* m_expandShaderResoruceView = nullptr;	//���[�U�[�g���̃V�F�[�_�[���\�[�X�B
	AlphaBlendMode m_alphaBlendMode = AlphaBlendMode_Trans;	//�A���t�@�u�����f�B���O���[�h�B
	std::array<DXGI_FORMAT, D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT> m_colorBufferFormat = {
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
	};	//�����_�����O����J���[�o�b�t�@�̃t�H�[�}�b�g�B
	UINT8 renderTargetMask = D3D12_COLOR_WRITE_ENABLE_ALL;
};
/// <summary>
/// �X�v���C�g�N���X�B
/// </summary>
class Sprite  {
public:
	static const Vector2	DEFAULT_PIVOT;					//!<�s�{�b�g�B
	virtual ~Sprite();
	/// <summary>
	/// �������B
	/// </summary>
	/// <param name="initData">�������f�[�^</param>
	void Init(const SpriteInitData& initData);
	/// <summary>
	/// �X�V�B
	/// </summary>
	/// <param name="pos">���W</param>
	/// <param name="rot">��]</param>
	/// <param name="scale">�g�嗦</param>
	/// <param name="pivot">
	/// <param name="isDraw3D">3D�`��H</param>
	/// �s�{�b�g
	/// 0.5, 0.5�ŉ摜�̒��S����_�B
	/// 0.0, 0.0�ŉ摜�̍����B
	/// 1.0, 1.0�ŉ摜�̉E��B
	/// Unity��uGUI�ɏ����B
	/// </param>
	void Update(const Vector3& pos, const Quaternion& rot, const Vector3& scale, const Vector2& pivot = DEFAULT_PIVOT, const bool& isDraw3D = false);
	/// <summary>
	/// �`��B
	/// </summary>
	/// <param name="renderContext"></param>
	/// <param name="view"></param>
	/// <param name="proj"></param>
	void Draw(RenderContext& renderContext, const Matrix& view, const Matrix& proj);
	/// <summary>
	/// ��Z�J���[��ݒ�B
	/// </summary>
	/// <param name="color"></param>
	void SetMulColor(const Vector4& color)
	{
		m_constantBufferCPU.mulColor = color;
	}
	/// <summary>
	/// �e�N�X�`�����擾�B
	/// </summary>
	/// <param name="index">�e�N�X�`���̓Y�����B</param>
	/// <returns></returns>
	Texture* GetTexture(int index)
	{
		return m_textureExternal[index];
	}
	/// <summary>
	/// ���[�U�[�ݒ�^�萔�o�b�t�@��ݒ�B
	/// </summary>
	/// <param name="entity"></param>
	void SetUserSetConstantBufferEntity(void* entity)
	{
		m_userSetConstantBufferCPU = entity;
	}
	/// <summary>
	/// �O���e�N�X�`����ݒ�BFromUser�B
	/// </summary>
	/// <param name="tex">�e�N�X�`��</param>
	/// <param name="registNumber">�o�^���W�X�^�B</param>
	void SetExternalTexture(Texture& tex, int registNumber)
	{
		//�O���o�^�����e�N�X�`����ݒ�B
		m_descriptorHeap.RegistShaderResource(registNumber, tex);
		//�ύX���m��B
		m_descriptorHeap.Commit();
	}
	/// <summary>
	/// �V�F�[�_�[���\�[�X�̓o�^�������B
	/// </summary>
	/// <param name="registNumber"></param>
	void UnRegistSRV(int registNumber)
	{
		//���W�X�g���O���B
		m_descriptorHeap.UnRegistShaderResource(registNumber);
		//�ύX���m��B
		m_descriptorHeap.Commit();
	}
	/// <summary>
	/// IBL�̋P�x��ݒ�B
	/// </summary>
	void SetIBLItensity(const float& itensity)
	{
		m_constantBufferCPU.lightIntensity = itensity;
	}
	/// <summary>
	/// �e�N�X�`���̕����擾�B
	/// </summary>
	/// <param name="texNo"></param>
	/// <returns></returns>
	int GetTextureWidth(int texNo) const
	{
		return m_textures[texNo].GetWidth();
	}
	/// <summary>
	/// �e�N�X�`���̕����擾�B
	/// </summary>
	/// <param name="texNo"></param>
	/// <returns></returns>
	int GetTextureHeight(int texNo) const
	{
		return m_textures[texNo].GetHeight();
	}
	/// <summary>
	/// 3D�`�悷�邩�B
	/// </summary>
	/// <param name="flag"></param>
	void SetDraw3DFlag(bool& flag)
	{
		m_isDraw3D = flag;
	}
private:
	/// <summary>
	/// �e�N�X�`�����������B
	/// </summary>
	/// <param name="initData"></param>
	void InitTextures(const SpriteInitData& initData);
	/// <summary>
	/// �V�F�[�_�[���������B
	/// </summary>
	/// <param name="initData"></param>
	void InitShader( const SpriteInitData& initData );
	/// <summary>
	/// �f�B�X�N���v�^�q�[�v���������B
	/// </summary>
	/// <param name="initData"></param>
	void InitDescriptorHeap(const SpriteInitData& initData);
	/// <summary>
	/// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@���������B
	/// </summary>
	/// <param name="initData"></param>
	void InitVertexBufferAndIndexBuffer(const SpriteInitData& initData);
	/// <summary>
	/// �p�C�v���C���X�e�[�g������������B
	/// </summary>
	void InitPipelineState(const SpriteInitData& initData);
	/// <summary>
	/// �萔�o�b�t�@���������B
	/// </summary>
	/// <param name="initData"></param>
	void InitConstantBuffer(const SpriteInitData& initData);
private:
	IndexBuffer m_indexBuffer;			//�C���f�b�N�X�o�b�t�@�B
	VertexBuffer m_vertexBuffer;		//���_�o�b�t�@�B
	int m_numTexture = 0;				//�e�N�X�`���̖����B
	Texture m_textures[MAX_TEXTURE];	//�e�N�X�`���B
	Texture* m_textureExternal[MAX_TEXTURE] = {nullptr};	//�O������w�肳�ꂽ�e�N�X�`��
	Vector3 m_position ;				//���W�B
	Vector2 m_size;						//�T�C�Y�B
	Quaternion m_rotation ;				//��]�B
	Matrix m_world;						//���[���h�s��B
	SpriteInitData m_initData;

	struct LocalConstantBuffer {
		Matrix mvp;
		Vector4 mulColor = Vector4::White;
		float lightIntensity = 4.0f;
	};
	LocalConstantBuffer m_constantBufferCPU;	//CPU���̒萔�o�b�t�@�B
	ConstantBuffer		m_constantBufferGPU;	//GPU���̒萔�o�b�t�@�B
	ConstantBuffer		m_userExpandConstantBufferGPU;	//���[�U�[�g���̒萔�o�b�t�@(GPU��)
	void* m_userExpandConstantBufferCPU = nullptr;		//���[�U�[�g���̒萔�o�b�t�@(CPU��)
	ConstantBuffer		m_userSetConstantBufferGPU;		//���[�U�[�ݒ�^�萔�o�b�t�@�B
	void* m_userSetConstantBufferCPU = nullptr;			//���[�U�[�ݒ�^�萔Entity�B
	DescriptorHeap		m_descriptorHeap;		//�f�B�X�N���v�^�q�[�v�B
	RootSignature		m_rootSignature;		//���[�g�V�O�l�`���B
	PipelineState		m_pipelineState;		//�p�C�v���C���X�e�[�g�B
	Shader				m_vs;					//���_�V�F�[�_�[�B
	Shader				m_ps;					//�s�N�Z���V�F�[�_�[�B
	bool m_isDraw3D = false;					//isDraw3D.viewProj�̊֌W�ケ���ɒ�`�B
};