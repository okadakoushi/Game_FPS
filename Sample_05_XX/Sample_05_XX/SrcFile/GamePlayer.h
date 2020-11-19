#pragma once
class GamePlayer : public IGameObject
{
public:
	~GamePlayer();
	/// <summary>
	/// �X�^�[�g�B
	/// </summary>
	/// <returns></returns>
	bool Start() override;
	/// <summary>
	/// �X�V�B
	/// </summary>
	void Update() override;
private:
	SkinModelRender* m_unityChan = nullptr;		//���j�e�B�[�����B
	Vector3 m_pos = g_vec3Zero;					//�ʒu�B
};

