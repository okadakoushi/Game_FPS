#pragma once

/// <summary>
/// ナビメッシュ。
/// </summary>
class NaviMesh
{
public:
	//バイナリデータのセルのデータ構造体。
	struct CellBin {
		Vector3 pos[3];
		//共用体。
		union {
			std::intptr_t linkCell64[3];
			CellBin* linkCell[3];
		};
	};
public:
	/// <summary>
	/// NaviMeshをロード。
	/// </summary>
	void Load(char* filePath);
public:
	//ここからはNaviMeshをテスト描画する際に必要なコードのため、
	//描画しないユーザーは使用/呼び出ししなくて大丈夫。

	/// <summary>
	/// 描画するために必要な初期化。
	/// </summary>
	void InitRender(bool isFill);
	/// <summary>
	/// 描画準備。
	/// </summary>
	void BeginRender();
	/// <summary>
	/// NaviMeshをライン描画。
	/// </summary>
	void LineRender(const Vector4& color);
	/// <summary>
	/// 描画後処理。
	/// </summary>
	void EndRender();
	/// <summary>
	/// パイプラインステート初期化。
	/// </summary>
	/// <param name="pipelineState"></param>
	/// <param name="rs"></param>
	/// <param name="vs"></param>
	/// <param name="ps"></param>
	void InitPipelineState(PipelineState& pipelineState, RootSignature& rs, Shader& vs, Shader& ps, bool isWire);
	/// <summary>
	/// バイナリデータセルを取得。。
	/// </summary>
	/// <returns>バイナリセルデーター。</returns>
	const CellBin* GetCell() const 
	{
		return m_cellBin;
	}
	/// <summary>
	/// セルの数を取得。
	/// </summary>
	/// <returns>セルの数。</returns>
	const int& GetCellNum() const
	{
		return m_numCell;
	}
private:
	//定数バッファの構造体定義。
	struct SConstantBuffer {
		//Matrix mWorld;		//ワールド行列。
		Matrix mView;			//ビュー行列。
		Matrix mProj;			//プロジェクション行列。
		Vector4 mColor;			//ポリゴンまたは線分の色。
	};
	struct SColorCB {
		Vector4 mColor;			//ポリゴンまたは線分の色。
	};
	ConstantBuffer m_CB;				//コンスタントバッファー。
	ConstantBuffer m_CBBuck;			//コンスタントバッファー。
	DescriptorHeap m_heap;				//ディスクリプタヒープ。
	CellBin* m_cellBin;					//バイナリデータセル。
	vector<Vector3> m_cellPos;			//セルのリスト。
	unsigned int m_numCell = 0;			//セルの数。
	VertexBuffer m_vertexBuffer;		//頂点バッファー。
	IndexBuffer m_indexBuffer;			//インデックスバッファー。
	VertexBuffer m_vertexBuck;			//背景用頂点バッファー。
	IndexBuffer m_indexBuck;			//背景用インデックスバッファー。
	vector<int> m_indexs;				//インデックスバッファーのリスト。
	vector<Vector3*> m_cellVertexPos;	//セルの頂点座標。
	RootSignature m_rootSignature;		//ルートシグネチャ。
	bool m_isLoad = false;				//ロードした？
	int ibCount = 0;					//インデックスカウント。
	PipelineState m_pipelineState;		//パイプラインステート。
	PipelineState m_pipelineStateBuck;	//背景用パイプラインステート。
	Vector3 m_eye;						//視点。
	Vector3 m_vMax = { -FLT_MAX, -FLT_MAX, -FLT_MAX };	//最大だった頂点。
	Vector3 m_vMin = { FLT_MAX,FLT_MAX ,FLT_MAX };		//最小だった頂点。
};

