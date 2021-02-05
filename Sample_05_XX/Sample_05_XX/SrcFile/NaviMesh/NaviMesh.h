#pragma once

/// <summary>
/// �i�r���b�V���B
/// </summary>
class NaviMesh
{
public:
	//�o�C�i���f�[�^�̃Z���̃f�[�^�\���́B
	struct CellBin {
		Vector3 pos[3];
		//���p�́B
		union {
			std::intptr_t linkCell64[3];
			CellBin* linkCell[3];
		};
	};
public:
	/// <summary>
	/// NaviMesh�����[�h�B
	/// </summary>
	void Load(char* filePath);
public:
	//���������NaviMesh���e�X�g�`�悷��ۂɕK�v�ȃR�[�h�̂��߁A
	//�`�悵�Ȃ����[�U�[�͎g�p/�Ăяo�����Ȃ��đ��v�B

	/// <summary>
	/// �`�悷�邽�߂ɕK�v�ȏ������B
	/// </summary>
	void InitRender(bool isFill);
	/// <summary>
	/// �`�揀���B
	/// </summary>
	void BeginRender();
	/// <summary>
	/// NaviMesh�����C���`��B
	/// </summary>
	void LineRender(const Vector4& color);
	/// <summary>
	/// �`��㏈���B
	/// </summary>
	void EndRender();
	/// <summary>
	/// �p�C�v���C���X�e�[�g�������B
	/// </summary>
	/// <param name="pipelineState"></param>
	/// <param name="rs"></param>
	/// <param name="vs"></param>
	/// <param name="ps"></param>
	void InitPipelineState(PipelineState& pipelineState, RootSignature& rs, Shader& vs, Shader& ps, bool isWire);
	/// <summary>
	/// �o�C�i���f�[�^�Z�����擾�B�B
	/// </summary>
	/// <returns>�o�C�i���Z���f�[�^�[�B</returns>
	const CellBin* GetCell() const 
	{
		return m_cellBin;
	}
	/// <summary>
	/// �Z���̐����擾�B
	/// </summary>
	/// <returns>�Z���̐��B</returns>
	const int& GetCellNum() const
	{
		return m_numCell;
	}
private:
	//�萔�o�b�t�@�̍\���̒�`�B
	struct SConstantBuffer {
		//Matrix mWorld;		//���[���h�s��B
		Matrix mView;			//�r���[�s��B
		Matrix mProj;			//�v���W�F�N�V�����s��B
		Vector4 mColor;			//�|���S���܂��͐����̐F�B
	};
	struct SColorCB {
		Vector4 mColor;			//�|���S���܂��͐����̐F�B
	};
	ConstantBuffer m_CB;				//�R���X�^���g�o�b�t�@�[�B
	ConstantBuffer m_CBBuck;			//�R���X�^���g�o�b�t�@�[�B
	DescriptorHeap m_heap;				//�f�B�X�N���v�^�q�[�v�B
	CellBin* m_cellBin;					//�o�C�i���f�[�^�Z���B
	vector<Vector3> m_cellPos;			//�Z���̃��X�g�B
	unsigned int m_numCell = 0;			//�Z���̐��B
	VertexBuffer m_vertexBuffer;		//���_�o�b�t�@�[�B
	IndexBuffer m_indexBuffer;			//�C���f�b�N�X�o�b�t�@�[�B
	VertexBuffer m_vertexBuck;			//�w�i�p���_�o�b�t�@�[�B
	IndexBuffer m_indexBuck;			//�w�i�p�C���f�b�N�X�o�b�t�@�[�B
	vector<int> m_indexs;				//�C���f�b�N�X�o�b�t�@�[�̃��X�g�B
	vector<Vector3*> m_cellVertexPos;	//�Z���̒��_���W�B
	RootSignature m_rootSignature;		//���[�g�V�O�l�`���B
	bool m_isLoad = false;				//���[�h�����H
	int ibCount = 0;					//�C���f�b�N�X�J�E���g�B
	PipelineState m_pipelineState;		//�p�C�v���C���X�e�[�g�B
	PipelineState m_pipelineStateBuck;	//�w�i�p�p�C�v���C���X�e�[�g�B
	Vector3 m_eye;						//���_�B
	Vector3 m_vMax = { -FLT_MAX, -FLT_MAX, -FLT_MAX };	//�ő傾�������_�B
	Vector3 m_vMin = { FLT_MAX,FLT_MAX ,FLT_MAX };		//�ŏ����������_�B
};

