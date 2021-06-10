#pragma once

/// <summary>
/// ライン描画。
/// </summary>
/// <code>
/// Init()
/// BeginDraw()->Draw()
/// </code>
class LineDraw
{
public:
	/// <summary>
	/// 初期化。
	/// </summary>
	void Init();
	/// <summary>
	/// 描画準備。
	/// </summary>
	void BeginDraw(RenderContext& rc);
	/// <summary>
	/// 描画するラインを追加。
	/// <para>ユーザーが呼び出しするのはこいつだけでOK。</para>
	/// </summary>
	/// <param name="from">fromベクトル</param>
	/// <param name="to">toベクトル。</param>
	void AddLine(const Vector3& from, const Vector3& to, const Vector4& color = {1.0f, 0.0f, 0.0f, 1.0f});
	/// <summary>
	/// 描画。
	/// </summary>
	void Draw();
private:
	struct SConstantBuffer {
		Matrix mView;
		Matrix mProj;
		Vector4 mColor;
	};
	ConstantBuffer m_cb;							//定数バッファ。
	SConstantBuffer s_cb;
	static const int VERTEX_MAX = 10000000;			//最大頂点。
	std::array<Vector4, VERTEX_MAX> m_vertexs;		//頂点バッファーとインデックス。
	RenderContext* m_rc;							//レンコン。
	PipelineState m_pipeline;						//パイプラインステート。
	VertexBuffer m_vertexBuffer;
	DescriptorHeap m_heap;
	RootSignature m_rootSignature;
	int m_numLine = 0;
};

