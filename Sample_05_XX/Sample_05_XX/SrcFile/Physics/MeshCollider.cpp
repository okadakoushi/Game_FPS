#include "stdafx.h"
#include "MeshCollider.h"

MeshCollider::MeshCollider()
{
}

MeshCollider::~MeshCollider()
{
}

void MeshCollider::CreateFromTkmFile(const TkmFile& tkmFile, const Matrix* offsetMatirx)
{
	//���C���B
	Matrix mBias;
	mBias.MakeRotationX(Math::PI * -0.5f);
	if (offsetMatirx != nullptr) {
		mBias = mBias * (*offsetMatirx);
	}
	m_stridingMeshInterface = std::make_unique<btTriangleIndexVertexArray>();
	int numMesh = 0;

	tkmFile.QueryMeshParts([&](const TkmFile::SMesh& mesh) {
		//���f���̒��_�o�b�t�@����A�����G���W���p�̒��_�o�b�t�@���쐬�B
		VertexBufferPtr vertexBuffer = std::make_unique<VertexBuffer>();
		for (auto& vb : mesh.vertexBuffer) {
			//���b�V���̃o�b�t�@�[���쐬�B
			auto pos = vb.pos;
			//�o�C�A�X�K�p�B
			mBias.Apply(pos);
			vertexBuffer->push_back(pos);
		}
		//���f���̃C���f�b�N�X�o�b�t�@�[����A�����G���W���p�C���f�b�N�X�o�[�t�@�[���쐬�B
		IndexBufferPtr indexBuffer = std::make_unique<IndexBuffer>();
		if (!mesh.indexBuffer16Array.empty()) {
			//16bit
			for (auto& ib : mesh.indexBuffer16Array) {
				for (auto index : ib.indices) {
					indexBuffer->push_back(index);
				}
			}
		}
		else {
			//32bit
			for (auto& ib : mesh.indexBuffer32Array) {
				for (auto index : ib.indices) {
					indexBuffer->push_back(index);
				}
			}
		}
		//�v�b�V���B
		m_vertexBufferArray.push_back(std::move(vertexBuffer));
		m_indexBufferArray.push_back(move(indexBuffer));
		numMesh++;
		//���f������o�b�t�@�[���쐬���I�����̂ŁABulletPhysics�̃C���f�b�N�X���b�V�����`���B
		btIndexedMesh indexMesh;
		//�Ō���B
		IndexBuffer* ib = m_indexBufferArray.back().get();
		VertexBuffer* vb = m_vertexBufferArray.back().get();
		//�p�����[�^�[�ݒ�B
		//�O�p�`�̐���(�C���f�b�N�X/3)
		indexMesh.m_numTriangles = (int)ib->size() / 3;
		//�x�[�X�͈�ԍŏ��̃C���f�b�N�X�B
		indexMesh.m_triangleIndexBase = (unsigned char*)(&ib->front());
		indexMesh.m_triangleIndexStride = 12;
		indexMesh.m_numVertices = (int)vb->size();
		//�x�[�X�͂P�ԍŏ��̃o�[�e�b�N�X�B
		indexMesh.m_vertexBase = (unsigned char*)(&vb->front());
		indexMesh.m_vertexStride = sizeof(Vector3);
		//�쐬�B
		m_stridingMeshInterface->addIndexedMesh(indexMesh);	
	}//obj
	);
	m_meshShape = std::make_unique<btBvhTriangleMeshShape>(m_stridingMeshInterface.get(), true);
}

void MeshCollider::CreateFromModel(const Model& model, const Matrix* offsetMatirx)
{
	CreateFromTkmFile(*model.GetTkmFile(), offsetMatirx);
}
