#pragma once

#include "TkFile/TkmFile.h"

class IShaderResource;
class MeshParts;
class SMesh;
class Shader;

/// <summary>
/// FlyWeightパターン。
/// </summary>
class ModelDataManager
{
public:
	~ModelDataManager();
	/// <summary>
	/// tkmロード。
	/// </summary>
	/// <param name="filePath">ファイルパス。</param>
	/// <returns>tkm。</returns>
	TkmFile* LoadTkm(const char* filePath);
	/// <summary>
	/// メッシュをロード。
	/// </summary>
	/// <param name="tkmMesh">tkmメッシュ。</param>
	/// <param name="meshNo">メッシュ番号。</param>
	/// <param name="fxPath">頂点シェーダーパス。</param>
	/// <returns>メッシュ。</returns>
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

