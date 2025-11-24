#include "Pipe.hpp"
#include "DEFINITIONS.hpp"

#include <iostream>
static sf::Clock gameclock;


namespace Sonar
{
	Pipe::Pipe(GameDataRef data) : _data(data)
	{
		_landHeight = this->_data->assets.GetTexture("Land").getSize().y;
		_pipeSpawnYOffset = 0;
	}

	void Pipe::SpawnBottomPipe()
	{
		sf::Sprite sprite(this->_data->assets.GetTexture("Pipe Up"));
		sprite.setPosition(this->_data->window.getSize().x,
			this->_data->window.getSize().y - sprite.getLocalBounds().height - _pipeSpawnYOffset);

		pipeSprites.push_back(sprite);
		baseYPositions.push_back(sprite.getPosition().y); // ✅ Lưu vị trí gốc Y
	}

	void Pipe::SpawnTopPipe()
	{
		sf::Sprite sprite(this->_data->assets.GetTexture("Pipe Down"));
		sprite.setPosition(this->_data->window.getSize().x, -_pipeSpawnYOffset);

		pipeSprites.push_back(sprite);
		baseYPositions.push_back(sprite.getPosition().y); // ✅ Lưu vị trí gốc Y

	}


	void Pipe::SpawnInvisiblePipe()
	{
		sf::Sprite sprite(this->_data->assets.GetTexture("Pipe Down"));

		sprite.setPosition(this->_data->window.getSize().x, -_pipeSpawnYOffset);
		sprite.setColor(sf::Color(0, 0, 0, 0));

		pipeSprites.push_back(sprite);
		baseYPositions.push_back(sprite.getPosition().y);// ✅ Lưu vị trí gốc Y
	}

	void Pipe::SpawnScoringPipe()
	{
		sf::Sprite sprite(this->_data->assets.GetTexture("Scoring Pipe"));

		sprite.setPosition(this->_data->window.getSize().x, 0);

		scoringPipes.push_back(sprite);
	}
	void Pipe::SetDifficultyActive(bool active) // level up
	{
		_difficultyActive = active;
	}


	void Pipe::MovePipes(float dt)
{
    for (int i = 0; i < pipeSprites.size(); i++)
    {
        // Nếu ống ra khỏi màn hình → xóa sprite và vị trí gốc tương ứng
        if (pipeSprites.at(i).getPosition().x < 0 - pipeSprites.at(i).getLocalBounds().width)
        {
            pipeSprites.erase(pipeSprites.begin() + i);
			// ✅ Chỉ xóa baseYPositions nếu còn phần tử tương ứng
			if (i < baseYPositions.size())
				baseYPositions.erase(baseYPositions.begin() + i);

            i--;
            continue;
        }

        float movementX = PIPE_MOVEMENT_SPEED * dt;
        sf::Vector2f pos = pipeSprites.at(i).getPosition();

        // Di chuyển sang trái
        pos.x -= movementX;

        // Nếu bật chế độ khó → dao động nhẹ quanh vị trí gốc
        if (_difficultyActive && i < baseYPositions.size())
        {
            float baseY = baseYPositions[i];
            float time = static_cast<float>(gameclock.getElapsedTime().asSeconds());
            pos.y = baseY + sin(time * 2.0f) * _pipeOscillationRange; // ✅ dao động quanh vị trí gốc
        }

        pipeSprites.at(i).setPosition(pos);
    }

    // Giữ nguyên phần di chuyển scoring pipes
    for (int i = 0; i < scoringPipes.size(); i++)
    {
        if (scoringPipes.at(i).getPosition().x < 0 - scoringPipes.at(i).getLocalBounds().width)
        {
            scoringPipes.erase(scoringPipes.begin() + i);
            i--;
            continue;
        }

        float movement = PIPE_MOVEMENT_SPEED * dt;
        scoringPipes.at(i).move(-movement, 0);
    }
}



	void Pipe::DrawPipes()
	{
		for (unsigned short int i = 0; i < pipeSprites.size(); i++)
		{
			this->_data->window.draw(pipeSprites.at(i));
		}
	}

	void Pipe::RandomisePipeOffset()
	{
		_pipeSpawnYOffset = rand() % (_landHeight + 1);
	}

	const std::vector<sf::Sprite> &Pipe::GetSprites() const
	{
		return pipeSprites;
	}

	std::vector<sf::Sprite> &Pipe::GetScoringSprites()
	{
		return scoringPipes;
	}
}