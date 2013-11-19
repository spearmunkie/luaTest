#include "drawFunctions.h"
SDL_Surface* screen;
TTF_Font *font = NULL;
Uint8* keyState;
int width,height;
SDL_Event event = {0};
bool forceQuit=false;
std::vector<SDL_Surface*> image;

IMPLEMENT_SCRIPT_CLASS(colour)
DEFINE_SCRIPT_CLASS(colour)
SCRIPT_METHOD(colour,setColour),
{ 0, 0 }
END_SCRIPT_CLASS

colour::colour()
{
    R=0;
    B=0;
    G=0;
}

colour::colour(int r,int g,int b)
{
    setColour(r,g,b);
}

void colour::setColour(int r,int g,int b)
{
    R=r;
    G=g;
    B=b;
}

colour::colour(lua_State* L)
{
    setColour((int)lua_tonumber(L, 1),(int)lua_tonumber(L, 2),(int)lua_tonumber(L, 3));
}

int colour::setColour(lua_State* L)
{
    R=(int)lua_tonumber(L, 1);
    G=(int)lua_tonumber(L, 2);
    B=(int)lua_tonumber(L, 3);
    return 0;
}

void cls (colour clearColour)
{
    Uint32 colorSDL = SDL_MapRGB(screen->format, clearColour.R, clearColour.G, clearColour.B);
    SDL_FillRect(screen, NULL, colorSDL);
}
void drawLine(int x1, int y1, int x2, int y2, colour toSet)
{
    if(x1 < 0) x1=0;
    if(x1 > screen->w - 1)x1=screen->w - 1;
    if(x2 < 0)x2=0;
    if(x2 > screen->w - 1)x2=screen->w - 1;
    if(y1 < 0)y1=0;
    if(y1 > screen->h - 1)y1=screen->h - 1;
    if(y2 < 0)y2=0;
    if(y2 > screen->h - 1)y2=screen->h - 1;

    int deltax = abs(x2 - x1);
    int deltay = abs(y2 - y1);
    int x = x1;
    int y = y1;
    int xinc1, xinc2, yinc1, yinc2, den, num, numadd, numpixels, curpixel;

    if(x2 >= x1)
    {
        xinc1 = 1;
        xinc2 = 1;
    }
    else
    {
        xinc1 = -1;
        xinc2 = -1;
    }
    if(y2 >= y1)
    {
        yinc1 = 1;
        yinc2 = 1;
    }
    else
    {
        yinc1 = -1;
        yinc2 = -1;
    }
    if (deltax >= deltay)
    {
        xinc1 = 0;
        yinc2 = 0;
        den = deltax;
        num = deltax / 2;
        numadd = deltay;
        numpixels = deltax;
    }
    else
    {
        xinc2 = 0;
        yinc1 = 0;
        den = deltay;
        num = deltay / 2;
        numadd = deltax;
        numpixels = deltay;
    }
    for (curpixel = 0; curpixel <= numpixels; curpixel++)
    {
        setPixel(x % screen->w, y % screen->h, toSet);
        num += numadd;
        if (num >= den)
        {
            num -= den;
            x += xinc1;
            y += yinc1;
        }
        x += xinc2;
        y += yinc2;
    }

}

void drawCircle(int xc, int yc, int radius, colour toSet)
{
    if(xc - radius < 0 || xc + radius >= screen->w || yc - radius < 0 || yc + radius >= screen->h) return;
  int x = 0;
  int y = radius;
  int p = 3 - (radius << 1);
  int a, b, c, d, e, f, g, h;
  while (x <= y)
  {
     a = xc + x; //8 pixels can be calculated at once thanks to the symmetry
     b = yc + y;
     c = xc - x;
     d = yc - y;
     e = xc + y;
     f = yc + x;
     g = xc - y;
     h = yc - x;
     setPixel(a, b, toSet);
     setPixel(c, d, toSet);
     setPixel(e, f, toSet);
     setPixel(g, f, toSet);
     if(x > 0) //avoid drawing pixels at same position as the other ones
     {
       setPixel(a, d, toSet);
       setPixel(c, b, toSet);
       setPixel(e, h, toSet);
       setPixel(g, h, toSet);
     }
     if(p < 0) p += (x++ << 2) + 6;
     else p += ((x++ - y--) << 2) + 10;
  }

}

