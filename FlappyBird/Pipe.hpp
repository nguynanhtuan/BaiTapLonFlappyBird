#pragma once

#include <SFML/Graphics.hpp>
#include "Game.hpp"
#include <vector>

namespace Sonar
{
	class Pipe
	{
	public:
		Pipe(GameDataRef data);

		void SpawnBottomPipe();
		void SpawnTopPipe();
		void SpawnInvisiblePipe();
		void SpawnScoringPipe();
		void MovePipes(float dt);
		void DrawPipes();
		void RandomisePipeOffset();
		void SetDifficultyActive(bool active); // Kích hoạt hoặc hủy kích hoạt chế độ khó
		


		const std::vector<sf::Sprite> &GetSprites() const;
		std::vector<sf::Sprite> &GetScoringSprites();

	private:
		GameDataRef _data;
		std::vector<sf::Sprite> pipeSprites;
		std::vector<sf::Sprite> scoringPipes;
		bool _difficultyActive = false;
		float _pipeOscillationSpeed = 60.0f;     // tốc độ dao động (pixel/giây)
		float _pipeOscillationRange = 80.0f;    // biên độ dao động (pixel)
		float _pipeOscillationDirection = 1.0f; // 1 = lên, -1 = xuống
		std::vector<float> baseYPositions;
		



		int _landHeight;
		int _pipeSpawnYOffset;

	};
}