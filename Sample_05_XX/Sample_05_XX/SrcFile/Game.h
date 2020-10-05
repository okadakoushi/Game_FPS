#pragma once
class Game
{
public:
	Game();
	~Game();

	void Update();
private:
	Model m_animSampleModel;					//�A�j���[�V����������T���v�����f���B
	Vector3 m_pos = Vector3::Zero;				//���W�B
	Vector3 m_scale = Vector3::One;				//�g�嗦
	Quaternion m_rot = Quaternion::Identity;	//��]�B
	Animation m_animation;						//�A�j���[�V�����B
	bool m_inited = false;
};

