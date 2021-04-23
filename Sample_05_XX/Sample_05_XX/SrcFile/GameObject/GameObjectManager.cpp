#include "stdafx.h"
#include "GameObjectManager.h"

/// <summary>
/// リストに積まれているオブジェクトの個数分
/// 優先度順(数字が若いほうが優先)で
/// 各オブジェクトの関数をここで一気に呼び出す処理
/// </summary>

void GameObjectManager::Start()
{
	for (GameObjectList objList : m_gameObjectListArray) {
		for (IGameObject* obj : objList) {
			obj->StartWrapper();
		}
	}
}

void GameObjectManager::Update()
{
	for (GameObjectList objList : m_gameObjectListArray) {
		for (IGameObject* obj : objList) {
			obj->UpdateWrapper();
		}
	}
}

void GameObjectManager::ForwardRender()
{
	for (GameObjectList objList : m_gameObjectListArray) {
		for (IGameObject* obj : objList) {
			obj->ForwardRenderWrapper();
		}
	}
}
	
void GameObjectManager::DrawHUD()
{
	for (GameObjectList objList : m_gameObjectListArray) {
		for (IGameObject* obj : objList) {
			obj->RenderHUD();
		}
	}
}

void GameObjectManager::StartManager()
{
	//初期化。
	Start();
}

/// <summary>
/// 更新処理、描画処理をまとめてしてくれるやつ
/// </summary>
void GameObjectManager::UpdateManager()
{
	/// 更新系処理
	{
		//Shadowの更新。
		GraphicsEngineObj()->GetShadowMap()->Update();
		//更新。
		Update();
		//エフェクトエンジン更新。
		EngineObj().GetEffectEngine().Update();
	}
	/// 描画系処理
	{
		//Shadowの描画。 todo:PreRender
		GraphicsEngineObj()->GetShadowMap()->RenderToShadowMap();
		//GBufferレンダリング。todo:PreRender
		GraphicsEngineObj()->GetGBuffer().Render(GraphicsEngineObj()->GetRenderContext());
		//GBufferを基にディファードレンダリング。
		GraphicsEngineObj()->DeffardRender(GraphicsEngineObj()->GetRenderContext());
		//フィジックスデバッグ描画。
		PhysicObj().DebugDrawWorld(GraphicsEngineObj()->GetRenderContext());
		//フォワードレンダリング。
		ForwardRender();
		//エフェクト描画処理。todo:PostRender?
		EngineObj().GetEffectEngine().Render();
		//HUDに描画。
		DrawHUD();
	}
	//削除
	ExcuteDeleteGameObject();
}
/// <summary>
/// オブジェクトを消す処理
/// </summary>
/// <remarks>
/// 削除予定リストに何のオブジェクトも登録されてないか確認
/// 入ってたら、メモリの解放処理を開始します。
/// </remarks>
void GameObjectManager::ExcuteDeleteGameObject()
{
	int preBufferNo = m_currentDeleteObjectBufferNo;
	//バッファの切り替え　{01 ,10 = 1}  {00 ,11 = 0} ^→排他的論理和
	m_currentDeleteObjectBufferNo = 1 ^ m_currentDeleteObjectBufferNo;
	for (GameObjectList& goList : m_deleteObjectArray[preBufferNo]) {
		for (IGameObject* go : goList) {
			GameObjectPrio prio = go->GetPriority();
			GameObjectList& goExecList = m_gameObjectListArray.at(prio);
			//Listからgoを検索 ※なかったらList.endを返すよ
			auto it = std::find(goExecList.begin(), goExecList.end(), go);
			//みつけたら
			if (it != goExecList.end()) {
				//削除リストから除外された。
				delete (*it);
			}
			//消す
			goExecList.erase(it);
		}
		//リスト自体の削除	
		goList.clear();
	}
}