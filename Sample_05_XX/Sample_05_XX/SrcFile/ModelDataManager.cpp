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
	//tkmの生ポインタ。
	TkmFile* retTkm = nullptr;
	//一時保存。
	m_filePath = filePath;
	//マップに登録済みか調べる。
	auto it = m_tkmMap.find(filePath);
	if (it == m_tkmMap.end()) {
		//未登録だったため作成。
		TkmFile* file = new TkmFile;
		//ロード。
		file->Load(filePath);
		retTkm = file;
		//tkmからメッシュ作成。
		//int meshNo = 0;
		//retTkm->QueryMeshParts([&](const TkmFile::SMesh& mesh) {
		//	//tkmファイルのメッシュ情報からメッシュを作成する。
		//	meshParts.CreateMeshFromTkmMesh(*retTkm, mesh, meshNo, fxPath);
		//	//meshParts.
		//	meshNo++;
		//	});
		//新規モデルなのでマップに追加。
		m_tkmMap.insert({ filePath, std::move(retTkm) });
	}
	else {
		retTkm = it->second;
		int meshNo = 0;
		//登録済み。
		//retTkm->QueryMeshParts([&](const TkmFile::SMesh& mesh) {
		//	//tkmファイルのメッシュ情報からメッシュを作成する。
		//	meshParts.CreateMeshFromTkmMesh(*retTkm, mesh, meshNo, fxPath);
		//	meshNo++;
		//	});
	}

	return retTkm;
}

SMesh* ModelDataManager::LoadMesh(const TkmFile::SMesh& tkmMesh, int meshNo, const wchar_t* fxPath)
{
	//返却値。
	SMesh* retMesh = nullptr;

	string buckFile = m_filePath;
	string numFile = std::to_string(meshNo);

	string file = buckFile + numFile;

	auto it = m_meshMap.find(file);
	if (it == m_meshMap.end()) {
		//頂点バッファを作成。
		int numVertex = (int)tkmMesh.vertexBuffer.size();
		int vertexStride = sizeof(TkmFile::SVertex);
		auto mesh = new SMesh;
		mesh->skinFlags.reserve(tkmMesh.materials.size());
		mesh->m_vertexBuffer.Init(vertexStride * numVertex, vertexStride);
		mesh->m_vertexBuffer.Copy((void*)&tkmMesh.vertexBuffer[0]);

		auto SetSkinFlag = [&](int index) {
			if (tkmMesh.vertexBuffer[index].skinWeights.x > 0.0f) {
				//スキンがある。
				mesh->skinFlags.push_back(1);
			}
			else {
				//スキンなし。
				mesh->skinFlags.push_back(0);
			}
		};
		//インデックスバッファを作成。
		if (!tkmMesh.indexBuffer16Array.empty()) {
			//インデックスのサイズが2byte
			mesh->m_indexBufferArray.reserve(tkmMesh.indexBuffer16Array.size());
			for (auto& tkIb : tkmMesh.indexBuffer16Array) {
				auto ib = new IndexBuffer;
				ib->Init(static_cast<int>(tkIb.indices.size()) * 2, 2);
				ib->Copy((void*)&tkIb.indices.at(0));
				//スキンがあるかどうかを設定する。
				SetSkinFlag(tkIb.indices[0]);

				mesh->m_indexBufferArray.push_back(ib);
			}
		}
		else {
			//インデックスのサイズが4byte
			mesh->m_indexBufferArray.reserve(tkmMesh.indexBuffer32Array.size());
			for (auto& tkIb : tkmMesh.indexBuffer32Array) {
				auto ib = new IndexBuffer;
				ib->Init(static_cast<int>(tkIb.indices.size()) * 4, 4);
				ib->Copy((void*)&tkIb.indices.at(0));

				//スキンがあるかどうかを設定する。
				SetSkinFlag(tkIb.indices[0]);

				mesh->m_indexBufferArray.push_back(ib);
			}
		}
		//マテリアルを作成。
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
		//済み
		retMesh = it->second;
	}
	return retMesh;
}



