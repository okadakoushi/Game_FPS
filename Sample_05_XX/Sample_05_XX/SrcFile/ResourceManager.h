/// <summary>
/// リソースの管理者
/// </summary>

#pragma once
#include <map>

template<class T>
class TResourceManager : Noncopyable {
public:
	TResourceManager()
	{
	}
	~TResourceManager()
	{
		Release();
	}
	/// <summary>
	/// リソースマップに登録
	/// </summary>
	/// <param name="nameKey">ハッシュ番号</param>
	/// <param name="resObj">登録するオブジェクト</param>
	void RegistResource(int nameKey, T* resObj)
	{
		//リソースマップに登録
		m_resourceMap.insert(std::pair<int, T*>(nameKey, resObj));
	}
	/// <summary>
	/// リソース取得。
	/// </summary>
	/// <param name="nameKey">ハッシュ番号</param>
	/// <returns></returns>
	T* GetResource(int nameKey) const
	{
		//イテレーター
		//検索
		auto it = m_resourceMap.find(nameKey);
		if (it != m_resourceMap.end()) {
			//見つかった。
			//pairで積んだ2番目(登録オブジェクト)を返却
			return it->second;
		}
		return nullptr;
	}
	/// <summary>
	/// 解放
	/// </summary>
	void Release()
	{
		for (auto& it : m_resourceMap) {
			//全部解放
			it.second->Get()->Release();
		}
	}
private:
	std::map<int, T*>	m_resourceMap;	//リソースマップ
};