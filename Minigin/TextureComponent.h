#ifndef TEXTURECOMPONENT
#define TEXTURECOMPONENT

#include <string>
#include <memory>
#include <vector>
#include "RenderComponent.h"
#include "Texture2D.h"
#include "Transform.h"

namespace dae
{
	class TextureComponent : public RenderComponent
	{
    public:
        void Render() const override;
                
        int AddTexture(const std::string& filename);
        int AddTexture(const std::shared_ptr<Texture2D>& texture);

        int GetCurrentIndex() const { return m_CurrentSpriteIndex; }
        void SetCurrentIndex(int const newIndex);

        glm::ivec2 GetSize() const { return m_pTextures[m_CurrentSpriteIndex]->GetSize(); }

		glm::vec2 GetOffset() const { return m_Offset; }
		void SetOffset(glm::vec2 offset) { m_Offset = offset; }

        TextureComponent(dae::GameObject& object);
        virtual ~TextureComponent() = default;
        TextureComponent(const TextureComponent& other) = delete;
        TextureComponent(TextureComponent&& other) = delete;
        TextureComponent& operator= (const TextureComponent& other) = delete;
        TextureComponent& operator=(TextureComponent&& other) = delete;

    protected:
        std::vector<std::shared_ptr<Texture2D>> m_pTextures{};
        int m_CurrentSpriteIndex{};
        glm::vec2 m_Offset{};
	};
}

#endif