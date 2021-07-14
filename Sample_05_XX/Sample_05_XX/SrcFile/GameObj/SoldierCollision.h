#pragma once

/// <summary>
/// ���m�p�R���W�����N���X�B
/// �~�N�T���ŏo�͂��Ă����A�j���[�V�����Ől�^�Ȃ炠����x�̓J�o�[�ł��邩���B
/// todo:�~�N�T���ŏo�͂��Ă����A�j���[�V�����Ől�`�Ȃ�S���Ή��ł���悤�ɂ���B
/// </summary>
class SoldierCollision
{
public:
	~SoldierCollision();
	/// <summary>
	/// �����I����B
	/// </summary>
	void Release();
	/// <summary>
	/// �������B
	/// </summary>
	/// <param name="render"></param>
	void Init(SkinModelRender* render);
	/// <summary>
	/// �X�V�B
	/// </summary>
	void Update();
	/// <summary>
	/// �{�̃|�C���^��ݒ�B
	/// <para>Init�̑O�ɌĂяo�����ƁB</para>
	/// </summary>
	/// <param name="p"></param>
	void SetPointer(void* p)
	{
		m_pointer = p;
	}
	/// <summary>
	/// �G�l�~�[���ǂ����̃t���O�B
	/// </summary>
	/// <param name="flag"></param>
	void SetEnemyFlag(const bool& flag)
	{
		m_isEnemy = flag;
	}
private:
	bool m_isInited = false;
	//�����蔻��Param�B
	static const unsigned int COLLISION_COUNT = 4;
	PhysicsStaticObject m_collision[COLLISION_COUNT];	//�����蔻��B
	std::array<const wchar_t*, COLLISION_COUNT> m_bonePaths{	//�e�{�[���̃t�@�C���p�X�B
		L"mixamorig:Head",		//���B
		L"mixamorig:Spine",		//���B
		L"mixamorig:RightLeg",	//�E���B
		L"mixamorig:LeftLeg"	//�����B
	};
	Vector3 m_collisionScales[COLLISION_COUNT]{		//�R���W�����T�C�Y�B
		{20.0f, 20.0f, 20.0f},	//���B
		{40.0f, 45.0f, 25.0f},	//���B
		{15.0f, 30.0f, 15.0f},	//�E��.
		{15.0f, 38.0f, 15.0f}	//�����B
	};
	Bone* m_boneList[COLLISION_COUNT]; 					//�{�[���B
	Vector3 m_bonePosition = g_vec3Zero;				//�{�[���̈ʒu�B
	Quaternion m_boneRotation = g_quatIdentity;			//�{�[���̉�]�B
	void* m_pointer = nullptr;
	bool m_isEnemy = true;
};

