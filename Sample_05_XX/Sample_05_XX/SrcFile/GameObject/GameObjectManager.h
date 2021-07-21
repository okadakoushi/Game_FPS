/// <summary>
/// GameObjectのマネージャー
/// </summary>
#pragma once
#include "IGameObject.h"
#include "SrcFile/util/Util.h"

typedef unsigned char	GameObjectPrio;
class GameObjectManager
{
public:
	GameObjectManager()
	{
		m_gameObjectListArray.resize(10);
		m_deleteObjectArray[0].resize(10);
		m_deleteObjectArray[1].resize(10);
	};
	~GameObjectManager()
	{
	};
	/// <summary>
	/// ゲームオブジェクトの名前キーを作成
	/// </summary>
	/// <remarks>
	/// FindGOするときに使うよ
	/// Util::MakeHashで文字列を数列(hash)に変えています
	/// </remarks>
	/// <param name="objectName">オブジェクトの名前</param>
	/// <returns>ハッシュの値</returns>
	static unsigned int MakeGameObjectNameKey(const char* objectName)
	{
		//デフォルトネームを代入
		static const unsigned int defaultNameKey = Util::MakeHash("Undefined");
		unsigned int hash;
		//なにも入ってないなら
		if (objectName == nullptr) {
			//デフォルトネーム
			hash = defaultNameKey;
		}
		//なんか入ってたら
		else {
			//ハッシュの作成
			hash = Util::MakeHash(objectName);
		}
		return hash;
	}
	/// <summary>
	/// インスタンスの取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static GameObjectManager& Instance()
	{
		static GameObjectManager instance;
		return instance;
	}

public:
	/*
	ラップする前の本体の関数です インライン関数です。
	*/
	/// <summary>
	/// <para>オブジェクトの作成</para>
	/// <para>この関数で作ったオブジェクトはDeleteGOで消すこと</para>
	/// </summary>
	template <class T>
	T* NewGameObject(GameObjPrio prio, const char* objectName)
	{
		(void*)objectName;
		//クラスの初期化
		T* newObject = new T();
		//優先度+ゲームオブジェクトをリストに積む
		m_gameObjectListArray.at(prio).push_back(newObject);
		//FindGOの為にハッシュを作成 hashについてはUtil.hを参照
		unsigned int hash = MakeGameObjectNameKey(objectName);
		//IGameObjectに積まれているprioを今回入ってきたprioで初期化
		newObject->m_priority = prio;
		//上と同じ感じでハッシュを代入
		newObject->m_nameKey = hash;
		//ハッシュとは別にオブジェクトの名前。
		newObject->m_tags = objectName;
		return newObject;
	}
	/// <summary>
	/// オブジェクトの削除
	/// </summary>
	void DeleteGameObject(IGameObject* gameObject)
	{
		if (gameObject->m_nameKey == 1460041493) {
			int a = 0;
		}
		if (gameObject->m_nameKey == 3617970110) {
			int a = 0;
		}
		if (gameObject != nullptr)
		{
			//リストに追加
			m_deleteObjectArray[m_currentDeleteObjectBufferNo].at(gameObject->GetPriority()).push_back(gameObject);
			gameObject->OnDestroy();
			gameObject->m_isDead = true;
			gameObject = nullptr;
		}
		else {
			//throw;
		}
	}
	/// <summary>
	/// ゲームオブジェクトの検索
	/// </summary>
	template<class T>
	T* FindGameObject(const char* objectName, bool enableErrorMessage)
	{
		unsigned int nameKey = Util::MakeHash(objectName);
		//goListをm_gameObjectListArray回、回す
		for (auto goList : m_gameObjectListArray) {
			//goをgoList回
			for (auto go : goList) {
				//見つけた
				if (go->m_nameKey == nameKey) {
					//型変換
					T* p = dynamic_cast<T*>(go);
					//型変換に失敗
					if (p == nullptr && enableErrorMessage == true) {
						/*
						throw!!	下記を参考に直してね！
						型変換が原因でFindGOできてない場合があるので
						引数に渡したクラスの再確認をしてみてください。
						*/
						throw;
					}
					//Find成功
					return p;
				}
			}
		}
		if (enableErrorMessage == true) {
			/*
			throw!!	下記を参考に直してね！
			FindGOに渡された名前が存在しないということなので
			FindGOとNewGOのオブジェクトネームを再確認してみてください。
			*/
			throw;
		}
		//見つからなかった
		return nullptr;
	}
	template<class T>
	void FindGameObjects(const char* objectName, std::function<bool(T* go)> func)
	{
		unsigned int nameKey = Util::MakeHash(objectName);
		for (auto goList : m_gameObjectListArray) {
			for (auto go : goList) {
				if (go->m_nameKey == nameKey) {
					//見つけた。
					T* p = dynamic_cast<T*>(go);
					if (func(p) == false) {
						//クエリ中断。
						return;
					}
				}
			}
		}
	}
public:
	/*
	メンバ関数定義です。
	こいつらの処理はcppに書いてあります
	*/
	//初期化
	void Start();
	//更新
	void Update();
	//遅延更新。
	void PostUpdate();
	/// <summary>
	/// 描画
	/// </summary>
	void ForwardRender();
	/// <summary>
	/// 最後の描画処理
	/// </summary>
	void DrawHUD();
	/// <summary>
	/// ゲームオブジェクトの初期化開始。
	/// </summary>
	void StartManager();
	/// <summary>
	/// ゲームオブジェクトマネージャーから呼ばれる各処理
	/// </summary>
	void UpdateManager();
	/// <summary>
	/// オブジェクトのデーター開放処理
	///	<para>本当にメモリの解放をする処理はこっち！</para>
	/// </summary>
	/// <remarks>
	/// DeleteGOは削除リストに積んでるだけだよ！
	/// </remarks>
	void ExcuteDeleteGameObject();

private:
	/*
	メンバ変数です
	*/

