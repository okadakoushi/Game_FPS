#include "stdafx.h"
#include "GameObjectManager.h"

/// <summary>
/// ���X�g�ɐς܂�Ă���I�u�W�F�N�g�̌���
/// �D��x��(�������Ⴂ�ق����D��)��
/// �e�I�u�W�F�N�g�̊֐��������ň�C�ɌĂяo������
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
	//�������B
	Start();
}

/// <summary>
/// �X�V�����A�`�揈�����܂Ƃ߂Ă��Ă������
/// </summary>
void GameObjectManager::UpdateManager()
{
	/// �X�V�n����
	{
		//Shadow�̍X�V�B
		GraphicsEngineObj()->GetShadowMap()->Update();
		//�X�V�B
		Update();
		//�G�t�F�N�g�G���W���X�V�B
		EngineObj().GetEffectEngine().Update();
	}
	/// �`��n����
	{
		//Shadow�̕`��B todo:PreRender
		GraphicsEngineObj()->GetShadowMap()->RenderToShadowMap();
		//GBuffer�����_�����O�Btodo:PreRender
		GraphicsEngineObj()->GetGBuffer().Render(GraphicsEngineObj()->GetRenderContext());
		//GBuffer����Ƀf�B�t�@�[�h�����_�����O�B
		GraphicsEngineObj()->DeffardRender(GraphicsEngineObj()->GetRenderContext());
		//�t�B�W�b�N�X�f�o�b�O�`��B
		PhysicObj().DebugDrawWorld(GraphicsEngineObj()->GetRenderContext());
		//�t�H���[�h�����_�����O�B
		ForwardRender();
		//�G�t�F�N�g�`�揈���Btodo:PostRender?
		EngineObj().GetEffectEngine().Render();
		//HUD�ɕ`��B
		DrawHUD();
	}
	//�폜
	ExcuteDeleteGameObject();
}
/// <summary>
/// �I�u�W�F�N�g����������
/// </summary>
/// <remarks>
/// �폜�\�胊�X�g�ɉ��̃I�u�W�F�N�g���o�^����ĂȂ����m�F
/// �����Ă���A�������̉���������J�n���܂��B
/// </remarks>
void GameObjectManager::ExcuteDeleteGameObject()
{
	int preBufferNo = m_currentDeleteObjectBufferNo;
	//�o�b�t�@�̐؂�ւ��@{01 ,10 = 1}  {00 ,11 = 0} ^���r���I�_���a
	m_currentDeleteObjectBufferNo = 1 ^ m_currentDeleteObjectBufferNo;
	for (GameObjectList& goList : m_deleteObjectArray[preBufferNo]) {
		for (IGameObject* go : goList) {
			GameObjectPrio prio = go->GetPriority();
			GameObjectList& goExecList = m_gameObjectListArray.at(prio);
			//List����go������ ���Ȃ�������List.end��Ԃ���
			auto it = std::find(goExecList.begin(), goExecList.end(), go);
			//�݂�����
			if (it != goExecList.end()) {
				//�폜���X�g���珜�O���ꂽ�B
				delete (*it);
			}
			//����
			goExecList.erase(it);
		}
		//���X�g���̂̍폜	
		goList.clear();
	}
}