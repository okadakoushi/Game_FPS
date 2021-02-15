#include "stdafx.h"
#include "TkFile/TkmFile.h"
#include "ModelDataManager.h"
#include "Resource/IResource.h"
#include "Material.h"

ModelDataManager::~ModelDataManager()
{
}

TkmFile* ModelDataManager::LoadTkm(const char* filePath)
{
	//tkm�̐��|�C���^�B
	TkmFile* retTkm = nullptr;
	//�ꎞ�ۑ��B
	m_filePath = filePath;
	//�}�b�v�ɓo�^�ς݂����ׂ�B
	auto it = m_tkmMap.find(filePath);
	if (it == m_tkmMap.end()) {
		//���o�^���������ߍ쐬�B
		TkmFile* file = new TkmFile;
		//���[�h�B
		file->Load(filePath);
		retTkm = file;
		//tkm���烁�b�V���쐬�B
		//int meshNo = 0;
		//retTkm->QueryMeshParts([&](const TkmFile::SMesh& mesh) {
		//	//tkm�t�@�C���̃��b�V����񂩂烁�b�V�����쐬����B
		//	meshParts.CreateMeshFromTkmMesh(*retTkm, mesh, meshNo, fxPath);
		//	//meshParts.
		//	meshNo++;
		//	});
		//�V�K���f���Ȃ̂Ń}�b�v�ɒǉ��B
		m_tkmMap.insert({ filePath, std::move(retTkm) });
	}
	else {
		retTkm = it->second;
		int meshNo = 0;
		//�o�^�ς݁B
		//retTkm->QueryMeshParts([&](const TkmFile::SMesh& mesh) {
		//	//tkm�t�@�C���̃��b�V����񂩂烁�b�V�����쐬����B
		//	meshParts.CreateMeshFromTkmMesh(*retTkm, mesh, meshNo, fxPath);
		//	meshNo++;
		//	});
	}

	return retTkm;
}

SMesh* ModelDataManager::LoadMesh(const TkmFile::SMesh& tkmMesh, int meshNo, const wchar_t* fxPath)
{
	//�ԋp�l�B
	SMesh* retMesh = nullptr;

	string buckFile = m_filePath;
	string numFile = std::to_string(meshNo);

	string file = buckFile + numFile;

	auto it = m_meshMap.find(file);
	if (it == m_meshMap.end()) {
		//���_�o�b�t�@���쐬�B
		int numVertex = (int)tkmMesh.vertexBuffer.size();
		int vertexStride = sizeof(TkmFile::SVertex);
		auto mesh = new SMesh;
		mesh->skinFlags.reserve(tkmMesh.materials.size());
		mesh->m_vertexBuffer.Init(vertexStride * numVertex, vertexStride);
		mesh->m_vertexBuffer.Copy((void*)&tkmMesh.vertexBuffer[0]);

		auto SetSkinFlag = [&](int index) {
			if (tkmMesh.vertexBuffer[index].skinWeights.x > 0.0f) {
				//�X�L��������B
				mesh->skinFlags.push_back(1);
			}
			else {
				//�X�L���Ȃ��B
				mesh->skinFlags.push_back(0);
			}
		};
		//�C���f�b�N�X�o�b�t�@���쐬�B
		if (!tkmMesh.indexBuffer16Array.empty()) {
			//�C���f�b�N�X�̃T�C�Y��2byte
			mesh->m_indexBufferArray.reserve(tkmMesh.indexBuffer16Array.size());
			for (auto& tkIb : tkmMesh.indexBuffer16Array) {
				auto ib = new IndexBuffer;
				ib->Init(static_cast<int>(tkIb.indices.size()) * 2, 2);
				ib->Copy((void*)&tkIb.indices.at(0));
				//�X�L�������邩�ǂ�����ݒ肷��B
				SetSkinFlag(tkIb.indices[0]);

				mesh->m_indexBufferArray.push_back(ib);
			}
		}
		else {
			//�C���f�b�N�X�̃T�C�Y��4byte
			mesh->m_indexBufferArray.reserve(tkmMesh.indexBuffer32Array.size());
			for (auto& tkIb : tkmMesh.indexBuffer32Array) {
				auto ib = new IndexBuffer;
				ib->Init(static_cast<int>(tkIb.indices.size()) * 4, 4);
				ib->Copy((void*)&tkIb.indices.at(0));

				//�X�L�������邩�ǂ�����ݒ肷��B
				SetSkinFlag(tkIb.indices[0]);

				mesh->m_indexBufferArray.push_back(ib);
			}
		}
		//�}�e���A�����쐬�B
		mesh->m_materials.reserve(tkmMesh.materials.size());
		for (auto& tkmMat : tkmMesh.materials) {
			auto mat = new Material;
			mat->InitFromTkmMaterila(tkmMat, fxPath);
			mesh->m_materials.push_back(mat);
		}
		retMesh = mesh;
		m_meshMap.insert({ file, std::move(retMesh) });
	}
	else {
		//�ς�
		retMesh = it->second;
	}
	return retMesh;
}



