#pragma once
class SkyBox : public IGameObject
{
public:
	SkyBox();
	~SkyBox();
	/// <summary>
	/// �������B
	/// </summary>
	/// <returns></returns>
	bool Start() override;
	/// <summary>
	/// �X�V�B
	/// </summary>
	void Update() override;
public:
	/// <summary>
	/// �g�嗦��ݒ�B
	/// </summary>
	/// <param name="scale"></param>
	void SetScale(float scale) {
		m_scale = { scale, scale, scale };
		m_isDirty = true;
	}
	/// <summary>
	/// ��̈ʒu���߁B
	/// </summary>
	/// <param name="pos"></param>
	void SetPosition(Vector3& pos) {
		m_position = pos;
		m_isDirty = true;
	}
private:
	SkinModelRender* m_modelRender = nullptr;			//���f�������_�[�B
	Texture m_skyCube;									//�X�J�C�L���[�u�B
	Shader m_psShader;									//�X�J�C�p�̃V�F�[�_�[�B
	Vector3 m_scale = { 8000.0f, 8000.0f, 8000.0f };	//��̊g��B
	Vector3 m_position = Vector3::Zero;					//�ʒu�B
	Vector3 m_emissionColor = Vector3::Zero;			//���Ȕ����B
	bool m_isDirty = false;								//�_�[�e�B�t���O�B
	std::wstring m_skyCubeMapFilePath = L"Assets/modelData/nature/skyCubeMap.dds";		//��}�b�v�̃t�@�C���p�X�B
	const char* m_skyModelFilePath = "Assets/modelData/nature/skyBox.tkm";				//�󃂃f���̃t�@�C���p�X
};

