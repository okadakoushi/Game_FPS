#pragma once
#include "ICollider.h"

/// <summary>
/// メッシュコライダー。
/// </summary>
class MeshCollider : public ICollider
{
public:
	MeshCollider();
	~MeshCollider();
	/// <summary>
	/// モデルからメッシュコライダーを作成。
	/// </summary>
	/// <param name="model">スキンモデル。</param>
	/// <param name="offsetMatirx">オフセット。</param>
	void CreateFromModel(const Model& model, const Matrix* offsetMatirx);
	/// <summary>
	/// tkmファイルからメッシュコライダーを生成。
	/// </summary>
	/// <param name="tkmFile"></param>
	/// <param name="offsetMatirx"></param>
	void CreateFromTkmFile(const TkmFile& tkmFile, const Matrix* offsetMatirx);
	/// <summary>
	/// ボディを取得。
	/// </summary>
	/// <returns></returns>
	btCollisionShape* GetBody() const override
	{
		return m_meshShape.get();
	}
private:
	typedef std::vector<Vector3>			VertexBuffer;			//頂点バッファー。
	typedef std::vector<unsigned int>		IndexBuffer;			//インデックスバッファー。
	typedef std::unique_ptr<VertexBuffer>	VertexBufferPtr;	
	typedef std::unique_ptr<IndexBuffer>	IndexBufferPtr;
	std::vector<VertexBufferPtr>				m_vertexBufferArray;		//頂点バッファの配列。
	std::vector<IndexBufferPtr>					m_indexBufferArray;			//インデックスバッファの配列。
	std::unique_ptr<btBvhTriangleMeshShape>		m_meshShape;				//メッシュ形成。
	std::unique_ptr<btTriangleIndexVertexArray>	m_stridingMeshInterface;	//三角形インデックス。
};

