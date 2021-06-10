#include "stdafx.h"
#include "MeshParts.h"
#include "Skeleton.h"
#include "Material.h"

MeshParts::~MeshParts()
{
	//for (auto& mesh : m_meshs) {
	//	//�C���f�b�N�X�o�b�t�@���폜�B
	//	for (auto& ib : mesh->m_indexBufferArray) {
	//		delete ib;
	//	}
	//	//�}�e���A�����폜�B
	//	for (auto& mat : mesh->m_materials) {
	//		delete mat;
	//	}
	//	//���b�V�����폜�B
	//	delete mesh;
	//}
}

void MeshParts::InitFromTkmFile(
	const TkmFile& tkmFile, 
	void* expandData,
	int expandDataSize,
	IShaderResource* expandShaderResourceView,
	const wchar_t* filePath, 
	const char* VSEntry, 
	const char* PSEntry
)
{
	m_meshs.resize(tkmFile.GetNumMesh());
	int meshNo = 0;
	tkmFile.QueryMeshParts([&](const TkmFile::SMesh& mesh) {
		//tkm�t�@�C���̃��b�V����񂩂烁�b�V�����쐬����B
		CreateMeshFromTkmMesh(mesh, meshNo, filePath, VSEntry, PSEntry);


		meshNo++;
	});
	//���ʒ萔�o�b�t�@�̍쐬�B
	m_commonConstantBuffer.Init(sizeof(SConstantBuffer), nullptr);
	//���[�U�[�g���p�̒萔�o�b�t�@���쐬�B
	if (expandData) {
		m_expandConstantBuffer.Init(expandDataSize, nullptr);
		m_expandData = expandData;
	}
	m_expandShaderResourceView = expandShaderResourceView;
	//�f�B�X�N���v�^�q�[�v���쐬�B
	//CreateDescriptorHeaps();
}

void MeshParts::CreateDescriptorHeaps()
{
	//�f�B�X�N���v�^�q�[�v�̓}�e���A���̐��������쐬�����B
	int numDescriptorHeap = 0;
	for (auto& mesh : m_meshs) {
		for (int matNo = 0; matNo < mesh->m_materials.size(); matNo++) {
			numDescriptorHeap++;
		}
	}
	//�f�B�X�N���v�^�q�[�v���h�J�b�Ɗm�ہB
	m_descriptorHeap.resize(numDescriptorHeap);
	//�f�B�X�N���v�^�q�[�v���\�z���Ă����B
	int descriptorHeapNo = 0;
	for (auto& mesh : m_meshs) {
		for (int matNo = 0; matNo < mesh->m_materials.size(); matNo++) {
			auto& descriptorHeap = m_descriptorHeap[descriptorHeapNo];
			//�f�B�X�N���v�^�q�[�v�Ƀf�B�X�N���v�^��o�^���Ă����B
			descriptorHeap.RegistShaderResource(0, mesh->m_materials[matNo]->GetAlbedoMap());		//�A���x�h�}�b�v�B
			descriptorHeap.RegistShaderResource(1, mesh->m_materials[matNo]->GetNormalMap());		//�@���}�b�v�B
			descriptorHeap.RegistShaderResource(2, mesh->m_materials[matNo]->GetSpecularMap());		//�X�y�L�����}�b�v�B
			//�V���h�E�}�b�v�o�^�B
			auto* shadowMap = EngineObj().GetGraphicsEngine()->GetShadowMap();
			descriptorHeap.RegistShaderResource(15, shadowMap->GetRenderTarget(0).GetRenderTargetTexture());
			descriptorHeap.RegistShaderResource(16, shadowMap->GetRenderTarget(1).GetRenderTargetTexture());
			descriptorHeap.RegistShaderResource(17, shadowMap->GetRenderTarget(2).GetRenderTargetTexture());
			descriptorHeap.RegistShaderResource(20, m_boneMatricesStructureBuffer);					//�{�[��
			if (m_expandShaderResourceView){
				descriptorHeap.RegistShaderResource(EXPAND_SRV_REG__START_NO, *m_expandShaderResourceView);
			}

			descriptorHeap.RegistConstantBuffer(0, m_commonConstantBuffer);	//WVP
			if (m_expandConstantBuffer.IsValid()) {
				descriptorHeap.RegistConstantBuffer(1, m_expandConstantBuffer); //Light
			}
			descriptorHeap.RegistConstantBuffer(2, EngineObj().GetGraphicsEngine()->GetShadowMap()->GetShadowCB());	//Shadow
			descriptorHeap.RegistConstantBuffer(3, mesh->m_materials[matNo]->GetConstantBuffer());					//matParam
			//�f�B�X�N���v�^�q�[�v�ւ̓o�^���m�肳����B
			descriptorHeap.Commit();
			descriptorHeapNo++;
		}
	}
	m_isCreateDescriptorHeap = true;
}
void MeshParts::CreateMeshFromTkmMesh(
	const TkmFile::SMesh& tkmMesh, 
	int meshNo,
	const wchar_t* filePath, 
	const char* VSEntry, 
	const char* PSEntry
)
{
	//���b�V���擾�B
	auto* mesh = EngineObj().GetModelDataManager().LoadMesh(tkmMesh, meshNo, filePath, VSEntry, PSEntry);

	m_meshs[meshNo] = mesh;
	
}

