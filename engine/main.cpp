#include "drawFunctions.h"
#include "luna.h"
#include <string>
using namespace std;
lua_State *L = lua_open();
void init()
{
    Luna<colour>::Register(L);
    lua_pushcfunction(L,cls );
    lua_setglobal(L, "cls");
    lua_pushcfunction(L,drawLine);
    lua_setglobal(L, "drawLine");
    lua_pushcfunction(L,drawCircle);
    lua_setglobal(L, "drawCircle");
    lua_pushcfunction(L,getAngle);
    lua_setglobal(L, "getAngle");
    lua_pushcfunction(L,setPixel);
    lua_setglobal(L, "setPixel");
    lua_pushcfunction(L,loadImageIndex);
    lua_setglobal(L, "loadImageIndex");
    lua_pushcfunction(L,rotatePic);
    lua_setglobal(L, "rotatePic");
    lua_pushcfunction(L,picWidth);
    lua_setglobal(L, "picWidth");
    lua_pushcfunction(L,picHeight);
    lua_setglobal(L, "picHeight");
    lua_pushcfunction(L,freeImageIndex);
    lua_setglobal(L, "freeImageIndex");
    lua_pushcfunction(L,drawImageIndex);
    lua_setglobal(L, "drawImageIndex");
    lua_pushcfunction(L,setScreen);
    lua_setglobal(L, "setScreen");
    lua_pushcfunction(L,loadFont);
    lua_setglobal(L, "loadFont");
    lua_pushcfunction(L,drawText);
    lua_setglobal(L, "drawText");
    lua_pushcfunction(L,getMouseState);
    lua_setglobal(L, "getMouseState");
    lua_pushcfunction(L,keyDown);
    lua_setglobal(L, "keyDown");
    lua_pushcfunction(L,getInput);
    lua_setglobal(L, "getInput");
    lua_pushcfunction(L,tryQuit);
    lua_setglobal(L, "tryQuit");
    lua_pushcfunction(L,update);
    lua_setglobal(L, "update");
    lua_pushcfunction(L,end);
    lua_setglobal(L, "endP");
}

int main ( int argc, char** argv )
{
    luaL_openlibs(L);
    string buff="dofile(\"main.lua\")";
    int error;
    init();
    int luaL_dofile (L,"init.lua");
    error = luaL_loadbuffer(L, buff.c_str(), buff.size(), "line") ||
            lua_pcall(L, 0, 0, 0);
    if (error) {
      fprintf(stderr, "%s", lua_tostring(L, -1));
      lua_pop(L, 1);  /* pop error message from the stack */
    }

      lua_close(L);
      return 0;
}
