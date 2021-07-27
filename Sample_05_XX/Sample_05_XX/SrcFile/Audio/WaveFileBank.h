#pragma once

class WaveFile;
//Wave�t�@�C���ƃ|�C���^�̋��L�B
using WaveFilePtr = std::shared_ptr<WaveFile>;
//Wave�t�@�C���}�b�v�̍쐬�B
using WaveFilePtrMap = std::map<unsigned int, WaveFilePtr>;

/// <summary>
/// �g�`�f�[�^�o���N�B
/// <para>���[�h�����g�`�f�[�^���o���N�ɓo�^����ƁA�ēx���[�h������Ȃ��B</para>
/// </summary>
class WaveFileBank
{
public:
	WaveFileBank();
	~WaveFileBank();
	/// <summary>
	/// �O���[�v�P�ʂł̉���B
	/// </summary>
	/// <param name="groupID"></param>
	void Release(int groupID);
	/// <summary>
	/// ���ׂĉ���B
	/// </summary>
	void ReleaseAll()
	{
		for (int i = 0; i < MAX_GROUP; i++) {
			Release(i);
		}
	}
	/// <summary>
	/// �g�`�f�[�^�̓o�^�B
	/// </summary>
	/// <param name="groupID">�O���[�vID�B</param>
	/// <param name="waveFile">�g�`�f�[�^�[�B</param>
	void RegistWaveFile(int groupID, WaveFilePtr waveFile);
	/// <summary>
	/// �����Ŏw�肵���t�@�C���p�X���o�^����Ă��邩�����B
	/// </summary>
	/// <param name="groupID"></param>
	/// <param name="filePath"></param>
	/// <returns></returns>
	WaveFilePtr FindWaveFile(int groupID, const wchar_t* filePath);
	/// <summary>
	/// �g�`�f�[�^���o���N����o�^�����B
	/// </summary>
	/// <param name="groupID">�O�[���vID�B</param>
	/// <param name="waveFile">�g�`�f�[�^�B</param>
	void UnregistWaveFile(int groupID, WaveFilePtr waveFile);

private:
	static const int MAX_GROUP = 256;
	WaveFilePtrMap m_waveFileMap[MAX_GROUP];	//�g�`�f�[�^�[���X�g�B
};

