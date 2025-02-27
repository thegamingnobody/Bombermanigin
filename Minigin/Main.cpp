#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Scene.h"
#include "TextureComponent.h"
#include "TextComponent.h"
#include "FPSComponent.h"
#include "RotatorComponent.h"
#include "ImGuiComponent.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	//*-----------------------------------------*
	//|				Background stuff			|
	//*-----------------------------------------*
	auto go = std::make_shared<dae::GameObject>("Background Texture");
	{
		auto& textureComponent = go->AddComponent<dae::TextureComponent>(*go.get());
		textureComponent.AddTexture("background.tga");
	}
	scene.Add(go);

	go = std::make_shared<dae::GameObject>("Logo Texture");
	{
		auto& textureComponent = go->AddComponent<dae::TextureComponent>(*go.get());
		textureComponent.AddTexture("logo.tga");
	}
	go->GetTransform()->SetLocalPosition(216.0f, 180.0f);
	scene.Add(go);

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	go = std::make_shared<dae::GameObject>("Title 'Programming 4 Assignment' ");
	go->AddComponent<dae::TextComponent>(*go.get(), "Programming 4 Assignment", font);
	go->GetTransform()->SetLocalPosition(80.0f, 20.0f);
	scene.Add(go);

	go = std::make_shared<dae::GameObject>("FPS Counter");
	go->AddComponent<dae::TextComponent>(*go.get(), " ", font);
	go->AddComponent<dae::FPSComponent>(*go.get());
	go->GetTransform()->SetLocalPosition(20.0f, 430.0f);
	scene.Add(go);

	//*-----------------------------------------*
	//|				     Player					|
	//*-----------------------------------------*

	go = std::make_shared<dae::GameObject>("Player");
	{
		auto& textureComponent = go->AddComponent<dae::TextureComponent		>(*go.get());
		textureComponent.AddTexture("Bomberman_S_1.png");
	}
	go->AddComponent<dae::RotatorComponent>(*go.get(), 360.0f / 0.10f, 25.0f);
	scene.Add(go);

	auto goParent = std::make_shared<dae::GameObject>("ParentOfPlayer");
	goParent->GetTransform()->SetLocalPosition(640.0f/2-16.0f, 480.0f/2-16.0f-50.0f);
	go->SetParentObject(goParent.get(), true);
	scene.Add(goParent);

	auto go2 = std::make_shared<dae::GameObject>("Player2");
	{
		auto& textureComponent = go2->AddComponent<dae::TextureComponent>(*go2.get());
		textureComponent.AddTexture("Bomberman_N_1.png");
	}
	go2->AddComponent<dae::RotatorComponent>(*go2.get(), -360.0f / 0.25f, 50.0f);
	go2->SetParentObject(go.get(), false);

	scene.Add(go2);

	//*-----------------------------------------*
	//|				     ImGui					|
	//*-----------------------------------------*
	go = std::make_shared<dae::GameObject>("ImGui");
	go->AddComponent<dae::ImGuiComponent>(*go.get());
	scene.Add(go);

}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}