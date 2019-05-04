/* noisette
 * Copyright (C) 2019 Pierre Vigier
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// STL
#include <iostream>
#include <chrono>
#include <algorithm>
// SFML
#include <SFML/Graphics.hpp>
// MyGAL
#include <noisette/noise.h>

using namespace nst;

constexpr auto TextureWidth = uint64_t(600); // in pixel
constexpr auto TextureHeight = uint64_t(600); // in pixel
constexpr auto NbPeriods = 10.0;

sf::Texture generateNoise()
{
    auto seed = static_cast<uint64_t>(std::chrono::system_clock::now().time_since_epoch().count());

    // Generate noise
    auto values = std::vector<double>(TextureWidth * TextureHeight);
    auto start = std::chrono::steady_clock::now();
    for (auto i = 0u; i < TextureHeight; ++i)
    {
        auto x = static_cast<double>(i) / static_cast<double>(TextureHeight) * NbPeriods;
        for (auto j = 0u; j < TextureWidth; ++j)
        {
            auto y = static_cast<double>(j) / static_cast<double>(TextureWidth) * NbPeriods;
            values[i * TextureWidth + j] = fractal_noise_2d(seed, x, y, 8, 2.0, 0.5);
        }
    }
    auto duration = std::chrono::steady_clock::now() - start;
    std::cout << "noise: " << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << "ms" << '\n';
    auto minmax = std::minmax_element(std::begin(values), std::end(values));
    auto min = *minmax.first;
    auto max = *minmax.second;
    auto delta = max - min;
    
    // Map to color
    auto pixels = std::vector<sf::Uint8>(4 * TextureWidth * TextureHeight);
    for (auto i = 0u; i < TextureHeight; ++i)
    {
        for (auto j = 0u; j < TextureWidth; ++j)
        {
            auto value = static_cast<sf::Uint8>((values[i * TextureWidth + j] - min) / delta * 255.0);
            for (auto k = 0u; k < 3; ++k)
                pixels[i * 4 * TextureWidth + j * 4 + k] = value;
            pixels[i * 4 * TextureWidth + j * 4 + 3] = 255;
        }
    }

    // Create a texture
    auto image = sf::Image();
    image.create(TextureWidth, TextureHeight, pixels.data());
    auto texture = sf::Texture();
    texture.loadFromImage(image);

    return texture;
}

int main()
{
    auto texture = generateNoise();

    // Display the texture
    sf::RenderWindow window(sf::VideoMode(TextureWidth, TextureHeight), "noisette");
    window.setFramerateLimit(60);
    auto sprite = sf::Sprite(texture);

    while (window.isOpen())
    {
        auto event = sf::Event();
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyReleased)
            {
                if (event.key.code == sf::Keyboard::Key::N)
                    texture = generateNoise();
            }
        }

        window.clear(sf::Color::Black);
        window.draw(sprite);
        window.display();
    }

    return 0;
}
