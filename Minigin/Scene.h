#pragma once
#include "SceneManager.h"
#include <vector>

namespace dae
{
	class GameObject;
	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		void Add(std::shared_ptr<GameObject> object);
		std::vector<std::shared_ptr<GameObject>>::iterator Remove(std::shared_ptr<GameObject> object);
		void RemoveAll();

		void Update(float const deltaTime);
		void FixedUpdate(float const fixedTimeStep);
		void LateUpdate();
		void Render() const;
		void RenderImGui() const;

		std::string GetName() const { return m_name; }

		std::shared_ptr<GameObject> GetObject(const std::string& name) const;

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		explicit Scene(const std::string& name);

		std::string m_name;
		std::vector < std::shared_ptr<GameObject>> m_objects{};

		static unsigned int m_idCounter; 
	};

}
