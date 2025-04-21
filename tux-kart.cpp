#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

// Class to handle the background
class Background {
public:
    sf::Sprite sprite;
    sf::Texture texture;

    Background(const std::string& textureFile, const sf::Vector2u& windowSize) {
        if (!texture.loadFromFile(textureFile)) {
            std::cerr << "Failed to load background texture!" << std::endl;
        }
        sprite.setTexture(texture);
        // Scale the background to fit the window
        float scaleX = static_cast<float>(windowSize.x) / texture.getSize().x;
        float scaleY = static_cast<float>(windowSize.y) / texture.getSize().y;
        sprite.setScale(scaleX, scaleY);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(sprite);
    }
};

// Class to handle a character (either Kart or Tux)
class Character {
    public:
        sf::Sprite sprite;
        sf::Texture texture;
        sf::Texture normalTuxTexture;
        float speed;
        bool isTux;
    
        Character(const std::string& textureFile, const sf::Vector2u& windowSize, float initialSpeed, bool initialIsTux) {
            if (!texture.loadFromFile(textureFile)) {
                std::cerr << "Failed to load character texture!" << std::endl;
            }
            sprite.setTexture(texture);
            sprite.setPosition(300.f, 300.f); // Initial position
            speed = initialSpeed;
            isTux = initialIsTux;
    
            // Load normal Tux texture
            if (!normalTuxTexture.loadFromFile("images/tux.png")) {
                std::cerr << "Failed to load normal Tux texture!" << std::endl;
            }
    
            setScaleForCharacter();
        }
    
        void move(const sf::Time& deltaTime) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                sprite.move(0, -speed * deltaTime.asSeconds());
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                sprite.move(0, speed * deltaTime.asSeconds());
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                sprite.move(-speed * deltaTime.asSeconds(), 0);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                sprite.move(speed * deltaTime.asSeconds(), 0);
            }
        }
    
        void switchToTux() {
            isTux = true;
            speed = 50.f;
            sprite.setTexture(normalTuxTexture);  // Use the normal Tux texture
            setScaleForCharacter();
        }
    
        void switchToKart() {
            isTux = false;
            speed = 280.f;
            sprite.setTexture(texture);  // Use the kart texture
            setScaleForCharacter();
        }
    
        void setScaleForCharacter() {
            if (isTux) {
                sprite.setScale(0.25f, 0.25f);  // Normal Tux smaller size
            } else {
                sprite.setScale(0.35f, 0.35f);  // Kart Tux slightly bigger
            }
        }
    
        void draw(sf::RenderWindow& window) {
            window.draw(sprite);
        }
    };

// Main Game Class that encapsulates the game loop and logic
class Game {
public:
    sf::RenderWindow window;
    Background background;
    Character kart;
    sf::Clock clock;
    bool tuxMode;

    Game() : window(sf::VideoMode(1920, 1080), "Kart Tux Game"),
         background("images/race_track.png", window.getSize()), 
         kart("images/kart_tux.png", window.getSize(), 200.f, false),
         tuxMode(false) 
{
    window.setFramerateLimit(60); // <-- Add this line
}

    void processEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                tuxMode = !tuxMode;
                if (tuxMode) {
                    kart.switchToTux();  // Switch to Tux mode
                } else {
                    kart.switchToKart();  // Switch to Kart mode
                }
            }
        }
    }

    void update() {
        sf::Time deltaTime = clock.restart();
        kart.move(deltaTime);
    }

    void render() {
        window.clear();
        background.draw(window);
        kart.draw(window);
        window.display();
    }

    void run() {
        while (window.isOpen()) {
            processEvents();
            update();
            render();
        }
    }
};

int main() {
    Game game;
    game.run();
    return 0;
}