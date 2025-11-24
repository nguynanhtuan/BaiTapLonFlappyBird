#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "State.hpp"
#include "Game.hpp"
#include "Pipe.hpp"
#include "Land.hpp"
#include "Bird.hpp"
#include "Collision.hpp"
#include "Flash.hpp"
#include "HUD.hpp"
#include "Item.hpp"

#define HEART_SOUND_FILEPATH "resources/audio/heart.wav"
#define SHIELD_SOUND_FILEPATH "resources/audio/heart.wav"
#define SHRINK_SOUND_FILEPATH "resources/audio/heart.wav"

namespace Sonar
{
	class GameState : public State
	{
	public:
		GameState(GameDataRef data);

		void Init();

		void HandleInput();
		void Update(float dt);
		void Draw(float dt);

	private:
		GameDataRef _data;

		sf::Sprite _background;


		Pipe *pipe;
		Land *land;
		Bird *bird;
		Collision collision;
		Flash *flash;
		HUD *hud;
		Item* item;
		sf::Clock clock;
		
		int shieldCounter = 0;
		int shrinkCounter = 0;
		bool hasShield = false;
		bool hasShrink = false;
		int lives = 1;

		int _gameState;

		sf::RectangleShape _gameOverFlash;
		bool _flashOn;

		int _score;

		sf::SoundBuffer _hitSoundBuffer;
		sf::SoundBuffer _wingSoundBuffer;
		sf::SoundBuffer _pointSoundBuffer;
		sf::SoundBuffer _heartSoundBuffer;
		sf::SoundBuffer _shieldSoundBuffer;
		sf::SoundBuffer _shrinkSoundBuffer;

		sf::Sound _hitSound;
		sf::Sound _wingSound;
		sf::Sound _pointSound;
		sf::Sound _heartSound;
		sf::Sound _shieldSound;
		sf::Sound _shrinkSound;
		sf::Sprite shieldEffect;

		int _pipeSpawnCount = 0; // Đếm số lần sinh ống
		bool hasHeart = false; // có trái tim
		bool usedHeart = false; // đã dùng hồi sinh chưa
		bool isInvincible = false;  // Trạng thái bất tử tạm thời
		sf::Clock invincibleClock;  // Đồng hồ đo thời gian bất tử


		float _speedMultiplier = 1.0f;     // Hệ số tốc độ (ban đầu = 1)
		sf::Clock _difficultyClock;        // Đồng hồ đếm thời gian để tăng độ khó


	};
}