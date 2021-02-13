#include "stdafx.h"
#include "NaviMesh.h"

void NaviMesh::Load(char* filePath, bool isBase)
{
	FILE* fp = fopen(filePath, "rb");
	m_isBase = isBase;

	if (fp == nullptr) {
		MessageBoxA(nullptr, "ナビゲーションメッシュのファイルパスが間違っています。", "NaviMesh::Error", MB_OK);
		std::abort();
	}

	if (fp) {
		//セルの数を読み込む
		fread(&m_numCell, sizeof(m_numCell), 1, fp);
		//セルの数分だけメモリをドカッと確保。
		m_cellBin = new CellBin[m_numCell];
		//配列の先頭アドレスをコピー。
		std::intptr_t topAddress = (std::intptr_t)m_cellBin;
		//セルを読み込んでいく。
		int vertNo = 0;
		for (int i = 0; i < m_numCell; i++) {
			//まずは頂点。
			fread(&m_cellBin[i].pos[0], sizeof(m_cellBin[i].pos[0]), 1, fp);
			fread(&m_cellBin[i].pos[1], sizeof(m_cellBin[i].pos[1]), 1, fp);
			fread(&m_cellBin[i].pos[2], sizeof(m_cellBin[i].pos[2]), 1, fp);
			//パディングをロード。
			fread(&m_cellBin[i].pad, sizeof(m_cellBin[i].pad), 1, fp);

			//リストに積み積み。
			m_cellPos.push_back(m_cellBin[i].pos[0]);
			m_cellPos.push_back(m_cellBin[i].pos[1]);
			m_cellPos.push_back(m_cellBin[i].pos[2]);				//ベース描画なら別に隣接せる構成しなくていい。
				//続いて隣接セル。ポインタだから64bit。
			fread(&m_cellBin[i].linkCell64[0], sizeof(m_cellBin[i].linkCell64[0]), 1, fp);
			fread(&m_cellBin[i].linkCell64[1], sizeof(m_cellBin[i].linkCell64[1]), 1, fp);
			fread(&m_cellBin[i].linkCell64[2], sizeof(m_cellBin[i].linkCell64[2]), 1, fp);


			//隣接セルの位置情報。
			//fread(, sizeof(Vector3), 1, fp);
			//ここでインデックスバッファー構成しとく。
			m_indexs.push_back(vertNo);
			m_indexs.push_back(vertNo + 1);
			m_indexs.push_back(vertNo + 2);
			vertNo += 3;
			//ファイル内相対アドレスを実アドレス化
			for (int linkCellNo = 0; linkCellNo < 3; linkCellNo++) {
				if (m_cellBin[i].linkCell64[linkCellNo] != -1) {
					m_cellBin[i].linkCell64[linkCellNo] += topAddress;
				}
				else {
					m_cellBin[i].linkCell64[linkCellNo] = 0;
				}
			}

		}
		fclose(fp);
	}

	//ここで全セルを調べて、隣接ライン情報を構築する。
	for (int i = 0; i < m_numCell; i++) {
		Vector3 CellCenter;
		for (int posC = 0; posC < 3; posC++) {
			CellCenter += m_cellBin[i].pos[posC];
		}
		//セルの中心。
		CellCenter /= 3.0f;


		//次は隣接セルの真ん中も計算する。todo
		for (int linkCell = 0; linkCell < 3; linkCell++) {
			//リンクセル分回す。
			if (m_cellBin[i].linkCell64[linkCell] != 0) {
				//隣接セルがある場合。
				Vector3 linkCellCenterPos;
				for (int posC = 0; posC < 3; posC++) {
					linkCellCenterPos += m_cellBin[i].linkCell[linkCell]->pos[posC];
				}
				linkCellCenterPos /= 3.0f;
				//ラインを格納する。
				Line line;
				line.start = CellCenter;
				line.end = linkCellCenterPos;
				m_linkCellLine.push_back(line);
			}
		}
	}
	
}

