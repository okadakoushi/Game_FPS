#pragma once

#include "TkFile/TkmFile.h"

class IShaderResource;
class MeshParts;
class SMesh;
class Shader;

/// <summary>
/// FlyWeight�p�^�[���B
/// </summary>
class ModelDataManager
{
public:
	~ModelDataManager();
	/// <summary>
	/// tkm���[�h�B
	/// </summary>
	/// <param name="filePath">�t�@�C���p�X�B</param>
	/// <returns>tkm�B</returns>
	TkmFile* LoadTkm(const char* filePath);
	/// <summary>
	/// ���b�V�������[�h�B
	/// </summary>
	/// <param name="tkmMesh">tkm���b�V���B</param>
	/// <param name="meshNo">���b�V���ԍ��B</param>
	/// <param name="fxPath">���_�V�F�[�_�[�p�X�B</param>
	/// <returns>���b�V���B</returns>
	SMesh* LoadMesh(
		const TkmFile::SMesh& tkmMesh,
		int meshNo,
		const wchar_t* filePath, 
		const char* VSEntry, 
		const char* PSEntry
	);
private:
	std::map<std::string, TkmFile*> m_tkmMap;
	std::map<std::string, MeshParts*> m_meshaaMap;
	std::map<std::string, SMesh*> m_meshMap;
	const char* m_filePath;
};

