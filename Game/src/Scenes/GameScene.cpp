#include "GameScene.h"

#include "ExampleActor.h"
#include "Actors/GroundPlane.h"
#include "Actors/EnemySpawner.h"
#include "Actors/PlayerActor.h"
#include "Actors/FPSCameraActor.h"
#include "Core/Components/Camera.h"
#include "Core/Application.h"
#include "Input/Input.h"
#include "MainMenuScene.h"

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
		mFontBold = io.Fonts->AddFontFromFileTTF("Assets/Fonts/Montserrat-SemiBold.ttf");
		mFontRegular = io.Fonts->AddFontFromFileTTF("Assets/Fonts/Montserrat-Regular.ttf");
		mFontLight = io.Fonts->AddFontFromFileTTF("Assets/Fonts/Montserrat-Regular.ttf");

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
		if (Input::GetKeyDown(Key::Escape) && !mIsGameOver)
		{
			SetPaused(!IsPaused());
			Input::SetCursorLocked(!IsPaused());
		}
	}
	
	void GameScene::ImGuiRender()
	{
		DrawKillCounter();
		DrawHealthBar();

		if (IsPaused() && !mIsGameOver)
			DrawPauseWindow();
		else if (mIsGameOver);
			// TODO: show game over screen
	}

	void GameScene::GameOver()
	{
		SetPaused(true);
		Input::SetCursorLocked(!false);
		mIsGameOver = true;
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
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0.8f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0.3f));

		// --- Pause Menu Window ---
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

		ImGui::Begin("Pause Menu", NULL,
			ImGuiWindowFlags_NoDecoration |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_AlwaysAutoResize);

		float windowWidth = ImGui::GetWindowSize().x;

		// --- Title ---
		ImGui::PushFont(mFontRegular, 45.0f);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.5f, 0.15f, 1.0f));
		const char* title = "PAUSED";
		float textWidth = ImGui::CalcTextSize(title).x;
		ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);

		ImGui::Text(title);
		ImGui::PopStyleColor();
		ImGui::PopFont();

		ImGui::Dummy(ImVec2(0, 20));

		// --- Buttons ---
		ImGui::PushFont(mFontLight, 25.0f);
		float buttonHeight = ImGui::CalcTextSize("DUMMY").y + 4;
		float buttonWidth = 200.0f;
		ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);

		if (ImGui::Button("RESUME", ImVec2(buttonWidth, buttonHeight)))
		{
			// Resume the game
			Application::Get()->GetScene()->SetPaused(false);
			Input::SetCursorLocked(true);
		}

		ImGui::Dummy(ImVec2(0, 10)); // spacing

		ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);
		if (ImGui::Button("EXIT", ImVec2(buttonWidth, buttonHeight)))
		{
			Application::Get()->LoadScene<MainMenuScene>();
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

		ImGui::PushFont(mFontRegular, 20.0f);
		ImGui::Text("Score: %d", mEnemySpawner->GetEnemiesKilled());
		ImGui::PopFont();

		ImGui::End();
		ImGui::PopStyleVar(2);
	}

	void GameScene::DrawHealthBar()
	{
		ImGui::SetNextWindowPos(ImVec2(10, 10));
		ImGui::SetNextWindowSize(ImVec2(200, 20));

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(10, 10));
		ImGui::SetNextWindowBgAlpha(0.2f);

		ImGui::Begin("HealthBarWindow", NULL,
			ImGuiWindowFlags_NoDecoration |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoResize);

		ImDrawList* drawList = ImGui::GetWindowDrawList();

		ImVec2 pos = ImGui::GetCursorScreenPos();
		ImVec2 size = ImGui::GetContentRegionAvail();

		// Background
		drawList->AddRectFilled(
			pos,
			ImVec2(pos.x + size.x, pos.y + size.y),
			IM_COL32(40, 40, 40, 255)
		);

		// Health
		float fraction = mPlayerHealth / mMaxPlayerHealth;
		drawList->AddRectFilled(
			pos,
			ImVec2(pos.x + fraction * size.x, pos.y + size.y),
			IM_COL32(40, 200, 40, 255)
		);
		ImGui::Dummy(size);

		ImGui::End();
		ImGui::PopStyleVar(2);
	}
}