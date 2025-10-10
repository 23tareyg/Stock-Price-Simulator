#include <iostream>
#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "Stock.hpp"
#include "Model.hpp"

using json = nlohmann::json;

int main() {
    // parsing json data into data object
    std::ifstream f("../data/test.json");
    json data = json::parse(f);

    std::mt19937 prng;
    std::shared_ptr<Stock> test_stock = std::make_shared<Stock>(data["stocks"][0]);
    int duration = static_cast<int>(data["duration_minutes"]);
    int timestep = static_cast<int>(data["timestep_minutes"]);

    GBMModel model(test_stock, duration, timestep);
    model.simulate(prng);
    model.print_data();

    ////////////

    // Create the main window
    sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML window");
 
    // Load a sprite to display
    const sf::Texture texture("cute_image.jpg");
    sf::Sprite sprite(texture);
 
    // Create a graphical text to display
    const sf::Font font("arial.ttf");
    sf::Text text(font, "Hello SFML", 50);
 
 
    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        while (const std::optional event = window.pollEvent())
        {
            // Close window: exit
            if (event->is<sf::Event::Closed>())
                window.close();
        }
 
        // Clear screen
        window.clear();
 
        // Draw the sprite
        window.draw(sprite);
 
        // Draw the string
        window.draw(text);
 
        // Update the window
        window.display();
    }


    return 0;
}