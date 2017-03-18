#include "unit.h"
#include "math.h"
#include <iostream>



Unit::Unit()
{
    srand( time( 0 ) );
    life = 1; // оживили
    hp = 100;
    max_hp = 150;
    srchfd = false;
    srchvr = false;
    parent = false;
    mkchld = false;
    mkvr = false;
    rlbck = false;
    food_id = 0;
    partner_id = 0;
    texture.loadFromFile("unit_s.png");     //  текстура
    texture.setSmooth(1);   // сглаживание текстур
    obj.setTexture(&texture);   // ссылка на текстуру
    tx=rand()%1000; //  случайная координата на цель X
    ty=rand()%800;  // случайная координата на цель Y
    max_speed = 0.2;  // максимальная скорость
    obj.setPosition(rand()%700, rand()%500);// установка первичного местоположения
    unitd = 3;  // 3 градуса в секунду при повороте объекта
    calculate();    //      первичный расчет
    odeg = deg;     //      приравниваем предыдущий угол поворота к новому, т.к. нет нужды в прорисовке поворота
    obj.setRotation(deg);   //  установка поворота объекта относительно первичной цели

}

void Unit::draw(sf::RenderWindow &w)//
{
    passed_time = global_clock.getElapsedTime().asMilliseconds();   // расчет пройденного времени
    if (life)   {
            if (passed_time > (4000 + rand()%2000) &&  odeg == deg && !srchfd && !mkchld)     //  прошло ли 4-6 секунды после получения новой координаты?
            {
                global_clock.restart();     // сброс часов
                tx = rand()%845;        // установка случайных координат
                ty = rand()%645;
            }
            if (srchfd)     {
                if (obj.getPosition() != food_xy) {
                    tx = food_xy.x;
                    ty = food_xy.y;
                }
                if (f_lenght(food_xy, obj.getPosition()) <= 1)   {
                        srchfd = false;
                        srchvr = true;
                    }
            }

            if (mkchld)  {
                if (obj.getPosition() != partner_xy)    {
                    tx = partner_xy.x;
                    ty = partner_xy.y;
                }
                if (f_lenght(partner_xy, obj.getPosition()) <= 1)   {
                        mkchld = false;
                        mkvr = true;
                }
            }






            if (tx != ox && ty != oy)   // пришли новые координаты?
                calculate();    //              расчет расстояния и шага
            if (odeg != deg)    // угол отличается?
                angle();    //               рассчитываем шаг поворота
            if ((odeg == deg))   {  // угол не отличается?
            odeg = deg;     // приравниваем предыдущий угол к новому
            crsz = false;   // сбрасываем флаг перехода через ноль
            }
            coll(); //      проверка (и установка) коллизий
            if ( obj.getPosition().x != tx && obj.getPosition().y != ty )   {   // объект не достиг целевой точки?
                if (odeg != deg)    // поворачиваем объект?
                    obj.move(unitVector*max_speed/4.f);   // Поворачиваем и двигаемся со скоростью, которая
                else                                         //меньше основной скорости движения в n раз
                    obj.move(unitVector*max_speed);     // иначе двигаемся с основной скоростью
            }

            w.draw(obj);    //      основная отрисовка в полученном по ссылке окне
    }
}

void Unit::calculate()
{
    direction = sf::Vector2f(tx, ty) - obj.getPosition();   //    вектор точки
    lenght = sqrt((direction.x * direction.x) + (direction.y * direction.y));   //  расстояние до точки
    unitVector.x = (direction.x / lenght);  //  векторы движения
    unitVector.y = (direction.y / lenght);
    ox = tx; oy = ty;   // ВНИМАНИЕ: Y=0 в SFML находится в самой верхней части поля, Y = 600 - в самом низу.
                    // X = 0 находится слева, X = 800 справа (от центра).
                    // посчитаем угол поворота, используя разницу в координатах текущей точки и целевой

    if (tx > obj.getPosition().x && ty > obj.getPosition().y)   {      // если целевые X и Y больше текущих X и Y
        deg = atan( direction.y / direction.x ) * 180 / pi;         // то целевой градус есть атангенс частного от деления
    }                                                              // противолежащего катета на прилежащий (y на x)
    if (tx > obj.getPosition().x && ty < obj.getPosition().y)   {       //  по аналогии
        deg = (atan ( direction.x / ((-1) * direction.y) ) * 180 / pi) + 270;       // избавляемся от минуса в знаменателе (целевая координата Y
    }                                                                               // меньше (численно) текущей для получения фактического градуса
    if (tx < obj.getPosition().x && ty > obj.getPosition().y)   {
        deg = (atan( (-1) * direction.x / direction.y ) * 180 / pi) + 90;
    }
    if (tx < obj.getPosition().x && ty < obj.getPosition().y)   {
        deg = ( atan (direction.y / direction.x ) * 180 / pi) +180;
    }

    if (tx == obj.getPosition().x && ty > obj.getPosition().y)  // случаи для передвижения в одной плоскости
        deg = 90;
    if (tx == obj.getPosition().x && ty < (-1) * obj.getPosition().y)
        deg = 270;
    if (tx > obj.getPosition().x && ty == obj.getPosition().y)
        deg = 0;
    if ( (-1) * tx < obj.getPosition().x && ty == obj.getPosition().y)
        deg = 180;

    if (deg<0) deg = 360 + deg; // было решено использовать ось от 0 до 360 градусов, без отрицательных градусов
}

