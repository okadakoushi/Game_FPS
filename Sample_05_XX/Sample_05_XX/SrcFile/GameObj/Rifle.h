#pragma once

class GamePlayer;

class Rifle : public IGameObject
{
public:
	//~Rifle();
	enum EnRifleEvent {
		EnRifleEvent_None,
		EnRifleEvent_NoAmo,
		EnRifleEvent_Reloading
	};
	/// <summary>
	/// �X�^�[�g�B
	/// </summary>
	/// <returns></returns>
	bool Start() override;
	/// <summary>
	/// �X�V�B
	/// </summary>
	void Update() override;
	/// <summary>
	/// �{�[�����Q�Ƃ��郌���_�[��ݒ�B
	/// </summary>
	/// <param name="mr"></param>
	void SetRefBoneRender(SkinModelRender* mr)
	{
		m_refBoneRender = mr;
	}
	/// <summary>
	/// �����[�h�^�C�������Z�B
	/// </summary>
	void AddReloadTime()
	{
		m_currentReloadTime += GameTime().GetFrameDeltaTime();
	}
	/// <summary>
	/// ���U�e���ւ炷�B
	/// </summary>
	void ReduseAmo()
	{
		m_currentAmo--;
	}
	/// <summary>
	/// �ʒu���擾�B
	/// </summary>
	/// <returns></returns>
	const Vector3& GetPos() const 
	{
		return m_pos;
	}
	/// <summary>
	/// ��]���擾�B
	/// </summary>
	/// <returns></returns>
	const Quaternion& GetRotation() const 
	{
		return m_rot;
	}
	/// <summary>
	/// ���C�t���C�x���g���擾�B
	/// </summary>
	/// <returns></returns>
	const EnRifleEvent& GetRifleEvent() const
	{
		return m_currentRifleEvent;
	}
	/// <summary>
	/// ���C�t���C�x���g��ݒ�B
	/// </summary>
	/// <param name="event"></param>
	void SetRifleEvent(const EnRifleEvent& event)
	{
		m_currentRifleEvent = event;
	}
	/// <summary>
	/// ���݂̑��U�����擾�B
	/// </summary>
	/// <returns></returns>
	const int& GetCurrentAmo() const
	{
		return m_currentAmo;
	}
	/// <summary>
	/// �ő呕�U���擾�B
	/// </summary>
	/// <returns></returns>
	const int& GetMAX_AMO() const
	{
		return MAX_AMO;
	}
private:
	//���f����b�p�����[�^�[�B
	SkinModelRender* m_render = nullptr;
	Vector3 m_pos = g_vec3Zero;
	Quaternion m_rot = g_quatIdentity;
	Vector3 m_scale = g_vec3One;	
	Bone* m_rHandBone = nullptr;					//�E��{�[���B
	SkinModelRender* m_refBoneRender = nullptr;		//�{�[�����Q�Ƃ��郌���_�[�B
	GamePlayer* pl = nullptr;
	bool m_isShoot = true;						//���C�\�H
	const int MAX_AMO = 30;						//�ő�e���Btodo:Rifle�Ɉڏ��B
	int m_currentAmo = MAX_AMO;					//���݂̒e���Btodo
	const float RELOADTIME = 0.5f;				//�����[�h���ԁBtodo
	float m_currentReloadTime;					//���݂̃����[�h���ԁB
	EnRifleEvent m_currentRifleEvent = EnRifleEvent_None;			//���C�t���C�x���g�B
};

