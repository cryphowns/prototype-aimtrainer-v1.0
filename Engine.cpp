#include "Engine.h"
#include <iostream>
#include <filesystem>

#define WIDTH 1600;
#define HEIGHT 900;
#define TITLE "Prototpe AimTrainer v1"
#define BACKGROUND_COLOR sf::Color(22,30,43,40)
#define DESIRED_FRAMERATE 60
#define EASY_TICK 30
#define HARD_TICK 10
#define EASY_SPEED 0.01f
#define HARD_SPEED 0.05f
#define DAMAGE_RATE 0.01f

float lerpFunction(float start, float goal, float inc)
{
	//Formula : a + (b - a) * c
	return (start + (goal - start) * inc);
}

float returnMagnitude(sf::Vector2f vector1, sf::Vector2f vector2)
{
	//Formula: |V| = sqrt((x2-x1)^2 + (y2-y1)^2))
	int dist = sqrtf(std::pow((vector2.x - vector1.x), 2.f) + std::pow((vector2.y - vector1.y), 2.f));
	return dist;
}

void Engine::initializeProperties()
{
	//Initialize general properties
	this->windowObject = nullptr;
	this->videoMode.height = HEIGHT;
	this->videoMode.width = WIDTH;
	this->lastGoalChangeTick = 0;
	this->currentTickUpdate = 0;
	this->currentPoints = 0;
	this->accuracyPoints = 0;
	this->mouseActivated = 0;
	this->goalPosition = sf::Vector2f((this->videoMode.width / 2.f) - (25.f), (this->videoMode.height / 2.f) - (25.f));
	this->objects.circle.setFillColor(sf::Color(43, 161, 75));
	if (this->mode == 1) {
		this->objects.circle.setRadius(25);
		this->objects.circle.setPosition(this->goalPosition);
	}
	else
	{
		this->objects.circle.setRadius(15);
		this->objects.circle.setPosition(this->goalPosition);
	}

	//Loading font file
	if (!this->font.loadFromFile("Koulen-Regular.ttf"))
	{
		printf("Font didnt load?");
	}

	//initiailize points text
	this->objects.points.setFont(this->font);
	this->objects.points.setCharacterSize(25);
	this->objects.points.setFillColor(sf::Color(43, 161, 75));
	this->objects.points.setStyle(sf::Text::Bold);
	this->objects.points.setString(std::string("Points: ") + std::to_string((int)this->currentPoints));

	//initiailize accuracy text
	this->objects.accuracy.setFont(this->font);
	this->objects.accuracy.setCharacterSize(25);
	this->objects.accuracy.setFillColor(sf::Color(43, 161, 75));
	this->objects.accuracy.setStyle(sf::Text::Bold);
	this->objects.accuracy.setPosition(sf::Vector2f(0.f, 25.f));
	this->objects.accuracy.setString(std::string("Accuracy: ") + std::to_string(this->accuracyPoints) + std::string("%"));
}

void Engine::initializeWindow()
{
	this->windowObject = new sf::RenderWindow(this->videoMode, TITLE, sf::Style::Default);
	this->windowObject->setFramerateLimit(DESIRED_FRAMERATE);
}

void Engine::pollEvents()
{
	while (this->windowObject->pollEvent(this->event))
	{
		switch (this->event.type)
		{
		case (sf::Event::Closed):
			this->windowObject->close();
			break;
		case (sf::Event::KeyPressed):
			switch (this->event.key.code)
			{
			case (sf::Keyboard::Escape):
				this->windowObject->close();
				break;
			}
			break;
		case(sf::Event::MouseMoved):
			this->storedMousePos = sf::Mouse::getPosition(*this->windowObject);
			break;
		case(sf::Event::MouseButtonPressed):
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				this->mouseActivated = 1;
			}

		case(sf::Event::MouseButtonReleased):
			if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				this->mouseActivated = 0;
			}
			break;
		}
	}
}

