#include "stdafx.h"
#include "NaviMesh.h"

void NaviMesh::Load(char* filePath)
{
	FILE* fp = fopen(filePath, "rb");

	if (fp == nullptr) {
		MessageBoxA(nullptr, "�i�r�Q�[�V�������b�V���̃t�@�C���p�X���Ԉ���Ă��܂��B", "NaviMesh::Error", MB_OK);
		std::abort();
	}

	if (fp) {
		//�Z���̐���ǂݍ���
		fread(&m_numCell, sizeof(m_numCell), 1, fp);
		//�Z���̐����������������h�J�b�Ɗm�ہB
		m_cellBin = new CellBin[m_numCell];
		//�z��̐擪�A�h���X���R�s�[�B
		std::intptr_t topAddress = (std::intptr_t)m_cellBin;
		//�Z����ǂݍ���ł����B
		int vertNo= 0;
		for (int i = 0; i < m_numCell; i++) {
			//�܂��͒��_�B
			fread(&m_cellBin[i].pos[0], sizeof(m_cellBin[i].pos[0]), 1, fp);
			fread(&m_cellBin[i].pos[1], sizeof(m_cellBin[i].pos[1]), 1, fp);
			fread(&m_cellBin[i].pos[2], sizeof(m_cellBin[i].pos[2]), 1, fp);
			//���X�g�ɐςݐς݁B
			m_cellPos.push_back(m_cellBin[i].pos[0]);
			m_cellPos.push_back(m_cellBin[i].pos[1]);
			m_cellPos.push_back(m_cellBin[i].pos[2]);
			//�ő�A�ŏ����_�����ׂĂ����B
			m_vMax.Max(m_cellBin[i].pos[0]);
			m_vMax.Max(m_cellBin[i].pos[1]);
			m_vMax.Max(m_cellBin[i].pos[2]);
			m_vMin.Min(m_cellBin[i].pos[0]);
			m_vMin.Min(m_cellBin[i].pos[1]);
			m_vMin.Min(m_cellBin[i].pos[2]);

			//�����ėאڃZ���B�|�C���^������64bit�B
			fread(&m_cellBin[i].linkCell64[0], sizeof(m_cellBin[i].linkCell64[0]), 1, fp);
			fread(&m_cellBin[i].linkCell64[1], sizeof(m_cellBin[i].linkCell64[1]), 1, fp);
			fread(&m_cellBin[i].linkCell64[2], sizeof(m_cellBin[i].linkCell64[2]), 1, fp);

			//�����ŃC���f�b�N�X�o�b�t�@�[�\�����Ƃ��B
			 m_indexs.push_back(vertNo);
			 m_indexs.push_back(vertNo+1);
			 m_indexs.push_back(vertNo+2);
			 vertNo += 3;
			//�t�@�C�������΃A�h���X�����A�h���X��
			for (int linkCellNo = 0; linkCellNo < 3; linkCellNo++) {
				m_cellBin[i].linkCell64[linkCellNo] += topAddress;
			}
			
		}

		fclose(fp);
	}
}

