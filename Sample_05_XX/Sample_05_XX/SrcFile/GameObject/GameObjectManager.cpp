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

void GameObjectManager::PostUpdate()
{
	for (GameObjectList objList : m_gameObjectListArray) {
		for (IGameObject* obj : objList) {
			obj->PostUpdateWrapper();
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
			obj->RenderHUDWrapper();
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
		//���C���`�揀���B
		PhysicObj().BeginDraw(GraphicsEngineObj()->GetRenderContext());
		//�X�V�B
		Update();
		//�x���X�V�B
		PostUpdate();
		//�G�t�F�N�g�G���W���X�V�B
		EngineObj().GetEffectEngine().Update();
	}

	
	/// �`��n����
	{
		//GraphicsEngineObj()->GetRenderingEngine().ZPrePass(GraphicsEngineObj()->GetRenderContext());
		//Shadow�̕`��B todo:PreRender
		GraphicsEngineObj()->GetShadowMap()->RenderToShadowMap();
		//GBuffer�����_�����O�Btodo:PreRender
		GraphicsEngineObj()->GetDefferd().Render(GraphicsEngineObj()->GetRenderContext(), GraphicsEngineObj()->GetCamera3D().GetViewMatrix(), GraphicsEngineObj()->GetCamera3D().GetProjectionMatrix());
		//GBuffer����Ƀf�B�t�@�[�h�����_�����O�B
		GraphicsEngineObj()->GetDefferd().DeffardRender(GraphicsEngineObj()->GetRenderContext(), GraphicsEngineObj()->GetCamera2D().GetViewMatrix(), GraphicsEngineObj()->GetCamera2D().GetProjectionMatrix());
#ifdef PHYSICS_DEBUG
		//�t�B�W�b�N�X�f�o�b�O�`��B
		PhysicObj().PhysicsDraw();
#endif
		//�f�v�X
		GraphicsEngineObj()->GetRenderContext().SetRenderTarget(
			GraphicsEngineObj()->GetRenderingEngine().GetMainRenderTarget(),
			GraphicsEngineObj()->GetDefferd().GetDSV()
		);
		//�t�H���[�h�����_�����O�B
		ForwardRender();
		//�G�t�F�N�g�`�揈���Btodo:PostRender?
		EngineObj().GetEffectEngine().Render();		
		//�t�H���g�̕`����J�n�B
		//GraphicsEngineObj()->GetFontEngine().BeginDraw();
		//�|�X�g���|����B
		GraphicsEngineObj()->GetRenderingEngine().BloomRender();
		//�t���[���o�b�t�@�ɃR�s�[�B
		GraphicsEngineObj()->GetRenderingEngine().CopyMainRenderTargetToFrameBuffer(GraphicsEngineObj()->GetRenderContext());
		//HUD�ɕ`��B
		DrawHUD();
		//�t�H���g�̕`��I���B
		//GraphicsEngineObj()->GetFontEngine().EndDraw();
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
				//���d����o�O�f�o�b�O�p�B
				//printf("DeleteObjectHash = %s\n", go->m_tags);
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