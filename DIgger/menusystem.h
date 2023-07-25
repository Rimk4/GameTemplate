#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <algorithm>
#include <random>

#include <glut.h>

using namespace std;

#ifndef MENUSYSTEM_H
#define MENUSYSTEM_H

// Класс Меню
class Menu {
private:
    string name;
    vector<Menu*> items; // вектор указателей на пункты меню
    Menu* submenu;
public:
    void addItem(Menu* i)
    {
        this->items.push_back(i);
    }
    Menu(string name) { // конструктор
        this->name = name;
    }
    void setItems(vector<Menu*> items)
    {
        this->items = items;
    }
    vector<Menu*> getItems() const
    {
        return this->items;
    }
    void setMenu(Menu* m)
    {
        this->submenu = m;
    }
    Menu* getMenu() const
    {
        return this->submenu;
    }
    string getName() const
    {
        return this->name;
    }
};

class DialogWindow {
private:
    string name;
    Menu* menu; // указатель на меню
public:
    void setMenu(Menu* m)
    {
        this->menu = m;
    }
    Menu* getMenu() const
    {
        return this->menu;
    }
    void setName(string name)
    {
        this->name = name;
    }
    string getName() const
    {
        return this->name;
    }
    DialogWindow(Menu* menu) { // конструктор
        this->menu = menu;
    }
};

void printMenu(Menu* m);

void printSystem(vector<DialogWindow> box);

vector<DialogWindow> readDataFromFile(string filename);

void drawString(const string& str, float x, float y);
void display();
void reshape(int width, int height);
void createDialog();
void displaySubMenu();
void mouse(int button, int state, int x, int y);
void initMenu();

void drawScene();
void startGame();
void generateMap();
vector<vector<bool>> readMap();

void drawBlock(float x1, float y1, float x2, float y2);
void loadTexture();

#endif