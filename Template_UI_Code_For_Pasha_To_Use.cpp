#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <optional>
using std::cout;
using std::endl;

int main(){
    sf::RenderWindow Jendela_UI(sf::VideoMode(sf::Vector2u{1200, 1080}), "Jendela UI", sf::Style::Titlebar | sf::Style::Close); //Untuk Buat Jendela
    Jendela_UI.setSize(sf::Vector2u{1200, 1080});
    sf::RectangleShape Tombol_Masuk ({100.f, 50.f});
    Tombol_Masuk.setFillColor(sf::Color::Green);
    Tombol_Masuk.setPosition({600.f, 400.f}); //Resolusi Jendela X dan Y dibagi dua biar ditengah
    sf::RectangleShape Tombol_Pengaturan ({100.f, 50.f});
    Tombol_Pengaturan.setFillColor(sf::Color::Red);
    Tombol_Pengaturan.setPosition({601.f, 480.f}); //Resolusi Jendela X dan Y dibagi dua biar ditengah
    sf::RectangleShape Tombol_Exit ({100.f, 50.f});
    Tombol_Exit.setFillColor(sf::Color::Blue);
    Tombol_Exit.setPosition({602.f, 560.f}); //Resolusi Jendela X dan Y dibagi dua biar ditengah


    while (Jendela_UI.isOpen()) {  // Untuk Masukin Code kamu
        while (const std::optional<sf::Event> Peristiwa = Jendela_UI.pollEvent()){
            if(Peristiwa->is<sf::Event::Closed>()) {
                Jendela_UI.close();
            } else if (const auto* pasha_bau = Peristiwa->getIf<sf::Event::MouseButtonPressed>()){ //Clickity
                float pasha_bau_x = pasha_bau->position.x;
                float pasha_bau_y = pasha_bau->position.y;
            
                
                if(Tombol_Masuk.getGlobalBounds().contains({pasha_bau_x, pasha_bau_y})) {
                    cout << "Tombol Masuk" << endl;
                } else if (Tombol_Pengaturan.getGlobalBounds().contains({pasha_bau_x, pasha_bau_y})) {
                    cout << "Tombol Pengaturan" << endl;
                } else if (Tombol_Exit.getGlobalBounds().contains({pasha_bau_x, pasha_bau_y})) {
                    cout << "Tombol Exit" << endl;
                } else {
                    cout << "Mouse di " << pasha_bau_x << " dan " << pasha_bau_y << endl; 
                }
            }

            
        }
        Jendela_UI.clear(sf::Color::Black);
        Jendela_UI.draw(Tombol_Masuk);
        Jendela_UI.draw(Tombol_Pengaturan);
        Jendela_UI.draw(Tombol_Exit);
        Jendela_UI.display();
        
    }
    

}