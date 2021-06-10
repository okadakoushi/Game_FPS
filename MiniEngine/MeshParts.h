/// <summary>
/// メッシュパーツクラス。
/// </summary>

#pragma once

#include "tkFile/TkmFile.h"
#include "StructuredBuffer.h"

class RenderContext;
class Skeleton;
class Material;
class IShaderResource;


/// <summary>
/// メッシュ
/// </summary>
struct SMesh {
	VertexBuffer m_vertexBuffer;						//頂点バッファ。
	std::vector< IndexBuffer* >		m_indexBufferArray;	//インデックスバッファ。
	std::vector< Material* >		m_materials;			//マテリアル。
	std::vector<int>				skinFlags;				//スキンを持っているかどうかのフラグ。
};

/// <summary>
/// メッシュパーツ。
/// </summary>
class MeshParts {
public:
	/// <summary>
	/// デストラクタ。
	/// </summary>
	~MeshParts();

	void Release();
	/// <summary>
	/// tkmファイルから初期化
	/// </summary>
	/// <param name="tkmFile">tkmファイル。</param>
	/// /// <param name="fxFilePath">fxファイルのファイルパス</param>
	/// <param name="vsEntryPointFunc">頂点シェーダーのエントリーポイントの関数名</param>
	/// <param name="psEntryPointFunc">ピクセルシェーダーのエントリーポイントの関数名</param>
	void InitFromTkmFile(
		const TkmFile& tkmFile,
		void* expandData,
		int expandDataSize,
		IShaderResource* expandShaderResourceView,
		const wchar_t* filePath, 
		const char* VSEntry, 
		const char* PSEntry
	) ;
	/// <summary>
	/// 描画。
	/// </summary>
	/// <param name="rc">レンダリングコンテキスト</param>
	/// <param name="mWorld">ワールド行列</param>
	/// <param name="mView">ビュー行列</param>
	/// <param name="mProj">プロジェクション行列</param>
	/// <param name="light">ライト</param>
	void Draw(RenderContext& rc, const Matrix& mWorld, const Matrix& mView, const Matrix& mProj, const bool isShadowReciever = false, const int renderMode = 0);
	/// <summary>
	/// スケルトンを関連付ける。
	/// </summary>
	/// <param name="skeleton">スケルトン</param>
	void BindSkeleton(Skeleton& skeleton) ;
	/// <summary>
	/// 乗算カラーを設定。
	/// </summary>
	void SetMulColor(const Vector4& color)
	{
		m_mulColor = color;
	}
	/// <summary>
	/// メッシュリストを取得。
	/// </summary>
	/// <returns></returns>
	std::vector< SMesh* >& GetMeshList()
	{
		return m_meshs;
	}

	std::vector<DescriptorHeap>& GetDescriptorHeap()
	{
		return m_descriptorHeap;
	}
private:
	/// <summary>
	/// tkmメッシュからメッシュを作成。
	/// </summary>
	/// <param name="mesh">メッシュ</param>
	/// <param name="meshNo">メッシュ番号</param>
	/// <param name="fxFilePath">fxファイルのファイルパス</param>
	/// <param name="vsEntryPointFunc">頂点シェーダーのエントリーポイントの関数名</param>
	/// <param name="psEntryPointFunc">ピクセルシェーダーのエントリーポイントの関数名</param>
	void CreateMeshFromTkmMesh(
		const TkmFile::SMesh& mesh, 
		int meshNo,
		const wchar_t* filePath, 
		const char* VSEntry, 
		const char* PSEntry
	);
	/// <summary>
	/// ディスクリプタヒープを作成。
	/// </summary>
	void CreateDescriptorHeaps();
private:
	//拡張SRVが設定されるレジスタの開始番号。
	const int EXPAND_SRV_REG__START_NO = 10;
	/// <summary>
	/// 定数バッファ。
	/// </summary>
	/// <remarks>
	/// この構造体を変更したら、SimpleModel.fxのCBも変更するように。
	/// </remarks>
	struct SConstantBuffer {
		Matrix mWorld;			//ワールド行列。
		Matrix mView;			//ビュー行列。
		Matrix mProj;			//プロジェクション行列。
		Vector4 mulColor;			//乗算カラー。
		int isShadowReciever;	//シャドウレシーバー？
	};
	ConstantBuffer m_commonConstantBuffer;					//メッシュ共通の定数バッファ。
	ConstantBuffer	m_expandConstantBuffer;					//ユーザー拡張用の定数バッファ
	IShaderResource* m_expandShaderResourceView = nullptr;	//ユーザー拡張シェーダーリソースビュー。
	StructuredBuffer m_boneMatricesStructureBuffer;			//ボーン行列の構造化バッファ。
	std::vector< SMesh* > m_meshs;							//メッシュ。
	std::vector< DescriptorHeap > m_descriptorHeap;			//ディスクリプタヒープ。
	Skeleton* m_skeleton = nullptr;							//スケルトン。
	void* m_expandData = nullptr;							//ユーザー拡張データ。
	bool m_isCreateDescriptorHeap = false;					//ディスクリプタヒープが作成済み？
	Vector4 m_mulColor = Vector4::White;					//乗算カラー。
};