void MeshParts::BindSkeleton(Skeleton& skeleton)
{
	m_skeleton = &skeleton;
	//�\�����o�b�t�@���쐬����B
	m_boneMatricesStructureBuffer.Init(
		sizeof(Matrix),
		m_skeleton->GetNumBones(),
		m_skeleton->GetBoneMatricesTopAddress()
	);
}
void MeshParts::Draw(
	RenderContext& rc,
	const Matrix& mWorld,
	const Matrix& mView,
	const Matrix& mProj,
	const bool isShadowReciever,
	const int renderMode
)
{
#if 1
	if (m_isCreateDescriptorHeap == false) {
		CreateDescriptorHeaps();
	}

	//���b�V�����ƂɃh���[
	//�v���~�e�B�u�̃g�|���W�[�̓g���C�A���O�����X�g�̂݁B
	rc.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//�萔�o�b�t�@���X�V����B todo : skinModel��cb���ڏ��B
	SConstantBuffer cb;
	cb.mWorld = mWorld;
	cb.mView = mView;
	cb.mProj = mProj;
	cb.mulColor = m_mulColor;
	cb.isShadowReciever = isShadowReciever;

	m_commonConstantBuffer.CopyToVRAM(&cb);

	if (m_expandData) {
		m_expandConstantBuffer.CopyToVRAM(m_expandData);
	}

	if (m_boneMatricesStructureBuffer.IsInited()) {
		//�{�[���s����X�V����B
		m_boneMatricesStructureBuffer.Update(m_skeleton->GetBoneMatricesTopAddress());
	}
	int descriptorHeapNo = 0;
	for (auto& mesh : m_meshs) {
		//���_�o�b�t�@��ݒ�B
		rc.SetVertexBuffer(mesh->m_vertexBuffer);
		//�}�e���A�����ƂɃh���[�B
		for (int matNo = 0; matNo < mesh->m_materials.size(); matNo++) {
			//���̃}�e���A�����\���Ă��郁�b�V���̕`��J�n�B
			mesh->m_materials[matNo]->BeginRender(rc, renderMode);
			//�f�B�X�N���v�^�q�[�v��o�^�B
			rc.SetDescriptorHeap(m_descriptorHeap.at(descriptorHeapNo));
			//�C���f�b�N�X�o�b�t�@��ݒ�B
			auto& ib = mesh->m_indexBufferArray[matNo];
			rc.SetIndexBuffer(*ib);

			//�h���[�B
			rc.DrawIndexed(ib->GetCount());
			descriptorHeapNo++;
		}
	}
#endif
}