#include "stdafx.h"
#include "Primitive.h"

Primitive::Primitive()
{
}

Primitive::~Primitive()
{
	Release();
}

void Primitive::Init(D3D_PRIMITIVE_TOPOLOGY topology, int numVertex, int vertexStride, int numIndex, IndexBuffer::EnIndexType indexType)
{
	Release();
	m_topology = topology;
	m_vertexBuffer.Init(numVertex, vertexStride);
	m_indexBuffer.Init(numIndex, indexType);
}

void Primitive::Release()
{
	m_vertexBuffer.Release();
	m_indexBuffer.Release();
}

void Primitive::Draw(RenderContext& rc)
{
	//バッファ設定。
	rc.SetVertexBuffer(m_vertexBuffer);
	rc.SetIndexBuffer(m_indexBuffer);
	//トポロジー設定。
	rc.SetPrimitiveTopology(m_topology);
	//描画。
	rc.DrawIndexed(m_indexBuffer.GetCount());
}

void Primitive::Draw(RenderContext& rc, int numVertex)
{
	rc.SetVertexBuffer(m_vertexBuffer);
	rc.SetIndexBuffer(m_indexBuffer);
	rc.SetPrimitiveTopology(m_topology);
	rc.Draw(numVertex, 0);
}

