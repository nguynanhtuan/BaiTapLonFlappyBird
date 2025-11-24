#pragma once

#include <SFML/Audio.hpp>
#include <map>
#include <SFML/Graphics.hpp>
#include <string>

namespace Sonar
{
	class AssetManager
	{
	public:
		AssetManager() {}
		~AssetManager() {}

		void LoadTexture(std::string name, std::string fileName);
		sf::Texture& GetTexture(std::string name);

		void LoadFont(std::string name, std::string fileName);
		sf::Font& GetFont(std::string name);

		//  Thêm 2 hàm mới:
		void LoadMusic(std::string name, std::string fileName);
		sf::Music& GetMusic(std::string name);

		void LoadSound(const std::string& name, const std::string& fileName)
		{
			sf::SoundBuffer buffer;
			if (buffer.loadFromFile(fileName))
			{
				_soundBuffers[name] = buffer;
			}
		}

		const sf::SoundBuffer& GetSound(const std::string& name) const
		{
			return _soundBuffers.at(name);
		}

	private:
		std::map<std::string, sf::Texture> _textures;
		std::map<std::string, sf::Font> _fonts;
		std::map<std::string, sf::SoundBuffer> _soundBuffers;
		std::map<std::string, std::unique_ptr<sf::Music>> _musics; // Thêm map quản lý nhạc nền
	};
}
