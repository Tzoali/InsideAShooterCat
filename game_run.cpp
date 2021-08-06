#if 0
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <math.h>
#include <windows.h>
//Screen dimension constants
const int SCREEN_WIDTH = 480;
const int SCREEN_HEIGHT = 400;
const int NUM_ENEMIGOS = 15;

using namespace std;
int cont;
 enum Status {WALK,DOWN,JUMP,NOTHING};
 enum EvilStatus{NOTHING2,JUMP2,SHOOT,ATTACK};
SDL_Texture *LoadTexture(string filePath, SDL_Renderer *renderTarget);
struct balas{
    int bulletWidth;
    int bulletHeight;
    SDL_Texture *bullet;
    SDL_Rect bulletPosition;
    bool bulletView;
    int bulletCont;
    int bulletDireccion;
     SDL_Rect rectBullet;
};
struct enemigo{
    int villainWidth;
    int villainHeight;
    SDL_Texture *villain;
    SDL_Rect villainPosition;
    SDL_Rect rectVillain;
    int die=0;
    int banDie=0;
};
int touch(SDL_Rect a,SDL_Rect b,SDL_Rect ta,SDL_Rect tb);
void new_bala(balas *b,SDL_Rect position,int *noBalas,bool *shootUP,int direccion,int bandera,SDL_Rect rectBullet2,SDL_Texture *bullet2,SDL_Texture *bullet1,SDL_Rect rectBullet1);
void evilBehavior(SDL_Rect *evilRect,SDL_Rect *evilPosition, SDL_Rect goodPosition,SDL_Rect goodRect);
void imprimirVillano(enemigo *e,SDL_Rect playerPosition,SDL_Rect rectImage,int z,bool *isRunning,SDL_Renderer *renderTarget);
void movimientosProtagonista(SDL_Rect *playerPosition,int velx,int vely,bool *salto,float *frameTime,float deltaTime,int *z,int *contshot,enemigo *e,bool active,Status *dir,bool *draw,SDL_Rect *rectImage,int direccion,int *shot);
/*int main( int argc, char* args[] )
{
    srand(time(0));
    bool shootUp=true;
    int noBalas=0;
    int z=3;
    int shot=0;
    int contshot=0;
    int direccion=0;
    SDL_Window *window = NULL;
    SDL_Renderer *renderTarget=NULL;
    int prevTime=0;
    int currentTime=0;
    float deltaTime=0;
    const Uint8 *keyState;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    if(TTF_Init()<0)
        cout<<"Error: "<<TTF_GetError()<<endl;
    int imgflags=IMG_INIT_PNG | IMG_INIT_JPG;
    int jump=0;
    if(IMG_Init(imgflags)!=imgflags)
        cout<<"Error: "<<IMG_GetError()<<endl;

    window=SDL_CreateWindow("LO que viene arriba",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
    renderTarget = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048)<0)
        cout<<"Error: "<<Mix_GetError()<<endl;
    Mix_Music *bgm =Mix_LoadMUS("song.mp3");
    Mix_Chunk *shoot = Mix_LoadWAV("rifle.wav");
    Mix_Chunk *punch = Mix_LoadWAV("punch.wav");
    Mix_Chunk *die = Mix_LoadWAV("die.wav");
    Mix_Chunk *jumpSound = Mix_LoadWAV("jump.wav");
    Mix_Chunk *charge = Mix_LoadWAV("charge.wav");
    Mix_Chunk *chargeEnd = Mix_LoadWAV("chargeEnd.wav");
//    Mix_Music *jump = Mix_LoadMUS("jump.mp3");

    SDL_SetRenderDrawColor(renderTarget,0,0,0,0);

     //Cargar Mapa
     SDL_Texture *mapa = LoadTexture("mapa.png",renderTarget);
     int mapaWidth, mapaHeight;
     SDL_Rect mapaRect;
     SDL_QueryTexture(mapa,NULL,NULL,&mapaWidth,&mapaHeight);
     mapaRect.x = mapaRect.y = 0;
     int mframeWidth,mframeHeight;
     mapaRect.w = SCREEN_WIDTH;
     mapaRect.h = SCREEN_HEIGHT;

    SDL_Texture *nubes = LoadTexture("nubes.png",renderTarget);
     int nubesWidth, nubesHeight;
     SDL_Rect nubesRect;
     SDL_QueryTexture(nubes,NULL,NULL,&nubesWidth,&nubesHeight);
     nubesRect.x = nubesRect.y = 0;
     int nframeWidth,nframeHeight;
     nubesRect.w = SCREEN_WIDTH;
     nubesRect.h = SCREEN_HEIGHT;
     //Personaje
    bool draw;

    int img=0;
    bool salto=false;
    bool active;
    int imageWidth,imageHeight;
    SDL_Texture *imagen1=LoadTexture("Cat/Sprite3.png",renderTarget);
    SDL_Texture *imagen2=LoadTexture("Cat/sprite.png",renderTarget);
    SDL_Texture *currentImage=imagen1;
    SDL_Rect rectImage;
    SDL_QueryTexture(currentImage,NULL,NULL,&imageWidth,&imageHeight);
    rectImage.x = rectImage.y=0;
    rectImage.w = imageWidth/8;
    rectImage.h = imageHeight/16;
    int ban;
    float velx,vely;
    velx = vely = 0;
    const float gravity=1;
    SDL_Rect playerPosition;
    playerPosition.x=20;
    playerPosition.y=0-rectImage.h;
    playerPosition.w=60;
    playerPosition.h=60;
    float jumpSpeed=17;
    Status dir=WALK;
    int life;
    int damage;
    int z2=0,z3=0;
    float frameTime=0.0f;

    balas b[15];
    for(int i=0;i<10;i++)
    {
        b[i].bulletView=false;
        b[i].bullet = LoadTexture("bullet.png",renderTarget);
        SDL_QueryTexture(b[i].bullet,NULL,NULL,&b[i].bulletWidth,&b[i].bulletHeight);
        b[i].bulletPosition.x=100;
        b[i].bulletPosition.y=365;
        b[i].bulletPosition.w=16;
        b[i].bulletPosition.h=10;
        b[i].rectBullet.h=10;
        b[i].rectBullet.w=16;
    }
    SDL_Texture *bullet2;
    bullet2 = LoadTexture("bullet2.png",renderTarget);
    SDL_Texture *bullet1=LoadTexture("bullet.png",renderTarget);
    SDL_Rect rectBullet2;
    rectBullet2.w=30;
    rectBullet2.h=15;
    SDL_Rect rectBullet1;
    rectBullet2.w=16;
    rectBullet2.h=10;

    enemigo e[50];
    for(int i=0;i<NUM_ENEMIGOS;i++)
    {
        e[i].villain=LoadTexture("monstruo/sprite.png",renderTarget);
        SDL_QueryTexture(e[i].villain,NULL,NULL,&e[i].villainWidth,&e[i].villainHeight);
        e[i].villainPosition.x=100+(100+rand()%30)*i+30;
        e[i].villainPosition.w=100;
        e[i].villainPosition.h=100;
        e[i].rectVillain.x=e[i].rectVillain.y=0;
        e[i].rectVillain.w=e[i].villainWidth/8;
        e[i].rectVillain.h=e[i].villainHeight/10;
        e[i].villainPosition.y=285-e[i].rectVillain.h;
        e[i].die=0;
        e[i].banDie=0;
    }
    contshot=7;
    bool isRunning=true;
    SDL_Event ev;
    Mix_PlayMusic(bgm,-1);
    while(isRunning)
    {
        prevTime=currentTime;
        currentTime=SDL_GetTicks();
        deltaTime=(currentTime-prevTime)/1400.0f;
        while(SDL_PollEvent(&ev)!=0)
        {
            if(ev.type == SDL_QUIT)
                isRunning=false;
            else if(ev.type ==SDL_KEYDOWN)
            {
                switch(ev.key.keysym.sym)
                {
                case SDLK_p:
                    if(!Mix_PlayingMusic())
                        Mix_PlayMusic(bgm,-1);
                    else if(Mix_PausedMusic())
                        Mix_ResumeMusic();
                    else

                        Mix_PauseMusic();
                    break;
                case SDLK_s:
                    Mix_HaltMusic();
                    break;
                case SDLK_1:
                    if(img)
                    {
                        currentImage=imagen1;
                        img=0;
                    }
                    else
                    {
                        currentImage=imagen2;
                        img=1;
                    }
                    break;
                case SDLK_DOWN:
                    Mix_PlayChannel(4,charge,0);
                    break;
                case SDLK_ESCAPE:
                    isRunning=false;
                    break;
                case SDLK_a:
                    if(shootUp && contshot>=7)
                    {
                        shot=1;
                        contshot=0;
                        if(dir!=DOWN)
                        {
                            if(img)
                            {
                                Mix_PlayChannel(0,shoot,0);
                                new_bala(b+noBalas,playerPosition,&noBalas,&shootUp,direccion,1,rectBullet2,bullet2,bullet1,rectBullet1);
                            }
                            else
                            {
                                Mix_PlayChannel(1,punch,0);
                                new_bala(b+noBalas,playerPosition,&noBalas,&shootUp,direccion,0,rectBullet2,bullet2,bullet1,rectBullet1);
                            }
                        }
                    }
                    break;
                }

            }
            else if(ev.type ==SDL_KEYUP)
            {
                switch(ev.key.keysym.sym)
                {
                case SDLK_DOWN:
                    dir=WALK;
                    Mix_PlayChannel(4,chargeEnd,0);
                    break;
                case SDLK_a:
                    shootUp=true;
                    break;
                }
            }

        }

        ban=0;
        active=true;
        keyState= SDL_GetKeyboardState(NULL);
        jump=0;
        if(keyState[SDL_SCANCODE_RIGHT])
        {
          if(playerPosition.x<300)
            velx=5;
          else
          {
             if(mapaRect.x<1570)
            {
                   mapaRect.x+=5;
                   nubesRect.x+=3;
                for(int i=0;i<10;i++)
                {
                    if(b[i].bulletView)
                    {
                        b[i].bulletPosition.x-=5;
                    }
                }
                for(int i=0;i<NUM_ENEMIGOS;i++)
                {
                    e[i].villainPosition.x-=5;
                }
            }

          }
            dir=WALK;
            direccion=0;
            if(keyState[SDL_SCANCODE_UP] && salto)
            {
                jump=5;
                vely=-jumpSpeed;
                salto=false;
                z2=0;
                Mix_PlayChannel(3,jumpSound,0);
            }

        }
        else if(keyState[SDL_SCANCODE_LEFT])
        {
            if(playerPosition.x>30)
                velx=-5;
            else
            {
                if(mapaRect.x>0)
                {
                       mapaRect.x-=5;
                       nubesRect.x-=3;
                    for(int i=0;i<10;i++)
                    {
                        if(b[i].bulletView)
                        {
                            b[i].bulletPosition.x-=5;
                        }
                    }
                    for(int i=0;i<NUM_ENEMIGOS;i++)
                         e[i].villainPosition.x+=5;
                }

            }
            dir=WALK;
            direccion=1;
            if(keyState[SDL_SCANCODE_UP]&& salto)
            {
                jump=5;
                vely=-jumpSpeed;
                salto=false;
                z2=0;
                Mix_PlayChannel(3,jumpSound,0);
            }
        }
        else
        {
            velx=0;
            active=false;
        }
        if(keyState[SDL_SCANCODE_DOWN])
        {
            dir=DOWN;
            active=true;
        }
        else if(keyState[SDL_SCANCODE_UP]&& salto)
        {
            jump=2;
            vely=-jumpSpeed;
            salto=false;
            active=true;
            z2=0;
            Mix_PlayChannel(3,jumpSound,0);
        }
        draw=true;
        if(!salto)
        {
            vely+=gravity;
               z3=z2++/3;
               dir=JUMP;
        }

        else
        {
            vely=0;
            z2=0;
        }
        movimientosProtagonista(&playerPosition,velx,vely,&salto,&frameTime,deltaTime,&z,&contshot,e,active,&dir,&draw,&rectImage,direccion,&shot);
        for(int i=0;i<NUM_ENEMIGOS;i++)
            e[i].rectVillain.y=2*e[i].rectVillain.h;
        SDL_RenderClear(renderTarget);
        SDL_RenderCopy(renderTarget,nubes,&nubesRect,NULL);
        SDL_RenderCopy(renderTarget,mapa,&mapaRect,NULL);
        for(int i=0;i<10;i++)
        {
            if(b[i].bulletView)
            {
                SDL_RenderCopy(renderTarget,b[i].bullet,NULL,&b[i].bulletPosition);
                b[i].bulletPosition.x+=10*b[i].bulletDireccion;
                b[i].bulletCont++;
                if(b[i].bulletCont>62)
                    b[i].bulletView=false;
                else
                {
                    if(b[i].bulletView)
                    {
                        for(int j=0;j<NUM_ENEMIGOS;j++)
                        {

                            if(!e[j].die)
                             {
                                e[j].die=touch(e[j].villainPosition,b[i].bulletPosition,e[j].rectVillain,b[i].rectBullet);
                                if(e[j].die)
                                {
                                    Mix_PlayChannel(2,die,0);
                                    b[i].bulletView=false;
                                    break;
                                }
                             }
                        }
                    }
                }
            }

        }
        imprimirVillano(e,playerPosition,rectImage,z,&isRunning,renderTarget);
        SDL_RenderCopy(renderTarget,currentImage,&rectImage,&playerPosition);
        SDL_RenderPresent(renderTarget);
    }
    SDL_DestroyTexture(currentImage);
    SDL_DestroyWindow(window);
    window=NULL;
    SDL_DestroyRenderer(renderTarget);
    Mix_FreeChunk(shoot);
    Mix_FreeMusic(bgm);
    bgm=NULL;
    shoot=NULL;
    renderTarget = NULL;

    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}*/
