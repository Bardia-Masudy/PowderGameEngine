#ifndef ROGUEFUN_GRIDUTIL_H
#define ROGUEFUN_GRIDUTIL_H
#include <functional>

// For use with castRay
using CoordPredicate = const std::function<bool(int x, int y)>;
using CoordAction = const std::function<void(int x, int y)>;

// Helper class for abstracted common grid functions.
class GridUtil {
public:

    // Traces a line from x0,y0 to x1,y1. Applies stepFn for each point along the line until stopFn returns true,
    // then runs endFn.
    static void castRay(float x0, float y0, float x1, float y1, const std::function<void(int x, int y)> &stepFn,
                 const std::function<bool(int x, int y)> &stopFn, const std::function<void(int x, int y)> &endFn);
};


#endif //ROGUEFUN_GRIDUTIL_H
