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
	//軸修正。
	Matrix mBias;
	mBias.MakeRotationX(Math::PI * -0.5f);
	if (offsetMatirx != nullptr) {
		mBias = mBias * (*offsetMatirx);
	}
	m_stridingMeshInterface = std::make_unique<btTriangleIndexVertexArray>();
	int numMesh = 0;

	tkmFile.QueryMeshParts([&](const TkmFile::SMesh& mesh) {
		//モデルの頂点バッファから、物理エンジン用の頂点バッファを作成。
		VertexBufferPtr vertexBuffer = std::make_unique<VertexBuffer>();
		for (auto& vb : mesh.vertexBuffer) {
			//メッシュのバッファー分作成。
			auto pos = vb.pos;
			//バイアス適用。
			mBias.Apply(pos);
			vertexBuffer->push_back(pos);
		}
		//モデルのインデックスバッファーから、物理エンジン用インデックスバーファーを作成。
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
		//プッシュ。
		m_vertexBufferArray.push_back(std::move(vertexBuffer));
		m_indexBufferArray.push_back(move(indexBuffer));
		numMesh++;
		//モデルからバッファーを作成し終えたので、BulletPhysicsのインデックスメッシュを形成。
		btIndexedMesh indexMesh;
		//最後尾。
		IndexBuffer* ib = m_indexBufferArray.back().get();
		VertexBuffer* vb = m_vertexBufferArray.back().get();
		//パラメーター設定。
		//三角形の数は(インデックス/3)
		indexMesh.m_numTriangles = (int)ib->size() / 3;
		//ベースは一番最初のインデックス。
		indexMesh.m_triangleIndexBase = (unsigned char*)(&ib->front());
		indexMesh.m_triangleIndexStride = 12;
		indexMesh.m_numVertices = (int)vb->size();
		//ベースは１番最初のバーテックス。
		indexMesh.m_vertexBase = (unsigned char*)(&vb->front());
		indexMesh.m_vertexStride = sizeof(Vector3);
		//作成。
		m_stridingMeshInterface->addIndexedMesh(indexMesh);	
	}//obj
	);
	m_meshShape = std::make_unique<btBvhTriangleMeshShape>(m_stridingMeshInterface.get(), true);
}

void MeshCollider::CreateFromModel(const Model& model, const Matrix* offsetMatirx)
{
	CreateFromTkmFile(*model.GetTkmFile(), offsetMatirx);
}
