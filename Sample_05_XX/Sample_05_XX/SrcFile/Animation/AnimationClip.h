#pragma once

struct KeyFrame {
	uint32_t boneIndex;	//���Ԗڂ̃{�[���Ɋ��蓖�Ă��Ă��邩�B
	float time;			//���ԁB
	Matrix transform;	//�g�����X�t�H�[���B
};

class AnimationClip {
public:
	//�L�[�t���[���̃��X�g�B
	using KeyFramePtrList = std::vector<KeyFrame*>;

	AnimationClip()
	{
	}
	~AnimationClip();
	/// <summary>
	/// �A�j���[�V�����N���b�v�̃��[�h�B
	/// </summary>
	/// <param name="filePath">�t�@�C���p�X�B</param>
	void Load(const char* filePath);
	/// <summary>
	/// �ǂݍ��݂��I���������ǂ����̃t���O�B
	/// </summary>
	/// <returns>�t���O�B</returns>
	bool IsLoaded() const
	{
		return m_tkaFile.IsLoaded();
	}
	/// <summary>
	/// �L�[�t���[���̍쐬�B
	/// </summary>
	/// <remarks>
	/// ��X�A�A�j���[�V�����C�x���g�����̊֐��ō쐬����B
	/// </remarks>
	void BuildKeyFrames();
	/// <summary>
	/// ���[�v���邩�̔���B
	/// </summary>
	/// <returns>�t���O�B</returns>
	bool IsLoop()
	{
		return m_isLoop;
	}
	/// <summary>
	/// �t���O�̐ݒ�B
	/// </summary>
	/// <param name="flag">�t���O�B</param>
	void SetLoopFlag(bool flag)
	{
		m_isLoop = flag;
	}
	/// <summary>
	/// �S���̃L�[�t���[�����X�g�̎擾�B
	/// </summary>
	/// <returns>�L�[�t���[�����X�g�B</returns>
	const std::vector<KeyFramePtrList>& GetKeyFramePtrListArray()
	{
		return m_keyFramePtrListArray;
	}
	/// <summary>
	/// �L�[�t���[�����X�g�̐擪���擾�B
	/// </summary>
	/// <returns>�L�[�t���[�����X�g�̐擪�B</returns>
	const KeyFramePtrList& GetTopBoneKeyFrameList() const
	{
		return *m_topBoneKeyFrameList;
	}
	/// <summary>
	/// �N���b�v�����擾�B
	/// </summary>
	/// <returns>�N���b�v���B</returns>
	const wchar_t* GetName() const
	{
		//������̃|�C���^��ԋp�B
		return m_clipName.c_str();
	}
private:
	using KeyFramePtr = std::unique_ptr<KeyFrame>;					//�L�[�t���[���|�C���^�B
	std::wstring				m_clipName;							//�A�j���[�V�����N���b�v�̖��O�B
	bool						m_isLoop;							//���[�v����B
	std::vector<KeyFramePtr>	m_keyFrames;						//�L�[�t���[���̃��X�g�B
	std::vector<KeyFramePtrList>		m_keyFramePtrListArray;		//�L�[�t���[�����X�g�̔z��B
	KeyFramePtrList* m_topBoneKeyFrameList = nullptr;				//�L�[�t���[���̈�ԏ�̍��B
	TkaFile					m_tkaFile;								//tka�t�@�C���B
	const int MAX_BONE = 512;												//���̍ő�B
};
//�A�j���[�V�����N���b�v�̃X�}�[�g�|�C���^�B
using AnimationClipPtr = std::unique_ptr<AnimationClip>;

