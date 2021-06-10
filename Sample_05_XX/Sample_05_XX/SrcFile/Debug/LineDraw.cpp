#include "stdafx.h"
#include "LineDraw.h"

void LineDraw::Init()
{
	//頂点バッファ初期化。
	m_vertexBuffer.Init(static_cast<int>(m_vertexs.size()), sizeof(Vector4));
	m_vertexBuffer.Copy((void*)&m_vertexs[0]);

	//シェーダーロード。
	Shader vs, ps;
	vs.LoadVS(L"Assets/shader/linePrimitive.fx", "VSMain");
	ps.LoadPS(L"Assets/shader/linePrimitive.fx", "PSMain");

	//定数バッファ。
	m_cb.Init(sizeof(SConstantBuffer), &s_cb);

	//ルートシグネチャ初期化。
	m_rootSignature.Init(
		D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP
	);

	//ヒープに定数バッファを設定。
	m_heap.RegistConstantBuffer(0, m_cb);
	m_heap.Commit();

	//パイプラインステート作成。
	{
		// 頂点レイアウトを定義する。
		D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};
		//デスク初期化。
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = { 0 };
		psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
		psoDesc.pRootSignature = m_rootSignature.Get();
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(vs.GetCompiledBlob());
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(ps.GetCompiledBlob());
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		psoDesc.DepthStencilState.DepthEnable = FALSE;
		psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		psoDesc.DepthStencilState.StencilEnable = FALSE;
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		psoDesc.SampleDesc.Count = 1;
		//作成。
		m_pipeline.Init(psoDesc);
	}
}

void LineDraw::BeginDraw(RenderContext& rc)
{
	m_rc = &rc;
	m_numLine = 0;
}

void LineDraw::AddLine(const Vector3& from, const Vector3& to, const Vector4& color)
{
	int baseIndex = m_numLine * 2;
	m_vertexs[baseIndex].Set(from);
	m_vertexs[baseIndex + 1].Set(to);
	s_cb.mColor = color;
	m_numLine++;
}

void LineDraw::Draw()
{
	//更新。
	m_vertexBuffer.Copy((void*)&m_vertexs[0]);
	s_cb.mView = GraphicsEngineObj()->GetCamera3D().GetViewMatrix();
	s_cb.mProj = GraphicsEngineObj()->GetCamera3D().GetProjectionMatrix();
	m_cb.CopyToVRAM(&s_cb);
	//レンコンに設定。
	m_rc->SetPipelineState(m_pipeline);
	m_rc->SetRootSignature(m_rootSignature);
	m_rc->SetVertexBuffer(m_vertexBuffer);
	m_rc->SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	m_rc->SetDescriptorHeap(m_heap);
	m_rc->DrawInstanced(m_numLine * 2);
}
