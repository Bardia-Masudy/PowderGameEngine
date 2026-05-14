#include "Cell.h"
#include "../simulator/Grid.h"
#include <algorithm>
#include <cstdlib>

#define MAX_VELOCITY 10.f

Cell::Cell(int x, int y, Grid *grid) : x{x}, y{y}, grid{grid} {
    vSpeed = 0;
    hSpeed = 0;
    setMaterial(AIR);
}

void Cell::step(const Chunk *chunk) {
    if (int currentFrame = grid->getCurrentFrame(); steppedFrame == currentFrame) return;
    else steppedFrame = currentFrame;

    if (gravity) {
        vSpeed += density * gravity;
    }

    if (hSpeed || vSpeed) {
        hSpeed = std::clamp(hSpeed, -MAX_VELOCITY, MAX_VELOCITY);
        vSpeed = std::clamp(vSpeed, -MAX_VELOCITY, MAX_VELOCITY);
        attemptMove(hSpeed, vSpeed, chunk);
    }
}

void Cell::attemptMove(int hDist, int vDist, const Chunk *chunk) {
    int dx = std::abs(hDist), sx = (hDist >= 0) ? 1 : -1;
    int dy = -std::abs(vDist), sy = (vDist >= 0) ? 1 : -1;
    int err = dx + dy, newX = x, newY = y, e2;

    while (true) {
        if (hSpeed == 0 && vSpeed == 0) { break; }
        e2 = 2 * err;
        if (e2 <= dx) {
            Cell *target = grid->getCell(newX, newY + sy);
            if (!chunk->isVisible(newX, newY + sy) || target == nullptr) {
                vSpeed = 0;
                break;
            }
            if (target != this && target->density >= density) {
                swapCells(newX, newY);
                return collideCells(target);
            }

            err += dx;
            vSpeed -= sy;
            newY += sy;
        }
        if (e2 >= dy) {
            Cell *target = grid->getCell(newX + sx, newY);
            if (!chunk->isVisible(newX + sx, newY) || target == nullptr) {
                hSpeed = 0;
                break;
            }
            if (target != this && target->density >= density) {
                swapCells(newX, newY);
                return collideCells(target);
            }

            err += dy;
            hSpeed -= sx;
            newX += sx;
        }
    }
    swapCells(newX, newY);
}

void Cell::collideCells(Cell *other) {
    float ratio = static_cast<float>(density) / (static_cast<float>(density) + static_cast<float>(other->density));
    if (hSpeed != 0 && x != other->x) {
        float momentum = ratio * hSpeed;
        other->hSpeed += orientToVector(std::abs(hSpeed - momentum), other->hSpeed, steppedFrame >> 2 & 1);
        vSpeed += orientToVector(std::abs(momentum), vSpeed, steppedFrame >> 2 & 1);
        hSpeed = 0;
    }
    if (vSpeed != 0 && y != other->y) {
        float momentum = ratio * vSpeed;
        other->vSpeed += orientToVector(std::abs(vSpeed - momentum), other->vSpeed, steppedFrame >> 2 & 1);
        hSpeed += orientToVector(std::abs(momentum), hSpeed, steppedFrame >> 2 & 1);
        vSpeed = 0;
    }
}

int Cell::orientToVector(float magnitude, float vector, bool rand) {
    return (rand) ? (vector > 0) ? magnitude : -magnitude : (vector >= 0) ? magnitude : -magnitude;
}

void Cell::swapCells(int newX, int newY) {
    int prevX = x;
    int prevY = y;
    Cell *target = grid->getCell(newX, newY);

    std::swap(*this, *target);

    this->setPosition(prevX, prevY);
    target->setPosition(newX, newY);
}

Uint32 Cell::getColour() const {
    return baseColour;
}

bool Cell::shouldStep() const {
    return gravity != 0; // TODO: temp implementation for ignoring air.
}

void Cell::setPosition(int x, int y) {
    this->x = x;
    this->y = y;
}

void Cell::setMaterial(int material) {
    vSpeed = 0;
    hSpeed = 0;
    switch (material) {
        case AIR:
            density = 0;
            gravity = 0;
            baseColour = 0xffffffFF;
            break;
        case WATER:
            density = 2;
            gravity = 1;
            baseColour = 0x4782d6FF;
            break;
        case FIRE:
            density = 1;
            gravity = -1;
            baseColour = 0xdb300dFF;
            break;
        case WOOD:
            density = 16;
            gravity = 0;
            baseColour = 0x703c1eFF;
            break;
        default:
            density = 0;
            gravity = 0;
            baseColour = 0x8716a1FF;
            break;
    }
}
