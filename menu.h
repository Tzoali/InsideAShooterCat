#if !defined(_MENU_H_)
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <iostream>

using namespace std;

typedef Uint32 bool32;
typedef Uint8 uint8;
typedef Sint32 int32;
#include "game.h"

bool Running = true;
SDL_RWops *RWFile;

struct AABB
{
  int Width;
  int Height;
  int X;
  int Y;
};

struct texture
{
    SDL_Texture *Tex;
    SDL_Rect Quads;
    int Width, Height;
    int FrameWidth, FrameHeight;
};

struct game_assets
{
        texture Map;
        texture Map1;
        texture Map3;
        texture Map4;
        texture Clouds;
        texture Clouds1;
        texture Clouds2;
        texture Clouds3;
        texture CatOne;
        texture CatTwo;
        texture BulletOne;
        texture BulletTwo;
        texture Monster;
        texture Monster2;
        texture Front;
        texture Fondo;
        texture Titulo;
        texture DieImage;
        texture WinImage;
        texture Story1;
        texture Story2;
        texture Story3;
        texture Go;
        texture Black;
        texture White;
        texture Front2;
        texture Kamehame;
        texture Fire;
        texture Spike;
        texture Gallo;
        texture Llama;
        texture Pila;
        texture Energia;
        texture Volador;

        texture Play;
        texture Exit;
        texture Records;
        texture Instructions;
        texture img_instructions;
        texture CharOne;
        texture CharTwo;

        Mix_Music *Back;
        Mix_Music *Intro;
        Mix_Music *Menu;
        Mix_Music *DieSong;
        Mix_Music *WinSong;
        Mix_Music *Level2;
        Mix_Music *Level3;
        Mix_Chunk *Rain;
        Mix_Chunk *Shoot;
        Mix_Chunk *Thunder;
        Mix_Chunk *Punch;
        Mix_Chunk *Die;
        Mix_Chunk *Jump;
        Mix_Chunk *Charge;
        Mix_Chunk *ChargeEnd;
        Mix_Chunk *Click;
        Mix_Chunk *Kame_Sound;
        Mix_Chunk *Coin;
        Mix_Chunk *Death;
        Mix_Chunk *OneUp;
        Mix_Chunk *Sing;
        Mix_Chunk *Burn;
        Mix_Chunk *Espinazo;
        Mix_Chunk *Alien;
};

static bool32 AABBCollision(AABB *Left, AABB *Right)
{

    if((Right->X < Left->X+Left->Width) &&
       (Right->Y < Left->Y+Left->Height) &&
       (Right->X+Right->Width > Left->X) &&
       (Right->Y+Right->Height > Left->Y))
        return true;


    return(false);
}

static bool32 DoButton(SDL_Rect Quads, game_input *Input)
{
    AABB Left = {0, 0, Input->MouseX, Input->MouseY};
    AABB Right = {Quads.w, Quads.h, Quads.x, Quads.y};

    bool32 OnButton = AABBCollision(&Left, &Right);

    return(OnButton);
}

static bool32 DrawButton(SDL_Renderer *Render, SDL_Texture* Texture, SDL_Rect Quads, game_input *Input)
{
    SDL_Rect Rect = {0};
    Rect.x = Quads.x - Quads.w/2;
    Rect.y = Quads.y - Quads.h/2;
    Rect.h = Quads.h;
    Rect.w = Quads.w;

    SDL_RenderCopy(Render, Texture, 0, &Rect);

    return(Input->MouseLeft.Down && DoButton(Rect, Input));
}

texture Current = {0};
game_controller *Controller = 0;

static void GetRecordIndex(record Records[], SDL_RWops * File)
{
    SDL_RWseek(File, 0, RW_SEEK_SET);
    for(int I = 0; I < 10; ++I)
    {
        SDL_RWread(File, &Records[I], sizeof(record), 1);
    }
}

