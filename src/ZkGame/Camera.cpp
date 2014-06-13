#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

#include <QtCore>

#include <algorithm>

#include "../ZkCommon/Constants.h"
#include "Config/Config.h"
#include "Config/GraphicsConfig.h"

#include "Camera.h"
#include "GameSystem.h"
#include "Entities/Entity.h"

using namespace Zk::Common;
using namespace Zk::Game;

Camera::Camera()
{
	const sf::VideoMode & vm = GameSystem::getInstance()->getConfig().graphicsConfig.videoMode;
	sf::Vector2f absScreenSize((float)vm.width, (float)vm.height);
	relativeScreenSize = absScreenSize * (float)Constants::METERS_PER_PIXEL;
}

std::vector<sf::View> Camera::getViews() const
{
	return computedViews;
}

void Camera::discardInvalidEntities()
{
	std::remove_if(
		trackedEntities.begin(), trackedEntities.end(),
		[&](std::weak_ptr<Entity> ent) -> bool
		{
			return ent.expired();
		}
	);
}

SplitScreenCamera::SplitScreenCamera(
	std::initializer_list<std::weak_ptr<Entity>> entities
)
{
	trackedEntities = entities;
	alignment = Alignment::Horizontal;
}

SplitScreenCamera::SplitScreenCamera(
	const std::vector<std::weak_ptr<Entity>> & entities
)
	: Camera()
{
	trackedEntities = entities;
	alignment = Alignment::Horizontal;
}

SplitScreenCamera::~SplitScreenCamera()
{
	
}

void SplitScreenCamera::setupViews()
{
	discardInvalidEntities();
	computedViews.clear();
	
	sf::View view;
	
	if (trackedEntities.size() > 0)
	{
		int id = 0;
		float step = 1.f / (float)trackedEntities.size();
		for (std::weak_ptr<Entity> weakEnt : trackedEntities)
		{
			std::shared_ptr<Entity> ent = weakEnt.lock();
			float factor = (float)id * step;
			switch (alignment)
			{
			case Alignment::Horizontal:
				view.reset(sf::FloatRect(
					factor * relativeScreenSize.x,
					0.f,
					(factor + step) * relativeScreenSize.x,
					relativeScreenSize.y
				));
				
				view.setViewport(sf::FloatRect(factor, 0.f, factor + step, 1.f));
				break;
				
			case Alignment::Vertical:
				view.reset(sf::FloatRect(
					0.f,
					factor * relativeScreenSize.y,
					relativeScreenSize.x,
					(factor + step) * relativeScreenSize.y
				));
				
				view.setViewport(sf::FloatRect(0.f, factor, 1.f, factor + step));
				break;
			}
			
			view.setCenter(ent->getCenterPosition());
			
			computedViews.push_back(view);
			
			id++;
		}
	}
	else
	{
		view.reset(
			sf::FloatRect(0.f, 0.f, relativeScreenSize.x, relativeScreenSize.y)
		);
		
		view.setViewport(
			sf::FloatRect(0.f, 0.f, 1.f, 1.f)
		);
		
		computedViews.push_back(view);
	}
	
	// if (trackedEntities.size() > 0)
	// {
	// 	for (std::weak_ptr<Entity> weakEnt : trackedEntities)
	// 	{
	// 		//discardInvalidEntities gwarantuje nam,
	// 		//że jest to poprawny wskaźnik
	// 		std::shared_ptr<Entity> ent = weakEnt.lock();
			
	// 		sf::Vector2f pos = ent->getCenterPosition();
	// 	}
	// }
}
