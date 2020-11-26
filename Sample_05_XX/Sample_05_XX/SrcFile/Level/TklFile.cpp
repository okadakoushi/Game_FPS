#include "stdafx.h"
#include "TklFile.h"

void TklFile::LoadImplement(const char* filePath)
{
	auto fp = fopen(filePath, "rb");
	if (fp == nullptr) {
		return;
	}
	//�o�[�W�����ǂݍ��݁B
	fread(&m_tklVersion, sizeof(m_tklVersion), 1, fp);
	//�I�u�W�F�N�g�̐����擾�B
	fread(&m_numObject, sizeof(m_numObject), 1, fp);
	m_objects.resize(m_numObject);
	for (int i = 0; i < m_numObject; i++) {
		auto& obj = m_objects.at(i);
		size_t nameCount = 0;
		//�I�u�W�F�N�g���擾�B
		fread(&nameCount, 1, 1, fp);
		obj.name = std::make_unique<char[]>(nameCount + 1);
		fread(obj.name.get(), nameCount + 1, 1, fp);
		//�e��ID���擾�B
		fread(&obj.parentNo, sizeof(obj.parentNo), 1, fp);
		//�o�C���h�|�[�Y�擾�B
		fread(obj.bindPose, sizeof(obj.bindPose), 1, fp);
		//�t�o�C���h�|�[�Y�擾�B
		fread(obj.invBindPose, sizeof(obj.invBindPose), 1, fp);
		//�I�u�W�F�N�g�ԍ��B
		obj.no = i;
		
		//vec3�̃p�����[�^�[�B
		int numVec3Data;
		fread(&numVec3Data, sizeof(numVec3Data), 1, fp);
		//vec3�p�����[�^�[�B
		for (int i = 0; i < numVec3Data; i++) {
			float x, y, z;
			fread(&x, sizeof(x), 1, fp);
			fread(&y, sizeof(y), 1, fp);
			fread(&z, sizeof(z), 1, fp);
			obj.vec3Datas.push_back(Vector3(x, y, z));
		}
	}

	fclose(fp);
	//�ǂݍ��ݏI���B
	SetLoadedMark();
}
