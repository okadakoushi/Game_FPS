#pragma once
class Game : public IGameObject
{
public:
	Game();
	~Game();
	
	bool Start() override;
	void Update() override;
	void ForwardRender() override;
private:
	SkinModelRender* m_unityChan = nullptr;		//���j�e�B�[�����B
	SkinModelRender* m_bg = nullptr;			//�X�e�[�W�B
	Vector3 m_pos = Vector3::Zero;				//���W�B
	Vector3 m_scale = Vector3::One;				//�g�嗦
	Quaternion m_rot = Quaternion::Identity;	//��]�B
	Animation m_animation;						//�A�j���[�V�����B
	int initState = 0;							//�������X�e�b�v�B
	bool m_inited = false;						//�������I������H�B
};

