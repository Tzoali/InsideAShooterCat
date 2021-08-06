#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <windows.h>
#include <iostream>
using namespace std;

#define RECT_SIZE 32
typedef Uint32 bool32;
typedef Uint8 uint8;
typedef Sint32 int32;
#include "menu.h"
#include "game_run.h"
#include "game_run2.h"
#include "game_run3.h"

static void GetKeyboardInput(game_controller *NewKeyboard, game_controller *OldKeyboard);
inline void
SetButtonState(game_button *New, game_button *Old, bool32 Value)
{
    New->Down = Value;
    New->Count = (New->Down == Old->Down) ? (New->Count + 1) : 0;
}


static void SDLUpdateInput(game_input *NewInput, game_input *OldInput);

static void LoadAllGameAssets(game_assets *Assets, SDL_Renderer *Render);
long long puntuacion;
int main(int Argc, char *Argv[])
{
    SDL_Color TextColor2 = {0xFF, 0xFF, 0xFF};
    int ran;
    int banImage1=0;
    SDL_Init(SDL_INIT_EVERYTHING);
    if(TTF_Init() == -1)
    {
        SDL_Log("%s\n", TTF_GetError());
        return(-1);
    }

    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    SDL_Window* Win = SDL_CreateWindow("ISC: Inside a Shooter Cat",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       640, 480,
                                       SDL_WINDOW_SHOWN);

    SDL_Renderer* Render = SDL_CreateRenderer(Win, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(Render, SDL_BLENDMODE_BLEND);
    Font = TTF_OpenFont("res/komika.ttf", 24);
    if(!Font)
    {
        SDL_Log("Could't load ttf file: %s\n", TTF_GetError());
        return(-1);
    }
    Font2 = TTF_OpenFont("res/gunmetl.ttf", 24);
    if(!Font2)
    {
        SDL_Log("Could't load ttf file: %s\n", TTF_GetError());
        return(-1);
    }
    SDL_Surface *logo;
    logo = SDL_LoadBMP("res/img/icon.bmp");
    SDL_SetWindowIcon(Win,
                       logo);
    SDL_FreeSurface(logo);
    bool32 Running = true;
    game_state GameState = {0};
    GameState.CurrentState = GAME_FRONT;
    game_input Input[2];
    game_input *NewInput = &Input[0];
    game_input *OldInput = &Input[1];

    Uint64 Start = 0;
    Uint64 End = 0;
    Uint64 Frequency = SDL_GetPerformanceFrequency();
    float dtInput = 0;


    game_assets Assets;
    LoadAllGameAssets(&Assets, Render);

    RWFile = SDL_RWFromFile("records.red", "r+b");

    GetRecordIndex(GameState.Records, RWFile);

    for(int i=0;i<10;i++)
    {
        b[i].bulletView=false;
        b[i].bullet = Assets.BulletOne.Tex;
        b[i].bulletWidth = Assets.BulletOne.Quads.w;
        b[i].bulletHeight = Assets.BulletOne.Quads.h;
        b[i].bulletPosition.x=100;
        b[i].bulletPosition.y=365;
        b[i].bulletPosition.w=16;
        b[i].bulletPosition.h=10;
        b[i].rectBullet.h=10;
        b[i].rectBullet.w=16;
    }

    for(int i=0;i<20;i++)
    {
        b2[i].bulletView=false;
        b2[i].bullet = Assets.BulletTwo.Tex;
        b2[i].bulletWidth = Assets.BulletTwo.Quads.w;
        b2[i].bulletHeight = Assets.BulletTwo.Quads.h;
        b2[i].bulletPosition.x=100;
        b2[i].bulletPosition.y=365;
        b2[i].bulletPosition.w=16;
        b2[i].bulletPosition.h=10;
        b2[i].rectBullet.h=10;
        b2[i].rectBullet.w=16;
    }

    for(int i=0;i<NUM_ENEMIGOS;i++)
    {
        e[i].villain=Assets.Monster.Tex;
        e[i].villainPosition.x=100+(100+rand()%30)*i+30;
        e[i].villainPosition.w=100;
        e[i].villainPosition.h=100;
        e[i].rectVillain.x=e[i].rectVillain.y=0;
        e[i].rectVillain.w=Assets.Monster.Quads.w;
        e[i].rectVillain.h=Assets.Monster.Quads.h;
        e[i].villainPosition.y=375-e[i].rectVillain.h;

        e[i].die=0;
        e[i].banDie=0;
    }
    Fuego.trampaTex=Assets.Fire.Tex;
    Fuego.trampaPosition.x=1200;
    Fuego.trampaPosition.w=32;
    Fuego.trampaPosition.h=50;
    Fuego.rectTrampa.x=Fuego.rectTrampa.y=0;
    Fuego.rectTrampa.w=Assets.Fire.Quads.w;
    Fuego.rectTrampa.h=Assets.Fire.Quads.h;
    Fuego.trampaPosition.y=385;

    for(int i=0;i<15;i++)
    {
        Incendio[i].trampaTex=Assets.Fire.Tex;
        Incendio[i].trampaPosition.x=(i+1)*(220+rand()%10);
        Incendio[i].trampaPosition.w=32;
        Incendio[i].trampaPosition.h=50;
        Incendio[i].rectTrampa.x=Incendio[i].rectTrampa.y=0;
        Incendio[i].rectTrampa.w=Assets.Fire.Quads.w;
        Incendio[i].rectTrampa.h=Assets.Fire.Quads.h;
        Incendio[i].trampaPosition.y=385;
    }
    for(int i=0;i<10;i++)
    {
        Picos[i].trampaTex=Assets.Spike.Tex;
        Picos[i].trampaPosition.x=300;
        Picos[i].trampaPosition.w=95;
        Picos[i].trampaPosition.h=25;
        Picos[i].rectTrampa.x=Picos[i].rectTrampa.y=0;
        Picos[i].rectTrampa.w=Assets.Spike.Quads.w;
        Picos[i].rectTrampa.h=Assets.Spike.Quads.h;
        Picos[i].trampaPosition.y=415;
    }
    Picos[0].trampaPosition.x=399;
    Picos[0].trampaPosition.x=1418;


    Mix_PlayMusic(Assets.Intro, -1);
    Uint32 Delay = 0;
    Uint32 DelayStart = SDL_GetTicks();
    Uint32 DelayEnd = 0;
    while(Running)
    {

        if(!GameState.GameOn && GameState.CurrentState == GAME_PLAY)
        {
            SDL_StartTextInput();
        }
        else
        {
            SDL_StopTextInput();
        }
        SDL_Event Events;
        while(SDL_PollEvent(&Events))
        {
            switch(Events.type)
            {
                case SDL_KEYDOWN:
                {
                    switch(Events.key.keysym.sym)
                    {
                            case SDLK_BACKSPACE:
                            {
                                if(GameState.Cursor > 0)
                                {
                                 GameState.CMDText[GameState.Cursor - 1] = 0;
                                 GameState.Cursor--;
                                }
                            }break;
                            case SDLK_p:
                            {
                                if(GameState.CurrentState!=GAME_PLAY || GameState.GameOn==true)
                                {
                                    if(!Mix_PlayingMusic())
                                    {
                                        switch(GameState.CurrentState)
                                        {
                                        case GAME_END:
                                            Mix_PlayMusic(Assets.DieSong, -1);
                                            break;
                                        case GAME_RECORDS:
                                        case GAME_INSTRUCTIONS:
                                        case GAME_PLAY:
                                        case GAME_MENU:
                                            if(GameState.GameOn)
                                                Mix_PlayMusic(Assets.Back, -1);
                                            else
                                                Mix_PlayMusic(Assets.Menu, -1);
                                            break;
                                        case GAME_RECORDS2:
                                        case GAME_INSTRUCTIONS2:
                                        case GAME_PLAY2:
                                        case GAME_MENU2:
                                            Mix_PlayMusic(Assets.Level2, -1);
                                            break;
                                         case GAME_RECORDS3:
                                        case GAME_INSTRUCTIONS3:
                                        case GAME_PLAY3:
                                        case GAME_MENU3:
                                            Mix_PlayMusic(Assets.Level3, -1);
                                            break;
                                        case GAME_FRONT:
                                                Mix_PlayMusic(Assets.Intro, -1);
                                            break;
                                        case GAME_WIN:
                                                Mix_PlayMusic(Assets.WinSong, -1);
                                            break;
                                        default:
                                            Mix_PlayMusic(Assets.Menu, -1);
                                        break;
                                        }
                                    }

                                    else if(Mix_PausedMusic())
                                    {
                                        Mix_ResumeMusic();
                                    }
                                    else
                                    {
                                        Mix_PauseMusic();
                                    }
                                }
                            }break;
                            case SDLK_s:
                            {
                                if(GameState.CurrentState!=GAME_PLAY || GameState.GameOn==true)
                                    Mix_HaltMusic();
                            }break;
                    }
                }break;
                case SDL_QUIT:
                {
                    Running = false;
                }break;
                case SDL_TEXTINPUT:
                {
                    switch(Events.key.keysym.sym)
                    {
                        case SDLK_BACKSPACE:
                        {
                            if(GameState.Cursor > 0)
                                GameState.CMDText[GameState.Cursor--] = 0;
                        }break;
                        default:
                        {
                               GameState.CMDText[GameState.Cursor++] = *Events.text.text;
                        }break;
                    }
                }break;
            }
        }

        SDLUpdateInput(NewInput, OldInput);

        SDL_SetRenderDrawColor(Render, 255, 255, 255, 255);
        SDL_RenderClear(Render);

        switch(GameState.CurrentState)
        {
            case GAME_FRONT:
            {
                Controller = GetGameController(Input, 0);
                if(Controller->Start.Down)
                {
                    Mix_PlayMusic(Assets.Menu, -1);
                    GameState.CurrentState = GAME_MENU;
                }
                SDL_RenderCopy(Render, Assets.Front.Tex, 0, 0);
                if(Delay>=8000)
                    Assets.Front.Tex = LoadTexture("res/ui/portada2.png", Render);
                if(Delay >= 16000)
                {
                    Mix_PlayMusic(Assets.Menu, -1);
                    GameState.CurrentState = GAME_MENU;
                    Delay = 0;
                }

            }break;
            case GAME_MENU:
            {
                GameMenu(Render, &GameState, NewInput, &Assets);
            }break;
            case GAME_MENU2:
            {
                GameMenu(Render, &GameState, NewInput, &Assets);
            }break;
            case GAME_MENU3:
            {
                GameMenu(Render, &GameState, NewInput, &Assets);
            }break;
            case GAME_PLAY:
            {
                if(GameState.GameOn)
                {
                    if(!banImage1)
                    {
                        SDL_RenderCopy(Render, Assets.Story1.Tex, 0, 0);
                        if(Delay>=4500)
                            banImage1=1;
                    }
                    else
                    {
                        ran=rand()%100;
                        GameRun(Render, &GameState, &Assets, NewInput,false);
                        /*if(ran==19)
                            SDL_RenderCopy(Render, Assets.White.Tex, 0, 0);
                        cout<<"Kame: "<<GameState.kame<<endl;*/
                        SDL_Delay(33);
                    }

                }
                else
                {

                    banImage1=0;
                    GameState.faltas[0]='0';
                    GameState.faltas[1]='0';
                    SelectCharacter(Render, &GameState, NewInput, &Assets);
                    if(GameState.GameOn)
                    {
                        GameState.kame=0;
                        Mix_PlayMusic(Assets.Back, -1);
                        Delay=0;
                        GameState.kills=0;
                        GameState.Current.Score = 0;
                        GameState.premios[0]=0;
                        GameState.premios[1]=0;

                    }

                }
                if(GameState.CurrentState==GAME_END)
                {
                    Mix_PlayChannel(5,Assets.Death,0);
                    Mix_PlayMusic(Assets.DieSong, -1);
                    Delay=0;
                }
                if(GameState.CurrentState==GAME_WIN)
                    Delay=0;
                if(GameState.CurrentState==GAME_PLAY2)
                {
                    banImage1=0;
                    Delay=0;
                    Mix_PlayMusic(Assets.Level2, -1);
                    GameRun2(Render, &GameState, &Assets, NewInput,true);
                    Mix_PlayChannel(5,Assets.Rain,-1);
                }
            }break;
            case GAME_PLAY2:
            {
                if(!banImage1)
                {
                    SDL_RenderCopy(Render, Assets.Story2.Tex, 0, 0);
                    if(Delay>=5850)
                        banImage1=1;
                }
                else
                {
                    ran=rand()%100;
                    GameRun2(Render, &GameState, &Assets, NewInput,false);
                    if(ran==19)
                    {
                        SDL_RenderCopy(Render, Assets.White.Tex, 0, 0);
                        Mix_PlayChannel(1,Assets.Thunder,0);
                    }
                    SDL_Delay(33);
                }

                if(GameState.CurrentState==GAME_END)
                {
                    Mix_PlayChannel(5,Assets.Death,0);
                    Mix_PlayMusic(Assets.DieSong, -1);
                    Delay=0;
                    Mix_PausedMusic();
                }
                if(GameState.CurrentState==GAME_PLAY3)
                {
                    banImage1=0;
                    Delay=0;
                    Mix_PlayMusic(Assets.Level3, -1);
                    GameRun3(Render, &GameState, &Assets, NewInput,true);
                    Mix_PlayChannel(5,Assets.Charge,0);
                }
            }break;
            case GAME_PLAY3:
            {
                if(!banImage1)
                {
                    SDL_RenderCopy(Render, Assets.Story3.Tex, 0, 0);
                    if(Delay>=5850)
                        banImage1=1;
                }
                else
                {
                    ran=rand()%100;
                    GameRun3(Render, &GameState, &Assets, NewInput,false);
                    if(ran==19)
                    {
                        SDL_RenderCopy(Render, Assets.White.Tex, 0, 0);
                        Mix_PlayChannel(1,Assets.Thunder,0);
                    }
                    SDL_Delay(33);
                }

                if(GameState.CurrentState==GAME_END)
                {
                    Mix_PlayChannel(5,Assets.Death,0);
                    Mix_PlayMusic(Assets.DieSong, -1);
                    Delay=0;
                    Mix_PausedMusic();
                }
                if(GameState.CurrentState==GAME_WIN)
                {
                    Mix_PlayMusic(Assets.WinSong, -1);
                    Delay=0;
                }
            }break;
            case GAME_RECORDS:
            {
                GameRecords(Render, &GameState, NewInput, &Assets);
            }break;
            case GAME_RECORDS2:
            {
                GameRecords(Render, &GameState, NewInput, &Assets);
            }break;
            case GAME_RECORDS3:
            {
                GameRecords(Render, &GameState, NewInput, &Assets);
            }break;
            case GAME_END:
                if(Delay<=34500)
                {
                    SDL_RenderCopy(Render, Assets.Fondo.Tex, 0, 0);
                    SDL_RenderCopy(Render,Assets.DieImage.Tex,0,&(Assets.DieImage.Quads));
                    SDL_RenderPresent(Render);
                }
                if(Delay >= 34500)
                {
                    Mix_PlayMusic(Assets.Menu, -1);
                    GameState.CurrentState = GAME_MENU;
                    Delay = 0;
                    GameRun(Render, &GameState, &Assets, NewInput,true);
                }
                if(Controller->Start.Down)
                {
                    Mix_PlayMusic(Assets.Menu, -1);
                    GameState.CurrentState = GAME_MENU;
                    GameRun(Render, &GameState, &Assets, NewInput,true);
                }
            break;
            case GAME_WIN:
                if(Delay<=55500)
                {
                    SDL_RenderCopy(Render, Assets.Fondo.Tex, 0, 0);
                    SDL_RenderCopy(Render,Assets.WinImage.Tex,0,&(Assets.WinImage.Quads));
                    SDL_RenderPresent(Render);
                }
                 if(Delay>=10000 && Delay<=19500)
                {
                    SDL_RenderCopy(Render, Assets.Black.Tex, 0, 0);
                    SDL_RenderPresent(Render);
                }
                if(Delay>=19500 && Delay<=55500)
                {
                    char finalScore[15];
                    char scoreLabel[15]="Score:";
                    SDL_itoa(GameState.WinScore.Score,finalScore,10);
                    SDL_Surface *TextSurface8 = TTF_RenderText_Blended(Font2, scoreLabel, TextColor2);
                    SDL_Texture *TextTexture8 = SDL_CreateTextureFromSurface(Render, TextSurface8);
                    SDL_Rect Rect8 = {60, 35, 240, 64};
                    SDL_RenderCopy(Render, TextTexture8, 0, &Rect8);
                    SDL_FreeSurface(TextSurface8);
                    SDL_DestroyTexture(TextTexture8);

                    SDL_Surface *TextSurface7 = TTF_RenderText_Blended(Font2, finalScore, TextColor2);
                    SDL_Texture *TextTexture7 = SDL_CreateTextureFromSurface(Render, TextSurface7);
                    SDL_Rect Rect7 = {320,35, 240, 64};
                    SDL_RenderCopy(Render, TextTexture7, 0, &Rect7);
                    SDL_FreeSurface(TextSurface7);
                    SDL_DestroyTexture(TextTexture7);
                }
                if(Delay >= 55500)
                {
                    Mix_PlayMusic(Assets.Menu, -1);
                    GameState.CurrentState = GAME_MENU;
                    Delay = 0;
                    GameRun(Render, &GameState, &Assets, NewInput,true);
                }
                if(Controller->Start.Down)
                {
                    Mix_PlayMusic(Assets.Menu, -1);
                    GameState.CurrentState = GAME_MENU;
                    GameRun(Render, &GameState, &Assets, NewInput,true);
                }
            break;

            case GAME_INSTRUCTIONS :
                SDL_RenderCopy(Render, Assets.Fondo.Tex, 0, 0);
                SDL_RenderCopy(Render, Assets.img_instructions.Tex, 0, 0);
                if(Controller->Start.Down)
                {
                    GameState.CurrentState = GAME_MENU;
                }

                break;
            case GAME_INSTRUCTIONS2 :
                SDL_RenderCopy(Render, Assets.Fondo.Tex, 0, 0);
                SDL_RenderCopy(Render, Assets.img_instructions.Tex, 0, 0);
                if(Controller->Start.Down)
                {
                    GameState.CurrentState = GAME_MENU2;
                }

                break;
                case GAME_INSTRUCTIONS3 :
                SDL_RenderCopy(Render, Assets.Fondo.Tex, 0, 0);
                SDL_RenderCopy(Render, Assets.img_instructions.Tex, 0, 0);
                if(Controller->Start.Down)
                {
                    GameState.CurrentState = GAME_MENU3;
                }

                break;
        }
        SDL_RenderPresent(Render);
        game_input Temp = *NewInput;
        *NewInput = *OldInput;
        *OldInput = Temp;
        End = SDL_GetPerformanceCounter();
        Uint64 Elapsed = End - Start;
        dtInput = (float)Elapsed/(float)Frequency;
        GameState.dtInput = dtInput;
        Start = End;
        DelayEnd = SDL_GetTicks();
        Delay += DelayEnd - DelayStart;
        DelayStart = DelayEnd;
    }

    SDL_RWseek(RWFile, 0, RW_SEEK_SET);
    SDL_RWwrite(RWFile, GameState.Records, sizeof(record)*10, 1);
    SDL_RWclose(RWFile);

    SDL_StopTextInput();
    SDL_DestroyRenderer(Render);
    Render = 0;
    SDL_DestroyWindow(Win);
    Win = 0;

    return(0);
}
static void
GetKeyboardInput(game_controller *NewKeyboard, game_controller *OldKeyboard)
{
    const Uint8 *KeyState = SDL_GetKeyboardState(0);

    SetButtonState(&NewKeyboard->MoveUp, &OldKeyboard->MoveUp,
                   KeyState[SDL_SCANCODE_UP]);
    SetButtonState(&NewKeyboard->MoveDown, &OldKeyboard->MoveDown,
                   KeyState[SDL_SCANCODE_DOWN]);
    SetButtonState(&NewKeyboard->MoveLeft, &OldKeyboard->MoveLeft,
                   KeyState[SDL_SCANCODE_LEFT]);
    SetButtonState(&NewKeyboard->MoveRight, &OldKeyboard->MoveRight,
                   KeyState[SDL_SCANCODE_RIGHT]);
    SetButtonState(&NewKeyboard->ActionUp, &OldKeyboard->ActionUp,
                   KeyState[SDL_SCANCODE_A]);

    SetButtonState(&NewKeyboard->Start, &OldKeyboard->Start,
                   KeyState[SDL_SCANCODE_ESCAPE]);
}

static void SDLUpdateInput(game_input *NewInput, game_input *OldInput)
{
    int X = 0;
    int Y = 0;
    bool32 Value = SDL_GetMouseState(&X, &Y);

    NewInput->MouseX = X;
    NewInput->MouseY = Y;
    SetButtonState(&NewInput->MouseLeft, &OldInput->MouseLeft, Value & SDL_BUTTON(SDL_BUTTON_LEFT));
    SetButtonState(&NewInput->MouseRight, &OldInput->MouseRight, Value & SDL_BUTTON(SDL_BUTTON_RIGHT));

    game_controller *New = GetGameController(NewInput, 0);
    game_controller *Old = GetGameController(OldInput, 0);
    GetKeyboardInput(New, Old);
}

static void
LoadAllGameAssets(game_assets *Assets, SDL_Renderer *Render)
{
    int Width = 0;
    int Height = 0;

    Assets->Map.Tex = LoadTexture("res/img/mapa.png", Render);
    SDL_QueryTexture(Assets->Map.Tex, 0, 0, &Width, &Height);
    Assets->Map.Quads.x = Assets->Map.Quads.y = 0;
    Assets->Map.Quads.w = 640;
    Assets->Map.Quads.h = 480;

    Assets->Map1.Tex = LoadTexture("res/img/mapa.png", Render);
    Assets->Map3.Tex = LoadTexture("res/img/mapa3.png", Render);
    Assets->Map4.Tex = LoadTexture("res/img/mapa4.png", Render);

    Width = 0;
    Height = 0;

    Assets->Clouds.Tex = LoadTexture("res/img/nubes.png", Render);
    SDL_QueryTexture(Assets->Clouds.Tex, 0, 0, &Width, &Height);
    Assets->Clouds.Quads.x = Assets->Clouds.Quads.y = 0;
    Assets->Clouds.Quads.w = 640;
    Assets->Clouds.Quads.h = 480;

    Assets->Clouds1.Tex = LoadTexture("res/img/nubes.png", Render);
    Assets->Clouds2.Tex = LoadTexture("res/img/nubes2.png", Render);
    Assets->Clouds3.Tex = LoadTexture("res/img/nubes3.png", Render);

    Width = 0;
    Height = 0;

    Assets->CatOne.Tex = LoadTexture("res/sprites/cat/Sprite3.png", Render);
    Assets->CatTwo.Tex = LoadTexture("res/sprites/cat/sprite.png", Render);
    SDL_QueryTexture(Assets->CatOne.Tex, 0, 0, &Width, &Height);
    Assets->CatOne.Quads.y = Assets->CatTwo.Quads.y = 0;
    Assets->CatOne.Quads.x = Assets->CatTwo.Quads.x = 0;
    Assets->CatOne.Quads.w = Assets->CatTwo.Quads.w = Width/8;
    Assets->CatOne.Quads.h = Assets->CatTwo.Quads.h = Height/16;
    Current = Assets->CatOne;

    Width = 0;
    Height = 0;

    Assets->BulletOne.Tex = LoadTexture("res/sprites/bullet.png", Render);
    Assets->BulletTwo.Tex = LoadTexture("res/sprites/bullet2.png", Render);
    SDL_QueryTexture(Assets->BulletOne.Tex, 0, 0, &Width, &Height);
    Assets->BulletOne.Quads.w = Width;
    Assets->BulletTwo.Quads.w = Width;
    Assets->BulletOne.Quads.h = Height;
    Assets->BulletTwo.Quads.h = Height*2;

    Width = 0;
    Height = 0;

    Assets->Monster.Tex = LoadTexture("res/sprites/monster/sprite.png", Render);
    SDL_QueryTexture(Assets->Monster.Tex, 0, 0, &Width, &Height);
    Assets->Monster.Quads.x = Assets->Monster.Quads.y = 0;
    Assets->Monster.Quads.w = Width/8;
    Assets->Monster.Quads.h = Height/10;

    Assets->Monster2.Tex = LoadTexture("res/sprites/monster/sprite_zombie.png", Render);
    SDL_QueryTexture(Assets->Monster2.Tex, 0, 0, &Width, &Height);
    Assets->Monster2.Quads.x = Assets->Monster2.Quads.y = 0;
    Assets->Monster2.Quads.w = Width/8;
    Assets->Monster2.Quads.h = Height/10;

    Assets->Volador.Tex = LoadTexture("res/sprites/monster/flyng.png", Render);
    SDL_QueryTexture(Assets->Volador.Tex, 0, 0, &Width, &Height);
    Assets->Volador.Quads.x = Assets->Volador.Quads.y = 0;
    Assets->Volador.Quads.w = Width/8;
    Assets->Volador.Quads.h = Height/10;

    Assets->Kamehame.Tex = LoadTexture("res/sprites/kame/sprite.png", Render);
    SDL_QueryTexture(Assets->Kamehame.Tex, 0, 0, &Width, &Height);
    Assets->Kamehame.Quads.x = Assets->Kamehame.Quads.y = 0;
    Assets->Kamehame.Quads.w = Width/7;
    Assets->Kamehame.Quads.h = Height/2;

    Assets->Fire.Tex = LoadTexture("res/sprites/fire/sprite.png", Render);
    SDL_QueryTexture(Assets->Fire.Tex, 0, 0, &Width, &Height);
    Assets->Fire.Quads.x = Assets->Fire.Quads.y = 0;
    Assets->Fire.Quads.w = Width/4;
    Assets->Fire.Quads.h = Height;

    Assets->Spike.Tex = LoadTexture("res/ui/spike.png", Render);
    SDL_QueryTexture(Assets->Spike.Tex, 0, 0, &Width, &Height);
    Assets->Spike.Quads.x = Assets->Spike.Quads.y = 0;
    Assets->Spike.Quads.w = Width;
    Assets->Spike.Quads.h = Height;

    Assets->Play.Tex = LoadTexture("res/ui/play.png", Render);
    SDL_QueryTexture(Assets->Play.Tex, 0, 0, &Width, &Height);
    Assets->Play.Quads.w = Width;
    Assets->Play.Quads.h = Height;
    Assets->Play.Quads.x = 320;
    Assets->Play.Quads.y = 170;

    Assets->Records.Tex = LoadTexture("res/ui/records.png", Render);
    SDL_QueryTexture(Assets->Records.Tex, 0, 0, &Width, &Height);
    Assets->Records.Quads.w = Width;
    Assets->Records.Quads.h = Height;
    Assets->Records.Quads.x = 320;
    Assets->Records.Quads.y = 255;

    Assets->Instructions.Tex = LoadTexture("res/ui/instructions.png", Render);
    SDL_QueryTexture(Assets->Instructions.Tex, 0, 0, &Width, &Height);
    Assets->Instructions.Quads.w = Width;
    Assets->Instructions.Quads.h = Height;
    Assets->Instructions.Quads.x = 320;
    Assets->Instructions.Quads.y = 340;

    Assets->Exit.Tex = LoadTexture("res/ui/Exit.png", Render);
    SDL_QueryTexture(Assets->Exit.Tex, 0, 0, &Width, &Height);
    Assets->Exit.Quads.w = Width;
    Assets->Exit.Quads.h = Height;
    Assets->Exit.Quads.x = 320;
    Assets->Exit.Quads.y = 425;

    Assets->DieImage.Tex = LoadTexture("res/ui/die.png", Render);
    SDL_QueryTexture(Assets->DieImage.Tex, 0, 0, &Width, &Height);
    Assets->DieImage.Quads.w = Width;
    Assets->DieImage.Quads.h = Height;
    Assets->DieImage.Quads.x = 132;
    Assets->DieImage.Quads.y = 147;

    Assets->WinImage.Tex = LoadTexture("res/ui/win.png", Render);
    SDL_QueryTexture(Assets->WinImage.Tex, 0, 0, &Width, &Height);
    Assets->WinImage.Quads.w = Width;
    Assets->WinImage.Quads.h = Height;
    Assets->WinImage.Quads.x = 132;
    Assets->WinImage.Quads.y = 147;

    Assets->Go.Tex = LoadTexture("res/ui/go.png", Render);
    SDL_QueryTexture(Assets->Go.Tex, 0, 0, &Width, &Height);
    Assets->Go.Quads.w = Width;
    Assets->Go.Quads.h = Height;
    Assets->Go.Quads.x = 332;
    Assets->Go.Quads.y = 82;

    Assets->Gallo.Tex = LoadTexture("res/ui/gallo.png", Render);
    SDL_QueryTexture(Assets->Gallo.Tex, 0, 0, &Width, &Height);
    Assets->Gallo.Quads.w = Width;
    Assets->Gallo.Quads.h = Height;

    Assets->Llama.Tex = LoadTexture("res/ui/llama.png", Render);
    SDL_QueryTexture(Assets->Llama.Tex, 0, 0, &Width, &Height);
    Assets->Llama.Quads.w = Width;
    Assets->Llama.Quads.h = Height;

    Assets->Pila.Tex = LoadTexture("res/ui/pila.png", Render);
    SDL_QueryTexture(Assets->Pila.Tex, 0, 0, &Width, &Height);
    Assets->Pila.Quads.w = Width;
    Assets->Pila.Quads.h = Height;
    Assets->Pila.Quads.x = 480;
    Assets->Pila.Quads.y = 24;

    Assets->Energia.Tex = LoadTexture("res/ui/energia.png", Render);
    SDL_QueryTexture(Assets->Energia.Tex, 0, 0, &Width, &Height);
    Assets->Energia.Quads.w = Width;
    Assets->Energia.Quads.h = Height;
    Assets->Energia.Quads.x = 485;
    Assets->Energia.Quads.y = 27;

    Assets->Titulo.Tex = LoadTexture("res/ui/titulo.png", Render);
    SDL_QueryTexture(Assets->Titulo.Tex, 0, 0, &Width, &Height);
    Assets->Titulo.Quads.w = Width;
    Assets->Titulo.Quads.h = Height;
    Assets->Titulo.Quads.x = 103;
    Assets->Titulo.Quads.y = 10;

    Assets->Fondo.Tex = LoadTexture("res/ui/fondo.png", Render);
    Assets->img_instructions.Tex = LoadTexture("res/ui/instrucciones.png", Render);
    Assets->Black.Tex = LoadTexture("res/ui/black.png", Render);
    Assets->White.Tex = LoadTexture("res/ui/white.png", Render);
    Assets->Story1.Tex = LoadTexture("res/ui/story1.png", Render);
    Assets->Story2.Tex = LoadTexture("res/ui/story2.png", Render);
    Assets->Story3.Tex = LoadTexture("res/ui/story3.png", Render);
    Assets->Front.Tex = LoadTexture("res/ui/portada.png", Render);
    Assets->Front2.Tex = LoadTexture("res/ui/portada2.png", Render);

    Width = 0;
    Height = 0;

    Assets->CharOne.Tex = LoadTexture("res/ui/gato2_boton.png", Render);
    Assets->CharTwo.Tex = LoadTexture("res/ui/gato1_boton.png", Render);

    SDL_QueryTexture(Assets->CharOne.Tex, 0, 0, &Width, &Height);
    Assets->CharOne.Quads.w = Assets->CharTwo.Quads.w = 220;
    Assets->CharOne.Quads.h = Assets->CharTwo.Quads.h = 220*(Height/Width);
    Assets->CharOne.Quads.x = Width/2 + 50;
    Assets->CharOne.Quads.y = Height/2 + 50;

    Assets->CharTwo.Quads.x = Width/2 + 300;
    Assets->CharTwo.Quads.y = Height/2 + 50;


    Assets->Back = Mix_LoadMUS("res/sfx/song.mp3");
    Assets->Shoot = Mix_LoadWAV("res/sfx/rifle.wav");
    Assets->Kame_Sound = Mix_LoadWAV("res/sfx/kame.wav");
    Assets->Punch = Mix_LoadWAV("res/sfx/punch.wav");
    Assets->Die = Mix_LoadWAV("res/sfx/die.wav");
    Assets->Shoot = Mix_LoadWAV("res/sfx/rifle.wav");
    Assets->Click = Mix_LoadWAV("res/sfx/click.wav");
    Assets->Jump = Mix_LoadWAV("res/sfx/jump.wav");
    Assets->Charge = Mix_LoadWAV("res/sfx/charge.wav");
    Assets->ChargeEnd = Mix_LoadWAV("res/sfx/chargeEnd.wav");
    Assets->Thunder = Mix_LoadWAV("res/sfx/thunder.wav");
    Assets->Rain = Mix_LoadWAV("res/sfx/rain.wav");
    Assets->Coin = Mix_LoadWAV("res/sfx/canto.wav");
    Assets->Sing = Mix_LoadWAV("res/sfx/coin.wav");
    Assets->Burn = Mix_LoadWAV("res/sfx/burn.wav");
    Assets->Espinazo = Mix_LoadWAV("res/sfx/espinazo.wav");
    Assets->Death = Mix_LoadWAV("res/sfx/death.wav");
    Assets->OneUp = Mix_LoadWAV("res/sfx/1_up.wav");
    Assets->Alien = Mix_LoadWAV("res/sfx/alien.wav");
    Assets->Intro = Mix_LoadMUS("res/sfx/intro.mp3");
    Assets->Menu = Mix_LoadMUS("res/sfx/menu.mp3");
    Assets->DieSong=Mix_LoadMUS("res/sfx/soundSilence.mp3");
    Assets->WinSong=Mix_LoadMUS("res/sfx/champion2.mp3");
    Assets->Level2=Mix_LoadMUS("res/sfx/level2.mp3");
    Assets->Level3=Mix_LoadMUS("res/sfx/level3.mp3");


}
