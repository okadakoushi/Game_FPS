#pragma once
#include "ICollider.h"

/// <summary>
/// ���b�V���R���C�_�[�B
/// </summary>
class MeshCollider : public ICollider
{
public:
	MeshCollider();
	~MeshCollider();
	/// <summary>
	/// ���f�����烁�b�V���R���C�_�[���쐬�B
	/// </summary>
	/// <param name="model">�X�L�����f���B</param>
	/// <param name="offsetMatirx">�I�t�Z�b�g�B</param>
	void CreateFromModel(const Model& model, const Matrix* offsetMatirx);
	/// <summary>
	/// tkm�t�@�C�����烁�b�V���R���C�_�[�𐶐��B
	/// </summary>
	/// <param name="tkmFile"></param>
	/// <param name="offsetMatirx"></param>
	void CreateFromTkmFile(const TkmFile& tkmFile, const Matrix* offsetMatirx);
	/// <summary>
	/// �{�f�B���擾�B
	/// </summary>
	/// <returns></returns>
	btCollisionShape* GetBody() const override
	{
		return m_meshShape.get();
	}
private:
	typedef std::vector<Vector3>			VertexBuffer;			//���_�o�b�t�@�[�B
	typedef std::vector<unsigned int>		IndexBuffer;			//�C���f�b�N�X�o�b�t�@�[�B
	typedef std::unique_ptr<VertexBuffer>	VertexBufferPtr;	
	typedef std::unique_ptr<IndexBuffer>	IndexBufferPtr;
	std::vector<VertexBufferPtr>				m_vertexBufferArray;		//���_�o�b�t�@�̔z��B
	std::vector<IndexBufferPtr>					m_indexBufferArray;			//�C���f�b�N�X�o�b�t�@�̔z��B
	std::unique_ptr<btBvhTriangleMeshShape>		m_meshShape;				//���b�V���`���B
	std::unique_ptr<btTriangleIndexVertexArray>	m_stridingMeshInterface;	//�O�p�`�C���f�b�N�X�B
};

