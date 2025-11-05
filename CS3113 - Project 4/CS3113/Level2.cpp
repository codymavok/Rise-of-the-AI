#include "Level2.h"

Level2::Level2()                                      : Scene{{0.0f}, nullptr} {}
Level2::Level2(Vector2 origin, const char *bgHexCode) : Scene{origin, bgHexCode} {}

Level2::~Level2() { shutdown(); }

void Level2::initialise()
{
    mGameState.nextSceneID = 0;
    mGameState.lives = nullptr;
    mPlayerHit = false;

    mGameState.bgm = LoadMusicStream("Assets/game/forest_of_hope.mp3");
    SetMusicVolume(mGameState.bgm, 0.33f);
    PlayMusicStream(mGameState.bgm);

    mGameState.jumpSound = LoadSound("Assets/game/apricotjumpbounce-jump.ogg");
    mGameState.collisonSound = LoadSound("Assets/game/pop.ogg");
    mGameState.gameOverSound = LoadSound("Assets/game/Metal Hit.mp3");

    /*
        ----------- MAP -----------
    */
    mGameState.map = new Map(
        LEVEL2_WIDTH, LEVEL2_HEIGHT,          // map grid cols & rows
        (unsigned int *) mLevelData,        // grid data
        "Assets/game/DarkCastleGrid.png",   // texture filepath
        TILE_DIMENSION,                     // tile size
        5, 5,                               // texture cols & rows
        mOrigin                             // in-game origin
    );

    /*
        ----------- PROTAGONIST -----------
    */
    std::map<Direction, std::vector<int>> vincentAnimationAtlas = {
        {DOWN,  {  0,  1,  2,  3,  4,  5,  6,  7 }},
        {LEFT,  {  8,  9, 10, 11, 12, 13, 14, 15 }},
        {UP,    { 24, 25, 26, 27, 28, 29, 30, 31 }},
        {RIGHT, { 40, 41, 42, 43, 44, 45, 46, 47 }},
    };

    float sizeRatio  = 48.0f / 64.0f;


    mGameState.vincent = new Entity(
        {mOrigin.x - 500.0f, mOrigin.y - 200.0f}, // position
        {250.0f * sizeRatio, 250.0f},             // scale
        "assets/game/vincent.png",                // texture file address
        ATLAS,                                    // single image or atlas?
        {6, 8},                                   // atlas dimensions
        vincentAnimationAtlas,                    // actual atlas
        PLAYER                                    // entity type
    );

    mGameState.vincent->setJumpingPower(550.0f);
    mGameState.vincent->setColliderDimensions({
        mGameState.vincent->getScale().x / 3.5f,
        mGameState.vincent->getScale().y / 3.0f
    });
    mGameState.vincent->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});

  
    std::map<Direction, std::vector<int>> bearAnimationAtlas = {
        {LEFT,  {3, 7, 11}},
        {RIGHT, {0, 4, 8}},
    };

   
    float mapLeftBoundary = mOrigin.x - (LEVEL2_WIDTH * TILE_DIMENSION) / 2.0f;
    float mapTopBoundary = mOrigin.y - (LEVEL2_HEIGHT * TILE_DIMENSION) / 2.0f;


    float bear1X = mapLeftBoundary + (8.0f * TILE_DIMENSION) + (TILE_DIMENSION / 2.0f);
    float bear1Y = mapTopBoundary + (4.0f * TILE_DIMENSION);  

    float bear2X = mapLeftBoundary + (18.0f * TILE_DIMENSION) + (TILE_DIMENSION / 2.0f);
    float bear2Y = mapTopBoundary + (4.0f * TILE_DIMENSION);  


    mBear1 = new Entity(
        {bear1X, bear1Y}, // position
        {100.0f * sizeRatio, 100.0f},             // scale
        "Assets/game/Peacock-walk-Sheet.png",     // texture file address
        ATLAS,                                    // single image or atlas?
        {3, 4},                                   // atlas dimensions
        bearAnimationAtlas,                       // actual atlas
        NPC                                       // entity type
    );

    mBear2 = new Entity(
        {bear2X, bear2Y}, // position
        {100.0f * sizeRatio, 100.0f},             // scale
        "Assets/game/Peacock-walk-Sheet.png",     // texture file address
        ATLAS,                                    // single image or atlas?
        {3, 4},                                   // atlas dimensions
        bearAnimationAtlas,                       // actual atlas
        NPC                                       // entity type
    );

    mBear1->setSpeed(0);
    mBear1->setColliderDimensions({
        mBear1->getScale().x / 2.0f,
        mBear1->getScale().y
    });
    mBear1->setAcceleration({0.0f, 0.0f});
    mBear1->setDirection(RIGHT);
    mBear1TopBound = mapTopBoundary + (2.0f * TILE_DIMENSION); 
    mBear1BottomBound = mapTopBoundary + (6.5f * TILE_DIMENSION);

    mBear2->setSpeed(0);
    mBear2->setColliderDimensions({
        mBear2->getScale().x / 2.0f,
        mBear2->getScale().y
    });
    mBear2->setAcceleration({0.0f, 0.0f}); 
    mBear2->setDirection(LEFT);

    
    mBear2TopBound = mapTopBoundary + (1.5f * TILE_DIMENSION);  
    mBear2BottomBound = mapTopBoundary + (6.5f * TILE_DIMENSION); 


    mEnemies[0] = mBear1;
    mEnemies[1] = mBear2;

    mBear1->setVelocity({0.0f, BEAR_SPEED});
    mBear2->setVelocity({0.0f, -BEAR_SPEED});

    /*
        ----------- CAMERA -----------
    */
    mGameState.camera = { 0 };                                    // zero initialize
    mGameState.camera.target = mGameState.vincent->getPosition(); // camera follows player
    mGameState.camera.offset = mOrigin;                           // camera offset to center of screen
    mGameState.camera.rotation = 0.0f;                            // no rotation
    mGameState.camera.zoom = 1.0f;                                // default zoom
}