double getAngle(double x1,double y1,double x2,double y2)
{
    double opp = y2 - y1;
    double adj = x2 - x1;
    if (adj == 0)
        if (y2 > y1)
            return 270;
        else
            return 90;
    else if (opp >= 0 && adj > 0)
        return 360 - atan  (opp / adj)*DEG_TO_RAD;
    else if (adj < 0)
        return 180-atan  (opp / adj)*DEG_TO_RAD;
    else if (opp < 0 && adj > 0)
        return 360-atan  (opp / adj)*DEG_TO_RAD;
    return 0;
}

void setPixel(int x, int y, colour toSet)
{
    if(x < 0 || y < 0 || x >= screen->w || y >= screen->h) return;
    Uint32 colorSDL = SDL_MapRGB(screen->format, toSet.R, toSet.G, toSet.B);
    Uint32* bufp;
    bufp = (Uint32*)screen->pixels + y * screen->pitch / 4 + x;
    *bufp = colorSDL;
}


SDL_Surface *loadImage(const char* fileName)
{

    SDL_Surface* loadedImage = NULL;
    SDL_Surface* optimizedImage = NULL;

    loadedImage = SDL_LoadBMP(fileName);
    if(loadedImage != NULL)
    {
        optimizedImage = SDL_DisplayFormat(loadedImage);
        SDL_FreeSurface(loadedImage);
    }
    return optimizedImage;
}

SDL_Surface *loadImage(const char* fileName, colour clrKey)
{
    SDL_Surface* loadedImage = NULL;
    SDL_Surface* optimizedImage = NULL;

    loadedImage = SDL_LoadBMP(fileName);
    if( loadedImage != NULL )
    {
        optimizedImage = SDL_DisplayFormat(loadedImage);
        if( optimizedImage != NULL )
        {
            Uint32 colorkey = SDL_MapRGB( optimizedImage->format, clrKey.R, clrKey.B, clrKey.G );
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, colorkey );
        }
        SDL_FreeSurface(loadedImage);
    }
    return optimizedImage;
}
void drawImage(int x,int y, SDL_Surface* image)
{
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface( image, NULL, screen, &offset );
}

int loadImageIndex(const char* fileName)
{
    image.push_back(loadImage(fileName));
    return image.size()-1;
}

int loadImageIndex(const char* fileName, colour clrKey)
{
    image.push_back(loadImage(fileName,clrKey));
    return image.size()-1;
}

void freeImageIndex(int index)
{
    SDL_FreeSurface(image[index]);
}
void drawImageIndex(int x,int y, int index)
{
    //std::cout<<index<<std::endl;
    drawImage(x,y,image[index]);
}

int rotatePic(int index,int angle,colour clrKey)
{
    SDL_Surface *img=rotozoomSurface(image[index],angle,1,0);
    //std::cout<<angle<<'\n';
    img=colourKey(img,clrKey);
    image.push_back(img);
    return image.size()-1;
}

int picWidth(int index)
{
    return image[index]->w;
}

int picHeight(int index)
{
    return image[index]->h;
}

SDL_Surface * colourKey(SDL_Surface* image,colour clrKey)
{
    SDL_Surface* optimizedImage = NULL;

    if( image != NULL )
    {
        optimizedImage = SDL_DisplayFormat(image);
        if( optimizedImage != NULL )
        {
            Uint32 colorkey = SDL_MapRGB( optimizedImage->format, clrKey.R, clrKey.B, clrKey.G );
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, colorkey );
        }
        SDL_FreeSurface(image);
    }
    return optimizedImage;
}

