#include "Scene.h"

Scene::Scene() : mOrigin{{}} {}

Scene::Scene(Vector2 origin, const char *bgHexCode) : mOrigin{origin}, mBGColourHexCode {bgHexCode} 
{
    ClearBackground(ColorFromHex(bgHexCode));
}

bool Scene::isColliding(const Vector2 *positionA, const Vector2 *scaleA, 
                        const Vector2 *positionB, const Vector2 *scaleB) const
{
    float xDistance = fabs(positionA->x - positionB->x) - ((scaleA->x + scaleB->x) / 2.0f);
    float yDistance = fabs(positionA->y - positionB->y) - ((scaleA->y + scaleB->y) / 2.0f);

    return (xDistance < 0.0f && yDistance < 0.0f);
}