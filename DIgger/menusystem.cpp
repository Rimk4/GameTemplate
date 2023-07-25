#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <glut.h>

#include "menusystem.h"

using namespace std;

void printMenu(Menu* m)
{
    cout << "Menu (" << (m->getItems()).size() << ") name:" << m->getName() << endl << "[" << endl;
    for (size_t i = 0; i < (m->getItems()).size(); i++)
    {
        cout << (m->getItems())[i]->getName() << " ";
    }
    if (m->getMenu() != nullptr)
        printMenu(m->getMenu());
    cout << endl << "]" << endl;
}

void printSystem(vector<DialogWindow> box)
{
    for (size_t i = 0; i < box.size(); i++)
    {
        cout << "Dialog:" << box[i].getName() << endl;
        printMenu(box[i].getMenu());
    }
}

// Функция чтения данных из файла
vector<DialogWindow> readDataFromFile(string filename) {
    ifstream file(filename);
    vector<DialogWindow> system;

    if (file.is_open()) {
        // считываем данные из файла и создаем объекты классов
        DialogWindow* dialogWindow = nullptr;
        Menu* menu = nullptr;
        Menu* subMenu = nullptr;
        string line;
        while (getline(file, line)) {
            if (line.find("Dialog window->(") != string::npos) {
                // создаем объект Диалоговое окно
                dialogWindow = new DialogWindow(menu);
                dialogWindow->setName(line.substr(line.find("Dialog window->(") + 16));
            }
            else if (line.find("Menu->(") != string::npos) {
                // создаем объект Меню
                menu = new Menu(line.substr(line.find("Menu->(") + 7));
            }
            else if (line.find("Submenu->(") != string::npos) {
                // создаем объект Подменю
                subMenu = new Menu(line.substr(line.find("Submenu->(") + 10));
            }
            else if (line.find("Item") != string::npos) {
                // создаем объект Пункт меню
                string name = line.substr(line.find("Item") + 5);
                Menu* item = new Menu(name);
                if (subMenu != nullptr) {
                    subMenu->addItem(item);
                }
                else {
                    menu->addItem(item);
                }
            }
            else if (line.find(")") != string::npos) {
                // закрываем объекты
                if (subMenu != nullptr) {
                    menu->setMenu(subMenu);
                    subMenu = nullptr;
                }
                else if (menu != nullptr) {
                    dialogWindow->setMenu(menu);
                    menu = nullptr;
                }
                else if (dialogWindow != nullptr)
                {
                    system.push_back(*dialogWindow);
                    dialogWindow = nullptr;
                }
            }
        }
        file.close();
    }
    else {
        cout << "Ошибка открытия файла\n";
    }
    printSystem(system);
    return system;
}

// Константы для размеров окна и элементов меню
int WINDOW_WIDTH;
int WINDOW_HEIGHT;
const int MENU_WIDTH = 208;
const int MENU_HEIGHT = 300;
const int MENU_PADDING = 10;
const int MENU_ITEM_HEIGHT = 30;
const int MAX_LENGTH_OF_NAME = 19;

vector<DialogWindow> menuItems = readDataFromFile("menu.txt");
int win_ind;
Menu* currentMenu;
bool subMenuIsShown;

void drawString(const string& str, float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(0.15, -0.15, 1); // исправляем перевернутый текст
    for (auto c : str) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, c);
    }
    glPopMatrix();
}

void reshape(int width, int height) {
    WINDOW_WIDTH = width;
    WINDOW_HEIGHT = height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, height, 0);
    glMatrixMode(GL_MODELVIEW);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    glTranslatef((WINDOW_WIDTH - MENU_WIDTH) / 2, (WINDOW_HEIGHT - MENU_HEIGHT) / 2, 0);
    // Рисуем меню
    glColor3f(0.5, 0.5, 0.5);
    glRectf(0, 0, MENU_WIDTH, MENU_HEIGHT);
    glColor3f(1, 1, 1);
    int k = 0;
    for (auto& item : currentMenu->getItems()) {
        glRectf(0, (MENU_ITEM_HEIGHT + MENU_PADDING) * k, MENU_WIDTH, (MENU_ITEM_HEIGHT + MENU_PADDING) * k + MENU_ITEM_HEIGHT);
        glColor3f(0, 0, 0);
        drawString(item->getName(), (MAX_LENGTH_OF_NAME - item->getName().size() + 1) / 2 * MENU_WIDTH / (MAX_LENGTH_OF_NAME - 1), \
            (MENU_ITEM_HEIGHT + MENU_PADDING) * k + MENU_ITEM_HEIGHT / 2 + 8);

        glColor3f(1, 1, 1);
        k++;
    }
    if (currentMenu->getMenu()->getName() != "")
    {
        glRectf(0, (MENU_ITEM_HEIGHT + MENU_PADDING) * k, MENU_WIDTH, (MENU_ITEM_HEIGHT + MENU_PADDING) * k + MENU_ITEM_HEIGHT);
        glColor3f(0, 0, 0);
        drawString(currentMenu->getMenu()->getName(), (MAX_LENGTH_OF_NAME - \
            currentMenu->getMenu()->getName().size() + 1) / 2 * MENU_WIDTH / (MAX_LENGTH_OF_NAME - 1), \
            (MENU_ITEM_HEIGHT + MENU_PADDING) * k + MENU_ITEM_HEIGHT / 2 + 8);

        glColor3f(1, 1, 1);
    }
    glPopMatrix();

    glFlush();
}

