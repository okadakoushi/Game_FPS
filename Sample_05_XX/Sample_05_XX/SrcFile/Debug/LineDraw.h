#pragma once

/// <summary>
/// ���C���`��B
/// </summary>
/// <code>
/// Init()
/// BeginDraw()->Draw()
/// </code>
class LineDraw
{
public:
	/// <summary>
	/// �������B
	/// </summary>
	void Init();
	/// <summary>
	/// �`�揀���B
	/// </summary>
	void BeginDraw(RenderContext& rc);
	/// <summary>
	/// �`�悷�郉�C����ǉ��B
	/// <para>���[�U�[���Ăяo������̂͂���������OK�B</para>
	/// </summary>
	/// <param name="from">from�x�N�g��</param>
	/// <param name="to">to�x�N�g���B</param>
	void AddLine(const Vector3& from, const Vector3& to, const Vector4& color = {1.0f, 0.0f, 0.0f, 1.0f});
	/// <summary>
	/// �`��B
	/// </summary>
	void Draw();
private:
	struct SConstantBuffer {
		Matrix mView;
		Matrix mProj;
		Vector4 mColor;
	};
	ConstantBuffer m_cb;							//�萔�o�b�t�@�B
	SConstantBuffer s_cb;
	static const int VERTEX_MAX = 10000000;			//�ő咸�_�B
	std::array<Vector4, VERTEX_MAX> m_vertexs;		//���_�o�b�t�@�[�ƃC���f�b�N�X�B
	RenderContext* m_rc;							//�����R���B
	PipelineState m_pipeline;						//�p�C�v���C���X�e�[�g�B
	VertexBuffer m_vertexBuffer;
	DescriptorHeap m_heap;
	RootSignature m_rootSignature;
	int m_numLine = 0;
};