	typedef std::list<IGameObject*>	GameObjectList;						//名前変更  IGameObjectのポインタが入った可変長配列
	std::vector<GameObjectList>		m_gameObjectListArray;				//ゲームオブジェクトの優先度付きリスト	こっちは可変長配列じゃないから初期化してね	
	std::vector<GameObjectList>		m_deleteObjectArray[2];				//削除するオブジェクトのリスト	削除処理中にDeleteGOがよばれる可能性があるからダブルバッファ化
	int								m_currentDeleteObjectBufferNo = 0;	//削除中のバッファー番号

};
	/*
		いちいちgameObjectManager().NewGameObject...みたいなの書くの面倒だから
		static inline化　→　こいつが呼び出された位置でコードを展開、コンパイルが通るようにする。
	*/
	/// <summary>
	/// ゲームオブジェクトマネージャー取得。
	/// </summary>
	/// <returns></returns>
	static inline GameObjectManager& gameObjectManager()
	{
		return GameObjectManager::Instance();
	}
	/// <summary>
	/// <para>オブジェクトの作成</para>
	/// <para>この関数で作ったオブジェクトはDeleteGOで消すこと</para>
	/// </summary>
	/// <remarks>
	/// DeleteGOで消さなかった場合は、m_gameObjectListArrayからも消してね
	/// </remarks>
	/// <typeparam name="T"><para>新しく作るオブジェクト</para></typeparam>
	/// <param name="prio">優先度</param>
	/// <param name="objectName">オブジェクトの名前</param>
	/// <returns>オブジェクト</returns>
	template<class T>
	static inline T* NewGO(int priority, const char* objectName = nullptr)
	{
		return gameObjectManager().NewGameObject<T>((GameObjectPrio)priority, objectName);
	}
	/// <summary>
	///	ゲームオブジェクトの複数検索
	/// </summary>
	/// <param name="objectName">オブジェクト名前</param>
	/// <param name="enableErrorMessage">エラーメッセージの表示</param>
	/// <remarks>
	/// </remarks>
	template<class T>
	static inline void QueryGOs(const char* objectName, std::function<bool(T* go)> func)
	{
		return gameObjectManager().FindGameObjects<T>(objectName, func);
	}
	/// <summary>
	/// ゲームオブジェクトの検索 重いよ！
	/// <para>エラーメッセージをtrueにするとFindGO失敗でthrowします。</para>
	/// <para>コメントを参考にして直してください。</para>
	/// </summary>
	/// <typeparam name="T">検索したいオブジェクト</typeparam>
	/// <param name="objectName">オブジェクトの名前</param>
	/// <param name="enableErrorMessage">エラー表示</param>
	template<class T>
	static inline T* FindGO(const char* objectName, bool enableErrorMessage = true)
	{
		return gameObjectManager().FindGameObject<T>(objectName, enableErrorMessage);
	}

	/// <summary>
	/// オブジェクトの削除
	/// <para>オブジェクトを削除予定リストに積む</para>
	/// </summary>
	/// <remarks>
	/// 実際にはここでオブジェクトを消しているわけではありません。
	/// 毎フレームm_deleteObjectArrayに積まれてないか確認して
	/// ExecuteDeleteGameObjectsという関数で削除処理を行います
	/// </remarks>
	///	<param name="IGameObject">消したいオブジェクトのクラス</param>
	static inline void DeleteGO(IGameObject* go)
	{
		gameObjectManager().DeleteGameObject(go);
	}