void displaySubMenu()
{
    display();

    glPushMatrix();
    glTranslatef((WINDOW_WIDTH - MENU_WIDTH) / 2, (WINDOW_HEIGHT - MENU_HEIGHT) / 2, 0);
    int k = currentMenu->getItems().size() + 1;
    for (auto& item : currentMenu->getMenu()->getItems()) {
        glRectf(0, (MENU_ITEM_HEIGHT + MENU_PADDING) * k, MENU_WIDTH, (MENU_ITEM_HEIGHT + MENU_PADDING) * k + MENU_ITEM_HEIGHT);
        glColor3f(0, 0, 0);
        drawString(item->getName(), (MAX_LENGTH_OF_NAME - item->getName().size() + 1) / 2 * MENU_WIDTH / (MAX_LENGTH_OF_NAME - 1), \
            (MENU_ITEM_HEIGHT + MENU_PADDING) * k + MENU_ITEM_HEIGHT / 2 + 8);

        glColor3f(1, 1, 1);
        k++;
    }

    glPopMatrix();
    glFlush();
}

void mouse(int button, int state, int x, int y) {
    x -= (WINDOW_WIDTH - MENU_WIDTH) / 2;
    y -= (WINDOW_HEIGHT - MENU_HEIGHT) / 2;
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Проверяем, было ли нажатие внутри меню
        if (x < MENU_WIDTH && y < MENU_HEIGHT) {
            int k = 0;
            // Ищем пункт меню, на который было сделано нажатие
            for (auto& item : currentMenu->getItems()) {
                if (x >= 0 && x <= MENU_WIDTH && y >= (MENU_ITEM_HEIGHT + MENU_PADDING) * k && y <= (MENU_ITEM_HEIGHT + MENU_PADDING) * k + MENU_ITEM_HEIGHT) {
                    subMenuIsShown = false;

                    // Действие при нажатии на пункт меню
                    cout << "Clicked on " << item->getName() << endl;
                    switch (win_ind)
                    {
                    case 0:
                        if (k == 0)
                        {
                            startGame();
                        }
                        else if (k == 1)
                        {
                            win_ind = 1;
                            createDialog();
                        }
                        else if (k == 2)
                        {
                            win_ind = 2;
                            createDialog();
                        }
                        else if (k == 3)
                        {
                            exit(0);
                        }
                        break;
                    case 1:
                        glClear(GL_COLOR_BUFFER_BIT);
                        if (k == 0)
                        {
                            glClearColor(1, 0, 0, 1);
                        }
                        else if (k == 1)
                        {
                            glClearColor(0, 1, 0, 1);
                        }
                        else if (k == 2)
                        {
                            glClearColor(0, 0, 0, 1);
                        }
                        display();
                        break;
                    case 2:
                        display();
                        break;
                    default:
                        break;
                    }

                    break;
                }
                k++;
            }
            if (subMenuIsShown)
            {
                k = currentMenu->getItems().size() + 1;
                for (auto& item : currentMenu->getMenu()->getItems()) {
                    if (x >= 0 && x <= MENU_WIDTH && y >= (MENU_ITEM_HEIGHT + MENU_PADDING) * k && y <= (MENU_ITEM_HEIGHT + MENU_PADDING) * k + MENU_ITEM_HEIGHT)
                    {
                        // Действие при нажатии на пункт меню
                        cout << "Clicked on " << item->getName() << endl;
                        switch (win_ind)
                        {
                        case 1:
                            if (k == 4)
                            {
                                win_ind = 0;
                                createDialog();
                            }
                            else if (k == 5)
                            {
                                win_ind = 2;
                                createDialog();
                            }
                            break;
                        case 2:
                            if (k == 3)
                            {
                                win_ind = 0;
                                createDialog();
                            }
                            else if (k == 4)
                            {
                                win_ind = 1;
                                createDialog();
                            }
                            break;
                        default:
                            break;
                        }

                        break;
                    }
                    k++;
                }
            }
            k = currentMenu->getItems().size();
            if (currentMenu->getMenu()->getName() != "" || subMenuIsShown)
            {
                if (x >= 0 && x <= MENU_WIDTH && y >= (MENU_ITEM_HEIGHT + MENU_PADDING) * k && y <= (MENU_ITEM_HEIGHT + MENU_PADDING) * k + MENU_ITEM_HEIGHT) {
                    // Действие при нажатии на пункт меню
                    cout << "Clicked on " << currentMenu->getMenu()->getName() << endl;
                    subMenuIsShown = true;
                    if (subMenuIsShown)
                    {
                        displaySubMenu();
                    }
                    else
                    {
                        glClear(GL_COLOR_BUFFER_BIT);
                        display();
                        glFlush();
                    }
                }
            }
        }
    }
}

