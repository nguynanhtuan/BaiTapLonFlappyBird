#pragma once

#include <sstream>
#include "DEFINITIONS.hpp"
#include "GameState.hpp"
#include "GameOverState.hpp"

#include <iostream>

namespace Sonar
{
	GameState::GameState(GameDataRef data) : _data(data)
	{

	}

	void GameState::Init()
	{
		if (!_hitSoundBuffer.loadFromFile(HIT_SOUND_FILEPATH))
		{
			std::cout << "Error Loading Hit Sound Effect" << std::endl;
		}

		if (!_wingSoundBuffer.loadFromFile(WING_SOUND_FILEPATH))
		{
			std::cout << "Error Loading Wing Sound Effect" << std::endl;
		}

		if (!_pointSoundBuffer.loadFromFile(POINT_SOUND_FILEPATH))
		{
			std::cout << "Error Loading Point Sound Effect" << std::endl;
		}

		if (!_heartSoundBuffer.loadFromFile(HEART_SOUND_FILEPATH))
		{
			std::cout << "Error Loading Heart Sound Effect" << std::endl;
		}

		if (!_shieldSoundBuffer.loadFromFile(SHIELD_SOUND_FILEPATH))
		{
			std::cout << "Error Loading Shield Sound Effect" << std::endl;
		}

		if (!_shrinkSoundBuffer.loadFromFile(SHRINK_SOUND_FILEPATH))
		{
			std::cout << "Error Loading Shrink Sound Effect" << std::endl;
		}

		_hitSound.setBuffer(_hitSoundBuffer);
		_wingSound.setBuffer(_wingSoundBuffer);
		_pointSound.setBuffer(_pointSoundBuffer);
		_heartSound.setBuffer(_heartSoundBuffer);
		_shieldSound.setBuffer(_shieldSoundBuffer);
		_shrinkSound.setBuffer(_shrinkSoundBuffer);


		this->_data->assets.LoadTexture("Game Background", GAME_BACKGROUND_FILEPATH);
		this->_data->assets.LoadTexture("Pipe Up", PIPE_UP_FILEPATH);
		this->_data->assets.LoadTexture("Pipe Down", PIPE_DOWN_FILEPATH);
		this->_data->assets.LoadTexture("Land", LAND_FILEPATH);
		this->_data->assets.LoadTexture("Bird Frame 1", BIRD_FRAME_1_FILEPATH);
		this->_data->assets.LoadTexture("Bird Frame 2", BIRD_FRAME_2_FILEPATH);
		this->_data->assets.LoadTexture("Bird Frame 3", BIRD_FRAME_3_FILEPATH);
		this->_data->assets.LoadTexture("Bird Frame 4", BIRD_FRAME_4_FILEPATH);
		this->_data->assets.LoadTexture("Scoring Pipe", SCORING_PIPE_FILEPATH);
		this->_data->assets.LoadFont("Flappy Font", FLAPPY_FONT_FILEPATH);
		this->_data->assets.LoadTexture("Item Shield", ITEM_SHIELD_FILEPATH);
		this->_data->assets.LoadTexture("Item Shrink", ITEM_SHRINK_FILEPATH);
		this->_data->assets.LoadTexture("Item Heart", ITEM_HEART_FILEPATH);
		this->_data->assets.LoadTexture("Shield Effect", SHIELD_EFFECT_FILEPATH);
		

		this->_data->assets.LoadSound("Shield Sound", SHIELD_SOUND_FILEPATH);
		this->_data->assets.LoadSound("Heart Sound", HEART_SOUND_FILEPATH);
		this->_data->assets.LoadSound("Shrink Sound", SHRINK_SOUND_FILEPATH);
		// Load và phát nhạc nền
		try
		{
			this->_data->assets.LoadMusic("Game Music", "Resources/audio/GameMusic.wav");

			sf::Music& bgMusic = this->_data->assets.GetMusic("Game Music");
			bgMusic.setLoop(true);      // Lặp lại liên tục
			bgMusic.setVolume(40.f);    // Âm lượng vừa phải
			bgMusic.play();             // Phát nhạc nền
		}
		catch (std::exception& e)
		{
			std::cout << "Error loading background music: " << e.what() << std::endl;
		}




		pipe = new Pipe(_data);
		land = new Land(_data);
		bird = new Bird(_data);
		flash = new Flash(_data);
		hud = new HUD(_data);
		item = new Item(_data);


		_background.setTexture(this->_data->assets.GetTexture("Game Background"));

		_score = 0;
		hud->UpdateScore(_score);

		_gameState = GameStates::eReady;
	}

