#include "Scene.h"

class WonGame : public Scene 
{
private:
    Font mText;

public:
    WonGame();
    WonGame(Vector2 origin, const char *bgHexCode);
    ~WonGame();
    
    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};
