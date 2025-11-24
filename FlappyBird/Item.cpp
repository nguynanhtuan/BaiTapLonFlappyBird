#include "Item.hpp"
#include "DEFINITIONS.hpp"
#include <cstdlib>

namespace Sonar
{
    Item::Item(GameDataRef data) : _data(data) {}

    void Item::SpawnRandomItem(const std::vector<sf::Sprite>& pipes)
    {
        int r = rand() % 100; // tỉ lệ xuất hiện

        if (r < 100)  // 5% cơ hội có vật phẩm
        {
            ItemType type;
            sf::Sprite sprite;

            int choose = rand() % 3;
            if (choose == 0)
            {
                sprite.setTexture(this->_data->assets.GetTexture("Item Shield"));
                sprite.setScale(0.1f, 0.1f); // nhỏ lại
                type = ITEM_SHIELD;
            }
            else if (choose == 1)
            {
                sprite.setTexture(this->_data->assets.GetTexture("Item Shrink"));
				sprite.setScale(0.1f, 0.1f); // nhỏ lại
                type = ITEM_SHRINK;
            }
            else
            {
                sprite.setTexture(this->_data->assets.GetTexture("Item Heart"));
				sprite.setScale(0.1f, 0.1f); // nhỏ lại
                type = ITEM_HEART;
            }

            // vị trí giữa ống
            // Lấy vị trí ống gần nhất (ống dưới cùng)
            if (pipes.size() >= 2)
            {
                // Tìm 2 ống gần nhau nhất theo trục X
                const sf::Sprite* bottomPipe = nullptr;
                const sf::Sprite* topPipe = nullptr;
                float minX = 999999.f;

                for (size_t i = 0; i + 1 < pipes.size(); i++)
                {
                    float dx = std::abs(pipes[i].getPosition().x - pipes[i + 1].getPosition().x);
                    if (dx < minX)
                    {
                        minX = dx;
                        if (pipes[i].getPosition().y > pipes[i + 1].getPosition().y)
                        {
                            bottomPipe = &pipes[i];
                            topPipe = &pipes[i + 1];
                        }
                        else
                        {
                            bottomPipe = &pipes[i + 1];
                            topPipe = &pipes[i];
                        }
                    }
                }

                if (bottomPipe && topPipe)
                {
                    float gapCenterY = (bottomPipe->getPosition().y - PIPE_GAP / 2.0f);
                    sprite.setPosition(this->_data->window.getSize().x, gapCenterY);
                }
            }
            else
            {
                // Nếu chưa có ống nào (lúc đầu game), spawn ngẫu nhiên
                float y = rand() % (this->_data->window.getSize().y - 200) + 100;
                sprite.setPosition(this->_data->window.getSize().x, y);
            }

            _items.push_back(sprite);
            _types.push_back(type);
        }
    }

    void Item::MoveItems(float dt)
    {
        for (int i = 0; i < _items.size(); i++)
        {
            _items[i].move(-PIPE_MOVEMENT_SPEED * dt, 0);

            if (_items[i].getPosition().x < -_items[i].getLocalBounds().width)
            {
                _items.erase(_items.begin() + i);
                _types.erase(_types.begin() + i);
                i--;
            }
        }
    }

    void Item::DrawItems()
    {
        for (auto& i : _items)
        {
            this->_data->window.draw(i);
        }
    }

    const std::vector<sf::Sprite>& Item::GetSprites() const { return _items; }
    const std::vector<ItemType>& Item::GetTypes() const { return _types; }

    void Item::RemoveItem(int index)
    {
        _items.erase(_items.begin() + index);
        _types.erase(_types.begin() + index);
    }
}
