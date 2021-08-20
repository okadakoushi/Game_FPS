#include "stdafx.h"
#include "Material.h"

//���[�g�V�O�l�`���ƃp�C�v���C���X�e�[�g����̓J���J���J��
enum {
	enDescriptorHeap_CB,
	enDescriptorHeap_SRV,
	enNumDescriptorHeap
};
	
void Material::InitTexture(const TkmFile::SMaterial& tkmMat)
{
	if (tkmMat.albedoMap != nullptr) {
		m_albedoMap.InitFromMemory(tkmMat.albedoMap.get(), tkmMat.albedoMapSize);
	}
	if (tkmMat.normalMap != nullptr) {
		m_normalMap.InitFromMemory(tkmMat.normalMap.get(), tkmMat.normalMapSize);
	}
	if (tkmMat.specularMap != nullptr) {
		m_specularMap.InitFromMemory(tkmMat.specularMap.get(), tkmMat.specularMapSize);
	}
}
void Material::InitFromTkmMaterila(
	const TkmFile::SMaterial& tkmMat,
	const wchar_t* filePath, 
	const char* VSEntry, 
	const char* PSEntry
)
{
	//�e�N�X�`�������[�h�B
	InitTexture(tkmMat);
	
	//�萔�o�b�t�@���쐬�B
	SMaterialParam matParam;
	matParam.hasNormalMap = m_normalMap.IsValid() ? 1 : 0;
	matParam.hasSpecMap = m_specularMap.IsValid() ? 1 : 0;
	m_constantBuffer.Init(sizeof(SMaterialParam), &matParam);

	//���[�g�V�O�l�`�����������B
	m_rootSignature.Init(
		D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP);

	//�V�F�[�_�[���������B
	InitShaders(filePath, VSEntry, PSEntry);

	//�p�C�v���C���X�e�[�g���������B
	InitPipelineState();

}
void Material::InitPipelineState()
{
	// ���_���C�A�E�g���`����B
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 48, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 56, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 72, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	//�p�C�v���C���X�e�[�g���쐬�B
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = { 0 };
	psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
	psoDesc.pRootSignature = m_rootSignature.Get();
	psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsSkinModel.GetCompiledBlob());
	psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psModel.GetCompiledBlob());
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState.DepthEnable = TRUE;
	psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	psoDesc.DepthStencilState.StencilEnable = FALSE;
	psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	//RTV//
	psoDesc.NumRenderTargets = Gbuffer_Num;
	psoDesc.RTVFormats[GBuffer_albed] = DXGI_FORMAT_R8G8B8A8_UNORM;				//�A���x�h�J���[�o�͗p�B
	psoDesc.RTVFormats[GBuffer_normal] = DXGI_FORMAT_R8G8B8A8_UNORM;			//�@���o�͗p�B	
	psoDesc.RTVFormats[GBuffer_spec] = DXGI_FORMAT_R32_FLOAT;					//�X�y�L�����o�͗p�B
	psoDesc.RTVFormats[GBuffer_worldPos] = DXGI_FORMAT_R32G32B32A32_FLOAT;		//���[���h���W�o��
	psoDesc.RTVFormats[GBuffer_Shadow] = DXGI_FORMAT_R32_FLOAT;					//�V���h�E�o�͗p�B
	psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	psoDesc.SampleDesc.Count = 1;

	m_skinModelPipelineState.Init(psoDesc);

	//�V���h�E�p�B
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R32_FLOAT;		//�A���x�h�J���[�o�͗p�B
	psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsSkinShadowDraw.GetCompiledBlob());
	psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psSkinShadowDraw.GetCompiledBlob());
	m_SkinShadowMapDraw.Init(psoDesc);

	//�m���X�L���V���h�E�p�B
	psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsNonSkinShadowDraw.GetCompiledBlob());
	m_NonSkinShadowMapDraw.Init(psoDesc);
	//�����Ĕ������}�e���A���p�B
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;		//�A���x�h�J���[�o�͗p�B
	psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psModel.GetCompiledBlob());
	psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsSkinModel.GetCompiledBlob());
	psoDesc.BlendState.IndependentBlendEnable = TRUE;
	psoDesc.BlendState.RenderTarget[0].BlendEnable = TRUE;
	psoDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	psoDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	psoDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;

	psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsSkinModel.GetCompiledBlob());
	m_transSkinModelPipelineState.Init(psoDesc);

	psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsNonSkinModel.GetCompiledBlob());

	m_transNonSkinModelPipelineState.Init(psoDesc);


	//D3D12_GRAPHICS_PIPELINE_STATE_DESC NonSkinDesc = { 0 };
	//D3D12_INPUT_ELEMENT_DESC NonSkinElementDesc[] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	////�����܂ŕς���Ƃ��Ȃ��̂Őݒ�����Ă���B
	//NonSkinDesc = psoDesc;
	//NonSkinDesc.InputLayout = {NonSkinElementDesc , _countof(NonSkinElementDesc)};
	//NonSkinDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsNonSkinModel.GetCompiledBlob());
	//m_transNonSkinModelPipelineState.Init(NonSkinDesc);

}
void Material::InitShaders(const wchar_t* filePath, const char* VSEntry, const char* PSEntry)
{
	m_vsSkinModel.LoadVS(filePath, VSEntry);
	m_vsNonSkinModel.LoadVS(filePath, "VSMainNonSkin");
	m_psModel.LoadPS(filePath, PSEntry);
	//�Ƃ肠�����V���h�E�`��͂���fx�ł������Ȃ��̂ŁB
	m_vsSkinShadowDraw.LoadVS(L"Assets/shader/NoAnimModel_LambertSpecularAmbient.fx", "VSMain_ShadowMapSkin");
	m_psSkinShadowDraw.LoadPS(L"Assets/shader/NoAnimModel_LambertSpecularAmbient.fx", "PSMain_ShadowMap");
	m_vsNonSkinShadowDraw.LoadVS(L"Assets/shader/NoAnimModel_LambertSpecularAmbient.fx", "VSMain_ShadowMapNonSkin");
}
void Material::BeginRender(RenderContext& rc, int renderMode)
{
	rc.SetRootSignature(m_rootSignature);
	
	switch (renderMode)
	{
	case enRenderMode_Skin:
		//�X�L������p�C�v���C���B
		rc.SetPipelineState(m_transSkinModelPipelineState);
		break;
	case enRenderMode_NonSkin:
		//�X�L���Ȃ��p�C�v���C���B
		rc.SetPipelineState(m_transNonSkinModelPipelineState);
		break;
	case enRenderMode_DrawShadow:
		//�V���h�E�}�b�v�`��p�C�v���C���B
		rc.SetPipelineState(m_SkinShadowMapDraw);
		break;
	case enRenderMode_NonSkinDrawShadow:
		//�m���X�L���V���h�E�}�b�v�`��B
		rc.SetPipelineState(m_NonSkinShadowMapDraw);
		break;
	}
}