static void SelectCharacter(SDL_Renderer *Render, game_state *State, game_input *Input, game_assets *Assets)
{
    Controller = GetGameController(Input, 0);
    if(Controller->Start.Down)
    {
        State->CurrentState = GAME_MENU;
    }

    SDL_RenderCopy(Render, Assets->Fondo.Tex, 0, 0);
    const static char *Title = "Select a character";
    SDL_Color TextColor = {0xff, 0xff, 0xff};
    SDL_Surface *TextSurface = TTF_RenderText_Blended(Font, Title, TextColor);
    SDL_Texture *TextTexture = SDL_CreateTextureFromSurface(Render, TextSurface);
    SDL_Rect Rect = {160, 60, 340, 64};

    SDL_RenderCopy(Render, TextTexture, 0, &Rect);
    SDL_FreeSurface(TextSurface);
    SDL_DestroyTexture(TextTexture);

   if(DrawButton(Render, Assets->CharOne.Tex, Assets->CharOne.Quads, Input))
   {
       Mix_PlayChannel(5,Assets->Click,0);
       Current = Assets->CatOne;
       Img = 0;
       State->GameOn = true;
   }

   if(DrawButton(Render, Assets->CharTwo.Tex, Assets->CharTwo.Quads, Input))
   {
        Current = Assets->CatTwo;
        Mix_PlayChannel(5,Assets->Click,0);
        Img = 1;
        State->GameOn = true;
   }

   SDL_Rect Back = {0};
   Back.x = 150;
   Back.y = 416;
   Back.w = 320;
   Back.h = 32;
    SDL_SetRenderDrawColor(Render, 255, 255, 255, 255);
    SDL_Color fg = {0};
    const static char *NameLabel = "Name:";
    SDL_Surface *TextSurface6 = TTF_RenderText_Blended(Font, NameLabel,TextColor);
    SDL_Texture *TextTexture6 = SDL_CreateTextureFromSurface(Render, TextSurface6);
    SDL_Rect Rect6 = {150, 380, 120, 32};
    SDL_RenderCopy(Render, TextTexture6, 0, &Rect6);
    SDL_FreeSurface(TextSurface6);
    SDL_DestroyTexture(TextTexture6);

    SDL_RenderFillRect(Render, &Back);
    SDL_strlcpy(State->Current.Name, State->CMDText, 32);
    SDL_Surface *NickSurface = TTF_RenderText_Blended(Font, State->Current.Name, fg);
    SDL_Texture *NickTexture = SDL_CreateTextureFromSurface(Render, NickSurface);
    Back.w=10*strlen(State->Current.Name);
    SDL_RenderCopy(Render, NickTexture, 0, &Back);

    SDL_FreeSurface(NickSurface);
    SDL_DestroyTexture(NickTexture);

}

static void GameMenu(SDL_Renderer *Render, game_state *State, game_input *Input, game_assets *Assets)
{
    const static char *Title = "Inside a Shooter Cat";

    SDL_Color TextColor = {255, 255, 255};
    SDL_Color BGColor = {0, 0, 0};
    SDL_Surface *TextSurface = TTF_RenderText_Shaded(Font, Title, TextColor, BGColor);
    SDL_Texture *TextTexture = SDL_CreateTextureFromSurface(Render, TextSurface);
    SDL_Rect fondoRect;
    fondoRect.h=480;
    fondoRect.w = 640;
    fondoRect.y=0;
    fondoRect.x = 0;
    SDL_RenderCopy(Render,Assets->Fondo.Tex,0,&fondoRect);
    SDL_RenderCopy(Render,Assets->Titulo.Tex,0,&Assets->Titulo.Quads);
    SDL_FreeSurface(TextSurface);
    SDL_DestroyTexture(TextTexture);
    if(DrawButton(Render, Assets->Play.Tex, Assets->Play.Quads, Input))
    {
        Mix_PlayChannel(5,Assets->Click,0);
        if(State->CurrentState==GAME_MENU)
            State->CurrentState = GAME_PLAY;
        else if(State->CurrentState==GAME_MENU2)
            State->CurrentState = GAME_PLAY2;
        else
            State->CurrentState = GAME_PLAY3;
        SDL_Delay(500);
    }
    if(DrawButton(Render, Assets->Instructions.Tex, Assets->Instructions.Quads, Input))
    {
        Mix_PlayChannel(5,Assets->Click,0);
        if(State->CurrentState==GAME_MENU)
            State->CurrentState = GAME_INSTRUCTIONS;
        else if(State->CurrentState==GAME_MENU2)
            State->CurrentState = GAME_INSTRUCTIONS2;
        else
            State->CurrentState = GAME_INSTRUCTIONS3;
    }
    if(DrawButton(Render, Assets->Records.Tex, Assets->Records.Quads, Input))
     {
        Mix_PlayChannel(5,Assets->Click,0);
        if(State->CurrentState==GAME_MENU)
            State->CurrentState = GAME_RECORDS;
        else if(State->CurrentState==GAME_MENU2)
            State->CurrentState = GAME_RECORDS2;
        else
            State->CurrentState = GAME_RECORDS3;
     }
    if(DrawButton(Render, Assets->Exit.Tex, Assets->Exit.Quads, Input))
    {
        Mix_PlayChannel(5,Assets->Click,0);
        SDL_RWseek(RWFile, 0, RW_SEEK_SET);
        SDL_RWwrite(RWFile, State->Records, sizeof(record)*10, 1);
        SDL_RWclose(RWFile);
        exit(0);
    }
}

