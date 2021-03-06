/* Tutorial Source: http://codeumbra.eu/complete-roguelike-tutorial-using-c-and-libtcod-part-1-setting-up
 * Creating classes in CodeBlocks: http://www.youtube.com/watch?v=2fTsbQUP_no
 */
#include "main.h"

Engine engine(80, 50);

int main()
{
    engine.load();

    //bool exitFlagged;
    while(!TCODConsole::isWindowClosed())
    {
        engine.update();
        //if(exitFlagged)
        //    break;

        engine.render();
        TCODConsole::flush();
    }

    engine.save();

    return 0;
}
