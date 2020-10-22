#pragma once
class Game : public IGameObject
{
public:
	Game();
	~Game();
	
	bool Start() override;
	void Update() override;
	void PreRender() override;
	void Draw() override;
private:
	Model m_unityChan;							//�A�j���[�V����������T���v�����f���B
	Model m_bg;									//�X�e�[�W�B
	Vector3 m_pos = Vector3::Zero;				//���W�B
	Vector3 m_scale = Vector3::One;				//�g�嗦
	Quaternion m_rot = Quaternion::Identity;	//��]�B
	Animation m_animation;						//�A�j���[�V�����B
	int initState = 0;							//�������X�e�b�v�B
	bool m_inited = false;						//�������I������H�B
};

