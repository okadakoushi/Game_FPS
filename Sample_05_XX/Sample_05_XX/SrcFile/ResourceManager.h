/// <summary>
/// ���\�[�X�̊Ǘ���
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
	/// ���\�[�X�}�b�v�ɓo�^
	/// </summary>
	/// <param name="nameKey">�n�b�V���ԍ�</param>
	/// <param name="resObj">�o�^����I�u�W�F�N�g</param>
	void RegistResource(int nameKey, T* resObj)
	{
		//���\�[�X�}�b�v�ɓo�^
		m_resourceMap.insert(std::pair<int, T*>(nameKey, resObj));
	}
	/// <summary>
	/// ���\�[�X�擾�B
	/// </summary>
	/// <param name="nameKey">�n�b�V���ԍ�</param>
	/// <returns></returns>
	T* GetResource(int nameKey) const
	{
		//�C�e���[�^�[
		//����
		auto it = m_resourceMap.find(nameKey);
		if (it != m_resourceMap.end()) {
			//���������B
			//pair�Őς�2�Ԗ�(�o�^�I�u�W�F�N�g)��ԋp
			return it->second;
		}
		return nullptr;
	}
	/// <summary>
	/// ���
	/// </summary>
	void Release()
	{
		for (auto& it : m_resourceMap) {
			//�S�����
			it.second->Get()->Release();
		}
	}
private:
	std::map<int, T*>	m_resourceMap;	//���\�[�X�}�b�v
};