#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <optional>
#include <string>
#include <vector>
using std::cout;
using std::endl;

class Koordinat {
    public:
    float x;
    float y;
    public:
    Koordinat(float x = 0.0f, float y=0.0f) : x(x), y(y) {}
};

class Bentuk {
    protected:
    Koordinat Posisi_dia;
    sf::Color Warna;
    float radius;
    public:
    Bentuk(Koordinat posisi = Koordinat(), sf::Color W = sf::Color::White) : Posisi_dia(posisi), Warna(W) {}
    virtual ~Bentuk() = default;
    virtual void kirim_ke_jendela(sf::RenderWindow& Jendela) = 0;
    virtual void update(float deltatime) = 0;
    Koordinat getposisi() const {return Posisi_dia;}
    void setposisi(const Koordinat& pos) {Posisi_dia = pos;}
    sf::Color getwarna() const {return Warna;}
    void setwarna(const sf::Color& w) {Warna = w;}
    sf::Vector2f getPosisiSFML() const {return sf::Vector2f{Posisi_dia.x, Posisi_dia.y};}
};

class Bola : public Bentuk {
    private:
    float radius;
    sf::Vector2f Kecepatan;
    std::string nama_bola;
    bool status_bola;
    sf::CircleShape wujud_bola;
    public:
    Bola(Koordinat pos = Koordinat(), float r = 15.0f, sf::Color w = sf::Color::White, std::string nama = "Bola") 
    : Bentuk(pos, w), radius(r), nama_bola(nama), status_bola(true), Kecepatan{0.f, 0.f} {
    wujud_bola.setRadius(radius);
    wujud_bola.setFillColor(Warna);
    wujud_bola.setOrigin({radius, radius});
    wujud_bola.setPosition(getPosisiSFML());
    }
    void update(float deltatime) override {
        if(!status_bola) return;
        Posisi_dia.x += Kecepatan.x * deltatime;
        Posisi_dia.y += Kecepatan.y * deltatime;
        wujud_bola.setPosition(getPosisiSFML());

    }
    void kirim_ke_jendela(sf::RenderWindow& Jendela) override {
        if(status_bola) {
            Jendela.draw(wujud_bola);
        }
    }
    void pukul(sf::Vector2f dorongan) {
        Kecepatan = dorongan;
    }
    sf::Vector2f getkecepatan() const {return Kecepatan;}
    void setKecepatan(sf::Vector2f v) {Kecepatan = v;}
    float getRadius() const { return radius; }
    bool isAktif() const { return status_bola; }
    void setStatus(bool status) { status_bola = status; }
    std::string getNama() const { return nama_bola; }
};











int main() {
    sf::RenderWindow Jendela_Fisika(sf::VideoMode(sf::Vector2u{1920, 1080}), "Jendela Fisika", sf::Style::Default, sf::State::Windowed); //Bikin Jendela FullScreen atau Windowed
    Jendela_Fisika.setFramerateLimit(60);
    Bola bolaPutih(Koordinat(300.f, 400.f), 20.f, sf::Color::White, "Bola Putih");
    Bola bolaMerah(Koordinat(800.f, 400.f), 20.f, sf::Color::Red, "Bola Merah");
    sf::Clock pewaktu;
    while (Jendela_Fisika.isOpen()){
        float deltatime = pewaktu.restart().asSeconds();
        while (const std::optional<sf::Event> Kejadian = Jendela_Fisika.pollEvent()) {
            if (Kejadian->is<sf::Event::Closed>()){
                Jendela_Fisika.close();
            } else if (const auto* Tekan_Mouse = Kejadian->getIf<sf::Event::MouseButtonPressed>()) {
                bolaPutih.pukul({600.f, 0.f});
                bolaMerah.pukul({600.f, 0.f});
                cout << "Bola Disodok!" << endl;
            }
        }
        bolaPutih.update(deltatime);
        bolaMerah.update(deltatime);

        // 3. RENDERING
        Jendela_Fisika.clear(sf::Color(20, 108, 44)); // Warna hijau meja billiard
        
        bolaPutih.kirim_ke_jendela(Jendela_Fisika);
        bolaMerah.kirim_ke_jendela(Jendela_Fisika);

        Jendela_Fisika.display();
    
    }
    

    
}