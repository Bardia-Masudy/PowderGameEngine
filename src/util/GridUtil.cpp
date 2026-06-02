#include "GridUtil.h"

#include <cmath>
#include <functional>

// Courtesy Amantides & Woo, grid traversal
void GridUtil::castRay(float x0, float y0, float x1, float y1, CoordAction& stepFn, CoordPredicate& stopFn, CoordAction& endFn) {
    float dx = x1 - x0, dy = y1 - y0;
    float len = std::sqrt(dx * dx + dy * dy);
    float rdx = dx / len, rdy = dy / len; // unit direction

    int cx = (int) x0, cy = (int) y0;
    int sx = (rdx >= 0) ? 1 : -1, sy = (rdy >= 0) ? 1 : -1;

    float tDeltaX = std::abs(1.0f / rdx), tDeltaY = std::abs(1.0f / rdy);
    float tMaxX = std::abs(((rdx != 0 ? cx + sx : cx) - x0) / rdx);
    float tMaxY = std::abs(((rdy != 0 ? cy + sy : cy) - y0) / rdy);

    while (true) {
        if (stopFn(cx, cy)) return endFn(cx, cy);
        stepFn(cx, cy);
        if (tMaxX < tMaxY)  { cx += sx; tMaxX += tDeltaX; }
        else                { cy += sy; tMaxY += tDeltaY; }
    }

}