#include "DawnGame.h"

#include <imgui/imgui.h>
#include "Core/Application.h"
#include "Input/Input.h"
#include "Actors/EnemySpawner.h"
#include "Scenes/GameScene.h"

namespace Dawn
{
	DawnGame::DawnGame()
		:mState(GameState::Gameplay)
	{
	}

	DawnGame::~DawnGame()
	{
	}
	
	void DawnGame::Start()
	{
		Application::Get()->LoadScene<GameScene>();
		Input::SetCursorLocked(true);

		ImGuiIO& io = ImGui::GetIO();
		mRobotoRegular = io.Fonts->AddFontFromFileTTF("Assets/Fonts/Roboto-Regular.ttf");
		mRobotoBold = io.Fonts->AddFontFromFileTTF("Assets/Fonts/Roboto-ExtraBold.ttf");
	}
	
	void DawnGame::Update(float deltaTime)
	{
		if (Input::GetKeyDown(Key::Escape))
		{
			if (mState == GameState::Gameplay)
			{
				// Pause the game
				mState = GameState::Paused;
				Application::Get()->GetScene()->SetPaused(true);
				Input::SetCursorLocked(false);

			}
			else if (mState == GameState::Paused)
			{
				// Resume the game
				mState = GameState::Gameplay;
				Application::Get()->GetScene()->SetPaused(false);
				Input::SetCursorLocked(true);
			}
		}
	}
	
	void DawnGame::ImGuiRender()
	{
		//ImGui::ShowDemoWindow();

		if (mState == GameState::Paused)
			DrawPauseWindow();
		else if (mState == GameState::Gameplay)
			DrawKillCounter();
	}

	void DawnGame::DrawPauseWindow()
	{
		// Shouldn't be done every frame
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
			mState = GameState::Gameplay;
			Application::Get()->GetScene()->SetPaused(false);
			Input::SetCursorLocked(true);
		}

		ImGui::Dummy(ImVec2(0, 10)); // spacing

		ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);
		if (ImGui::Button("Exit", ImVec2(buttonWidth, 50)))
		{
			// Temporary
			// TODO: proper closure
			exit(0);
		}

		ImGui::PopFont();

		ImGui::PopStyleColor(4);

		ImGui::End();	// Pause Menu
		ImGui::End();	// Overlay
	}

	void DawnGame::DrawKillCounter()
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
		// Since this part only runs when the state is Gameplay
		// We know the Scene is GameScene
		if (GameScene* scene = dynamic_cast<GameScene*>(Application::Get()->GetScene()))
		{
			ImGui::Text("Kill Count: %d", scene->GetEnemySpawner()->GetEnemiesKilled());
		}
		ImGui::PopFont();

		ImGui::End();
		ImGui::PopStyleVar(2);
	}
}