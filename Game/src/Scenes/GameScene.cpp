#include "GameScene.h"

#include "ExampleActor.h"
#include "Actors/GroundPlane.h"
#include "Actors/EnemySpawner.h"
#include "Actors/PlayerActor.h"
#include "Actors/FPSCameraActor.h"
#include "Core/Components/Camera.h"
#include "Core/Application.h"
#include "Input/Input.h"

namespace Dawn
{
	GameScene::GameScene()
	{
	}

	GameScene::~GameScene()
	{
	}
	
	void GameScene::Init()
	{
		Input::SetCursorLocked(true);

		ImGuiIO& io = ImGui::GetIO();
		mRobotoRegular = io.Fonts->AddFontFromFileTTF("Assets/Fonts/Roboto-Regular.ttf");
		mRobotoBold = io.Fonts->AddFontFromFileTTF("Assets/Fonts/Roboto-ExtraBold.ttf");

		// TEST ACTOR
		ExampleActor* ea = new ExampleActor(this);
		ea->SetPosition(glm::vec3(5, 0, -5));
		ea->SetScale(glm::vec3(0.6f));

		// GROUND
		GroundPlane* ground = new GroundPlane(this);
		// PLAYER
		FPSCameraActor* cameraActor = new FPSCameraActor(this);
		PlayerActor* player = new PlayerActor(this, cameraActor);
		player->SetPosition(glm::vec3(0, 0, 4));
		// ENEMY SPAWNER
		mEnemySpawner = new EnemySpawner(this, player);

		Camera* cam = cameraActor->GetComponent<Camera>();
		if (cam)
		{
			SetActiveCamera(cam);
		}
	}

	void GameScene::Update(float deltaTime)
	{
		if (Input::GetKeyDown(Key::Escape))
		{
			SetPaused(!IsPaused());
			Input::SetCursorLocked(!IsPaused());
		}
	}
	
	void GameScene::ImGuiRender()
	{
		DrawKillCounter();

		if (IsPaused())
			DrawPauseWindow();
	}

	void GameScene::DrawPauseWindow()
	{
		ImGuiIO& io = ImGui::GetIO();

		// --- FullScreen dark overlay ---
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(io.DisplaySize);

		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0.5f));

		ImGui::Begin("Overlay", NULL,
			ImGuiWindowFlags_NoDecoration |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoBringToFrontOnFocus);

		ImGui::PopStyleColor();

		// --- Style Tweaks
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowBorderSize = 0;
		style.FrameRounding = 6.0f;
		style.FrameBorderSize = 0;

		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.15f, 0.15f, 0.15f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.85f, 0.40f, 0.05f, 0.9f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.70f, 0.30f, 0.05f, 1.0f));

		// --- Pause Menu Window ---
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

		ImGui::Begin("Pause Menu", NULL,
			ImGuiWindowFlags_NoDecoration |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_AlwaysAutoResize);

		float windowWidth = ImGui::GetWindowSize().x;

		// --- Title ---
		ImGui::PushFont(mRobotoBold, 50.0f);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.5f, 0.15f, 1.0f));
		const char* title = "Paused";
		float textWidth = ImGui::CalcTextSize(title).x;
		ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);

		ImGui::Text(title);
		ImGui::PopStyleColor();
		ImGui::PopFont();

		ImGui::Dummy(ImVec2(0, 20));

		// --- Buttons ---
		float buttonWidth = 200.0f;

		ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);
		ImGui::PushFont(mRobotoRegular, 25.0f);

		if (ImGui::Button("Resume", ImVec2(buttonWidth, 50)))
		{
			// Resume the game
			Application::Get()->GetScene()->SetPaused(false);
			Input::SetCursorLocked(true);
		}

		ImGui::Dummy(ImVec2(0, 10)); // spacing

		ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);
		if (ImGui::Button("Exit", ImVec2(buttonWidth, 50)))
		{
			Application::Get()->Quit();
		}

		ImGui::PopFont();

		ImGui::PopStyleColor(4);

		ImGui::End();	// Pause Menu
		ImGui::End();	// Overlay
	}
	
	void GameScene::DrawKillCounter()
	{
		ImGuiIO& io = ImGui::GetIO();
		float windowCenterX = ImGui::GetMainViewport()->GetCenter().x;

		ImGui::SetNextWindowBgAlpha(0.4f);
		ImGui::SetNextWindowPos(ImVec2(windowCenterX, 0.0f), ImGuiCond_Always, ImVec2(0.5f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);

		ImGui::Begin("KillCounter", NULL,
			ImGuiWindowFlags_NoDecoration |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_AlwaysAutoResize);

		ImGui::PushFont(mRobotoRegular, 18.0f);
		ImGui::Text("Score: %d", mEnemySpawner->GetEnemiesKilled());
		ImGui::PopFont();

		ImGui::End();
		ImGui::PopStyleVar(2);
	}
}