#pragma once

#include "SrcFile/Primitive.h"
#include "PipelineState.h"
#include "ConstantBuffer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RootSignature.h"
#include "DescriptorHeap.h"

class PhysicsDebugDraw : public btIDebugDraw
{
private:
	struct SConstantBuffer {
		Matrix mView;
		Matrix mProj;
	};
	int m_debugMode = btIDebugDraw::DBG_NoDebug;	//デバッグモード。
	ConstantBuffer m_cb;							//定数バッファ。
	SConstantBuffer s_cb;
	static const int VERTEX_MAX = 10000000;			//最大頂点。
	std::array<Vector4, VERTEX_MAX> m_vertexs;		//頂点バッファーとインデックス。
	RenderContext* m_rc;							//レンコン。
	PipelineState m_pipeline;						//パイプラインステート。
	VertexBuffer m_vertexBuffer;
	DescriptorHeap m_heap;
	RootSignature m_rootSignature;
	
	int m_numLine;		//ライン数。
public:
	void Init();

	void BeginDraw(RenderContext& rc)
	{
		m_rc = &rc;
		m_numLine = 0;
	}
	void EndDraw();
	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;
	void drawContactPoint(const btVector3&, const btVector3&, btScalar, int, const btVector3&) override
	{
	}
	void reportErrorWarning(const char*) override
	{
	}
	void draw3dText(const btVector3&, const char*) override
	{
	}
	/// <summary>
	/// デバッグモードを設定。
	/// </summary>
	/// <param name="debugMode">モード。</param>
	void setDebugMode(int debugMode) override
	{
		m_debugMode = debugMode;
	}
	/// <summary>
	/// デバッグモード取得。
	/// </summary>
	/// <returns>デバッグモード。</returns>
	int getDebugMode() const override
	{
		return m_debugMode;
	}
};

