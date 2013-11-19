#ifndef DRAWFUNCS_H
#define DRAWFUNCS_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include "SDL_rotozoom.h"
#include <cmath>
#include <string>
#include <vector>
#include "luna.h"
#include <iostream>
class colour
{
    DECLARE_SCRIPT_CLASS(colour)
    public:
        int R,B,G;
        colour();
        colour(int r,int b,int g);
        void setColour(int r,int b,int g);

        colour(lua_State* L);
        int setColour(lua_State* L);
};

static const colour BLACK    (  0,   0,   0);
static const colour RED      (255,   0,   0);
static const colour GREEN    (  0, 255,   0);
static const colour BLUE     (  0,   0, 255);
static const colour CYAN     (  0, 255, 255);
static const colour MAGENTA  (255,   0, 255);
static const colour YELLOW   (255, 255,   0);
static const colour WHITE    (255, 255, 255);
static const colour GREY     (192, 192, 192);
static const colour MAROON   (128,   0,   0);
static const colour DARKGREEN(  0, 128,   0);
static const colour NAVY     (  0,   0, 128);
static const colour TEAL     (  0, 128, 128);
static const colour PURPLE   (128,   0, 128);
static const colour OLIVE    (128, 128,   0);

static const double DEG_TO_RAD=57.29578;

void cls (colour clearColour);
void drawLine(int x1, int y1, int x2, int y2, colour toSet);
void drawCircle(int xc, int yc, int radius, colour toSet);
double getAngle(double x1,double y1,double x2,double y2);
void setPixel(int x, int y, colour toSet);
///////////////////////////////////
SDL_Surface *loadImage(const char* filename);
SDL_Surface *loadImage(const char* filename, colour clrKey);
void drawImage(int x,int y, SDL_Surface* image);
///////////////////////////////////
int loadImageIndex(const char* filename);
int loadImageIndex(const char* filename, colour clrKey);
int rotatePic(int index,int angle,colour clrKey);
int picWidth(int index);
int picHeight(int index);
void freeImageIndex(int index);
void drawImageIndex(int x,int y, int index);
///////////////////////////////////
SDL_Surface * colourKey(SDL_Surface* image,colour clrKey);
///////////////////////////////////
void setScreen(int w=800, int h=600, bool fullScreen=false, const char* caption="");
void loadFont(const char* fileName,int size=16);
void drawText(int x,int y,const char* str, colour textColour=BLACK);
void getMouseState(int& x, int& y);
void getMouseState(int& x, int& y, bool& left, bool& right);
bool keyDown(int k);
///////////////////////////////////
Uint8 getChar();
//////////////////////////////////
char* getInput(int x,int y, int maxLenth=50);
bool tryQuit();
void update();
void end();

int cls (lua_State* L);
int drawLine(lua_State* L);
int drawCircle(lua_State* L);
int getAngle(lua_State* L);
int setPixel(lua_State* L);
int loadImageIndex(lua_State* L);
int rotatePic(lua_State* L);
int picWidth(lua_State* L);
int picHeight(lua_State* L);
int freeImageIndex(lua_State* L);
int drawImageIndex(lua_State* L);
int setScreen(lua_State* L);
int loadFont(lua_State* L);
int drawText(lua_State* L);
int getMouseState(lua_State* L);
int getMouseState(lua_State* L);
int keyDown(lua_State* L);
int getInput(lua_State* L);
int tryQuit(lua_State* L);
int update(lua_State* L);
int end(lua_State* L);
#endif