int touch(SDL_Rect a,SDL_Rect b,SDL_Rect ta,SDL_Rect tb)
{
    if(a.x>b.x+tb.w || a.y>b.y+tb.h || b.x>a.x+ta.w || b.y>a.y+ta.y)
        return 0;
    return 1;

}
SDL_Texture *LoadTexture(string filePath, SDL_Renderer *renderTarget)
{
    SDL_Texture *texture = NULL;
    SDL_Surface *surface = IMG_Load(filePath.c_str());
    if(surface==NULL)
        cout<<"Error"<<endl;
    else
    {
        SDL_SetColorKey(surface,SDL_TRUE,SDL_MapRGB(surface->format,237,28,36));
        texture = SDL_CreateTextureFromSurface(renderTarget,surface);
       // SDL_SetTextureColorMod(texture,12,125,125);
        if(texture == NULL)
            cout<<"Error"<<endl;
    }
    SDL_FreeSurface(surface);
    return texture;
}
void new_bala(balas *b,SDL_Rect position,int *noBalas,bool *shootUP,int direccion,int bandera,SDL_Rect rectBullet2,SDL_Texture *bullet2,SDL_Texture *bullet1,SDL_Rect rectBullet1)
{

    (*b).bulletView=true;
    (*b).bulletDireccion=direccion;
    (*b).bulletPosition.y=position.y+4*position.h/7;
    (*b).bulletPosition.x=position.x+position.w/2;
    (*b).bulletCont=0;
    if((*b).bulletDireccion==0)
        (*b).bulletDireccion=1;
    else
        (*b).bulletDireccion=-1;
    if(*noBalas>=9)
        *noBalas=0;
    else
        (*noBalas)++;
    *shootUP=false;
}
void evilBehavior(SDL_Rect *evilRect,SDL_Rect *evilPosition, SDL_Rect goodPosition,SDL_Rect goodRect)
{
    int direccion=0;
    EvilStatus dir;
    if((*evilPosition).x<goodPosition.x)
        direccion=1;
    if(abs((*evilPosition).x-goodPosition.x)>=1000)
        dir=NOTHING2;
    else if((abs((*evilPosition).x-goodPosition.x)>=170))
        dir=JUMP2;
    else
        dir=SHOOT;
    if(touch(*evilPosition,goodPosition,*evilRect,goodRect))
        dir=ATTACK;
    (*evilRect).y=(2*dir+direccion)*(*evilRect).h;
    if(dir==JUMP2)
        (*evilPosition).x+=((*evilPosition).x<goodPosition.x ? 1+rand()%4 : -1-rand()%4);
}
void imprimirVillano(enemigo *e,SDL_Rect playerPosition,SDL_Rect rectImage,int z,bool *isRunning,SDL_Renderer *renderTarget)
{
    for(int i=0;i<NUM_ENEMIGOS;i++)
    {
        if(e[i].die==0)
        {
            e[i].rectVillain.y=(4+2*(abs(e[i].villainPosition.x-playerPosition.x)>150)+(e[i].villainPosition.x<playerPosition.x))*e[i].rectVillain.h;
            evilBehavior(&e[i].rectVillain,&e[i].villainPosition,playerPosition,rectImage);
            if(touch(playerPosition,e[i].villainPosition,rectImage,e[i].rectVillain) && z==6)
                {
                    cout<<"die"<<endl;
                    *isRunning=false;
                    break;
                }
            SDL_RenderCopy(renderTarget,e[i].villain,&e[i].rectVillain,&e[i].villainPosition);
        }
        else if(e[i].banDie<24)
        {
            e[i].rectVillain.y=(8+(e[i].villainPosition.x<playerPosition.x))*e[i].rectVillain.h;
            e[i].die++;
            SDL_RenderCopy(renderTarget,e[i].villain,&e[i].rectVillain,&e[i].villainPosition);
        }
    }
}
void movimientosProtagonista(SDL_Rect *playerPosition,int velx,int vely,bool *salto,float *frameTime,float deltaTime,int *z,int *contshot,enemigo *e,bool active,Status *dir,bool *draw,SDL_Rect *rectImage,int direccion,int *shot)
{
     playerPosition->x+=velx;
    if(playerPosition->x<30)
        playerPosition->x=30;
    if(playerPosition->x >300)

        playerPosition->x=300;
    playerPosition->y+=vely;

    *salto=(playerPosition->y>=285);
    if(*salto)
        playerPosition->y=285;

    (*frameTime)+=deltaTime;
    if(*frameTime >= 0.035f)
    {
        *frameTime=0;
        (*z)++;
        if(*z >= 8)
            *z=0;
        if(*shot)
        {
            (*contshot)++;
            if(*contshot>7)
                *shot=0;
        }
        for(int i=0;i<NUM_ENEMIGOS;i++)
        {
            if(!e[i].die)
                e[i].rectVillain.x=(*z)*e[i].rectVillain.w;
            else
            {
                e[i].rectVillain.x=0+(e[i].banDie/3)*e[i].rectVillain.w;
                e[i].banDie++;
            }
        }
    }
    if(!active)
    {
        *dir=NOTHING;
    }
    if(*draw)
    {
        *draw=false;
    }
    if(!(*salto))
        *dir=JUMP;
    rectImage->y=(2* (*dir) +8* (*shot)+direccion)*rectImage->h;
    rectImage->x = (*shot ? *contshot : *z)*rectImage->w;

}
#endif