static void GameRecords(SDL_Renderer *Render, game_state *State, game_input *Input, game_assets *Assets)
{
    SDL_RenderCopy(Render, Assets->Fondo.Tex, 0, 0);
    Controller = GetGameController(Input, 0);
    if(Controller->Start.Down)
    {
        if(State->CurrentState==GAME_RECORDS)
            State->CurrentState = GAME_MENU;
        else if(State->CurrentState==GAME_RECORDS2)
            State->CurrentState = GAME_MENU2;
        else
            State->CurrentState = GAME_MENU3;
    }

    int const X = 80;

    SDL_Rect Rect = {0};
    Rect.h = 32;
    Rect.w = 160;
    Rect.x = X;
    Rect.y = 32;

    SDL_Color color = {255, 255, 255};

    SDL_Surface *Tit1 = TTF_RenderText_Blended(Font, "Names:", color);
    SDL_Texture *TexTit1 = SDL_CreateTextureFromSurface(Render, Tit1);
    SDL_Surface *Tit2 = TTF_RenderText_Blended(Font, "Score: ", color);
    SDL_Texture *TexTit2 = SDL_CreateTextureFromSurface(Render, Tit2);
    SDL_RenderCopy(Render, TexTit1, 0, &Rect);
    Rect.x += 280;
    SDL_RenderCopy(Render, TexTit2, 0, &Rect);

    SDL_FreeSurface(Tit1);
    SDL_DestroyTexture(TexTit1);

    SDL_FreeSurface(Tit2);
    SDL_DestroyTexture(TexTit2);

    Rect.y += 32;

    for(int I = 0; I < 10; ++I)
    {
        char Puntos[10] = {0};
        SDL_itoa(State->Records[I].Score, Puntos, 10);
        Rect.x = X;
        Rect.y += 32;
        SDL_Surface *Tit1 = TTF_RenderText_Blended(Font, State->Records[I].Name, color);
        SDL_Texture *TexTit1 = SDL_CreateTextureFromSurface(Render, Tit1);
        SDL_Surface *Tit2 = TTF_RenderText_Blended(Font, Puntos, color);
        SDL_Texture *TexTit2 = SDL_CreateTextureFromSurface(Render, Tit2);
        Rect.w = 160;
        SDL_RenderCopy(Render, TexTit1, 0, &Rect);
        Rect.w = 80;
        Rect.x += 280;
        SDL_RenderCopy(Render, TexTit2, 0, &Rect);

        SDL_FreeSurface(Tit1);
        SDL_DestroyTexture(TexTit1);

        SDL_FreeSurface(Tit2);
        SDL_DestroyTexture(TexTit2);

    }

}

#define _MENU_H_
#endif
