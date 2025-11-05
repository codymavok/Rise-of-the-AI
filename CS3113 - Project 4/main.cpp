/**
* Author: Karan Singh 
* Assignment: Rise of the AI
* Date due: 2025-11-08, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/
#include "raylib.h"
#include <math.h>
#include "CS3113/cs3113.h"
#include "CS3113/Entity.h"
#include "CS3113/Level1.h"
#include "CS3113/Level2.h"
#include "CS3113/Level3.h"
#include "CS3113/StartingMenu.h"
#include "CS3113/GameOverScreen.h" 
#include "CS3113/WonGame.h" 

// Global Constants
constexpr int SCREEN_WIDTH  = 1000,
              SCREEN_HEIGHT = 600,
              FPS           = 120;

constexpr char    BG_COLOUR[]      = "#FFD8BF";
constexpr Vector2 ORIGIN           = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };


constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;

// Global Variables
AppStatus gAppStatus   = RUNNING;
float gPreviousTicks   = 0.0f,
      gTimeAccumulator = 0.0f;


constexpr int NUM_LIVES = 3;
int gPlayerLives = NUM_LIVES; 

Scene *gCurrentScene = nullptr;
std::vector<Scene*> gLevels = {};

StartingMenu *gMenu = nullptr;
Level1 *gLevel1 = nullptr;
Level2 *gLevel2 = nullptr;
Level3 *gLevel3 = nullptr;
GameOverScreen *gGameOver = nullptr;
WonGame *gWonGame = nullptr;

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();
void switchToScene(Scene *scene);


void switchToScene(Scene *scene)
{   
    gCurrentScene = scene;
    gCurrentScene->initialise();
}


void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Project 4");
    InitAudioDevice();

    gPlayerLives = NUM_LIVES;

    gMenu = new StartingMenu(ORIGIN, "#2C5F2D");
    gLevel1 = new Level1(ORIGIN, "#FFD8BF");
    gLevel2 = new Level2(ORIGIN, "#FFD8BF");
    gLevel3 = new Level3(ORIGIN, "#FFD8BF");
    gGameOver = new GameOverScreen(ORIGIN, "#8B0000");
    gWonGame = new WonGame(ORIGIN, "#ADD8E6");

    gLevels.push_back(gMenu);
    gLevels.push_back(gLevel1);
    gLevels.push_back(gLevel2);
    gLevels.push_back(gLevel3);
    gLevels.push_back(gGameOver);
    gLevels.push_back(gWonGame);
    switchToScene(gLevels[0]);

    gCurrentScene->setLives(&gPlayerLives);

    SetTargetFPS(FPS);
}

void processInput() 
{
    if (gCurrentScene->getState().vincent != nullptr)
    {
        gCurrentScene->getState().vincent->resetMovement();

        if      (IsKeyDown(KEY_A)) gCurrentScene->getState().vincent->moveLeft();
        else if (IsKeyDown(KEY_D)) gCurrentScene->getState().vincent->moveRight();

        if (IsKeyPressed(KEY_W) && gCurrentScene->getState().vincent->isCollidingBottom())
        {
            gCurrentScene->getState().vincent->jump();
            PlaySound(gCurrentScene->getState().jumpSound);
        }

        if (GetLength(gCurrentScene->getState().vincent->getMovement()) > 1.0f) 
            gCurrentScene->getState().vincent->normaliseMovement();
    }

    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;
}

void update() 
{
    // Delta time
    float ticks = (float) GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;

    // Fixed timestep
    deltaTime += gTimeAccumulator;

    if (deltaTime < FIXED_TIMESTEP)
    {
        gTimeAccumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP)
    {
        gCurrentScene->update(FIXED_TIMESTEP);
        
        deltaTime -= FIXED_TIMESTEP;
    }
    
    gTimeAccumulator = deltaTime;
}

void render()
{
     BeginDrawing();
     if (gCurrentScene->getState().vincent != nullptr)
     {
           BeginMode2D(gCurrentScene->getState().camera);
           gCurrentScene->render();
           EndMode2D();
    
          char livesText[32];
          snprintf(livesText, sizeof(livesText), "Lives: %d", gPlayerLives);
          DrawText(livesText, 10, 10, 30, BLACK);
     }
        else
        {
            gCurrentScene->render();
        }

    EndDrawing();
}

void shutdown() 
{
    delete gMenu;
    delete gLevel1;
    delete gLevel2; 
    delete gLevel3;  
    delete gGameOver;
    delete gWonGame;
    
    for (size_t i = 0; i < gLevels.size(); i++) 
        gLevels[i] = nullptr;

    CloseAudioDevice();
    CloseWindow();
}

int main(void)
{
   initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();
        
        if (gCurrentScene->getState().nextSceneID != 0)  
        {
            int id = gCurrentScene->getState().nextSceneID;
            
            if (id == -1)  // Game over
            {
                gPlayerLives = NUM_LIVES; 
                switchToScene(gLevels[4]);  
    
            }
            else if (id == -2)  
            {
             gPlayerLives = NUM_LIVES;
             switchToScene(gLevels[0]);  // Go to menu
            }

            else if (id > 0 && id < (int)gLevels.size())  
            {
                switchToScene(gLevels[id]);
                if (gCurrentScene != gMenu)
                    {
                     gCurrentScene->setLives(&gPlayerLives);
                    }

            }
        }
        
        render();
    }

    shutdown();

    return 0;
}