#pragma once

class WaveFile;
//Waveファイルとポインタの共有。
using WaveFilePtr = std::shared_ptr<WaveFile>;
//Waveファイルマップの作成。
using WaveFilePtrMap = std::map<unsigned int, WaveFilePtr>;

/// <summary>
/// 波形データバンク。
/// <para>ロードした波形データをバンクに登録すると、再度ロードが走らない。</para>
/// </summary>
class WaveFileBank
{
public:
	WaveFileBank();
	~WaveFileBank();
	/// <summary>
	/// グループ単位での解放。
	/// </summary>
	/// <param name="groupID"></param>
	void Release(int groupID);
	/// <summary>
	/// すべて解放。
	/// </summary>
	void ReleaseAll()
	{
		for (int i = 0; i < MAX_GROUP; i++) {
			Release(i);
		}
	}
	/// <summary>
	/// 波形データの登録。
	/// </summary>
	/// <param name="groupID">グループID。</param>
	/// <param name="waveFile">波形データー。</param>
	void RegistWaveFile(int groupID, WaveFilePtr waveFile);
	/// <summary>
	/// 引数で指定したファイルパスが登録されているか検索。
	/// </summary>
	/// <param name="groupID"></param>
	/// <param name="filePath"></param>
	/// <returns></returns>
	WaveFilePtr FindWaveFile(int groupID, const wchar_t* filePath);
	/// <summary>
	/// 波形データをバンクから登録解除。
	/// </summary>
	/// <param name="groupID">グールプID。</param>
	/// <param name="waveFile">波形データ。</param>
	void UnregistWaveFile(int groupID, WaveFilePtr waveFile);

private:
	static const int MAX_GROUP = 256;
	WaveFilePtrMap m_waveFileMap[MAX_GROUP];	//波形データーリスト。
};

