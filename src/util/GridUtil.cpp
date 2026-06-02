#include "GridUtil.h"

#include <cmath>
#include <functional>

// Courtesy Amantides & Woo, grid traversal
void GridUtil::castRay(float x0, float y0, float x1, float y1, CoordAction& stepFn, CoordPredicate& stopFn, CoordAction& endFn) {
    float dx = x1 - x0, dy = y1 - y0;
    float len = std::sqrt(dx * dx + dy * dy);
    float rdx = dx / len, rdy = dy / len; // unit direction

    if (len < 1e-4f) {
        int cx = (int) x0, cy = (int) y0;
        if (!stopFn(cx, cy)) stepFn(cx, cy);
        return;
    }

    int cx = (int) x0, cy = (int) y0;
    int sx = (rdx >= 0) ? 1 : -1, sy = (rdy >= 0) ? 1 : -1;

    const float inf =  std::numeric_limits<float>::infinity();
    float tDeltaX = std::abs(1.0f / rdx), tDeltaY = std::abs(1.0f / rdy);
    float tMaxX = (rdx != 0) ? std::abs((cx + sx - x0) / rdx) : inf;
    float tMaxY = (rdy != 0) ? std::abs((cy + sy - y0) / rdy) : inf;

    while (true) {
        if (stopFn(cx, cy)) return endFn(cx, cy);
        stepFn(cx, cy);
        if ((tMaxX < tMaxY ? tMaxX : tMaxY) >= len) return;
        if (tMaxX < tMaxY)  { cx += sx; tMaxX += tDeltaX; }
        else                { cy += sy; tMaxY += tDeltaY; }
    }

}