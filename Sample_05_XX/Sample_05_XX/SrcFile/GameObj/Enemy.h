#pragma once

class GamePlayer;

class Enemy : public IGameObject
{
public:
	/// <summary>
	/// �������B
	/// </summary>
	/// <returns></returns>
	bool Start() override;
	/// <summary>
	/// �X�V�B
	/// </summary>
	void Update() override;
	/// <summary>
	/// �ړ������B
	/// </summary>
	void Move();
	/// <summary>
	/// �ʒu��ݒ�B
	/// </summary>
	/// <param name="pos"></param>
	void SetPosition(const Vector3& pos)
	{
		m_pos = pos;
	}
	/// <summary>
	/// �^�[�Q�b�g(A*�̃S�[���m�[�h)���w��B
	/// </summary>
	/// <param name="pos"></param>
	void SetTarget(const Vector3& pos)
	{
		m_targetPos = pos;
	}
private:
	/// <summary>
	/// �v���C���[������p���ɑ��݂��Ă��邩�ǂ����B
	/// </summary>
	/// <returns>true,false</returns>
	bool IsFindPlayer();
private:
	enum EnEnemyState
	{
		En_Move,
		En_Hello,
		En_Idle
	};
	EnEnemyState m_currentState = EnEnemyState::En_Move;	//���݂̃X�e�[�g�B
	SkinModelRender* m_modelRender;			//���f�������_�[�B
	Vector3 m_pos = g_vec3Zero;				//�ʒu�B
	Quaternion m_rot = g_quatIdentity;		//��]�B
	Vector3 m_targetPos = {1230, 0, 960};		//�ړI�n�_�B
	Vector3 m_nextTarget = g_vec3Zero;		//���̖ړI�n�B
	const float m_spped = 1.0f;				//�X�s�[�h�B
	AStar m_astar;							//�o�H�T���B
	NaviMesh::Cell* m_nodeCell = nullptr;	//�m�[�h�Z���B
	bool m_dirty = false;					//�X�V�K�v�H
	std::vector<NaviMesh::Cell*> m_nodeList;		//�m�[�h���X�g�B
	float m_kakudo = 0.0f;
	GamePlayer* m_player = nullptr;			//�v���C���[�B
	const float m_helloTime = 15.0f;	//���A�N�[���^�C���B
	float time = m_helloTime;			//�^�C���B
	const float m_FOV = 45.0f;			//�G�l�~�[���̎���p�B
	const float m_vision = 500.0f;		//�G�l�~�[�̃r�W�����B
};

