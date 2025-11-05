#include "StartingMenu.h"

StartingMenu::StartingMenu() : Scene{{0.0f}, nullptr} {}
StartingMenu::StartingMenu(Vector2 origin, const char *bgHexCode) : Scene{origin, bgHexCode} {}

StartingMenu::~StartingMenu() { shutdown(); }

void StartingMenu::initialise()
{
    mGameState.nextSceneID = 0;  
    mGameState.lives = nullptr;
    
    mText = GetFontDefault();
    
    mGameState.vincent = nullptr;
    mGameState.map = nullptr;
    mGameState.camera = { 0 };
}

void StartingMenu::update(float deltaTime)
{
    if (IsKeyPressed(KEY_ENTER))
    {
        mGameState.nextSceneID = 1;  // Go to Level 1
    }
}

void StartingMenu::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));
    
    const char* title = "PEACOCK DUNGEON";
    int titleFontSize = 60;
    Vector2 titleSize = MeasureTextEx(mText, title, titleFontSize, 2);
    float titleX = (1000 - titleSize.x) / 2.0f; 
    float titleY = 200.0f;
    
    DrawTextEx(mText, title, {titleX + 3, titleY + 3}, titleFontSize, 2, BLACK);
    DrawTextEx(mText, title, {titleX, titleY}, titleFontSize, 2, GREEN);

    const char* instruction = "PRESS ENTER TO START";
    int instructionFontSize = 25;
    Vector2 instructionSize = MeasureTextEx(mText, instruction, instructionFontSize, 2);
    float instructionX = (1000 - instructionSize.x) / 2.0f;
    float instructionY = 400.0f;
    DrawTextEx(mText, instruction, {instructionX, instructionY}, instructionFontSize, 2, LIGHTGRAY);
    
    
    const char* credits = "By: Karan Singh";
    int creditsFontSize = 20;
    Vector2 creditsSize = MeasureTextEx(mText, credits, creditsFontSize, 1);
    float creditsX = (1000 - creditsSize.x) / 2.0f;
    float creditsY = 550.0f;
    DrawTextEx(mText, credits, {creditsX, creditsY}, creditsFontSize, 1, LIGHTGRAY);
}

void StartingMenu::shutdown()
{
  
}