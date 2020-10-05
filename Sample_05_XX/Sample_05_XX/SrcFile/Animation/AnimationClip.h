#pragma once

struct KeyFrame {
	uint32_t boneIndex;	//何番目のボーンに割り当てられているか。
	float time;			//時間。
	Matrix transform;	//トランスフォーム。
};

class AnimationClip {
public:
	//キーフレームのリスト。
	using KeyFramePtrList = std::vector<KeyFrame*>;

	AnimationClip()
	{
	}
	~AnimationClip();
	/// <summary>
	/// アニメーションクリップのロード。
	/// </summary>
	/// <param name="filePath">ファイルパス。</param>
	void Load(const char* filePath);
	/// <summary>
	/// 読み込みが終了したかどうかのフラグ。
	/// </summary>
	/// <returns>フラグ。</returns>
	bool IsLoaded() const
	{
		return m_tkaFile.IsLoaded();
	}
	/// <summary>
	/// キーフレームの作成。
	/// </summary>
	/// <remarks>
	/// 後々、アニメーションイベントもこの関数で作成する。
	/// </remarks>
	void BuildKeyFrames();
	/// <summary>
	/// ループするかの判定。
	/// </summary>
	/// <returns>フラグ。</returns>
	bool IsLoop()
	{
		return m_isLoop;
	}
	/// <summary>
	/// フラグの設定。
	/// </summary>
	/// <param name="flag">フラグ。</param>
	void SetLoopFlag(bool flag)
	{
		m_isLoop = flag;
	}
	/// <summary>
	/// 全骨のキーフレームリストの取得。
	/// </summary>
	/// <returns>キーフレームリスト。</returns>
	const std::vector<KeyFramePtrList>& GetKeyFramePtrListArray()
	{
		return m_keyFramePtrListArray;
	}
	/// <summary>
	/// キーフレームリストの先頭を取得。
	/// </summary>
	/// <returns>キーフレームリストの先頭。</returns>
	const KeyFramePtrList& GetTopBoneKeyFrameList() const
	{
		return *m_topBoneKeyFrameList;
	}
	/// <summary>
	/// クリップ名を取得。
	/// </summary>
	/// <returns>クリップ名。</returns>
	const wchar_t* GetName() const
	{
		//文字列のポインタを返却。
		return m_clipName.c_str();
	}
private:
	using KeyFramePtr = std::unique_ptr<KeyFrame>;					//キーフレームポインタ。
	std::wstring				m_clipName;							//アニメーションクリップの名前。
	bool						m_isLoop;							//ループ判定。
	std::vector<KeyFramePtr>	m_keyFrames;						//キーフレームのリスト。
	std::vector<KeyFramePtrList>		m_keyFramePtrListArray;		//キーフレームリストの配列。
	KeyFramePtrList* m_topBoneKeyFrameList = nullptr;				//キーフレームの一番上の骨。
	TkaFile					m_tkaFile;								//tkaファイル。
	const int MAX_BONE = 512;												//骨の最大。
};
//アニメーションクリップのスマートポインタ。
using AnimationClipPtr = std::unique_ptr<AnimationClip>;

