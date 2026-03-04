#include "DawnGameLayer.h"

#include "Utils/Log.h"
#include "Core/Scene.h"

#include <Vendor/imgui/imgui.h>

#include "Input/Input.h"
#include "Core/Components/Camera.h"
#include "ExampleActor.h"
#include "Actors/FPSCameraActor.h"
#include "Actors/PlayerActor.h"
#include "Actors/GroundPlane.h"
#include "Actors/EnemySpawner.h"

namespace Dawn
{
	void DawnGameLayer::OnAttach()
	{
		LOG_WARN("Example Layer attached!");

		// Right now creating the scene manually
		// TODO: via serialization, load scene object
		Scene* scene = new Scene("Assets/Scenes/scene.scene");
		LoadScene(scene);

		Input::SetCursorLocked(true);

		// TEST ACTOR
		ExampleActor* ea = new ExampleActor(GetScene());
		ea->SetPosition(glm::vec3(5, 0, -5));
		ea->SetScale(glm::vec3(0.6f));

		// GROUND
		GroundPlane* ground = new GroundPlane(mScene);
		// PLAYER
		FPSCameraActor* cameraActor = new FPSCameraActor(mScene);
		PlayerActor* player = new PlayerActor(mScene, cameraActor);
		player->SetPosition(glm::vec3(0, 0, 4));
		// ENEMY SPAWNER
		mEnemySpawner = new EnemySpawner(mScene, player);

		Camera* cam = cameraActor->GetComponent<Camera>();
		if (cam)
		{
			GetScene()->SetActiveCamera(cam);
		}
	}
	void DawnGameLayer::OnDetach()
	{
		LOG_WARN("Example Layer detached!");
	}

	void DawnGameLayer::OnGameUpdate(float deltaTime)
	{
		if (Input::GetKeyDown(Key::Escape))
		{
			if (mState == State::Gameplay)
			{
				// Pause the game
				mState = State::Paused;
				Input::SetCursorLocked(false);

			}
			else if (mState == State::Paused)
			{
				// Resume the game
				mState = State::Gameplay;
				Input::SetCursorLocked(true);
			}
		}
	}
	void DawnGameLayer::OnImGuiRender()
	{
		if (mState == State::Paused)
		{
			DrawPauseWindow();
			//ImGui::ShowDemoWindow();
		}
		else
		{
			DrawKillCounter();
		}
	}

	void DawnGameLayer::DrawKillCounter()
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

		ImGui::PushFont(NULL, 18.0f);
		ImGui::Text("Kill Count: %d", mEnemySpawner->GetEnemiesKilled());
		ImGui::PopFont();

		ImGui::End();
		ImGui::PopStyleVar(2);
	}

	void DawnGameLayer::DrawPauseWindow()
	{
		// Shouldn't be done every frame
		ImGuiIO& io = ImGui::GetIO();
		ImFont* robotoRegular = io.Fonts->AddFontFromFileTTF("Assets/Fonts/Roboto-Regular.ttf");
		ImFont* robotoBold = io.Fonts->AddFontFromFileTTF("Assets/Fonts/Roboto-ExtraBold.ttf");

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
		ImGui::PushFont(robotoBold, 50.0f);
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
		ImGui::PushFont(robotoRegular, 25.0f);

		if (ImGui::Button("Resume", ImVec2(buttonWidth, 50)))
		{
			// Resume the game
			mState = State::Gameplay;
			Input::SetCursorLocked(true);
		}

		ImGui::Dummy(ImVec2(0, 10)); // spacing

		ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);
		if (ImGui::Button("Exit", ImVec2(buttonWidth, 50)))
		{
			// Temporary
			exit(0);
		}

		ImGui::PopFont();

		ImGui::PopStyleColor(4);

		ImGui::End();	// Pause Menu
		ImGui::End();	// Overlay
	}
}