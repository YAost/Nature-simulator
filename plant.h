#ifndef PLANT_H
#define PLANT_H
#include <SFML/Graphics.hpp>

class Plant {
    public:
        Plant();
        void draw(sf::RenderWindow &w);
        void gen(int, float, float, float);
        sf::Vector2f pos();
        int p_id();
        void delete_p();
        bool is_alive();
        int retPlantId();
        int retPlantHp();
        void RollBack(bool);
    private:
        sf::RectangleShape obj;
        sf::CircleShape coll;
        sf::Texture texture;
        int hp;
        int id;
        bool life;
        float w, h;
        bool rlbck;

};
#endif // PLANT_H

