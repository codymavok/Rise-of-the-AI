#include "Scene.h"

constexpr int LEVEL3_WIDTH  = 20,
              LEVEL3_HEIGHT = 8;

class Level3 : public Scene 
{
private:
    Entity* mBear;
    bool mPlayerHit = false; 
    Entity* mEnemies[1];
    
    unsigned int mLevelData[LEVEL3_WIDTH * LEVEL3_HEIGHT] = {

   3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
   
   1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,
   1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9,
   
   1, 0, 0, 2, 0, 2, 2, 0, 2, 2, 2, 2, 0, 0, 2, 0, 0, 0, 0, 1,
   1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
   
   1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
   1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
   
   2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 10, 2
};

public:
    static constexpr float TILE_DIMENSION = 75.0f;
    static constexpr float ACCELERATION_OF_GRAVITY = 981.0f;
    static constexpr float END_GAME_THRESHOLD      = 800.0f;
    static constexpr float BEAR_SPEED = 100.0f;

    Level3();
    Level3(Vector2 origin, const char *bgHexCode);
    ~Level3();
    
    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
    
    void panCamera(Camera2D *camera, const Vector2 *targetPosition);
};
