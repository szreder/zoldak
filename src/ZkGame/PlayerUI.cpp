#include <SFML/Graphics.hpp>

#include <QDebug>

#include <memory>

#include "PlayerUI.h"
#include "Entities/PlayerEntity.h"
#include "GameSystem.h"
#include "TextureCache.h"

using namespace Zk::Game;

PlayerUI::PlayerUI(TextureCache & tc)
{
	font.loadFromFile("../data/SourceSansPro/SourceSansPro-Regular.otf");
	healthTexture = tc.getTexture("../data/life.png");
	grenadeTexture = tc.getTexture("../data/grenade.png");
}

PlayerUI::~PlayerUI()
{
	
}

void PlayerUI::paint(
	sf::RenderTarget * rt,
	std::weak_ptr<PlayerEntity> pe,
	sf::FloatRect area
)
{
	// sf::Text text;
	// text.setFont(font);
	// text.setString("Hello, world!");
	// text.setCharacterSize(24);
	// text.setColor(sf::Color::Blue);
	// rt->draw(text);
	auto p = pe.lock();
	double health = p->getHealth();
	double healthFraction = health / 100.0;
	int numGrenades = p->getGrenadeCount();
	
	sf::Vector2u htSize = healthTexture->getSize();
	sf::Vector2u gtSize = grenadeTexture->getSize();
	
	//Narysuj życie
	sf::Sprite sprite;
	sprite.setTexture(*healthTexture, true);
	sprite.setPosition(sf::Vector2f(
		area.left,
		area.top + area.height - htSize.y
	));
	rt->draw(sprite);
	
	//Pasek życia
	sf::RectangleShape rs;
	rs.setPosition(sf::Vector2f(
		area.left + htSize.x + 2.f,
		area.top + area.height - htSize.y + 2.f
	));
	rs.setSize(sf::Vector2f((256.f - 4.f) * healthFraction, htSize.y - 4.f));
	rs.setOutlineColor(sf::Color::Transparent);
	rs.setFillColor(sf::Color::Green);
	rt->draw(rs);
	
	rs.setSize(sf::Vector2f(256.f - 4.f, htSize.y - 4.f));
	rs.setOutlineColor(sf::Color::Black);
	rs.setFillColor(sf::Color::Transparent);
	rs.setOutlineThickness(2.f);
	rt->draw(rs);
	
	//Narysuj granaty
	for (int i = 0; i < numGrenades; i++)
	{
		sprite.setTexture(*grenadeTexture, true);
		sprite.setPosition(sf::Vector2f(
			area.left + (float)i * gtSize.x,
			area.top + area.height - htSize.y - gtSize.y
		));
		rt->draw(sprite);
	}
}
