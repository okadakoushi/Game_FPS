#pragma once

/// <summary>
/// wave�t�@�C���N���X�B
/// </summary>
class WaveFile
{
public:
	WaveFile();
	~WaveFile();
	/// <summary>
	/// �����I����B
	/// </summary>
	void Release();
	/// <summary>
	/// wave�t�@�C�����I�[�v���B
	/// </summary>
	/// <param name="fileName">�t�@�C�����B</param>
	/// <returns></returns>
	bool Open(const wchar_t* fileName);
	/// <summary>
	/// �g�`�f�[�^�[��ǂݍ��݁B
	/// </summary>
	/// <param name="pBuffer">�g�`�f�[�^���������ރo�b�t�@�B</param>
	/// <param name="sizeToRead">�ǂݍ��݃T�C�Y�f�[�^�B</param>
	/// <param name="currentReadSize"></param>
	void Read(char* pBuffer, unsigned int sizeToRead, unsigned int* currentReadSize);
	/// <summary>
	/// �g�`�f�[�^�[��񓯊��ǂݍ��݁B
	/// </summary>
	/// <param name="pBuffer">�g�`�f�[�^���������ރo�b�t�@�B</param>
	/// <param name="sizeToRead">�ǂݍ��݃f�[�^�T�C�Y</param>
	/// <param name="currentReadSize"></param>
	void ReadAsync(char* pBuffer, unsigned int sizeToRead, unsigned int* currentReadSize);
	/// <summary>
	/// �ǂݍ��ݏI������B
	/// </summary>
	/// <returns></returns>
	bool IsReadEnd() const
	{
		return m_isReadEnd;
	}
	/// <summary>
	/// �g�`�f�[�^�̃V�[�N�ʒu�����Z�b�g�B
	/// <para>�V�[�N�ʒu�Ƃ͍Đ��ʒu�̂��ƁB</para>
	/// </summary>
	void ResetFile();
	/// <summary>
	/// �t�@�C���T�C�Y���擾�B
	/// </summary>
	/// <returns></returns>
	DWORD GetSize() const
	{
		return m_dwSize;
	}
	/// <summary>
	/// �t�@�C���t�H�[�}�b�g���擾�B
	/// </summary>
	/// <returns></returns>
	WAVEFORMATEX* GetFormat() const
	{
		return m_pwfx;
	}
	/// <summary>
	/// �t�@�C���p�X�̃n�b�V�����擾�B
	/// </summary>
	/// <returns></returns>
	unsigned int GetFilePathHash() const
	{
		return m_filePathHash;
	}
	/// <summary>
	/// �ǂݍ��ݐ�̃o�b�t�@���m�ہB
	/// </summary>
	/// <param name="size"></param>
	void AllocReadBuffer(int size)
	{
		m_readBuffer.reset(new char[size]);
	}
	/// <summary>
	/// �ǂݍ��ݐ�̃o�b�t�@���擾�B
	/// </summary>
	/// <returns></returns>
	char* GetReadBuffer()
	{
		return m_readBuffer.get();
	}
private:
	std::unique_ptr<char[]> m_readBuffer = nullptr;				//�ǂݍ��ݐ�̃o�b�t�@�B
	HMMIO					m_hmmio = nullptr;					//WAVE�t�@�C���̃n���h���B
	WAVEFORMATEX*			m_pwfx = nullptr;					//WAVE�t�@�C���̃t�H�[�}�b�g�B
	MMCKINFO				m_ckRiff;							//Use int opening a Wave file
	DWORD					m_dwSize = 0;						//wave�t�@�C���̃T�C�Y�B
	MMCKINFO				m_ck;								//Riff�t�@�C���`�����N���B
	BYTE*					m_pbData = nullptr;					//
	BYTE*					m_pbDataCur = nullptr;				//
	ULONG					m_ulDataSize = 0;					//
	volatile bool			m_isReadEnd = true;					//�ǂݍ��ݏI���t���O�B
	std::thread				m_readAsyncThread;					//�񓯊��ǂݍ��݃X���b�h�B
	bool					m_isInvokeReadAsyncThread = false;	//
	std::wstring			m_filePath;							//�t�@�C���p�X�B
	unsigned int			m_filePathHash = 0;					//�t�@�C���p�X�̃n�b�V���B
};

