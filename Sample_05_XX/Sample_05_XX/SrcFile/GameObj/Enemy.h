#pragma once
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

	void ForwardRender() override;
private:
	SkinModelRender* m_modelRender;			//���f�������_�[�B
	Vector3 m_pos = g_vec3Zero;				//�ʒu�B
	Quaternion m_rot = g_quatIdentity;		//��]�B
	Vector3 m_targetPos = {0, 0, -800};		//�ړI�n�_�B
	const float m_spped = 1.0f;				//�X�s�[�h�B
	AStar m_astar;							//�o�H�T���B
	NaviMesh::Cell* m_nodeCell = nullptr;	//�m�[�h�Z���B
	bool m_dirty = false;					//�X�V�K�v�H
	std::vector<NaviMesh::Cell*> m_nodeList;		//�m�[�h���X�g�B
	float m_kakudo = 0.0f;
};

