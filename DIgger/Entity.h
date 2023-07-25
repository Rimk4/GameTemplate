#pragma once

#include <iostream>
#include <vector>

class Entity {
public:
    Entity(int x, int y) : x(x), y(y) {}
    virtual ~Entity() {}

    virtual void move(int dx, int dy) {
        x += dx;
        y += dy;
    }

    int getX() const {
        return this->x;
    }

    int getY() const {
        return this->y;
    }

protected:
    int x;
    int y;
};

class Enemy : public Entity {
public:
    Enemy(int x, int y) : Entity(x, y) {}

    void move(int dx, int dy) override {
        x += dx * 2;
        y += dy * 2;
    }
};

class Player : public Entity {
public:
    Player(int x, int y) : Entity(x, y) {}

    void move(int dx, int dy) override {
        x += dx;
        y += dy;
    }
};

class Boulder : public Entity {
public:
    Boulder(int x, int y) : Entity(x, y) {}

    void move(int dx, int dy) override {
        // Булыжник не может двигаться по вертикали
        if (dx != 0) {
            x += dx;
        }
    }
};

class Block : public Entity {
public:
    Block(int x, int y) : Entity(x, y) {}
};

class Scene {
public:
    Scene(int width, int height) : width(width), height(height) {
        // Инициализируем карту
        map.resize(width);
        for (int x = 0; x < width; ++x) {
            map[x].resize(height);
        }
    }

    void addEntity(Entity* entity) {
        entities.push_back(entity);
        // Добавляем сущность на карту
        map[entity->getX()][entity->getY()] = entity;
    }

    void removeEntity(Entity* entity) {
        // Удаляем сущность из вектора
        for (auto it = entities.begin(); it != entities.end(); ++it) {
            if (*it == entity) {
                entities.erase(it);
                break;
            }
        }
        // Удаляем сущность с карты
        map[entity->getX()][entity->getY()] = nullptr;
    }

    void update() {
        // Обновляем каждую сущность
        for (auto entity : entities) {
            entity->move(1, 0); // Пример движения вправо
            // Проверяем, что сущность не вышла за границы карты
            if (entity->getX() < 0 || entity->getX() >= width || entity->getY() < 0 || entity->getY() >= height) {
                removeEntity(entity);
            }
        }
    }

private:
    int width;
    int height;
    std::vector<std::vector<Entity*>> map;
    std::vector<Entity*> entities;
};