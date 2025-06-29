#pragma once
#include <Singleton.h>
#include <TextComponent.h>

namespace bomberman
{
    class HUDManager : public dae::Singleton<HUDManager>
    {
    public:
        void Init(dae::TextComponent& textComponent, glm::vec3 screenPos);

		bool IsInitialized() const { return m_IsInitialized; }

		void SetLivesText(int lives);
		void SetScoreText(int score);

        void Update();

    private:
        void SetText();

        bool m_IsInitialized{ false };
		dae::TextComponent* m_TextComponent{ nullptr };
        glm::vec3 m_ScreenPos{};

		std::string m_LeftTextStatic{ "Left " };
		std::string m_ScoreTextStatic{ "Score " };
        
		int m_Lives{ 0 };
		int m_Score{ 0 };
    };
}