	void GameState::HandleInput()
	{
		sf::Event event;

		while (this->_data->window.pollEvent(event))
		{
			if (sf::Event::Closed == event.type)
			{
				this->_data->window.close();
			}

			if (this->_data->input.IsSpriteClicked(this->_background, sf::Mouse::Left, this->_data->window))
			{
				if (GameStates::eGameOver != _gameState)
				{
					_gameState = GameStates::ePlaying;
					bird->Tap();

					_wingSound.play();
				}
			}
			if (this->_data->input.IsSpriteClicked(this->_background, sf::Mouse::Left, this->_data->window)
				|| (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space))
			{
				if (GameStates::eGameOver != _gameState)
				{
					_gameState = GameStates::ePlaying;
					bird->Tap();

					_wingSound.play();
				}
			}

		}
	}

	void GameState::Update(float dt)
	{
		//  Xử lý trạng thái bất tử
		if (isInvincible && invincibleClock.getElapsedTime().asSeconds() > 2.0f)
		{
			isInvincible = false;
		}

		if (GameStates::eGameOver != _gameState)
		{
			if (_difficultyClock.getElapsedTime().asSeconds() > 10.0f) // mỗi 10 giây
			{
				_speedMultiplier += 0.1f; // tăng tốc độ 10%
				if (_speedMultiplier > 2.0f) _speedMultiplier =	2.0f; // giới hạn tối đa
				std::cout << "Toc do tang ! x" << _speedMultiplier << std::endl;
				_difficultyClock.restart();
			}
			bird->Animate(dt);
		

			//  Cập nhật tốc độ đất theo độ khó
			land->MoveLand(dt * _speedMultiplier);
		}

		if (GameStates::ePlaying == _gameState)
		{
			pipe->MovePipes(dt * _speedMultiplier);

			float currentSpawnFrequency = PIPE_SPAWN_FREQUENCY / _speedMultiplier;

			if (clock.getElapsedTime().asSeconds() > currentSpawnFrequency)		
			{
				pipe->RandomisePipeOffset();

				pipe->SpawnInvisiblePipe();
				pipe->SpawnBottomPipe();
				pipe->SpawnTopPipe();
				pipe->SpawnScoringPipe();

				clock.restart();
				_pipeSpawnCount++;

				if (_pipeSpawnCount >= 10)  // Cứ sau 10 lần sinh ống
				{
					const std::vector<sf::Sprite>& pipes = pipe->GetSprites();
					item->SpawnRandomItem(pipes); // Bắt buộc spawn 1 item
					_pipeSpawnCount = 0;          // Reset lại bộ đếm
				}
			}
			
			

			bird->Update(dt);

			std::vector<sf::Sprite> landSprites = land->GetSprites();

			for (int i = 0; i < landSprites.size(); i++)
			{
				if (isInvincible) continue; //  Bỏ qua va chạm nếu đang hồi sinh
				if (collision.CheckSpriteCollision(bird->GetSprite(), 0.625f, landSprites.at(i), 1.0f))
				{
					if (hasShield)
					{
						//  Nếu có khiên, bỏ qua va chạm và tắt khiên
						shieldCounter++;
						if (shieldCounter >= 500) // sau khoảng 500 frame
						{
							hasShield = false;
						}
						shieldEffect.setPosition(
							bird->GetSprite().getPosition().x,
							bird->GetSprite().getPosition().y
						);
					}
					else if (hasShrink)
					{
						shrinkCounter++;
						if (shrinkCounter >= 800)
						{
							hasShrink = false;
							bird->SetScale(1.0f, 1.0f); // trở lại kích thước ban đầu
						}
					}
					else if (lives >= 1)
					{
						//  Nếu còn nhiều mạng, trừ 1 mạng
						lives--;
						std::cout << "Lost one life! Lives left: " << lives << std::endl;
						continue;
					}
					else if (hasHeart && !usedHeart)
					{
						usedHeart = true;
						hasHeart = false;
					
						_heartSound.play();
						continue; // tiếp tục chơi, không chết
					}
					else
					{
						{
							_gameState = GameStates::eGameOver; /////////////////
							clock.restart();
							_hitSound.play();
						}

					}

				}
			}
			// Di chuyển các item sang trái
			item->MoveItems(dt);

			// Xử lý va chạm vật phẩm
			auto& itemSprites = item->GetSprites();
			auto& itemTypes = item->GetTypes();

			for (int i = 0; i < itemSprites.size(); i++)
			{
				if (collision.CheckSpriteCollision(bird->GetSprite(), 1.0f, itemSprites[i], 1.0f))
				{
					if (itemTypes[i] == ITEM_SHIELD)
					{
						hasShield = true;
						shieldCounter = 0;
						shieldEffect.setTexture(this->_data->assets.GetTexture("Shield Effect"));
						shieldEffect.setOrigin(
							shieldEffect.getGlobalBounds().width / 2.f,
							shieldEffect.getGlobalBounds().height / 2.f
						);
						shieldEffect.setScale(1.5f, 1.5f); // shield to hơn chim một chút
						//  Phát âm thanh khi ăn Khiên
						_shieldSound.play();
					}
					else if (itemTypes[i] == ITEM_SHRINK)
					{
						hasShrink = true;
						shrinkCounter = 0;
						bird->SetScale(0.3f, 0.3f); // Shrink bird
						//  Phát âm thanh khi ăn Thuốc
						_shrinkSound.play();
					}
					else if (itemTypes[i] == ITEM_HEART)
					{
						hasHeart = true;
						usedHeart = false;
					}

					item->RemoveItem(i);
					break;
				}
			}


			std::vector<sf::Sprite> pipeSprites = pipe->GetSprites();

			for (int i = 0; i < pipeSprites.size(); i++)
			{
				if (isInvincible) continue; //  Bỏ qua va chạm nếu đang hồi sinh
				if (collision.CheckSpriteCollision(bird->GetSprite(), 0.625f, pipeSprites.at(i), 1.0f))
				{
					if (hasShield) continue; // Nếu có khiên, bỏ qua va chạm
					else if (lives > 0) {
						lives--; // mất 1 mạng
						continue;
					}
					else if (hasHeart && !usedHeart)
					{
						usedHeart = true;
						hasHeart = false;
						isInvincible = true;              //  Kích hoạt trạng thái bất tử
						invincibleClock.restart();        // reset thời gian
						std::cout << "Ban vua va cot  " << std::endl;
						_heartSound.play();
						continue; // bỏ qua chết, chim tiếp tục bay
					}
					else
					{
						{
							_gameState = GameStates::eGameOver;//////////////////////
							clock.restart();
							_hitSound.play();
						}

					}

				}
			}

			if (GameStates::ePlaying == _gameState)
			{
				std::vector<sf::Sprite> &scoringSprites = pipe->GetScoringSprites();

				for (int i = 0; i < scoringSprites.size(); i++)
				{
					if (collision.CheckSpriteCollision(bird->GetSprite(), 0.625f, scoringSprites.at(i), 1.0f))
					{
						_score++;

						hud->UpdateScore(_score);

						scoringSprites.erase(scoringSprites.begin() + i);

						_pointSound.play();
						if (_score >= 10)
						{
							pipe->SetDifficultyActive(true);
						}

						if (hasShield && ++shieldCounter >= 5)
							hasShield = false;
						if (hasShrink && ++shrinkCounter >= 10)
						{
							hasShrink = false;
							bird->SetScale(1.0f, 1.0f); // Trả lại kích thước ban đầu
						}
					}
					
				}
			}
		}

		if (GameStates::eGameOver == _gameState)
		{
			flash->Show(dt);

			if (clock.getElapsedTime().asSeconds() > TIME_BEFORE_GAME_OVER_APPEARS)
			{
				this->_data->machine.AddState(StateRef(new GameOverState(_data, _score)), true);
			}
		}
	}

	void GameState::Draw(float dt)
	{
		this->_data->window.clear( sf::Color::Red );

		this->_data->window.draw(this->_background);

		pipe->DrawPipes();
		land->DrawLand();
		
		bird->Draw();
		if (hasShield)
		{
			// Cập nhật vị trí theo chim
			shieldEffect.setPosition(bird->GetSprite().getPosition());

			// Hiệu ứng nhấp nháy (tùy chọn)
			static float alpha = 200;
			static bool fade = false;
			if (fade) alpha -= 100 * dt;
			else alpha += 100 * dt;
			if (alpha > 255) { alpha = 255; fade = true; }
			if (alpha < 100) { alpha = 100; fade = false; }

			shieldEffect.setColor(sf::Color(255, 255, 255, alpha));

			//  Vẽ đúng sprite khiên gốc
			this->_data->window.draw(shieldEffect);
		}



		item->DrawItems(); // Vẽ các item


		flash->Draw();

		hud->Draw();

		this->_data->window.display();
	}
}