void Unit::angle()
{
    if (odeg > deg) {       // если текущий угол поворота объекта численно меньше нового
        if (360-odeg+deg < odeg - deg)  {   // выбираем, в какую сторону поворачиваться
            if (odeg == 360 || (odeg < 360 && (odeg+unitd >= 360))) {
                odeg = 360 - odeg; crsz = true;
            }
            if (odeg+unitd > deg)   {
                odeg += unitd;
                obj.rotate(unitd);
            }
            else if (odeg != unitd && !crsz)   {
                odeg = deg;
                obj.setRotation(deg);
            }
        }
        else  {
            if (odeg-unitd > deg)   {
                odeg -= unitd;
                obj.rotate(-1*unitd);
            }
            else {
            odeg = deg;
            obj.setRotation(deg);
            }
        }
    }
    else
        if (odeg < deg)  {
            if (360-deg+odeg > deg - odeg ) {
                if (odeg+unitd < deg)  {
                    odeg+=unitd;
                    obj.rotate(unitd);
                }
                else {
                    odeg = deg;
                    obj.setRotation(deg);
                }
            }
            else    {
                if (odeg == 0 || (odeg > 0 && odeg - unitd <= 0)) {
                    odeg = 360 - unitd; crsz = true;
                }
                if (odeg-unitd < deg)   {
                    odeg-=unitd;
                    obj.rotate(-1*unitd);
                }
                else if (odeg!= deg && !crsz)  {
                    obj.setRotation(deg); odeg = deg;
                }
            }
        }

}

void Unit::coll()
{
    if (obj.getPosition().x - w/4 < 0)  {
        obj.setPosition(800.f - (w/6), obj.getPosition().y);
        srchfd = 0;
        srchvr = 0;
    }
    if (obj.getPosition().x + w/4 > 800)    {
        obj.setPosition((w/4), obj.getPosition().y);
        srchfd = 0;
        srchvr = 0;
    }
    if (obj.getPosition().y - w/4 < 0)  {
        obj.setPosition(obj.getPosition().y , 600.f - (w/4));
        srchfd = 0;
        srchvr = 0;
    }
    if (obj.getPosition().y + w/4 > 600 )   {
        obj.setPosition(obj.getPosition().y, (w/4));
        srchfd = 0;
        srchvr = 0;
    }
}

void Unit::gen(int i, float x, float y, float k)
{
    id = i;
    w= 30 / k, h= 20 / k; // размеры
    obj.setSize(sf::Vector2f(w, h));    //размеры объекта 45 на 30
    obj.setOrigin(w/2, h/2);     // центр объекта по середине сторон
    obj.setPosition(x, y);
}

sf::Vector2f Unit::pos(){
    return obj.getPosition();
}

void Unit::food(int i, sf::Vector2f xy) {
    food_id = i;
    food_xy = xy;
    srchfd = true;
}

int Unit::retFoodId()   {
    return food_id;
}void child(int , sf::Vector2f);

bool Unit::search_food() {
    return srchfd;
}

bool Unit::search_over()    {
    return srchvr;
}

void Unit::set_over()   {
    srchvr = false;
}

float Unit::f_lenght(sf::Vector2f p_pos, sf::Vector2f u_pos)    {
    sf::Vector2f direction = p_pos - u_pos;
    return sqrt(direction.x*direction.x + direction.y*direction.y);
}

int Unit::retUnitHp()   {
    if ((max_hp - hp) > 0) return true; // если HP не максимальное, то вернет 1
    else return false;  // если HP максимальное, вернет ноль
}

void Unit::setUnitHp(int t_hp)  {
    if (hp + t_hp <= max_hp)
        hp = hp + t_hp;
    else
        hp = max_hp;
    //std::cout << id << "'s health point = " << hp << std::endl;
}

int Unit::retUnitId()   {
    return id;
}

bool Unit::retMakeChild()   {
    return mkchld;
}

bool Unit::retMakeChildOver()   {
    return mkvr;
}

bool Unit::is_alive()   {
    return life;
}

void Unit::child(int i, sf::Vector2f part_xy) {
    partner_id = i;
    partner_xy = part_xy;
    mkchld = true;

}

void Unit::setMakeChildOver()   {
    mkvr = false;
}

int Unit::getPartnerId()    {
    return partner_id;
}

bool Unit::retUnitParent()  {
    return parent;
}

void Unit::setParentTrue()    {
    parent = true;
}

bool Unit::isUnitOld()  {
    if (w == 30) return true;
    else return false;
}

void Unit::RollBack(float x, float y, bool lf)   {
    obj.setPosition(x, y);
    life = lf;
}
