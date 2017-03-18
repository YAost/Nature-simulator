

#include <math.h>
#include <mysql++.h>
#include <ssqls.h>
#include <iostream>
#include "unit.h"
#include "plant.h"
#include <vector>
#include <iterator>
#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>
#include <thread>
#include <iomanip>



using namespace std;


float f_lenght(sf::Vector2f, sf::Vector2f);
string to_string(int );

sql_create_4( U1, 4,1, mysqlpp::sql_float, x, mysqlpp::sql_float, y, mysqlpp::sql_int, t, mysqlpp::sql_bool, life)
mysqlpp::Connection con( "cpp", "localhost", "tdb_user", "tdb_password" );
mysqlpp::Query query = con.query();
sf::Clock Clock2;   int time2; int real_time = 0;   string ShowCurrentTime;
int passed_time;

bool stored = false;





int main()
{

    int u_id = 1; // счётчик особей (иии их идентификатор)
    int u_number = 8; // количество появляющихся особей
    int p_id = 1;   // счётчик растений (иии их идентификатор)
    int p_number = 100;   // количество появляющихся растений
    bool rlbck = false;
    sf::Clock clock1;   float time1;    // время времечко
    float rand_time = 4 + rand()%2;

    std::vector<Unit>::iterator uit;    // счётчик элементов динамического массива uit
    std::vector<Unit>::iterator ujt;    // счетчик элементов динамического массива для сравнения Unitов между собой
    std::vector<int>::iterator udt;     // счетчик элементов динамического массива ID особей на удаление из массива
    std::vector<int>::iterator pdt;     // счетчик элементов динамического массива ID растений на удаление их же. НЕГОДНИКОВ!
    std::vector<Unit> units;        // динамический массив Unitов
    std::vector<Unit> units_1;
    std::vector<int> unitsDelete;
    std::vector<int> plantsDelete;

    std::vector<Plant>::iterator pit;
    std::vector<Plant> plants;

    srand( time( 0 ) );
    Unit u1;
    Plant p1;

    sf::RenderWindow window(sf::VideoMode(800, 600), "Bio shit stuff", sf::Style::Close|sf::Style::Titlebar);
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(1);
    window.setKeyRepeatEnabled(false);

    sf::Text pause; // описание текста паузы
    sf::Font font;
    font.loadFromFile("1.ttf");
    pause.setFont(font);
    pause.setCharacterSize(30);
    pause.setString("Loading...");
    pause.setPosition(650.f, 0);
    pause.setColor(sf::Color::Black);

    // описание часов, показывающих время жизни
    sf::Text CurrentTime;
    CurrentTime.setFont(font);
    CurrentTime.setCharacterSize(30);
    CurrentTime.setOrigin(0, 0);
    CurrentTime.setColor(sf::Color::Black);
    CurrentTime.setPosition(12.f, 0);


    while (window.isOpen()) {
        sf::Event event;
        window.clear(sf::Color::White);
        while (window.pollEvent(event)) { // опрос событий
            if (event.type == sf::Event::Closed)    {
                query << "DROP DATABASE cpp;";
                query.execute();
                query << "CREATE DATABASE cpp;";
                query.execute();
                window.close();
                return 0;
            }
            if (event.type == sf::Event::KeyPressed && (sf::Keyboard::isKeyPressed(sf::Keyboard::Insert)))   {   // Возврат на 22 секунду
                //rlbck = !rlbck;                                                                      // по нажатию пробела
                if (real_time > 22) {
                    real_time = 22;
                    window.clear(sf::Color::White);
                    for (pit=plants.begin();pit<plants.end();++pit) pit->draw(window);
                    for (uit=units.begin();uit<units.end();++uit) uit->draw(window);
                    window.draw(pause);
                    window.display();
                    for (uit=units.begin();uit<units.end();++uit)   {
                            query << "SELECT * FROM spawnTime WHERE id = " << uit->retUnitId() << "&& time <= 22;";
                            mysqlpp::StoreQueryResult res1 = query.store();
                            if  (!res1.empty()) {
                                query << "SELECT * FROM U" << uit->retUnitId() << " WHERE t = 22;";
                                if ( mysqlpp::UseQueryResult res = query.use())  {
                                    while ( mysqlpp::Row row = res.fetch_row() )    {   cout << uit->retUnitId() << " has it's history on server" << endl;
                                        cout << "Do rollback for " << uit->retUnitId() <<  endl;
                                        uit->RollBack(row["x"], row["y"], row["life"]);
                                    }
                                    query << "DELETE FROM U" << uit->retUnitId() << " WHERE t >= 22;";
                                    query.exec();
                                }
                                else  {
                                    cout << uit->retUnitId() << " doesn't have an answer " << query.error() << endl;
                                }
                            }
                            else    {   cout << "for " << uit->retUnitId() << " we DON'T have data" << endl;
                                query << "DELETE FROM spawnTime WHERE id = " << uit->retUnitId() << ";";
                                query.execute();
                                query << "DROP TABLE U" << uit->retUnitId() << ";";
                                query.execute();
                                unitsDelete.push_back(uit->retUnitId());
                                u_id--;

                            }
                    }
                    for (pit=plants.begin();pit<plants.end();++pit) {
                        query << "SELECT * FROM P WHERE id = " << pit->retPlantId() << ";";
                        if ( mysqlpp::UseQueryResult res = query.use())  {  //cout << "We have something" << endl;
                                int tSpawn, tEaten;
                                while ( mysqlpp::Row row = res.fetch_row() )    {
                                    tSpawn = row["tSpawn"];
                                    tEaten = row["tEaten"];
                                }
                                //cout << "Spawn time = " << tSpawn << " Eaten time = " << tEaten << " where id = " << pit->retPlantId() << endl;
                                if (tSpawn > 22) {
                                    query << "DELETE FROM P WHERE id = " << pit->retPlantId() << ";";
                                    query.execute();
                                    plantsDelete.push_back(pit->retPlantId());  //удаление из массива
                                    p_id--;
                                }
                                if ((tSpawn <= 22) && (tEaten > 22))  {
                                    query << "UPDATE P SET tEaten = " << tSpawn << ", life = 1 WHERE id = " << pit->retPlantId() << ";";
                                    query.execute();
                                    pit->RollBack(1);
                                }
                                if ((tSpawn <= 22) && (tEaten < 22))  {
                                    //cout << pit->retPlantId() << " was eaten after this moment" << endl;
                                }
                        }
                        if ( con.errnum()) {
                            cerr << "No result for " << uit->retUnitId() << endl;
                        }
                    }
                    if (!unitsDelete.empty())   {
                        /*cout << "units:";
                        for (uit=units.begin();uit<units.end();++uit)
                            cout << setw(3) << uit->retUnitId();
                        cout << endl << "todel:";
                        for (udt=unitsDelete.begin();udt<unitsDelete.end();++udt)
                            cout << setw(3) << *udt;
                        cout << endl;*/
                        for (uit=units.begin();uit<units.end();uit++)   {
                            for (udt=unitsDelete.begin();udt<unitsDelete.end();++udt)   {
                                if (uit->retUnitId()==*udt)  {
                                    units.erase(uit);
                                }
                            }
                        }
                        unitsDelete.clear();
                        /*cout << "newun:";
                        for (uit=units.begin();uit<units.end();++uit)
                            cout << setw(3) << uit->retUnitId();
                        cout << endl;
                        cout << "delun:";
                        for (udt=unitsDelete.begin();udt<unitsDelete.end();++udt)
                            cout << setw(3) << *udt;
                        cout << endl;*/
                    }
                    if (!plantsDelete.empty())  {
                        /*cout << "plants:";
                        for (pit=plants.begin();pit<plants.end();++pit)
                            cout << setw(3) << pit->retPlantId();
                        cout << endl << "todel:";
                        for (pdt=plantsDelete.begin();pdt<plantsDelete.end();++pdt)
                            cout << setw(3) << *pdt;
                        cout << endl;*/
                        for (pit=plants.begin();pit<plants.end();pit++)   {
                            for (pdt=plantsDelete.begin();pdt<plantsDelete.end();++pdt)   {
                                if (pit->retPlantId()==*pdt)  {
                                    plants.erase(pit);
                                }
                            }
                        }
                        plantsDelete.clear();
                        /*for (pit=plants.begin();pit<plants.end();++pit)
                            cout << setw(3) << pit->retPlantId();
                        cout << "Current number of plants available to spawn is " << p_id << endl;*/
                    }

                    window.display();
                }

            }

        }
        if (!rlbck) {
            time1 = clock1.getElapsedTime().asSeconds();
            if (!units_1.empty())   {
                units.reserve(units_1.size());
                for (uit=units_1.begin();uit<units_1.end();++uit)
                    units.insert(units.end(), *uit);
                units_1.clear();
            }
            if (time1 > rand_time && u_id < u_number) {     // заполнение динамического массива (вектора)  ПЕРВИЧНЫЙ СПАВН N ОСОБЕЙ
                if (u_id == 1)  {   // создаем таблицу спавна на сервере
                        query <<"CREATE TABLE spawnTime(id int, time int) ENGINE=MEMORY ";
                        query.execute();
                }
                std::cout << u_id << " is ready on " << real_time << " sec" << std::endl;
                u1.gen(u_id, rand()%800, rand()%600, 1);    // элементами класса Unit с случайными координатами и последовательным id
                units.push_back(u1);
                clock1.restart();
                //cout << "creating table" << endl;
                query << "CREATE TABLE U" << u_id << "(x float, y float, t int, life bool) ENGINE=MEMORY;";
                query.execute();
                query << "INSERT INTO spawnTime VALUES(" << u_id << ", " << real_time << ");";
                query.execute();
                u_id++;
                rand_time = 4 + rand()%2;

            }

            if (p_id < p_number )    {       //  заполнения вектора растений объекта класса plant    ПЕРВИЧНЫЙ СПАВН N РАСТЕНИЙ
                float p_x = rand()%1000; float p_y = rand()%800; float k = 1 + rand()%2;
                p1.gen(p_id, p_x, p_y, k);
                plants.push_back(p1);
                if (p_id == 1) {
                    query << "CREATE TABLE IF NOT EXISTS P(id int, x float, y float, tSpawn int, tEaten int, life bool, k float) ENGINE=MEMORY;";
                    query.exec();
                }
                query << "INSERT INTO P VALUES(" << p_id << ", " << p_x << ", " << p_y << ", " << real_time << ", " << real_time << ", "  << 1 << ", " << k << ");";
                query.exec();
                p_id++;
            }

            for (uit=units.begin();uit<units.end();++uit)   {   // проверяем, съел ли объект еду
                if (uit->search_over()) {   // закончил поиск?
                    for (pit=plants.begin();pit<plants.end();++pit)
                        if (uit->retFoodId()==pit->p_id())  {   // если ID съеденного совпадает с ID в массиве
                            pit->delete_p();    // убивашки
                            query << "UPDATE P SET tEaten = " << real_time << ", life = 0 WHERE id = " << pit->p_id() << ";";
                            query.execute();
                            //std::cout << "plant's health point = " << pit->retPlantHp() << std::endl;
                            uit->setUnitHp(pit->retPlantHp());  // добавляем HP еды к HP объекта
                            uit->set_over();    // сбрасываем флаг окончания поиска
                        }
                }

            }
            for (uit=units.begin();uit<units.end();++uit)   {       //  генерация детишек
                if (uit->retMakeChildOver())    {
                    for (ujt=units.begin();ujt<units.end();++ujt)   {
                        if (uit->getPartnerId()==ujt->retUnitId())  {
                            uit->setMakeChildOver();
                            ujt->setMakeChildOver();
                            uit->setParentTrue();
                            ujt->setParentTrue();
                            //std::cout << "SUCCEED" << std::endl;
                            u1.gen(u_id, uit->pos().x, uit->pos().y, 2);
                            //std::cout << "PUSHING" << std::endl;
                            //cout << "creating table" << endl;
                            query << "CREATE TABLE U" << u_id << "(x float, y float, t int, life bool) ENGINE=MEMORY;";
                            query.execute();
                            query << "INSERT INTO spawnTime VALUES(" << u_id << ", " << real_time << ");";
                            query.execute();
                            units_1.push_back(u1);
                            u_id++;
                        }
                    }
                }
            }
            for (uit=units.begin();uit<units.end();++uit)   {   // сравнение координат блуждающих объектов и стоящих растений
                if (!uit->search_food() && !uit->search_over() && uit->retUnitHp())    {    // если объект: 1) не идет к еде 2) не ест 3) здоровье не максимальное
                    bool tar = false;   // флаг присутствия в округе объекта Unit желанного Plant
                    sf::Vector2f t_pos;
                    float lenght = 100;
                    int t_id = 0;
                    for (pit=plants.begin();pit<plants.end();++pit) {   // сравниваем каждый объект со всем списком растений
                        if ((pit->is_alive()) && (f_lenght(pit->pos(), uit->pos()) <= 50) && (f_lenght(pit->pos(), uit->pos()) < lenght))    { //если растение живо
                            t_pos = pit->pos();                             //и объект находится в 50 пикселях от растения и расстояние до растения
                            lenght = f_lenght(pit->pos(), uit->pos());      //является самым меньшим из возможных (выбираем самый короткий маршрут)
                            t_id = pit->p_id();                             // то запоминаем координату, длину и id растения
                            tar = true;                                     // устанавливаем флаг наличия хотя бы одного объекта в 1
                        }
                    }
                    if (tar) uit->food(t_id, t_pos);  // если в округе существует растение, то передаем объекту id и координаты лучшего варианта
                }
            }

            for (uit=units.begin();uit<units.end();++uit)   {   // Сравнение объектов между собой: если живы, взрослы, здоровы и не являтся родителем
                if (uit->isUnitOld() && (!uit->retMakeChild()) && (uit->is_alive()) && (!uit->retUnitHp()) && (!uit->retUnitParent())) {
                    bool tar = false;
                    sf::Vector2f t_pos;
                    float lenght = 100;
                    int t_id = 0;
                    for (ujt=units.begin();ujt<units.end();++ujt) {
                        if (uit->isUnitOld() && (!ujt->retMakeChild()) && (uit->retUnitId() != ujt->retUnitId()) && (ujt->is_alive()) && (!ujt->retUnitHp()) && (!ujt->retUnitParent())
                               && (f_lenght(ujt->pos(), uit->pos()) <= 30) && (f_lenght(ujt->pos(), uit->pos()) < lenght))  {
                            t_pos = ujt->pos();
                            lenght = f_lenght(ujt->pos(), uit->pos());
                            t_id = ujt->retUnitId();
                            tar = true;
                        }

                    }
                    if (tar)    {    std::cout << uit->retUnitId() << " and " << t_id << std::endl;
                        uit->child(t_id, t_pos);
                        for (ujt=units.begin();ujt<units.end();++ujt)   {
                            if (ujt->retUnitId() == t_id)
                                ujt->child(uit->retUnitId(), uit->pos());
                        }

                    }
                }
            }

            time2 = Clock2.getElapsedTime().asSeconds();
            for (pit=plants.begin();pit<plants.end();++pit)    {    // выбираем элемент массива растений
                pit->draw(window);                          // отрисовываем
            }
            for (uit=units.begin();uit<units.end();++uit)  {   // выбираем элемент массива особей (объектов)
                uit->draw(window);       // отрисовываем его
                if (time2>=1)   {
                    //query << "CREATE TABLE IF NOT EXISTS U" << uit->retUnitId() << "(x float, y float, t int, life bool) ENGINE=MEMORY; ";
                    //query.exec();
                    query << "INSERT INTO U" << uit->retUnitId() << " VALUES(" << uit->pos().x << ", " << uit->pos().y << ", " << real_time << ", " << uit->is_alive() << ");";
                    query.exec();
                }
            }

            if (time2>=1) {
                //cout << "Current number of Units is " << u_id - 1 << " and spawnmax is " << u_number << endl;
                real_time++;
                Clock2.restart();
            }


            ShowCurrentTime = to_string(real_time);
            CurrentTime.setString(ShowCurrentTime);
            window.draw(CurrentTime);
            window.display();
        }   // глобальная отрисовка всех элементов
    }
    return EXIT_SUCCESS;
}

float f_lenght(sf::Vector2f t_pos, sf::Vector2f u_pos)    {
    sf::Vector2f direction = t_pos - u_pos;
    return sqrt(direction.x*direction.x + direction.y*direction.y);
}


string to_string(int param)
{
    string str = "";
    stringstream ss;
    ss<<param;
    getline(ss, str);
    return str;
}
