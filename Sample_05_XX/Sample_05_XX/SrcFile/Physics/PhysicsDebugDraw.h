#pragma once

#include "SrcFile/Primitive.h"
#include "PipelineState.h"
#include "ConstantBuffer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RootSignature.h"
#include "DescriptorHeap.h"
#include "SrcFile/Debug/LineDraw.h"


/// <summary>
/// こいつ使い勝手よくするために、デバッグドロー統括オブジェにしよう。
/// Physicsだけの描画じゃない。
/// </summary>
class PhysicsDebugDraw : public btIDebugDraw
{
private:
	int m_debugMode = btIDebugDraw::DBG_NoDebug;	//デバッグモード。
	LineDraw m_lineDraw;
	
	int m_numLine;		//ライン数。
public:
	void Init();

	void BeginDraw(RenderContext& rc)
	{
		m_lineDraw.BeginDraw(rc);
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

