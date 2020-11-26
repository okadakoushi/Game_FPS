#pragma once

#include "Level.h"

/// <summary>
/// マップチップレンダー。
/// </summary>
class MapChipRender : public IGameObject
{
public:
	MapChipRender();
	~MapChipRender();
	/// <summary>
	/// デストロイ。
	/// </summary>
	void OnDestroy() override;
	/// <summary>
	/// 更新。
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画するオブジェクトの数を追加。
	/// </summary>
	/// <param name="objData">オブジェクトデーター。</param>
	void AddRenderObject(const LevelObjectData& objData)
	{
		m_renderObjDatas.push_back(objData);
	}
	/// <summary>
	/// 初期化。
	/// </summary>
	void InitRenderObject();
	/// <summary>
	/// モデルレンダー取得。
	/// </summary>
	/// <returns></returns>
	SkinModelRender* GetModelRender()
	{
		return m_modelRender;
	}
	/// <summary>
	/// 描画オブジェクトにクエリを行う。
	/// </summary>
	/// <param name="queryFunc">関数オブジェクト。</param>
	void QueryRenderObjDatas(std::function<void(const LevelObjectData& objData)> queryFunc)
	{
		for (const auto& renderObjData : m_renderObjDatas) {
			queryFunc(renderObjData);
		}
	}
private:
	int m_numRenderCount = 0;						//描画するオブジェクトの数。
	SkinModelRender* m_modelRender = nullptr;		//描画するオブジェクトのデーター配列。
	std::vector<LevelObjectData> m_renderObjDatas;	//モデルレンダー。
};

