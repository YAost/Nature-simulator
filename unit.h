#ifndef UNIT_H
#define UNIT_H
#include <SFML/Graphics.hpp>
#include <math.h>

const double pi = atan(1.0)*4;


class Unit {
    public:
        Unit();
        bool is_alive();
        void draw(sf::RenderWindow &w);    // ��������� ������� � ��� ���������
        void calculate();   // ������ unitVector, deg
        void angle();   //  ������� �������
        void coll();    // ��������
        void gen(int, float, float, float); // ��������� ��������� ������ ����� (��� ��� �� �������� ������)
        sf::Vector2f pos();     // ���������� ������� ��������� �������
        void food(int , sf::Vector2f);   // ������������� ���������
        bool search_food(); // ���������� ����, ���� �� ������ ��� � ������ ������
        bool search_over(); // ���������� ����, ���� �� ������ ��� � "��������" �����
        void set_over();    // ���������� ���� ��������� ��������
        float f_lenght(sf::Vector2f, sf::Vector2f);
        int retFoodId();    // ���������� ID ����������
        int retUnitHp();    // ���������� HP �������
        void setUnitHp(int);    // ������������� HP ������� �� ������ HP ���
        bool retUnitParent();   // �������� �� ������ ���������
        void setParentTrue();   // ������ ������ ���������
        int retUnitId();   // ���������� ID �������
        bool retMakeChild();    //  �������� �� �������
        bool retMakeChildOver();    // ����������� �� ��������
        void setMakeChildOver();    // ���������� ���� �������� ��������
        void child(int , sf::Vector2f);
        int getPartnerId();
        bool isUnitOld();
        void RollBack(float, float, bool);
    private:
        sf::CircleShape col;
        int id; // �������������
        bool life; // ��� �� ������
        bool parent; // �������� �� ���������
        int partner_id;  // id ���� / ����
        bool mkchld; // ���� �������� �������
        bool mkvr; // ���� ��������� �������� �������
        int hp; // ������� �������� ��������
        int max_hp; // ������������ �������� ��������
        float w, h; //  ������� ������� (����� ��� ��������)
        sf::RectangleShape obj; //      ������ �� ���������� SFML ������������� �����
        sf::Texture texture;    //      ��������
        float tx, ty; //    ���������� ������� �����
        float ox, oy; //   ���������� ����������
        float max_speed;//  ������������ ��������
        sf::Vector2f direction; //  ������ �� ������� �����
        float lenght;   //  ���������� �� �����
        sf::Vector2f unitVector;  //    ��������� ������ ��� ����������� �� ����� � �������, ���� direction / lenght
        float odeg, deg; // ���� �������� ������� (������ � �����)
        bool crsz;  //  ����, ������������ ������� ������� �������� ����� ���� (���� �������� �� 0 �� 360)
        float unitd;    //  ������ �������� � �������, ����������
        sf::Clock global_clock; //  ������������� SFML �����
        float passed_time;  // ���� ����������� �������
        int food_id;  //  ������������� ���� "��������"
        bool srchfd;    //  ���� ������ ��� (�����������)
        bool srchvr; // ���� ��������� ������ ���
        float food_lenght; // ��������� ���������� �� ���� "��������"
        sf::Vector2f food_xy;   // ���������� ���
        sf::Vector2f partner_xy;
        bool rlbck;

};
#endif
