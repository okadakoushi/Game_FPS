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
	int m_debugMode = btIDebugDraw::DBG_DrawWireframe;	//�f�o�b�O���[�h�B
	ConstantBuffer m_cb;							//�萔�o�b�t�@�B
	static const int VERTEX_MAX = 10000000;			//�ő咸�_�B
	//std::array<Vector4, VERTEX_MAX> m_vertexBuffer;	//���_�o�b�t�@�[�B
	RenderContext* m_rc;							//�����R���B
	//Primitive m_primitive;
	PipelineState m_pipeline;						//�p�C�v���C���X�e�[�g�B
	std::vector<int> m_indexs;						//�C���f�b�N�X���X�g�B
	std::vector<Vector3> m_vertexs;					//���_���X�g�B
	VertexBuffer m_vertexBuffer;
	IndexBuffer m_indexBuffer;
	DescriptorHeap m_heap;
	RootSignature m_rootSignature;
	
	int m_numLine;		//���C�����B
public:
	void Init();

	void BeginDraw(RenderContext& rc)
	{
		m_rc = &rc;
		m_numLine = 0;;
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

