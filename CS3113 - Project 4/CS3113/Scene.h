#include "Entity.h"

#ifndef SCENE_H
#define SCENE_H

struct GameState
{
    Entity *vincent;
    Map *map;

    Music bgm;
    Sound jumpSound;
    Sound collisonSound;
    Sound gameOverSound;

    Camera2D camera;

    int nextSceneID;
    int *lives; // Pointer to the number of lives from main
};

class Scene 
{
protected:
    GameState mGameState;
    Vector2 mOrigin;
    const char *mBGColourHexCode = "#000000";
    bool isColliding(const Vector2 *positionA, const Vector2 *scaleA, const Vector2 *positionB, const Vector2 *scaleB) const;
    
public:
    Scene();
    Scene(Vector2 origin, const char *bgHexCode);

    virtual void initialise() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
    virtual void shutdown() = 0;
    
    GameState   getState()           const { return mGameState; }
    Vector2     getOrigin()          const { return mOrigin;    }
    const char* getBGColourHexCode() const { return mBGColourHexCode; }

    void setLives(int* lives) { mGameState.lives = lives; }
};

#endif