void setScreen(int w, int h, bool fullScreen, const char* caption)
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
        SDL_Quit();
    if(fullScreen)
    {
        screen = SDL_SetVideoMode(w, h, 32, SDL_HWSURFACE | SDL_FULLSCREEN);
        if(SDL_MUSTLOCK(screen))
            SDL_LockSurface(screen);
    }
    else
        screen = SDL_SetVideoMode(w, h, 32, SDL_HWSURFACE | SDL_HWPALETTE);
    width=w;
    height=h;
    SDL_WM_SetCaption(caption, NULL);
    SDL_EnableUNICODE(1);
    TTF_Init();
    update();
}

void loadFont(const char* fileName,int size)
{
    if (font!=NULL)
        TTF_CloseFont(font);
    font=TTF_OpenFont(fileName, size);
}

void drawText(int x,int y,const char* str, colour textColour)
{
    SDL_Color col= {textColour.R, textColour.G, textColour.B};
    if (font)
    {
        SDL_Surface* message= TTF_RenderText_Solid( font, str,col );
        drawImage(x,y,message);
        SDL_FreeSurface(message);
    }
}

void getMouseState(int& x, int& y)
{
    SDL_GetMouseState(&x, &y);
}

void getMouseState(int& x, int& y, bool& left, bool& right)
{
    Uint8 mouseState = SDL_GetMouseState(&x, &y);
    left=(mouseState & 1);
    right=(mouseState & 4);
}

bool keyDown(int k)
{
  return (keyState[k] != 0);
}

const int ASCII_ENTER = 13;
const int ASCII_BACKSPACE = 8;
const int ASCII_SPACE = 32; //smallest printable ascii char

Uint8 getChar()
{
  int ascii = 0;
  static int prev = 0;

    if ((event.key.keysym.unicode & 0xFF80) == 0)
        if(event.type == SDL_KEYDOWN)
            ascii = event.key.keysym.unicode & 0x7F;

    if(ascii < ASCII_SPACE && ascii != ASCII_ENTER && ascii != ASCII_BACKSPACE)
        ascii = 0;

    if(ascii != prev)
        prev = ascii;
    else ascii = 0;

    return ascii;
}

char* getInput(int x,int y, int maxLength,colour clr)
{
    bool enter = false;
    std::string buff;
    Uint32 *buffer, *bufp;

    bufp = (Uint32*)screen->pixels;

    buffer=new Uint32[screen->w*screen->h];
    for(int i = 0; i < screen->w*screen->h; i++)
        buffer[i] = bufp[i];

    while(!enter)
    {
        SDL_PollEvent(&event);
        Uint8 tmp=getChar();
        if (tmp==ASCII_ENTER)
            enter=true;
        else if ((tmp==ASCII_BACKSPACE)&&(buff.size()>0))
                buff.resize(buff.size()-1);
        else if ((tmp>0)&&((signed int)buff.size()<maxLength)&&(tmp>=32))
                buff.push_back(tmp);
        for(int i = 0; i < screen->w*screen->h; i++)
            bufp[i] = buffer[i];
        drawText(x,y,buff.c_str(),clr);
        update();
    }
    return (char*)buff.c_str();
}

bool tryQuit()
{
    return forceQuit;
}

void update()
{
    while (SDL_PollEvent(&event))
        if(event.type == SDL_QUIT)
            forceQuit=true;
    keyState = SDL_GetKeyState(NULL);
    SDL_Flip(screen);
}

void end()
{
    SDL_FreeSurface(screen);
    TTF_CloseFont( font );
    for(int x=0;x<(signed int)image.size();x++)
        SDL_FreeSurface(image[x]);
}

int cls (lua_State* L)
{
    colour *tmp=(colour*)*((void**)lua_touserdata(L,1));
    //std::cout<<tmp->R<<" "<<tmp->G<<" "<<tmp->B<<'\n';
    cls(*tmp);
    return 0;
}

