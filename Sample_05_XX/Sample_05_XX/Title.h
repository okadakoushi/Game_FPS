#pragma once

class StageGenerator;
class GamePlayer;
class GameCamera;
class SkyBox;

class Title : public IGameObject
{
public:
	Title();
	~Title();
	void OnDestroy() override;
	bool Start() override;
	void Update() override;
private:
	SpriteRender* m_titleSprite = nullptr;			//�^�C�g����2D�B
	myEngine::CFontRender* m_pressFont = nullptr;	//Press Mouse Right Button�B
	wchar_t m_pressButtonChar[32] = L"Press Mouse Right Button";
	StageGenerator* m_stageGenerator = nullptr;		//stageGenerator�B
	GamePlayer* m_player = nullptr;
	GameCamera* m_camera = nullptr;
	SkyBox* m_sky = nullptr;			//��B
	int m_securityCameraCount = 0;
	int m_currentCameraCount = 0;
	const float CHANGE_CAMERAPOS_TIMER = 3.0f;
	float m_changeCameraPosTimer = 0.0f;
	Vector3 m_backTarget = g_vec3Zero;
	Vector3 m_cameraPos = g_vec3Zero;
	Quaternion m_cameraRot = g_quatIdentity;
	float m_angle = 0.0f;
	float m_addAngle = 0.1f;
	bool m_isonce = true;
};

