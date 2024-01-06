#define OLC_PGE_APPLICATION
#include "engine.h"

int main()
{
    Engine engine;
    if (engine.Construct(1366, 768, 1, 1, true))
        engine.Start();

    return 0;
}
