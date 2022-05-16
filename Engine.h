#ifndef GAME_ENGINE

#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

struct GameObjects {
	sf::Text points;
	sf::Text accuracy;
	sf::CircleShape circle;
};

class Engine
{
private:
	//Engine Objects
	sf::RenderWindow* windowObject;
	sf::VideoMode videoMode;
	sf::Event event;
	sf::Font font;
	

	//Properties
	int mode;
	int lastGoalChangeTick;
	int currentTickUpdate;
	float currentPoints;
	float accuracyPoints;
	int mouseActivated;
	sf::Vector2f goalPosition;
	sf::Vector2i storedMousePos;
	sf::Vector2f lastPosition;
	GameObjects objects;

	//Private Methods
	void initializeProperties();
	void initializeWindow();
	void pollEvents();
	void updateObject();
	void drawObjects();

public:
	Engine(int mode);
	virtual ~Engine();

	//Public Methods
	const bool isWindowOpen();
	void update();
	void render();
};

#endif