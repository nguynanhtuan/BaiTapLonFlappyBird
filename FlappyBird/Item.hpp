#pragma once
#include <SFML/Graphics.hpp>
#include "Game.hpp"

namespace Sonar
{
    enum ItemType
    {
        ITEM_SHIELD,
        ITEM_SHRINK,
        ITEM_HEART
    };

    class Item
    {
    public:
        Item(GameDataRef data);

        void SpawnRandomItem(const std::vector<sf::Sprite>& pipes);
        // tạo vật phẩm ngẫu nhiên
        void MoveItems(float dt);
        void DrawItems();
        void RemoveItem(int index);

        const std::vector<sf::Sprite>& GetSprites() const;
        const std::vector<ItemType>& GetTypes() const;

    private:
        GameDataRef _data;
        std::vector<sf::Sprite> _items;
        std::vector<ItemType> _types;
    };
}
#pragma once