void Level2::panCamera(Camera2D *camera, const Vector2 *targetPosition)
{
    Vector2 positionDifference = Vector2Subtract(*targetPosition, camera->target);
    camera->target = Vector2Add(camera->target, Vector2Scale(positionDifference, 0.1f));
}

void Level2::update(float deltaTime)
{
    UpdateMusicStream(mGameState.bgm);

    mGameState.vincent->update(
        deltaTime,             // delta time
        nullptr,               // player
        mGameState.map,        // map
        nullptr,               // collidable entities
        0                      // col. entity count
    );

    Vector2 bear1Pos = mBear1->getPosition();
    Vector2 bear1Vel = mBear1->getVelocity();
    
    if (bear1Pos.y <= mBear1TopBound)
    {
        bear1Vel.y = BEAR_SPEED;
    }
    else if (bear1Pos.y >= mBear1BottomBound)
    {
        bear1Vel.y = -BEAR_SPEED;
    }

    mBear1->setVelocity(bear1Vel);
    mBear1->update(deltaTime, nullptr, nullptr, nullptr, 0);


    Vector2 bear2Pos = mBear2->getPosition();
    Vector2 bear2Vel = mBear2->getVelocity();
    
    if (bear2Pos.y <= mBear2TopBound)
    {
        bear2Vel.y = BEAR_SPEED;
    }
    else if (bear2Pos.y >= mBear2BottomBound)
    {
        bear2Vel.y = -BEAR_SPEED;
    }
    
    mBear2->setVelocity(bear2Vel);
    mBear2->update(deltaTime, nullptr, nullptr, nullptr, 0);

    // Collision check 
    if (!mPlayerHit)
    {
        if (!mPlayerHit && mGameState.vincent->isCollidingWithEnemy(mEnemies, 2))
        {
            mPlayerHit = true;
            PlaySound(mGameState.collisonSound);
            
            if (mGameState.lives != nullptr)
            {
                (*mGameState.lives)--;
                
             if (*mGameState.lives > 0)
                {
                    mGameState.nextSceneID = 2;
                }
                else
                {
                    mGameState.nextSceneID = -1;
                    PlaySound(mGameState.gameOverSound); 
                }
            }
        }
    }

    Vector2 currentPlayerPosition = {mGameState.vincent->getPosition().x, mOrigin.y};
    panCamera(&mGameState.camera, &currentPlayerPosition);

    // Player fell off
    if (!mPlayerHit && mGameState.vincent->getPosition().y > 800.0f) 
    {
        mPlayerHit = true;
        
        if (mGameState.lives != nullptr)
        {
            (*mGameState.lives)--;
            
            if (*mGameState.lives > 0)
                {
                    mGameState.nextSceneID = 2;
                }
                else
                {
                 mGameState.nextSceneID = -1;
                PlaySound(mGameState.gameOverSound); 
                }
        }
    }

    // Player reached the end so go to the next level
    Vector2 playerPos = mGameState.vincent->getPosition();
    Vector2 playerFeet = {
    playerPos.x,
    playerPos.y + (mGameState.vincent->getColliderDimensions().y / 2.0f)
    }; // Technically it is the players feet touching the tile
    unsigned int currentTile = mGameState.map->getTileAtPosition(playerFeet);
    if (currentTile == 10)
    {
        mGameState.nextSceneID = 3;  // Go to Level 3
    }
        
}

void Level2::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));

    mGameState.map->render();
    mGameState.vincent->render();
    mBear1->render();
    mBear2->render();

}

void Level2::shutdown()
{
    delete mGameState.vincent;
    delete mGameState.map;
    delete mBear1;
    delete mBear2;

    UnloadMusicStream(mGameState.bgm);
    UnloadSound(mGameState.jumpSound);
    UnloadSound(mGameState.collisonSound);
    UnloadSound(mGameState.gameOverSound);
}