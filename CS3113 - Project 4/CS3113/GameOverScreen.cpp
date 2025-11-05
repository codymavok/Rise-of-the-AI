#include "GameOverScreen.h"

GameOverScreen::GameOverScreen() : Scene{{0.0f}, nullptr} {}
GameOverScreen::GameOverScreen(Vector2 origin, const char *bgHexCode) : Scene{origin, bgHexCode} {}

GameOverScreen::~GameOverScreen() { shutdown(); }

void GameOverScreen::initialise()
{
    mGameState.nextSceneID = 0;  
    mGameState.lives = nullptr;
    
    mText = GetFontDefault();
    
    mGameState.vincent = nullptr;
    mGameState.map = nullptr;
    mGameState.camera = { 0 };
}

void GameOverScreen::update(float deltaTime)
{

    if (IsKeyPressed(KEY_ENTER))
    {
        mGameState.nextSceneID = -2; //  Need to hard code and change later to be negative of the last position in the scene vector and then need to add a new case in main.cpp to handle that case! 
    }
}

void GameOverScreen::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));
    

    const char* title = "GAME OVER";
    int titleFontSize = 80;
    Vector2 titleSize = MeasureTextEx(mText, title, titleFontSize, 2);
    float titleX = (1000 - titleSize.x) / 2.0f; 
    float titleY = 200.0f;
    
    DrawTextEx(mText, title, {titleX + 3, titleY + 3}, titleFontSize, 2, BLACK);
    DrawTextEx(mText, title, {titleX, titleY}, titleFontSize, 2, RED);
    

    const char* message = "You Lost All Your Lives!";
    int messageFontSize = 30;
    Vector2 messageSize = MeasureTextEx(mText, message, messageFontSize, 2);
    float messageX = (1000 - messageSize.x) / 2.0f;
    float messageY = 320.0f;
    DrawTextEx(mText, message, {messageX, messageY}, messageFontSize, 2, WHITE);

    const char* instruction = "PRESS ENTER TO RETURN TO MAIN MENU";
    int instructionFontSize = 25;
    Vector2 instructionSize = MeasureTextEx(mText, instruction, instructionFontSize, 2);
    float instructionX = (1000 - instructionSize.x) / 2.0f;
    float instructionY = 400.0f;
    DrawTextEx(mText, instruction, {instructionX, instructionY}, instructionFontSize, 2, LIGHTGRAY);
    
}

void GameOverScreen::shutdown()
{

}