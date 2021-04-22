#pragma once

#include "IndexBuffer.h"
#include "VertexBuffer.h"

class RenderContext;

class Primitive
{
public:
	/// <summary>
	/// �R���X�g���N�^�B
	/// </summary>
	Primitive();
	/// <summary>
	/// �f�X�g���N�^�B
	/// </summary>
	~Primitive();
	/// <summary>
	/// �v���~�e�B�u�쐬�B
	/// </summary>
	/// <param name="topology">�g�|���W�[�B</param>
	/// <param name="numVertex">���_�̐��B</param>
	/// <param name="vertexStride">���_�X�g���C�h�B</param>
	/// <param name="pSrcVertexBuffer">�\�[�X�ƂȂ钸�_�o�b�t�@�B</param>
	/// <param name="numIndex">�C���f�b�N�X�̐��B</param>
	/// <param name="indexType">�C���f�b�N�X�̌^�B</param>
	/// <param name="pSrcIndexBuffer">�\�[�X�ƂȂ�C���f�b�N�X�o�b�t�@�B</param>
	/// <returns></returns>
	void Init(
		D3D_PRIMITIVE_TOPOLOGY topology,
		int numVertex,
		int vertexStride,
		int numIndex,
		IndexBuffer::EnIndexType indexType
	);
	/// <summary>
	/// �����[�X�B
	/// </summary>
	void Release();
	/// <summary>
	/// �`��B
	/// </summary>
	/// <param name="rc"></param>
	void Draw(RenderContext& rc);
	/// <summary>
	/// �`��B
	/// </summary>
	/// <param name="rc">�����_�[�R���e�L�X�g�B</param>
	/// <param name="numIndex">���_�̐��B</param>
	void Draw(RenderContext& rc, int numVertex);
	/// <summary>
	/// ���_�o�b�t�@�擾�B
	/// </summary>
	/// <returns></returns>
	VertexBuffer& GetVertexBuffer()
	{
		return m_vertexBuffer;
	}
private:
	VertexBuffer			m_vertexBuffer;		//!<���_�o�b�t�@�B
	IndexBuffer				m_indexBuffer;		//!<�C���f�b�N�X�o�b�t�@�B
	D3D_PRIMITIVE_TOPOLOGY	m_topology;			//!<�g�|���W�[�B
};