int drawLine(lua_State* L)
{
    colour *tmp=(colour*)*((void**)lua_touserdata(L, 5));
    drawLine((int)lua_tonumber(L,1),(int)lua_tonumber(L,2),(int)lua_tonumber(L,3),(int)lua_tonumber(L,4),*tmp);
    return 0;
}
int drawCircle(lua_State* L)
{
    colour *tmp=(colour*)*((void**)lua_touserdata(L, 4));
    drawCircle((int)lua_tonumber(L,1),(int)lua_tonumber(L,2),(int)lua_tonumber(L,3),*tmp);
    return 0;
}

int getAngle(lua_State* L)
{
    lua_pushnumber(L,(lua_Number)getAngle((int)lua_tonumber(L,1),(int)lua_tonumber(L,2),(int)lua_tonumber(L,3),(int)lua_tonumber(L,4)));
    return 1;
}

int setPixel(lua_State* L)
{
    colour *tmp=(colour*)*((void**)lua_touserdata(L, 3));
    setPixel((int)lua_tonumber(L,1),(int)lua_tonumber(L,2),*tmp);
    return 0;
}

int loadImageIndex(lua_State* L)
{
    colour *tmp=(colour*)*((void**)lua_touserdata(L,2));
    lua_pushnumber(L,(lua_Number)loadImageIndex(lua_tostring(L,1),*tmp));
    return 1;
}

int rotatePic(lua_State* L)
{
    colour *tmp=(colour*)*((void**)lua_touserdata(L, 3));
    lua_pushinteger(L,(lua_Number)rotatePic((int)lua_tonumber(L,1),(int)lua_tonumber(L,2),*tmp));
    return 1;
}

int picWidth(lua_State* L)
{
    lua_pushnumber(L,(lua_Number)picWidth((int)lua_tonumber(L,1)));
    return 1;
}

int picHeight(lua_State* L)
{
    lua_pushnumber(L,(lua_Number)picHeight((int)lua_tonumber(L,1)));
    return 1;
}

int freeImageIndex(lua_State* L)
{
    freeImageIndex((int)lua_tonumber(L,1));
    return 0;
}

int drawImageIndex(lua_State* L)
{
    drawImageIndex((int)lua_tonumber(L,1),(int)lua_tonumber(L,2),(int)lua_tonumber(L,3));
    return 0;
}

int setScreen(lua_State* L)
{
    setScreen((int)lua_tonumber(L,1),(int)lua_tonumber(L,2),(bool)lua_toboolean(L,3),(char*)lua_tostring(L,4));
    return 0;
}

int loadFont(lua_State* L)
{
    loadFont((char*)lua_tostring(L,1),(int)lua_tonumber(L,2));
    return 0;
}

int drawText(lua_State* L)
{
    colour *tmp=(colour*)*((void**)lua_touserdata(L, 4));
    drawText((int)lua_tonumber(L,1),(int)lua_tonumber(L,2),(char*)lua_tostring(L,3),*tmp);
    return 0;
}

int getMouseState(lua_State* L)
{
    int x,y;
    bool r,l;
    getMouseState(x,y,l,r);
    lua_pushnumber(L,(lua_Number)x);
    lua_pushnumber(L,(lua_Number)y);
    lua_pushboolean(L,l);
    lua_pushboolean(L,r);
    return 4;
}

int keyDown(lua_State* L)
{
    lua_pushboolean(L,keyDown((int)lua_tonumber(L,1)));
    return 1;
}

int getInput(lua_State* L)
{
    colour *tmp=(colour*)*((void**)lua_touserdata(L, 4));
    lua_pushstring(L,getInput((int)lua_tonumber(L,1),(int)lua_tonumber(L,2),(int)lua_tonumber(L,3),*tmp));
    return 1;
}

int tryQuit(lua_State* L)
{
    lua_pushboolean(L,tryQuit());
    return 1;
}

int update(lua_State* L)
{
    update();
    return 0;
}

int end(lua_State* L)
{
    end();
    return 0;
}