void NaviMesh::InitRender(bool isWire)
{
	//頂点バッファー初期化。
	m_vertexBuffer.Init(sizeof(m_cellPos[0]) * m_cellPos.size(), sizeof(m_cellPos[0]));
	m_vertexBuffer.Copy(&m_cellPos[0]);
	//インデックスバッファー初期化。
	m_indexBuffer.Init(sizeof(m_indexs[0]) * m_indexs.size(), sizeof(m_indexs[0]));
	m_indexBuffer.Copy(&m_indexs[0]);
	//セルから、隣接セルに向かう線分の頂点バッファーとインデックスバッファーの形成。
	//頂点バッファを形成していく。
	if (!m_isBase) {
		//ベースは線分を構築しない。
		m_lineVertexBuffer.Init(sizeof(Line) * m_linkCellLine.size(), sizeof(Line::start));
		m_lineVertexBuffer.Copy(&m_linkCellLine[0]);
		//次にインデックスバッファー。
		//インデックスを形成。
		for (int indexs = 0; indexs < m_linkCellLine.size() * 2; indexs++) {
			m_lineIndexs.push_back(indexs);
		}
		//バッファー作成。
		m_lineIndexBuffer.Init(sizeof(m_lineIndexs[0]) * m_lineIndexs.size(), sizeof(m_lineIndexs[0]));
		m_lineIndexBuffer.Copy(&m_lineIndexs[0]);
	}
	
	//////背景用頂点バッファー初期化。背景は四角形なので４頂点しかいらない。
	//Vector3 v[6]{
	//	//左下の頂点。
	//	{m_vMin.x, m_vMin.y, 0.0f},
	//	//左上の頂点。
	//	{m_vMin.x, m_vMax.y, 0.0f},
	//	//右上の頂点。
	//	{m_vMax.x, m_vMax.y, 0.0f},
	//	//右上の頂点。
	//	{m_vMax.x, m_vMax.y, 0.0f},
	//	//右下の頂点。
	//	{m_vMax.x, m_vMin.y, 0.0f},
	//	//左下の頂点。
	//	{m_vMin.x, m_vMin.y, 0.0f},
	//};
	////初期化。
	//m_vertexBuck.Init(sizeof(v), sizeof(v[0]));
	//m_vertexBuck.Copy(&v[0]);
	////背景用インデックス。
	//int index[6]{
	//	0,1,2,3,4,5
	//};
	//m_indexBuck.Init(sizeof(index), sizeof(index[0]));
	//m_indexBuck.Copy(&index[0]);
	
	//定数バッファ初期化。
	m_CB.Init(sizeof(SConstantBuffer), nullptr);

	//ルートシグネチャを初期化。
	m_rootSignature.Init(
		D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP
	);

	Shader vs, ps, psWire, psLine;
	vs.LoadVS(L"Assets/shader/sample.fx", "VSMain");
	ps.LoadPS(L"Assets/shader/sample.fx", "PSMain");
	psWire.LoadPS(L"Assets/shader/sample.fx", "PSMainWire");
	psLine.LoadPS(L"Assets/shader/sample.fx", "PSMainLine");

	//ディスクリプタヒープ設定。
	m_heap.RegistConstantBuffer(0, m_CB);
	m_heap.Commit();

	//パイプラインステートを作成。
	InitPipelineState(m_pipelineState, m_rootSignature, vs, ps, isWire, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	//背景用のパイプラインステート作成。
	InitPipelineState(m_pipelineStateBuck, m_rootSignature, vs, psWire, true, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	//線分描画用のパイプラインステート作成。
	InitPipelineState(m_lineDrawPipelineState, m_rootSignature, vs, psLine, isWire, D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE);

	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	m_eye = GraphicsEngineObj()->GetCamera3D().GetPosition();
}

void NaviMesh::BeginRender()
{
}

void NaviMesh::Render(const Vector4& color)
{
	//まずはカメラの行列を送る。
	SConstantBuffer cb;
	cb.mView = GraphicsEngineObj()->GetCamera3D().GetViewMatrix();
	cb.mProj = GraphicsEngineObj()->GetCamera3D().GetProjectionMatrix();
	cb.mColor = color;
	m_CB.CopyToVRAM(&cb);
	
	//描画。
	GraphicsEngineObj()->GetRenderContext().SetRootSignature(m_rootSignature);
	GraphicsEngineObj()->GetRenderContext().SetPipelineState(m_pipelineState);
	GraphicsEngineObj()->GetRenderContext().SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	GraphicsEngineObj()->GetRenderContext().SetDescriptorHeap(m_heap);
	GraphicsEngineObj()->GetRenderContext().SetVertexBuffer(m_vertexBuffer);
	GraphicsEngineObj()->GetRenderContext().SetIndexBuffer(m_indexBuffer);
	GraphicsEngineObj()->GetRenderContext().DrawIndexed(m_indexs.size());

	//パラメーターをパイプライン描画ように変更。
	GraphicsEngineObj()->GetRenderContext().SetPipelineState(m_pipelineStateBuck);
	GraphicsEngineObj()->GetRenderContext().DrawIndexed(m_indexs.size());

	//パラメーターを線分用描画に変更して、描画。
	GraphicsEngineObj()->GetRenderContext().SetPipelineState(m_lineDrawPipelineState);
	GraphicsEngineObj()->GetRenderContext().SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	GraphicsEngineObj()->GetRenderContext().SetVertexBuffer(m_lineVertexBuffer);
	GraphicsEngineObj()->GetRenderContext().SetIndexBuffer(m_lineIndexBuffer);
	GraphicsEngineObj()->GetRenderContext().DrawIndexed(m_lineIndexs.size());
}

void NaviMesh::EndRender()
{
}

void NaviMesh::InitPipelineState(PipelineState& pipelineState, RootSignature& rs, Shader& vs, Shader& ps, bool isWire, D3D12_PRIMITIVE_TOPOLOGY_TYPE topology)
{

	// 頂点レイアウトを定義する。
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	//パイプラインステートを作成。
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
	psoDesc.PrimitiveTopologyType = topology;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	psoDesc.SampleDesc.Count = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	pipelineState.Init(psoDesc);
}
