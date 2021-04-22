#pragma once

#include "IndexBuffer.h"
#include "VertexBuffer.h"

class RenderContext;

class Primitive
{
public:
	/// <summary>
	/// コンストラクタ。
	/// </summary>
	Primitive();
	/// <summary>
	/// デストラクタ。
	/// </summary>
	~Primitive();
	/// <summary>
	/// プリミティブ作成。
	/// </summary>
	/// <param name="topology">トポロジー。</param>
	/// <param name="numVertex">頂点の数。</param>
	/// <param name="vertexStride">頂点ストライド。</param>
	/// <param name="pSrcVertexBuffer">ソースとなる頂点バッファ。</param>
	/// <param name="numIndex">インデックスの数。</param>
	/// <param name="indexType">インデックスの型。</param>
	/// <param name="pSrcIndexBuffer">ソースとなるインデックスバッファ。</param>
	/// <returns></returns>
	void Init(
		D3D_PRIMITIVE_TOPOLOGY topology,
		int numVertex,
		int vertexStride,
		int numIndex,
		IndexBuffer::EnIndexType indexType
	);
	/// <summary>
	/// リリース。
	/// </summary>
	void Release();
	/// <summary>
	/// 描画。
	/// </summary>
	/// <param name="rc"></param>
	void Draw(RenderContext& rc);
	/// <summary>
	/// 描画。
	/// </summary>
	/// <param name="rc">レンダーコンテキスト。</param>
	/// <param name="numIndex">頂点の数。</param>
	void Draw(RenderContext& rc, int numVertex);
	/// <summary>
	/// 頂点バッファ取得。
	/// </summary>
	/// <returns></returns>
	VertexBuffer& GetVertexBuffer()
	{
		return m_vertexBuffer;
	}
private:
	VertexBuffer			m_vertexBuffer;		//!<頂点バッファ。
	IndexBuffer				m_indexBuffer;		//!<インデックスバッファ。
	D3D_PRIMITIVE_TOPOLOGY	m_topology;			//!<トポロジー。
};