void NaviMesh::InitRender(bool isFill)
{
	//���_�o�b�t�@�[�������B
	m_vertexBuffer.Init(sizeof(m_cellPos[0]) * m_cellPos.size(), sizeof(m_cellPos[0]));
	m_vertexBuffer.Copy(&m_cellPos[0]);
	//�C���f�b�N�X�o�b�t�@�[�������B
	m_indexBuffer.Init(sizeof(m_indexs[0]) * m_indexs.size(), sizeof(m_indexs[0]));
	m_indexBuffer.Copy(&m_indexs[0]);
	
	////�w�i�p���_�o�b�t�@�[�������B�w�i�͎l�p�`�Ȃ̂łS���_��������Ȃ��B
	Vector3 v[6]{
		//�����̒��_�B
		{m_vMin.x, m_vMin.y, 0.0f},
		//����̒��_�B
		{m_vMin.x, m_vMax.y, 0.0f},
		//�E��̒��_�B
		{m_vMax.x, m_vMax.y, 0.0f},
		//�E��̒��_�B
		{m_vMax.x, m_vMax.y, 0.0f},
		//�E���̒��_�B
		{m_vMax.x, m_vMin.y, 0.0f},
		//�����̒��_�B
		{m_vMin.x, m_vMin.y, 0.0f},
	};
	//�������B
	m_vertexBuck.Init(sizeof(v), sizeof(v[0]));
	m_vertexBuck.Copy(&v[0]);
	//�w�i�p�C���f�b�N�X�B
	int index[6]{
		0,1,2,3,4,5
	};
	m_indexBuck.Init(sizeof(index), sizeof(index[0]));
	m_indexBuck.Copy(&index[0]);
	
	//�萔�o�b�t�@�������B
	m_CB.Init(sizeof(SConstantBuffer), nullptr);

	//���[�g�V�O�l�`�����������B
	m_rootSignature.Init(
		D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP
	);

	Shader vs, ps;
	vs.LoadVS(L"Assets/shader/sample.fx", "VSMain");
	ps.LoadPS(L"Assets/shader/sample.fx", "PSMain");

	//�f�B�X�N���v�^�q�[�v�ݒ�B
	m_heap.RegistConstantBuffer(0, m_CB);
	m_heap.Commit();

	//�p�C�v���C���X�e�[�g���쐬�B
	InitPipelineState(m_pipelineState, m_rootSignature, vs, ps, isFill);
	InitPipelineState(m_pipelineStateBuck, m_rootSignature, vs, ps, isFill);

	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	m_eye = GraphicsEngineObj()->GetCamera3D().GetPosition();
}

void NaviMesh::BeginRender()
{
}

void NaviMesh::LineRender(const Vector4& color)
{
	//�܂��̓J�����̍s��𑗂�B
	SConstantBuffer cb;
	cb.mView = GraphicsEngineObj()->GetCamera3D().GetViewMatrix();
	cb.mProj = GraphicsEngineObj()->GetCamera3D().GetProjectionMatrix();
	cb.mColor = color;
	m_CB.CopyToVRAM(&cb);
	
	//�`��B
	GraphicsEngineObj()->GetRenderContext().SetRootSignature(m_rootSignature);
	GraphicsEngineObj()->GetRenderContext().SetPipelineState(m_pipelineState);
	GraphicsEngineObj()->GetRenderContext().SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	GraphicsEngineObj()->GetRenderContext().SetDescriptorHeap(m_heap);
	GraphicsEngineObj()->GetRenderContext().SetVertexBuffer(m_vertexBuffer);
	GraphicsEngineObj()->GetRenderContext().SetIndexBuffer(m_indexBuffer);
	GraphicsEngineObj()->GetRenderContext().DrawIndexed(m_indexs.size());

	////�p�C�v���C���X�e�[�g��؂�ւ��āB
	//cb.mColor = { 0.0, 0.0f, 1.0f, 1.0f };
	//m_CB.CopyToVRAM(&cb);
	//GraphicsEngineObj()->GetRenderContext().SetPipelineState(m_pipelineStateBuck);
	//GraphicsEngineObj()->GetRenderContext().SetRootSignature(m_rootSignature);
	//GraphicsEngineObj()->GetRenderContext().SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//GraphicsEngineObj()->GetRenderContext().SetDescriptorHeap(m_heap);
	//GraphicsEngineObj()->GetRenderContext().SetVertexBuffer(m_vertexBuffer);
	//GraphicsEngineObj()->GetRenderContext().SetIndexBuffer(m_indexBuffer);
	//GraphicsEngineObj()->GetRenderContext().DrawIndexed(m_indexs.size());
}

void NaviMesh::EndRender()
{
}

void NaviMesh::InitPipelineState(PipelineState& pipelineState, RootSignature& rs, Shader& vs, Shader& ps, bool isWire)
{

	// ���_���C�A�E�g���`����B
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	//�p�C�v���C���X�e�[�g���쐬�B
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = { 0 };
	psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
	psoDesc.pRootSignature = rs.Get();
	psoDesc.VS = CD3DX12_SHADER_BYTECODE(vs.GetCompiledBlob());
	psoDesc.PS = CD3DX12_SHADER_BYTECODE(ps.GetCompiledBlob());
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	if(isWire){
		psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	}
	else {
		psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	}

	psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState.DepthEnable = FALSE;
	psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	psoDesc.DepthStencilState.StencilEnable = FALSE;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	psoDesc.SampleDesc.Count = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	pipelineState.Init(psoDesc);
}
