#pragma once

/// <summary>
/// waveファイルクラス。
/// </summary>
class WaveFile
{
public:
	WaveFile();
	~WaveFile();
	/// <summary>
	/// 明示的解放。
	/// </summary>
	void Release();
	/// <summary>
	/// waveファイルをオープン。
	/// </summary>
	/// <param name="fileName">ファイル名。</param>
	/// <returns></returns>
	bool Open(const wchar_t* fileName);
	/// <summary>
	/// 波形データーを読み込み。
	/// </summary>
	/// <param name="pBuffer">波形データを書き込むバッファ。</param>
	/// <param name="sizeToRead">読み込みサイズデータ。</param>
	/// <param name="currentReadSize"></param>
	void Read(char* pBuffer, unsigned int sizeToRead, unsigned int* currentReadSize);
	/// <summary>
	/// 波形データーを非同期読み込み。
	/// </summary>
	/// <param name="pBuffer">波形データを書き込むバッファ。</param>
	/// <param name="sizeToRead">読み込みデータサイズ</param>
	/// <param name="currentReadSize"></param>
	void ReadAsync(char* pBuffer, unsigned int sizeToRead, unsigned int* currentReadSize);
	/// <summary>
	/// 読み込み終了判定。
	/// </summary>
	/// <returns></returns>
	bool IsReadEnd() const
	{
		return m_isReadEnd;
	}
	/// <summary>
	/// 波形データのシーク位置をリセット。
	/// <para>シーク位置とは再生位置のこと。</para>
	/// </summary>
	void ResetFile();
	/// <summary>
	/// ファイルサイズを取得。
	/// </summary>
	/// <returns></returns>
	DWORD GetSize() const
	{
		return m_dwSize;
	}
	/// <summary>
	/// ファイルフォーマットを取得。
	/// </summary>
	/// <returns></returns>
	WAVEFORMATEX* GetFormat() const
	{
		return m_pwfx;
	}
	/// <summary>
	/// ファイルパスのハッシュを取得。
	/// </summary>
	/// <returns></returns>
	unsigned int GetFilePathHash() const
	{
		return m_filePathHash;
	}
	/// <summary>
	/// 読み込み先のバッファを確保。
	/// </summary>
	/// <param name="size"></param>
	void AllocReadBuffer(int size)
	{
		m_readBuffer.reset(new char[size]);
	}
	/// <summary>
	/// 読み込み先のバッファを取得。
	/// </summary>
	/// <returns></returns>
	char* GetReadBuffer()
	{
		return m_readBuffer.get();
	}
private:
	std::unique_ptr<char[]> m_readBuffer = nullptr;				//読み込み先のバッファ。
	HMMIO					m_hmmio = nullptr;					//WAVEファイルのハンドル。
	WAVEFORMATEX*			m_pwfx = nullptr;					//WAVEファイルのフォーマット。
	MMCKINFO				m_ckRiff;							//Use int opening a Wave file
	DWORD					m_dwSize = 0;						//waveファイルのサイズ。
	MMCKINFO				m_ck;								//Riffファイルチャンク情報。
	BYTE*					m_pbData = nullptr;					//
	BYTE*					m_pbDataCur = nullptr;				//
	ULONG					m_ulDataSize = 0;					//
	volatile bool			m_isReadEnd = true;					//読み込み終了フラグ。
	std::thread				m_readAsyncThread;					//非同期読み込みスレッド。
	bool					m_isInvokeReadAsyncThread = false;	//
	std::wstring			m_filePath;							//ファイルパス。
	unsigned int			m_filePathHash = 0;					//ファイルパスのハッシュ。
};

