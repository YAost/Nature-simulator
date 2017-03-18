#include "plant.h"
#include <SFML/Graphics.hpp>

Plant::Plant()  {
    life = 1;
    coll.setRadius(50);
    coll.setOrigin(50,50);
    coll.setOutlineColor(sf::Color::Red);
    coll.setOutlineThickness(0.1);
    texture.loadFromFile("plant_1.png");
    texture.setSmooth(1);
    obj.setTexture(&texture);
}


void Plant::draw(sf::RenderWindow &w)    {
    if (life)   {
        //w.draw(coll);
        w.draw(obj);
    }
}

void Plant::gen(int i, float x, float y, float k)   {
    id = i;
    hp = 20 / k;
    w = 20 / k; h = 20 / k;
    obj.setSize(sf::Vector2f(w, h));
    obj.setOrigin(w/2, h/2);
    obj.setPosition(x, y);
    coll.setPosition(x, y);
}

sf::Vector2f Plant::pos()   {
    return obj.getPosition();
}

int Plant::p_id()   {
    return id;
}

void Plant::delete_p()  {
    life = 0;
}

bool Plant::is_alive()   {
    return life;
}

int Plant::retPlantHp() {
    return hp;
}

void Plant::RollBack(bool lf)  {
    life = lf;
}

int Plant::retPlantId()    {
    return id;
}
