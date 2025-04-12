#pragma once
#include <string>
#include <functional>

namespace dae
{
	class Minigin
	{
	public:
		explicit Minigin(const std::string& dataPath);
		~Minigin();
		void Run(const std::function<void()>& load);

		Minigin(const Minigin& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;
	private:
		int const m_windowWidthBase{ 256 };
		int const m_windowHeightBase{ 240 };
		//makes the window bigger but doesn't scale any of the sprites
		float const m_windowScale{ 3.0f };
	};
}