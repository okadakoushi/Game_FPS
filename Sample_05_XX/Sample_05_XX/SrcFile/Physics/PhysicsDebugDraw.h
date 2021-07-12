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
/// �����g������悭���邽�߂ɁA�f�o�b�O�h���[�����I�u�W�F�ɂ��悤�B
/// Physics�����̕`�悶��Ȃ��B
/// </summary>
class PhysicsDebugDraw : public btIDebugDraw
{
private:
	int m_debugMode = btIDebugDraw::DBG_NoDebug;	//�f�o�b�O���[�h�B
	LineDraw m_lineDraw;
	
	int m_numLine;		//���C�����B
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
	/// �f�o�b�O���[�h��ݒ�B
	/// </summary>
	/// <param name="debugMode">���[�h�B</param>
	void setDebugMode(int debugMode) override
	{
		m_debugMode = debugMode;
	}
	/// <summary>
	/// �f�o�b�O���[�h�擾�B
	/// </summary>
	/// <returns>�f�o�b�O���[�h�B</returns>
	int getDebugMode() const override
	{
		return m_debugMode;
	}
};

