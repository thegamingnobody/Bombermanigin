#ifndef TEXTCOMPONENT
#define TEXTCOMPONENT
#include "TextureComponent.h"
#include "Font.h"
#include "Observer.h"

namespace dae
{
	class TextComponent : public TextureComponent, public Observer
	{
	public:
		void Update(float const deltaTime) override;

		void SetText(const std::string& text);

		TextComponent(dae::GameObject& object, const std::string& text, std::shared_ptr<Font> font, GameObject* objectToTakeStatsFrom = nullptr);
		virtual ~TextComponent() = default;
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;

		virtual void Notify(const Event&) override {};

	private:
		std::string m_text;
		std::shared_ptr<Font> m_font;
		bool m_needsUpdate;
		GameObject* m_ObjectToTakeStatsFrom;
	};
}

#endif 