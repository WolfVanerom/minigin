#include "TileMapComponent.h"

#include "GameObject.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Texture2D.h"
#include <iostream>

namespace dae
{
	TileMapComponent::TileMapComponent(GameObject* pOwner, float tileWidth, float tileHeight)
		: Component(pOwner)
		, m_tileWidth(tileWidth)
		, m_tileHeight(tileHeight)
	{
	}

	void TileMapComponent::Render() const
	{
		const auto& origin = m_parent->GetWorldPosition();

		for (size_t y{}; y < m_tiles.size(); ++y)
		{
			for (size_t x{}; x < m_tiles[y].size(); ++x)
			{
				const auto type = m_tiles[y][x];
				if (type == LevelObjectType::empty || type == LevelObjectType::none)
				{
					continue;
				}

				auto textureIt = m_tileTextures.find(type);
				if (textureIt == m_tileTextures.end() || textureIt->second == nullptr)
				{
					continue;
				}

				Renderer::GetInstance().RenderTexture(
					*textureIt->second,
					origin.x + static_cast<float>(x) * m_tileWidth,
					origin.y + static_cast<float>(y) * m_tileHeight,
					m_tileWidth,
					m_tileHeight);

				std::cerr << "Rendered tile of type " << static_cast<int>(type) << " at (" << x << ", " << y << ")\n";
			}
		}
	}

	void TileMapComponent::SetTiles(const std::vector<std::vector<LevelObjectType>>& tiles)
	{
		m_tiles = tiles;
	}

	void TileMapComponent::SetTileTexture(LevelObjectType type, const std::string& filename)
	{
		m_tileTextures[type] = ResourceManager::GetInstance().LoadTexture(filename);
	}

	void TileMapComponent::SetTileSize(float tileWidth, float tileHeight)
	{
		m_tileWidth = tileWidth;
		m_tileHeight = tileHeight;
	}
}
