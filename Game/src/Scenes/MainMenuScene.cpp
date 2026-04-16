#include "MainMenuScene.h"

#include "Input/Input.h"
#include "Core/Assets.h"
#include "GameScene.h"
#include "Core/Application.h"
#include "Audio/AudioSystem.h"

#include "Actors/Arena.h"
#include "Actors/SkyDome.h"
#include "Actors/DummyCamera.h"
#include "Actors/DummyEnemy.h"

namespace Dawn
{
	MainMenuScene::MainMenuScene()
	{
		GetEnvironmentSettings().bloomRadius = 2.0f;
		GetEnvironmentSettings().bloomStrength = 0.01f;
		GetEnvironmentSettings().fogDensity = 0.02f;
		GetEnvironmentSettings().fogColor = glm::vec3(0.06f, 0.075f, 0.10f) * 0.65f;
		GetEnvironmentSettings().ambientColor = glm::vec3(0.09f);
		GetEnvironmentSettings().directionalLight.color = glm::vec3(0.6f, 0.7f, 0.85f) * 1.3f;
		GetEnvironmentSettings().directionalLight.direction = glm::vec3(-0.5f, -0.2f, -0.2f);
		GetEnvironmentSettings().directionalLight.intensity = 1.0f;
	}

	MainMenuScene::~MainMenuScene()
	{
		if (mBgmSE.IsValid())
			mBgmSE.Stop();

		Application::Get()->GetAudioSystem()->UnloadBank("Assets/Audio/MainMenu.bank");
	}

	void MainMenuScene::Init()
	{
		Input::SetCursorLocked(false);

		mBGTexture = Assets::GetTexture("Assets/Textures/MainMenuBG_AI.png");

		ImGuiIO& io = ImGui::GetIO();
		mFontBold = io.Fonts->AddFontFromFileTTF("Assets/Fonts/Montserrat-SemiBold.ttf");
		mFontRegular = io.Fonts->AddFontFromFileTTF("Assets/Fonts/Montserrat-Regular.ttf");
		mFontLight = io.Fonts->AddFontFromFileTTF("Assets/Fonts/Montserrat-Light.ttf");

		ImGuiStyle& style = ImGui::GetStyle();
		style.FrameRounding = 6.0f;

		Application::Get()->GetAudioSystem()->LoadBank("Assets/Audio/MainMenu.bank");
		Application::Get()->GetAudioSystem()->LoadBank("Assets/Audio/UI.bank");
		mBgmSE = Application::Get()->GetAudioSystem()->PlayEvent("event:/mainmenu_bgm");

		// Populating the scene
		DummyCamera* cameraActor = new DummyCamera(this);
		cameraActor->SetPosition(glm::vec3(0, 2.0f, 0.0f));
		Camera* cam = cameraActor->GetComponent<Camera>();
		if (cam)
		{
			cam->SetFarPlane(1000.0f);	// So that skydome is visible
			SetActiveCamera(cam);
		}

		Arena* arena = new Arena(this);
		SkyDome* skydome = new SkyDome(this);
		DummyEnemy* enemy = new DummyEnemy(this);
	}

	void MainMenuScene::Update(float deltaTime)
	{
	}

	void MainMenuScene::ImGuiRender()
	{
		DrawMainMenu();
	}
	
	void MainMenuScene::DrawMainMenu()
	{
		ImGuiIO& io = ImGui::GetIO();
		ImVec2 windowSize = io.DisplaySize;

		// --- FullScreen Window ---
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
		ImGui::SetNextWindowSize(windowSize);
		ImGui::Begin("MainMenu", NULL,
			ImGuiWindowFlags_NoDecoration |
			ImGuiWindowFlags_NoBackground |
			ImGuiWindowFlags_NoMove);

		// --- TITLE TEXT ---
		ImGui::PushFont(mFontLight, 60);
		const char* title = "DAWN";
		float titleTextWidth = ImGui::CalcTextSize(title).x;
		float titleTexPosX = (windowSize.x - titleTextWidth) / 2.0f;
		ImGui::SetCursorPos(ImVec2(titleTexPosX, 200));
		ImGui::Text(title);
		ImGui::PopFont();

		// --- MAIN MENU ---
		ImGui::PushFont(mFontLight, 30);

		float buttonHeight = ImGui::CalcTextSize("DUMMY").y + 4;
		ImVec2 buttonSize(200, buttonHeight);

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0.8f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0.3f));

		ImGui::SetCursorPosX((windowSize.x - buttonSize.x) / 2.0f);
		ImGui::SetCursorPosY(windowSize.y - 5 * buttonSize.y);
		if (ImGui::Button("START", buttonSize))
		{
			Application::Get()->GetAudioSystem()->PlayEvent("event:/button_click");
			Application::Get()->LoadScene<GameScene>();
		}
		if (!mStartButtonHovered && ImGui::IsItemHovered())
		{
			Application::Get()->GetAudioSystem()->PlayEvent("event:/button_hover");
			mStartButtonHovered = true;
		}
		else if (!ImGui::IsItemHovered())
			mStartButtonHovered = false;

		ImGui::SetCursorPosX((windowSize.x - buttonSize.x) / 2.0f);
		if (ImGui::Button("QUIT", buttonSize))
		{
			Application::Get()->GetAudioSystem()->PlayEvent("event:/button_click");
			Application::Get()->Quit();
		}
		if (!mQuitButtonHovered && ImGui::IsItemHovered())
		{
			Application::Get()->GetAudioSystem()->PlayEvent("event:/button_hover");
			mQuitButtonHovered = true;
		}
		else if (!ImGui::IsItemHovered())
			mQuitButtonHovered = false;

		ImGui::PopStyleColor(3);
		ImGui::PopFont();

		ImGui::End();
	}
}