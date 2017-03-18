#ifndef UNIT_H
#define UNIT_H
#include <SFML/Graphics.hpp>
#include <math.h>

const double pi = atan(1.0)*4;


class Unit {
    public:
        Unit();
        bool is_alive();
        void draw(sf::RenderWindow &w);    // отрисовка объекта и его поведения
        void calculate();   // расчёт unitVector, deg
        void angle();   //  поворот объекта
        void coll();    // коллизия
        void gen(int, float, float, float); // генерация начальных данных извне (ТАК КАК НЕ РАБОТАЕТ РАНДОМ)
        sf::Vector2f pos();     // возвращает текущее положение объекта
        void food(int , sf::Vector2f);   // устанавливает параметры
        bool search_food(); // возвращает инфу, ищет ли объект еду в данный момент
        bool search_over(); // возвращает инфу, съел ли объект еду и "закончил" поиск
        void set_over();    // сбрасывает флаг окончания поедания
        float f_lenght(sf::Vector2f, sf::Vector2f);
        int retFoodId();    // возвращает ID съедаемого
        int retUnitHp();    // возвращает HP объекта
        void setUnitHp(int);    // устанавливает HP объекта на основе HP еды
        bool retUnitParent();   // является ли объект родителем
        void setParentTrue();   // делает объект родителем
        int retUnitId();   // возвращает ID объекта
        bool retMakeChild();    //  создаётся ли ребенок
        bool retMakeChildOver();    // закончилось ли создание
        void setMakeChildOver();    // сбрасывает флаг процесса рождения
        void child(int , sf::Vector2f);
        int getPartnerId();
        bool isUnitOld();
        void RollBack(float, float, bool);
    private:
        sf::CircleShape col;
        int id; // идентификатор
        bool life; // жив ли вообще
        bool parent; // является ли родителем
        int partner_id;  // id мужа / жены
        bool mkchld; // флаг создания ребенка
        bool mkvr; // флаг окончания создания ребенка
        int hp; // текущее значение здоровья
        int max_hp; // максимальное значение здоровья
        float w, h; //  размеры объекта (нужно для коллизии)
        sf::RectangleShape obj; //      объект из библиотеки SFML прямоугольной формы
        sf::Texture texture;    //      текстура
        float tx, ty; //    координаты целевой точки
        float ox, oy; //   предыдущая координата
        float max_speed;//  максимальная скорость
        sf::Vector2f direction; //  вектор до целевой точки
        float lenght;   //  расстояние до точки
        sf::Vector2f unitVector;  //    единичный вектор для перемещения до точки в секунду, есть direction / lenght
        float odeg, deg; // угол поворота объекта (старый и новый)
        bool crsz;  //  флаг, обозначающий переход градуса поворота через ноль (угол поворота от 0 до 360)
        float unitd;    //  градус поворота в секунду, постоянный
        sf::Clock global_clock; //  инициализация SFML часов
        float passed_time;  // учет пройденного времени
        int food_id;  //  идентификатор цели "покушать"
        bool srchfd;    //  флаг поиска еды (нацеливание)
        bool srchvr; // флаг окончания поиска еды
        float food_lenght; // возможное расстояние до цели "покушать"
        sf::Vector2f food_xy;   // координаты еды
        sf::Vector2f partner_xy;
        bool rlbck;

};
#endif