void createDialog()
{
    subMenuIsShown = false;
    glutDestroyWindow(glutGetWindow());
    glutCreateWindow((menuItems[win_ind].getName()).c_str());
    currentMenu = menuItems[win_ind].getMenu();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutReshapeFunc(reshape);
}

void initMenu()
{
    WINDOW_WIDTH = 600;
    WINDOW_HEIGHT = 480;
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    win_ind = 0;
    glutCreateWindow((menuItems[win_ind].getName()).c_str());
    createDialog();
}

const int rows = 10; // количество строк
const int cols = 15; // количество столбцов
vector<vector<bool>> Map;

#include <algorithm>
#include <random>

void generateMap() {
    std::ofstream mapFile("map.txt"); // открываем файл для записи

    // коэффициенты вероятности для пустых и закрытых клеток
    const double emptyProb = 0.4;
    const double wallProb = 0.6;

    // генерируем карту
    std::vector<std::vector<int>> map(rows, std::vector<int>(cols, 0)); // создаем пустую карту
    std::random_device rd; // используем случайный генератор
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0, 1); // распределение вероятностей

    // заполняем карту случайными препятствиями и свободными клетками
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (i == 0 || i == rows - 1 || j == 0 || j == cols - 1) {
                // граница карты
                map[i][j] = 1;
            }
            else if (i == 1 && j == 1) {
                // место, где появляется персонаж
                map[i][j] = 0;
            }
            else {
                // случайное препятствие или свободное место
                if (dis(gen) < wallProb) {
                    map[i][j] = 1;
                }
                else {
                    map[i][j] = 0;
                }
            }
        }
    }

    // алгоритм генерации лабиринта
    for (int i = 2; i < rows - 2; i += 2) {
        for (int j = 2; j < cols - 2; j += 2) {
            // соединяем пустые клетки в лабиринт
            std::vector<std::pair<int, int>> neighbors;
            if (map[i - 1][j] == 0) {
                neighbors.push_back(std::make_pair(i - 1, j));
            }
            if (map[i][j - 1] == 0) {
                neighbors.push_back(std::make_pair(i, j - 1));
            }
            if (map[i + 1][j] == 0) {
                neighbors.push_back(std::make_pair(i + 1, j));
            }
            if (map[i][j + 1] == 0) {
                neighbors.push_back(std::make_pair(i, j + 1));
            }
            if (!neighbors.empty()) {
                std::shuffle(neighbors.begin(), neighbors.end(), gen);
                int ni = neighbors[0].first;
                int nj = neighbors[0].second;
                map[ni][nj] = 0;
            }
        }
    }

    // записываем карту в файл
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (map[i][j] == 1) {
                mapFile << "#";
            }
            else {
                mapFile << ".";
            }
        }
        mapFile << ';';
    }

    mapFile.close(); // закрываем файл
}

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLuint texture;

void loadTexture() {
    // Load texture from file
    int width, height;
    unsigned char* data = stbi_load("stone.jpg", &width, &height, 0, STBI_rgb);
   
    // Bind texture
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // Set texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    // Free image data
    stbi_image_free(data);
}

void drawBlock(float x1, float y1, float x2, float y2) {
    // Enable texturing
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Draw block
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x1, y1, 0.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x2, y1, 0.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x2, y2, 0.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x1, y2, 0.0f);
    glEnd();

    // Disable texturing
    glDisable(GL_TEXTURE_2D);
}

vector<vector<bool>> readMap()
{
    vector<vector<bool>> map(rows, vector<bool>(cols, true));
    // открываем файл для чтения карты
    std::ifstream mapFile("map.txt");
    if (!mapFile.is_open()) {
        std::cerr << "Error: cannot open map file" << std::endl;
        return map;
    }

    // отрисовываем карту
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j <= cols; j++) {
            char c;
            mapFile >> c;
            cout << c;
            if (c == ';') continue;
            if (c == '.') {
                cout << "(" << i << ", " << j << ")" << endl;
                cout << map.size() << map.at(i).size() << endl;
                map.at(i).at(j) = false;
            }
        }
        cout << endl;
    }

    mapFile.close(); // закрываем файл

    return map;
}

void drawScene()
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    // отрисовываем карту
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (Map.at(i).at(j)) {
                // препятствие
                glColor3f(0.5, 0.25, 0.0); // коричневый цвет
            }
            else {
                // свободное место
                glColor3f(0.0, 0.0, 0.0); // черный цвет
            }
            drawBlock(j * WINDOW_WIDTH / 15, i * WINDOW_HEIGHT / 10, (j + 1) * WINDOW_WIDTH / 15, (i + 1) * WINDOW_HEIGHT / 10); // отрисовываем квадрат
        }
    }

    glFlush();
}

void startGame()
{
    WINDOW_HEIGHT = 300;
    WINDOW_WIDTH = 450;

    glutDestroyWindow(glutGetWindow());
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Map");

    generateMap();
    Map = readMap();
    loadTexture();
    glutDisplayFunc(drawScene);
    glutReshapeFunc(reshape);
}