#include "GameScene.h"

#include <glm/vec3.hpp>
#include "ExampleActor.h"
#include "Actors/GroundPlane.h"
#include "Actors/WaveManager.h"
#include "Actors/PlayerActor.h"
#include "Actors/FPSCameraActor.h"
#include "Actors/Gun.h"
#include "Actors/SkyDome.h"
#include "Core/Components/Camera.h"
#include "Core/Application.h"
#include "Audio/AudioSystem.h"
#include "Input/Input.h"
#include "MainMenuScene.h"

namespace Dawn
{
	GameScene::GameScene()
	{
		GetEnvironmentSettings().fogDensity = 0.03f;
		GetEnvironmentSettings().fogColor = glm::vec3(0.12f, 0.15f, 0.20f);
		GetEnvironmentSettings().ambientColor = glm::vec3(0.4f);
		GetEnvironmentSettings().directionalLight.color = glm::vec3(0.6f, 0.7f, 0.85f);
		GetEnvironmentSettings().directionalLight.direction = glm::vec3(-0.5f, -0.8f, -0.2f);;
	}

	GameScene::~GameScene()
	{
		if (mBgmSE.IsValid())
			mBgmSE.Stop();

		Application::Get()->GetAudioSystem()->UnloadBank("Assets/Audio/Game.bank");
	}
	
	void GameScene::Init()
	{
		Input::SetCursorLocked(true);

		ImGuiIO& io = ImGui::GetIO();
		mFontBold = io.Fonts->AddFontFromFileTTF("Assets/Fonts/Montserrat-SemiBold.ttf");
		mFontRegular = io.Fonts->AddFontFromFileTTF("Assets/Fonts/Montserrat-Regular.ttf");
		mFontLight = io.Fonts->AddFontFromFileTTF("Assets/Fonts/Montserrat-Light.ttf");

		// Sky dome
		SkyDome* sky = new SkyDome(this);

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
		Gun* gun = new Gun(this, player);

		// --- WAVE MANAGER ---
		mWaveManager = new WaveManager(this, player);

		Camera* cam = cameraActor->GetComponent<Camera>();
		if (cam)
		{
			cam->SetFarPlane(500.0f);	// So that skydome is visible
			SetActiveCamera(cam);
		}

		Application::Get()->GetAudioSystem()->LoadBank("Assets/Audio/Game.bank");
		Application::Get()->GetAudioSystem()->LoadBank("Assets/Audio/UI.bank");
		mBgmSE = Application::Get()->GetAudioSystem()->PlayEvent("event:/game_bgm");
	}

	void GameScene::Update(float deltaTime)
	{
		if (Input::GetKeyDown(Key::Escape) && !mIsGameOver)
		{
			SetPaused(!IsPaused());
			Input::SetCursorLocked(!IsPaused());
		}

		if (mIsGameOver)
			mTimeSinceGameOver += deltaTime;
	}
	
	void GameScene::ImGuiRender()
	{
		if (!mIsGameOver && !IsPaused())
		{
			DrawWaveUI();
			DrawCrossHair();
			DrawHealthBar();
		}

		if (IsPaused() && !mIsGameOver)
			DrawPauseWindow();
		else if (mIsGameOver && mWaveManager->GetWaveState() != WaveState::End)
			DrawGameOver(false);
		else if (mIsGameOver && mWaveManager->GetWaveState() == WaveState::End)
			DrawGameOver(true);
	}

