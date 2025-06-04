#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);
		void RemoveScene(const std::string& name);

		void Update(float const deltaTime);
		void FixedUpdate(float const fixedTimeStep);
		void LateUpdate();
		void Render();
		void RenderImGui();

		std::shared_ptr<Scene> GetScene(const std::string& name) const;

		void SetSceneActive(const std::string& name, bool shouldBeActive);

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_scenes;
	};
}
