#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <optional>
#include <string>
#include <cmath>
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
class Meja : public Bentuk {
    private:
    float panjang;
    float lebar;
    float tebal_dinding;
    sf::RectangleShape wujud_meja;
    public:
    Meja(Koordinat posisi_kiri_atas, float p, float l, float tb= 30.f ) : 
    Bentuk(posisi_kiri_atas, sf::Color(20, 108, 44)), panjang(p), lebar(l), tebal_dinding(tb) 
    {
        wujud_meja.setSize({lebar, panjang});
        wujud_meja.setFillColor(Warna);
        wujud_meja.setPosition(getPosisiSFML());
        wujud_meja.setOutlineThickness(tebal_dinding);
        wujud_meja.setOutlineColor(sf::Color(101, 55, 24));
    }
    void update(float deltatime) override {}
    void kirim_ke_jendela(sf::RenderWindow& Jendela) override {
        Jendela.draw(wujud_meja);
    }
    float getbataskiri() const{return Posisi_dia.x;}
    float getbataskanan() const{return Posisi_dia.x + lebar;}
    float getbatasatas() const{return Posisi_dia.y;}
    float getbatasbawah() const {return Posisi_dia.y + panjang;}
};

class Bola : public Bentuk {
    private:
    float radius;
    sf::Vector2f Kecepatan;
    std::string nama_bola;
    bool status_bola;
    sf::CircleShape wujud_bola;
    const Meja* meja_ref;
    
    public:
    Bola(Koordinat pos = Koordinat(), float r = 15.0f, sf::Color w = sf::Color::White, std::string nama = "Bola", const Meja* meja = nullptr) 
    : Bentuk(pos, w), radius(r), nama_bola(nama), status_bola(true), Kecepatan{0.f, 0.f}, meja_ref(meja) {
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
        if (std::abs(Kecepatan.x) < 3.f && std::abs(Kecepatan.y) < 3.f) {
            Kecepatan = {0.f, 0.f};
        }
        if (meja_ref != nullptr) {
            float koefisien_pantul = -0.85f;

            // Dinding Kiri & Kanan (Sumbu X)
            if (Posisi_dia.x - radius <= meja_ref->getbataskiri()) {
                Posisi_dia.x = meja_ref->getbataskiri() + radius;
                Kecepatan.x *= koefisien_pantul;
            } 
            else if (Posisi_dia.x + radius >= meja_ref->getbataskanan()) {
                Posisi_dia.x = meja_ref->getbataskanan() - radius;
                Kecepatan.x *= koefisien_pantul;
            }

            // Dinding Atas & Bawah (Sumbu Y)
            if (Posisi_dia.y - radius <= meja_ref->getbatasatas()) {
                Posisi_dia.y = meja_ref->getbatasatas() + radius;
                Kecepatan.y *= koefisien_pantul;
            } 
            else if (Posisi_dia.y + radius >= meja_ref->getbatasbawah()) {
                Posisi_dia.y = meja_ref->getbatasbawah() - radius;
                Kecepatan.y *= koefisien_pantul;
            }
        }

        // Pindahkan setPosition ke paling bawah setelah koreksi dinding selesai
        wujud_bola.setPosition(getPosisiSFML());
    

    }
    void kirim_ke_jendela(sf::RenderWindow& Jendela) override {
        if (status_bola) {
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
    Meja mejaBilliard(Koordinat(260.f, 190.f), 700.f, 1400.f, 40.f);
    Bola bolaPutih(Koordinat(610.f, 540.f), 20.f, sf::Color::White, "Bola Putih", &mejaBilliard);
    Bola bolaMerah(Koordinat(1310.f, 540.f), 20.f, sf::Color::Red, "Bola Merah", &mejaBilliard);
    
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
        Jendela_Fisika.clear(sf::Color(30, 30, 30)); // Background ruangan gelap
        mejaBilliard.kirim_ke_jendela(Jendela_Fisika);
        bolaPutih.update(deltatime);
        bolaMerah.update(deltatime);
        bolaPutih.kirim_ke_jendela(Jendela_Fisika);
        bolaMerah.kirim_ke_jendela(Jendela_Fisika);

        Jendela_Fisika.display();
    
    }
    

    
}