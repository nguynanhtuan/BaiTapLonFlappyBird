#include <SFML/Graphics.hpp>
#include "AssetManager.hpp"
#include <SFML/Audio.hpp> //Add music

namespace Sonar
{
	void AssetManager::LoadTexture(std::string name, std::string fileName)
	{
		sf::Texture tex;

		if (tex.loadFromFile(fileName))
		{
			this->_textures[name] = tex;
		}
	}

	sf::Texture &AssetManager::GetTexture(std::string name)
	{
		return this->_textures.at(name);
	}

	void AssetManager::LoadFont(std::string name, std::string fileName)
	{
		sf::Font font;

		if (font.loadFromFile(fileName))
		{
			this->_fonts[name] = font;
		}
	}

	sf::Font &AssetManager::GetFont(std::string name)
	{
		return this->_fonts.at(name);
	}
	//Thêm nhạc nền
	void Sonar::AssetManager::LoadMusic(std::string name, std::string fileName)
	{
		std::unique_ptr<sf::Music> music = std::make_unique<sf::Music>();
		if (!music->openFromFile(fileName))
		{
			throw std::runtime_error("Failed to load music: " + fileName);
		}

		this->_musics[name] = std::move(music);
	}

	sf::Music& Sonar::AssetManager::GetMusic(std::string name)
	{
		return *this->_musics.at(name);
	}

}