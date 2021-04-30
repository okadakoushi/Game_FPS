#pragma once


/// <summary>
/// ���_�o�b�t�@
/// </summary>
class VertexBuffer {
		
public:
	/// <summary>
	/// �f�X�g���N�^�B
	/// </summary>
	~VertexBuffer();
	/// <summary>
	/// �������B
	/// </summary>
	/// <param name="size">���_�o�b�t�@�̃T�C�Y</param>
	/// <param name="stride">���_�o�b�t�@�̃X�g���C�h�B��̒��_�̃T�C�Y</param>
	void Init(int size, int stride);
	/// <summary>
	/// ���_�f�[�^�𒸓_�o�b�t�@�ɃR�s�[�B
	/// </summary>
	/// <param name="srcVertices">�R�s�[���̒��_�f�[�^�B</param>
	void Copy(void* srcVertices) ;
	/// <summary>
	/// ���_�o�b�t�@�r���[���擾�B
	/// </summary>
	/// <returns>�r���[</returns>
	D3D12_VERTEX_BUFFER_VIEW& GetView()
	{
		return m_vertexBufferView;
	}

	ID3D12Resource* GetResource()
	{
		return m_vertexBuffer;
	}
	/// <summary>
	/// ����B
	/// </summary>
	void Release()
	{
		if (m_vertexBuffer != nullptr) {
			m_vertexBuffer->Release();
			m_vertexBuffer = nullptr;
		}
	}
private:
	ID3D12Resource* m_vertexBuffer = nullptr;		//���_�o�b�t�@�B
	D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;	//���_�o�b�t�@�r���[�B
};