void Engine::updateObject()
{
	//Tracking Mode
	if (this->mode == 1)
	{
		/*
		* Check for when the circle goes out of bounds
		* Temporary solution to an issue where the circle goes haywire and goes out of bounds, i suspect its a problem with the rand() function
		*/
		if (returnMagnitude(this->objects.circle.getPosition(), sf::Vector2f(this->videoMode.width / 2.f, this->videoMode.height / 2.f)) >= 2000.f)
		{
			//printf("Out of bounds!\n");
			this->objects.circle.setPosition(sf::Vector2f(this->videoMode.width / 2.f, this->videoMode.height / 2.f));
		}
		//When the circle gets too small, reset
		if (this->objects.circle.getRadius() <= 5.f) {
			//Granted I should have encapsulated this in a function but im writting this at 2 am and im lazy
			this->objects.circle.setFillColor(sf::Color(43, 161, 75));
			this->objects.circle.setRadius(25);
			this->objects.circle.setPosition(sf::Vector2f((float)(rand() % this->videoMode.width - 20), (float)(rand() % this->videoMode.height - 20)));

		}
		//set new random position every n ticks
		if ((this->currentTickUpdate - this->lastGoalChangeTick) >= EASY_TICK)
		{
			this->goalPosition = sf::Vector2f((float)(rand() % this->videoMode.width - 20), (float)(rand() % this->videoMode.height - 20));
			this->lastGoalChangeTick = this->currentTickUpdate;
		}
		//compare mouse position to circle center position, account for offset
		if (returnMagnitude((sf::Vector2f)this->storedMousePos, sf::Vector2f(this->objects.circle.getPosition().x + this->objects.circle.getRadius(),
			this->objects.circle.getPosition().y + this->objects.circle.getRadius())) <= 25.f)
		{
			// if condition is true mouse is on object, therefore shrink and increment points
			this->objects.circle.setRadius(lerpFunction(this->objects.circle.getRadius(), 0.f, DAMAGE_RATE));
			this->objects.circle.setFillColor(sf::Color(lerpFunction((float)this->objects.circle.getFillColor().r, 255.f, DAMAGE_RATE),
				lerpFunction((float)this->objects.circle.getFillColor().g, 0.f, DAMAGE_RATE),
				lerpFunction((float)this->objects.circle.getFillColor().b, 0.f, DAMAGE_RATE)));
			this->currentPoints++;
		}
		else
		{
			this->accuracyPoints++;
		}
		this->objects.circle.setPosition(lerpFunction(this->objects.circle.getPosition().x, this->goalPosition.x, EASY_SPEED),
			lerpFunction(this->objects.circle.getPosition().y, this->goalPosition.y, EASY_SPEED));
	}
	//Reaction Mode
	else if (mode == 2)
	{
		/*
		* Check for when the circle goes out of bounds
		* Temporary solution to an issue where the circle goes haywire and goes out of bounds, i suspect its a problem with the rand() function
		*/
		if (returnMagnitude(this->objects.circle.getPosition(), sf::Vector2f(this->videoMode.width / 2.f, this->videoMode.height / 2.f)) >= 2000.f)
		{
			this->objects.circle.setPosition(sf::Vector2f(this->videoMode.width / 2.f, this->videoMode.height / 2.f));
		}

		/*
		* Check for time difference, if too great set new position
		* Check for when the mouse is activated
		* Do distance check on the object
		* If condition is true respawn object with different size and color
		* Reset mouseActivated variable
		*/
		if ((this->currentTickUpdate - this->lastGoalChangeTick) >= EASY_TICK*3)
		{
			this->goalPosition = sf::Vector2f((float)(rand() % this->videoMode.width - 20), (float)(rand() % this->videoMode.height - 20));
			this->objects.circle.setRadius((rand() % 15) + 5);
			this->objects.circle.setFillColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
			this->objects.circle.setPosition(goalPosition);
			this->lastGoalChangeTick = this->currentTickUpdate;
		}
		else if (this->mouseActivated == 1)
		{
			if (returnMagnitude((sf::Vector2f)this->storedMousePos, sf::Vector2f(this->objects.circle.getPosition().x + this->objects.circle.getRadius(),
				this->objects.circle.getPosition().y + this->objects.circle.getRadius())) <= 25.f)
			{
				// if condition is true mouse is on object, therefore respawn with different size and color
				this->goalPosition = sf::Vector2f((float)(rand() % this->videoMode.width - 20), (float)(rand() % this->videoMode.height - 20));
				this->objects.circle.setRadius((rand() % 15) + 5);
				this->objects.circle.setFillColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
				this->objects.circle.setPosition(goalPosition);

				this->currentPoints++;
				this->lastGoalChangeTick = this->currentTickUpdate;
			}
			this->accuracyPoints++;
			this->mouseActivated = 0;
		}
			
	}
	
	this->objects.points.setString(std::string("Points: ") + std::to_string((int)this->currentPoints));
	this->objects.accuracy.setString(std::string("Accuracy: ") + std::to_string((int)round((this->currentPoints / this->accuracyPoints) * 100.f)) + std::string("%"));

	//Increment update tick
	this->currentTickUpdate++;
}

void Engine::drawObjects()
{
	this->windowObject->draw(this->objects.circle);
	this->windowObject->draw(this->objects.points);
	this->windowObject->draw(this->objects.accuracy);
}

Engine::Engine(int mode)
{
	this->mode = mode;
	this->initializeProperties();
	this->initializeWindow();
}

Engine::~Engine()
{
	delete this->windowObject;
}

const bool Engine::isWindowOpen()
{
	return this->windowObject->isOpen();
}

void Engine::update()
{
	/*
	@returns void

	- Updates game logic
	- Updates window
	*/

	this->pollEvents();
	updateObject();
	
}

void Engine::render()
{
	/*
	@returns void

	- Draws game objects

	*/
	this->windowObject->clear(BACKGROUND_COLOR);
	drawObjects();
	this->windowObject->display();
}