	void GameScene::GameOver()
	{
		SetPaused(true);
		Input::SetCursorLocked(false);
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
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, ImVec2(6, 6));
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, ImVec2(0, 0));

		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.5f, 0.15f, 0.8f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.5f, 0.15f, 0.3f));

		// --- Pause Menu Window ---
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

		ImGui::Begin("Pause Menu", NULL,
			ImGuiWindowFlags_NoDecoration |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_AlwaysAutoResize);

		float windowWidth = ImGui::GetWindowSize().x;

		// --- Title ---
		ImGui::PushFont(mFontBold, 50.0f);
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
			Application::Get()->GetAudioSystem()->PlayEvent("event:/button_click");
			Application::Get()->GetScene()->SetPaused(false);
			Input::SetCursorLocked(true);
		}
		if (!mResumeButtonHovered && ImGui::IsItemHovered())
		{
			Application::Get()->GetAudioSystem()->PlayEvent("event:/button_hover");
			mResumeButtonHovered = true;
		}
		else if (!ImGui::IsItemHovered())
			mResumeButtonHovered = false;

		ImGui::Dummy(ImVec2(0, 10)); // spacing

		ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);
		if (ImGui::Button("MAIN MENU", ImVec2(buttonWidth, buttonHeight)))
		{
			Application::Get()->GetAudioSystem()->PlayEvent("event:/button_click");
			Application::Get()->LoadScene<MainMenuScene>();
		}
		if (!mMainMenuButtonHovered && ImGui::IsItemHovered())
		{
			Application::Get()->GetAudioSystem()->PlayEvent("event:/button_hover");
			mMainMenuButtonHovered = true;
		}
		else if (!ImGui::IsItemHovered())
			mMainMenuButtonHovered = false;

		ImGui::PopFont();

		ImGui::PopStyleColor(4);
		ImGui::PopStyleVar(3);

		ImGui::End();	// Pause Menu
		ImGui::End();	// Overlay
	}

	void GameScene::DrawHealthBar()
	{
		ImDrawList* drawList = ImGui::GetForegroundDrawList();

		ImVec2 pos(10, 10);
		ImVec2 size(200, 20);

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
	}

	void GameScene::DrawCrossHair()
	{
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImDrawList* drawList = ImGui::GetForegroundDrawList();
		drawList->AddCircleFilled(center, 2.0f, IM_COL32(255, 255, 255, 255), 8);
	}

	void GameScene::DrawGameOver(bool win)
	{
		ImGuiIO& io = ImGui::GetIO();

		// --- BackGround Window ---
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(io.DisplaySize);
		
		float alpha = mTimeSinceGameOver / mGameOverTransitionDuration;
		alpha = glm::clamp(alpha, 0.0f, 1.0f);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, alpha));

		ImGui::Begin("Game Over Window", NULL,
			ImGuiWindowFlags_NoDecoration |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoMove);

		ImGui::PopStyleColor();

		// --- GAME OVER TEXT ---
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();

		ImGui::PushFont(mFontBold, 50.0f);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.5f, 0.15f, 1.0f));
		const char* gameOverText;
		if (win)
			gameOverText = "YOU WIN";
		else
			gameOverText = "GAME OVER";
		ImVec2 textSize = ImGui::CalcTextSize(gameOverText);
		ImGui::SetCursorPos(ImVec2(center.x - textSize.x/2.0f, center.y - 100));

		ImGui::Text(gameOverText);
		
		ImGui::PopStyleColor();
		ImGui::PopFont();

		// --- BUTTONS ---
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.5f, 0.15f, 0.8f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.5f, 0.15f, 0.3f));

		float windowWidth = ImGui::GetWindowSize().x;

		ImGui::PushFont(mFontLight, 25.0f);
		float buttonHeight = ImGui::CalcTextSize("DUMMY").y + 4;
		float buttonWidth = 200.0f;
		ImGui::SetCursorPosY(center.y + 100);
		ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);

		// --- RESTART GAME ---
		if (ImGui::Button("RESTART", ImVec2(buttonWidth, buttonHeight)))
		{
			Application::Get()->GetAudioSystem()->PlayEvent("event:/button_click");
			Application::Get()->LoadScene<GameScene>();
			Input::SetCursorLocked(true);
		}
		if (!mRestartButtonHovered && ImGui::IsItemHovered())
		{
			Application::Get()->GetAudioSystem()->PlayEvent("event:/button_hover");
			mRestartButtonHovered = true;
		}
		else if (!ImGui::IsItemHovered())
			mRestartButtonHovered = false;

		ImGui::Dummy(ImVec2(0, 10)); 
		// --- EXIT TO MAIN MENU ---
		ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);
		if (ImGui::Button("MAIN MENU", ImVec2(buttonWidth, buttonHeight)))
		{
			Application::Get()->GetAudioSystem()->PlayEvent("event:/button_click");
			Application::Get()->LoadScene<MainMenuScene>();
		}
		if (!mMainMenuButtonHovered && ImGui::IsItemHovered())
		{
			Application::Get()->GetAudioSystem()->PlayEvent("event:/button_hover");
			mMainMenuButtonHovered = true;
		}
		else if (!ImGui::IsItemHovered())
			mMainMenuButtonHovered = false;

		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::End();
	}

	void GameScene::DrawWaveUI()
	{
		WaveState waveState = mWaveManager->GetWaveState();
		unsigned int currentWaveNumber = mWaveManager->GetCurrentWaveIndex() + 1;

		ImGuiIO& io = ImGui::GetIO();
		float windowCenterX = ImGui::GetMainViewport()->GetCenter().x;

		ImGui::SetNextWindowBgAlpha(0.4f);
		ImGui::SetNextWindowPos(ImVec2(windowCenterX, 0.0f), ImGuiCond_Always, ImVec2(0.5f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);

		ImGui::Begin("Wave UI", NULL,
			ImGuiWindowFlags_NoDecoration |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_AlwaysAutoResize);

		switch (waveState)
		{
		case Dawn::WaveState::Begin:
			break;
		case Dawn::WaveState::WaveStarting:
		{
			ImGui::PushFont(mFontRegular, 20.0f);
			int waveTimer = (int)std::ceil(mWaveManager->GetWaveCountdown());
			ImGui::Text("WAVE %d STARTS IN: %d", currentWaveNumber, waveTimer);
			ImGui::PopFont();
			break;
		}
		case Dawn::WaveState::WaveActive:
		{
			ImGui::PushFont(mFontRegular, 20.0f);
			unsigned int enemiesRemaining = mWaveManager->GetWaveEnemiesRemaining();
			ImGui::Text("WAVE %d | ENEMIES REMAINING: %d", currentWaveNumber, enemiesRemaining);
			ImGui::PopFont();
			break;
		}
		case Dawn::WaveState::WaveClear:
			break;
		case Dawn::WaveState::End:
			GameOver();
			break;
		default:
			break;
		}

		ImGui::End();
		ImGui::PopStyleVar(2);
	}
}