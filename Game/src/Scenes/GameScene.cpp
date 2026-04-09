#include "GameScene.h"

#include <string>
#include <glm/vec3.hpp>
#include "Actors/Arena.h"
#include "Actors/WaveManager.h"
#include "Actors/Player.h"
#include "Actors/FPSCameraActor.h"
#include "Actors/Gun.h"
#include "Actors/SkyDome.h"
#include "Core/Components/Camera.h"
#include "Core/Application.h"
#include "Audio/AudioSystem.h"
#include "Input/Input.h"
#include "MainMenuScene.h"
#include "Actors/UpgradeManager.h"

namespace Dawn
{
	GameScene::GameScene()
	{
		GetEnvironmentSettings().bloomRadius = 2.0f;
		GetEnvironmentSettings().bloomStrength = 0.01f;
		GetEnvironmentSettings().fogDensity = 0.06f;
		GetEnvironmentSettings().fogColor = glm::vec3(0.02f, 0.03f, 0.04f);
		GetEnvironmentSettings().ambientColor = glm::vec3(0.25f);
		GetEnvironmentSettings().directionalLight.color = glm::vec3(0.6f, 0.7f, 0.85f);
		GetEnvironmentSettings().directionalLight.direction = glm::vec3(-0.5f, -0.2f, -0.2f);
		GetEnvironmentSettings().directionalLight.intensity = 1.0f;
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

		// ARENA
		Arena* arena = new Arena(this);

		// PLAYER
		FPSCameraActor* cameraActor = new FPSCameraActor(this);
		Player* player = new Player(this, cameraActor, arena);
		player->SetPosition(glm::vec3(0, 0, 4));
		Gun* gun = new Gun(this, player);

		// UPGRADE MANAGER
		mUpgradeManager = new UpgradeManager(this, player, gun);

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
			DrawHealthBar();
			if (mWaveManager->GetWaveState() == WaveState::WaveActive)
				DrawCrossHair();
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
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0);

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
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.85f, 1.0f, 1.0f, 1.0f));
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
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.85f, 1.0f, 1.0f, 1.0f));
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
		float viewPortCenterX = ImGui::GetMainViewport()->GetCenter().x;
		float viewPortCenterY = ImGui::GetMainViewport()->GetCenter().y;

		ImGui::SetNextWindowBgAlpha(0.0f);
		ImGui::SetNextWindowPos(ImVec2(viewPortCenterX, 0.0f), ImGuiCond_Always, ImVec2(0.5f, 0.0f));
		ImGui::SetNextWindowSize(io.DisplaySize);
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
			float waveTimer = mWaveManager->GetWaveTimer();
			const float waveStartDuration = mWaveManager->GetWaveStartDuration();
			float alpha = glm::sin((waveTimer / waveStartDuration) * glm::pi<float>());
			ImGui::PushFont(mFontLight, 45.0f);
			std::string waveText = "WAVE " + std::to_string(currentWaveNumber);
			ImVec2 textSize = ImGui::CalcTextSize(waveText.c_str());
			float x = viewPortCenterX - textSize.x / 2.0f;
			float y = viewPortCenterY - textSize.y;
			ImGui::SetCursorPos(ImVec2(x, y));
			ImGui::TextColored(ImVec4(0.8f, 1, 1, alpha), waveText.c_str());
			ImGui::PopFont();
			break;
		}
		case Dawn::WaveState::WaveActive:
		{
			unsigned int enemiesRemaining = mWaveManager->GetWaveEnemiesRemaining();
			ImGui::PushFont(mFontRegular, 25.0f);
			ImGui::SetCursorPosX(viewPortCenterX);
			ImGui::Text("%d", enemiesRemaining);
			ImGui::PopFont();
			break;
		}
		case Dawn::WaveState::WaveClear:
		{
			// Background
			ImDrawList* drawList = ImGui::GetBackgroundDrawList();

			drawList->AddRectFilled(ImVec2(viewPortCenterX - 300, viewPortCenterY - 200),
				ImVec2(viewPortCenterX + 300, viewPortCenterY + 200),
				ImGui::GetColorU32(ImVec4(0.1f, 0.1f, 0.1f, 0.3f)), 40.0f);

			float timer = mWaveManager->GetWaveTimer();
			float duration = mWaveManager->GetWaveClearDuration();
			int timeLeft = (int)ceil(duration - timer);

			// Title
			ImGui::PushFont(mFontLight, 35.0f);
			std::string title = "CHOOSE UPGRADE (" + std::to_string(timeLeft) + ")";
			ImVec2 titleSize = ImGui::CalcTextSize(title.c_str());
			ImGui::SetCursorPos(ImVec2(viewPortCenterX - titleSize.x / 2.0f, viewPortCenterY - 100));
			ImGui::TextColored(ImVec4(0.8f, 1, 1, 1), title.c_str());
			ImGui::PopFont();

			ImGui::PushFont(mFontRegular, 22.0f);

			float startY = viewPortCenterY - 20;

			ImVec4 normal = ImVec4(1, 1, 1, 1);
			ImVec4 highlight = ImVec4(1.0f, 0.8f, 0.2f, 1);
			ImVec4 max = ImVec4(0.85f, 0.30f, 0.14f, 1);

			bool canUpgradeSpread = mUpgradeManager->IsSpreadUpgradeable();
			bool canUpgradeDamage= mUpgradeManager->IsDamageUpgradeable();
			bool canUpgradeDash = mUpgradeManager->IsDashUpgradeable();

			// Option 1
			ImGui::SetCursorPos(ImVec2(viewPortCenterX - 80, startY));
			ImGui::TextColored(canUpgradeDamage ? (Input::GetKey(Key::Num1) ? highlight : normal) : max, "[1] DAMAGE");
			// Option 2
			ImGui::SetCursorPos(ImVec2(viewPortCenterX - 80, startY + 40));
			ImGui::TextColored(canUpgradeSpread ? (Input::GetKey(Key::Num2) ? highlight : normal) : max, "[2] SPREAD");
			// Option 3
			ImGui::SetCursorPos(ImVec2(viewPortCenterX - 80, startY + 80));
			ImGui::TextColored(canUpgradeDash ? (Input::GetKey(Key::Num3) ? highlight : normal) : max, "[3] DASH");

			ImGui::PopFont();

			if (Input::GetKeyUp(Key::Num1) && canUpgradeDamage)
			{
				mUpgradeManager->UpgradeDamage();
				mWaveManager->SkipWaveClearDelay();
			}
			if (Input::GetKeyUp(Key::Num2) && canUpgradeSpread)
			{
				mUpgradeManager->UpgradeSpread();
				mWaveManager->SkipWaveClearDelay();
			}
			if (Input::GetKeyUp(Key::Num3) && canUpgradeDash)
			{
				mUpgradeManager->UpgradeDash();
				mWaveManager->SkipWaveClearDelay();
			}
			break;
		}
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