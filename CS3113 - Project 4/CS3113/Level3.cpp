#include "Level3.h"

Level3::Level3()                                      : Scene{{0.0f}, nullptr} {}
Level3::Level3(Vector2 origin, const char *bgHexCode) : Scene{origin, bgHexCode} {}

Level3::~Level3() { shutdown(); }

void Level3::initialise()
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
        LEVEL3_WIDTH, LEVEL3_HEIGHT,          // map grid cols & rows
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

    mBear = new Entity(
        {mOrigin.x + 200.0f, mOrigin.y - 200.0f}, // position
        {100.0f * sizeRatio, 100.0f},             // scale
        "Assets/game/Peacock-walk-Sheet.png",     // texture file address
        ATLAS,                                    // single image or atlas?
        {3, 4},                                   // atlas dimensions
        bearAnimationAtlas,                       // actual atlas
        NPC                                       // entity type
    );

    mBear->setAIType(WANDERER);
    mBear->setAIState(WALKING);
    mBear->setSpeed(Entity::DEFAULT_SPEED * 2.0f);
    mBear->setColliderDimensions({
        mBear->getScale().x / 2.0f,
        mBear->getScale().y
    });
    mBear->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});
    mBear->setDirection(RIGHT);

    mEnemies[0] = mBear;

    /*
        ----------- CAMERA -----------
    */
    mGameState.camera = { 0 };                                    // zero initialize
    mGameState.camera.target = mGameState.vincent->getPosition(); // camera follows player
    mGameState.camera.offset = mOrigin;                           // camera offset to center of screen
    mGameState.camera.rotation = 0.0f;                            // no rotation
    mGameState.camera.zoom = 1.0f;                                // default zoom
}

void Level3::panCamera(Camera2D *camera, const Vector2 *targetPosition)
{
    Vector2 positionDifference = Vector2Subtract(*targetPosition, camera->target);
    camera->target = Vector2Add(camera->target, Vector2Scale(positionDifference, 0.1f));
}

void Level3::update(float deltaTime)
{
    UpdateMusicStream(mGameState.bgm);

    mGameState.vincent->update(
        deltaTime,             // delta time
        nullptr,               // player
        mGameState.map,        // map
        nullptr,               // collidable entities
        0                      // col. entity count
    );

    mBear->update(deltaTime, mGameState.vincent, mGameState.map, 
        nullptr, 0);

    // Collision check 
    if (!mPlayerHit)
    {
        if (!mPlayerHit && mGameState.vincent->isCollidingWithEnemy(mEnemies, 1))
        {
            mPlayerHit = true;
            PlaySound(mGameState.collisonSound);
            
            if (mGameState.lives != nullptr)
            {
                (*mGameState.lives)--;
        
                if (*mGameState.lives > 0)
                {
                    mGameState.nextSceneID = 3;  // Restart level
                }
                else
                {
                mGameState.nextSceneID = -1;  // Game over
                PlaySound(mGameState.gameOverSound); 
                }
            }
        }
    }

    Vector2 currentPlayerPosition = {mGameState.vincent->getPosition().x, mOrigin.y};
    panCamera(&mGameState.camera, &currentPlayerPosition);

    // // Player fell off
    // if (!mPlayerHit && mGameState.vincent->getPosition().y > 800.0f) 
    // {
    //     mPlayerHit = true;
        
    //     if (mGameState.lives != nullptr)
    //     {
    //         (*mGameState.lives)--;
            
    //         if (*mGameState.lives > 0)
    //             mGameState.nextSceneID = 1;  // Restart
    //         else
    //             mGameState.nextSceneID = -1;
    //             PlaySound(mGameState.collisonSound);
    //     }
    // }

    // Player reached the end so go to the next level
    Vector2 playerPos = mGameState.vincent->getPosition();
    Vector2 playerFeet = {
    playerPos.x,
    playerPos.y + (mGameState.vincent->getColliderDimensions().y / 2.0f)
    }; // Technically it is the players feet touching the tile
    unsigned int currentTile = mGameState.map->getTileAtPosition(playerFeet);
    if (currentTile == 10)
    {
        mGameState.nextSceneID = 5; 
    }
        
}

void Level3::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));

    mGameState.map->render();
    mGameState.vincent->render();
    mBear->render();

}

void Level3::shutdown()
{
    delete mGameState.vincent;
    delete mGameState.map;
    delete mBear;

    UnloadMusicStream(mGameState.bgm);
    UnloadSound(mGameState.jumpSound);
    UnloadSound(mGameState.collisonSound);
    UnloadSound(mGameState.gameOverSound);
}