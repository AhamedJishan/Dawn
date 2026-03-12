#include "MainMenuScene.h"

#include "Input/Input.h"
#include "Core/Assets.h"
#include "GameScene.h"


#include "Core/Application.h"
#include "Audio/AudioSystem.h"

namespace Dawn
{
	MainMenuScene::~MainMenuScene()
	{
		if (mBgmSE.IsValid())
			mBgmSE.Stop();
	}

	void MainMenuScene::Init()
	{
		Input::SetCursorLocked(false);

		mBGTexture = Assets::GetTexture("Assets/Textures/MainMenuBG_AI.png");

		ImGuiIO& io = ImGui::GetIO();
		mFontBold = io.Fonts->AddFontFromFileTTF("Assets/Fonts/Montserrat-SemiBold.ttf");
		mFontRegular = io.Fonts->AddFontFromFileTTF("Assets/Fonts/Montserrat-Regular.ttf");
		mFontLight = io.Fonts->AddFontFromFileTTF("Assets/Fonts/Montserrat-Regular.ttf");

		ImGuiStyle& style = ImGui::GetStyle();
		style.FrameRounding = 6.0f;

		Application::Get()->GetAudioSystem()->LoadBank("Assets/Audio/MainMenu.bank");
		Application::Get()->GetAudioSystem()->LoadBank("Assets/Audio/UI.bank");
		mBgmSE = Application::Get()->GetAudioSystem()->PlayEvent("event:/mainmenu_bgm");
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

		// --- BACKGROUND IMAGE ---
		float imgWidth = mBGTexture->GetWidth();
		float imgHeight = mBGTexture->GetHeight();

		float windowAspect = windowSize.x / windowSize.y;
		float imgAspect = imgWidth / imgHeight;

		ImVec2 uv0 = ImVec2(0.0f, 0.0f);
		ImVec2 uv1 = ImVec2(1.0f, 1.0f);

		if (windowAspect > imgAspect)
		{
			// Window is wider than the image. Fit width, crop top/bottom.
			float scale = windowSize.x / imgWidth;
			// between 0-1 how much of top should be cropped
			float vOffset = ((scale * imgHeight) - windowSize.y) / (2 * scale * imgHeight);
			uv0.y = vOffset;
			uv1.y = 1.0f - vOffset;
		}
		else
		{
			// Window is taller than the image. Fit height, crop left/right.
			float scale = windowSize.y / imgHeight;
			// between 0-1 how much of left should be cropped
			float uOffset = ((scale * imgWidth) - windowSize.x) / (2 * scale * imgWidth);
			uv0.x = uOffset;
			uv1.x = 1.0f - uOffset;
		}
		// flip the image vertically
		uv0.y = 1 - uv0.y;
		uv1.y = 1 - uv1.y;

		ImGui::SetCursorPos(ImVec2(0, 0));
		ImGui::Image((ImTextureID)(intptr_t)mBGTexture->GetId(), windowSize, uv0, uv1);

		// --- TITLE TEXT ---
		ImGui::PushFont(mFontRegular, 75);
		const char* title = "DAWN";
		float titleTextWidth = ImGui::CalcTextSize(title).x;
		float titleTexPosX = (windowSize.x - titleTextWidth) / 2.0f;
		ImGui::SetCursorPos(ImVec2(titleTexPosX, 50));
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