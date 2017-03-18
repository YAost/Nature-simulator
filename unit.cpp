#include "unit.h"
#include "math.h"
#include <iostream>



Unit::Unit()
{
    srand( time( 0 ) );
    life = 1; // �������
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
    texture.loadFromFile("unit_s.png");     //  ��������
    texture.setSmooth(1);   // ����������� �������
    obj.setTexture(&texture);   // ������ �� ��������
    tx=rand()%1000; //  ��������� ���������� �� ���� X
    ty=rand()%800;  // ��������� ���������� �� ���� Y
    max_speed = 0.2;  // ������������ ��������
    obj.setPosition(rand()%700, rand()%500);// ��������� ���������� ��������������
    unitd = 3;  // 3 ������� � ������� ��� �������� �������
    calculate();    //      ��������� ������
    odeg = deg;     //      ������������ ���������� ���� �������� � ������, �.�. ��� ����� � ���������� ��������
    obj.setRotation(deg);   //  ��������� �������� ������� ������������ ��������� ����

}

void Unit::draw(sf::RenderWindow &w)//
{
    passed_time = global_clock.getElapsedTime().asMilliseconds();   // ������ ����������� �������
    if (life)   {
            if (passed_time > (4000 + rand()%2000) &&  odeg == deg && !srchfd && !mkchld)     //  ������ �� 4-6 ������� ����� ��������� ����� ����������?
            {
                global_clock.restart();     // ����� �����
                tx = rand()%845;        // ��������� ��������� ���������
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






            if (tx != ox && ty != oy)   // ������ ����� ����������?
                calculate();    //              ������ ���������� � ����
            if (odeg != deg)    // ���� ����������?
                angle();    //               ������������ ��� ��������
            if ((odeg == deg))   {  // ���� �� ����������?
            odeg = deg;     // ������������ ���������� ���� � ������
            crsz = false;   // ���������� ���� �������� ����� ����
            }
            coll(); //      �������� (� ���������) ��������
            if ( obj.getPosition().x != tx && obj.getPosition().y != ty )   {   // ������ �� ������ ������� �����?
                if (odeg != deg)    // ������������ ������?
                    obj.move(unitVector*max_speed/4.f);   // ������������ � ��������� �� ���������, �������
                else                                         //������ �������� �������� �������� � n ���
                    obj.move(unitVector*max_speed);     // ����� ��������� � �������� ���������
            }

            w.draw(obj);    //      �������� ��������� � ���������� �� ������ ����
    }
}

void Unit::calculate()
{
    direction = sf::Vector2f(tx, ty) - obj.getPosition();   //    ������ �����
    lenght = sqrt((direction.x * direction.x) + (direction.y * direction.y));   //  ���������� �� �����
    unitVector.x = (direction.x / lenght);  //  ������� ��������
    unitVector.y = (direction.y / lenght);
    ox = tx; oy = ty;   // ��������: Y=0 � SFML ��������� � ����� ������� ����� ����, Y = 600 - � ����� ����.
                    // X = 0 ��������� �����, X = 800 ������ (�� ������).
                    // ��������� ���� ��������, ��������� ������� � ����������� ������� ����� � �������

    if (tx > obj.getPosition().x && ty > obj.getPosition().y)   {      // ���� ������� X � Y ������ ������� X � Y
        deg = atan( direction.y / direction.x ) * 180 / pi;         // �� ������� ������ ���� �������� �������� �� �������
    }                                                              // ��������������� ������ �� ���������� (y �� x)
    if (tx > obj.getPosition().x && ty < obj.getPosition().y)   {       //  �� ��������
        deg = (atan ( direction.x / ((-1) * direction.y) ) * 180 / pi) + 270;       // ����������� �� ������ � ����������� (������� ���������� Y
    }                                                                               // ������ (��������) ������� ��� ��������� ������������ �������
    if (tx < obj.getPosition().x && ty > obj.getPosition().y)   {
        deg = (atan( (-1) * direction.x / direction.y ) * 180 / pi) + 90;
    }
    if (tx < obj.getPosition().x && ty < obj.getPosition().y)   {
        deg = ( atan (direction.y / direction.x ) * 180 / pi) +180;
    }

    if (tx == obj.getPosition().x && ty > obj.getPosition().y)  // ������ ��� ������������ � ����� ���������
        deg = 90;
    if (tx == obj.getPosition().x && ty < (-1) * obj.getPosition().y)
        deg = 270;
    if (tx > obj.getPosition().x && ty == obj.getPosition().y)
        deg = 0;
    if ( (-1) * tx < obj.getPosition().x && ty == obj.getPosition().y)
        deg = 180;

    if (deg<0) deg = 360 + deg; // ���� ������ ������������ ��� �� 0 �� 360 ��������, ��� ������������� ��������
}

void Unit::angle()
{
    if (odeg > deg) {       // ���� ������� ���� �������� ������� �������� ������ ������
        if (360-odeg+deg < odeg - deg)  {   // ��������, � ����� ������� ��������������
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
    w= 30 / k, h= 20 / k; // �������
    obj.setSize(sf::Vector2f(w, h));    //������� ������� 45 �� 30
    obj.setOrigin(w/2, h/2);     // ����� ������� �� �������� ������
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
    if ((max_hp - hp) > 0) return true; // ���� HP �� ������������, �� ������ 1
    else return false;  // ���� HP ������������, ������ ����
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
