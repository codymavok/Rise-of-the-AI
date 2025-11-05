#include "Scene.h"

constexpr int LEVEL2_WIDTH  = 20,
              LEVEL2_HEIGHT = 8;

class Level2 : public Scene 
{
private:
    Entity* mBear1;
    Entity* mBear2;
    bool mPlayerHit = false; 
    Entity* mEnemies[2];

    float mBear1TopBound;
    float mBear1BottomBound;
    float mBear2TopBound;
    float mBear2BottomBound;
    
    unsigned int mLevelData[LEVEL2_WIDTH * LEVEL2_HEIGHT] = {

   3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
   
   1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,
   1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9,
   
   1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 1,
   1, 0, 0, 0, 0, 2, 2, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 1,
   
   1, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
   1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
   
   2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 10, 2
};

public:
    static constexpr float TILE_DIMENSION = 75.0f;
    static constexpr float ACCELERATION_OF_GRAVITY = 981.0f;
    static constexpr float END_GAME_THRESHOLD      = 800.0f;
    static constexpr float BEAR_SPEED = 150.0f;

    Level2();
    Level2(Vector2 origin, const char *bgHexCode);
    ~Level2();
    
    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
    
    void panCamera(Camera2D *camera, const Vector2 *targetPosition);
};
