#if !defined (_GAME_RUN_H_)
#include "menu.h"
int rnd;
float jumpSpeed = 17;
const int NUM_ENEMIGOS = 15;
int cont;
int temporal;
int contshot = 7;
int direccion = 0;
int ban = 0;
int active = 0;
int jump = 0;
int bulletImpact;
bool salto = true;
Status dir = NOTHING;
float velX = 0;
float velY = 0;
int z2 = 0, z3 = 0;
int damage;
bool draw = false;
const float gravity = 1;
int shoot;
bool shootUp;
float frametime = 0;
int noBalas = 0;
int noBalas2 = 0;
int z = 0;
const static char *faltas = "Absences: ";
const static char *score = "Score: ";
char cantSco[5];

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
struct trampa
{
    int trampaWidth;
    int trampaHeight;
    SDL_Texture *trampaTex;
    SDL_Rect trampaPosition;
    SDL_Rect rectTrampa;
};

balas b[15];
balas b2[35];
enemigo e[50];
trampa Fuego;
trampa Picos[15];
SDL_Rect Solution;
trampa Incendio[20];
int touch(SDL_Rect a,SDL_Rect b,SDL_Rect ta,SDL_Rect tb)
{
   AABB cosa1;
   AABB cosa2;
   cosa1.Height=ta.h;
   cosa1.Width=ta.w;
   cosa1.X=a.x;
   cosa1.Y=a.y;
   cosa2.Height=tb.h;
   cosa2.Width=tb.w;
   cosa2.X=b.x;
   cosa2.Y=b.y;
   return AABBCollision(&cosa1,&cosa2);
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
void bala_mala(balas *b,SDL_Rect position,int *noBalas,int direccion,SDL_Texture *bullet1,SDL_Rect rectBullet1)
{

    (*b).bulletView=true;
    (*b).bulletDireccion=direccion;
    (*b).bulletPosition.y=position.y+3*position.h/5;
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
}
void evilBehavior(SDL_Rect *evilRect,SDL_Rect *evilPosition, SDL_Rect goodPosition,SDL_Rect goodRect, game_assets *Assets,int i)
{
    int ban=rand()%125;
    int direccion=0;
    EvilStatus dir;
    if((*evilPosition).x<goodPosition.x)
        direccion=1;
    if(abs((*evilPosition).x-goodPosition.x)>=620)
        dir=NOTHING2;
    else if((abs((*evilPosition).x-goodPosition.x)>=170))
        dir=JUMP2;
    else
        dir=SHOOT;
    if(touch(*evilPosition,goodPosition,Solution,goodRect))
        dir=ATTACK;
    (*evilRect).y=(2*dir+direccion)*(*evilRect).h;
    if(dir==JUMP2)
        (*evilPosition).x+=((*evilPosition).x<goodPosition.x ? 3+rand()%4 : -3-rand()%4);
    if(dir==SHOOT && ban==11)
    {
        Mix_PlayChannel(7,Assets->Alien,0);
        bala_mala(b2+i,(*evilPosition),&noBalas2,!direccion,Assets->BulletTwo.Tex,Assets->BulletTwo.Quads);
    }
}
void imprimirVillano(enemigo *e,SDL_Rect *playerPosition,SDL_Rect rectImage,int z,bool *isRunning,SDL_Renderer *renderTarget,game_state *State,int contshot, game_assets *Assets)
{
    for(int i=0;i<NUM_ENEMIGOS;i++)
    {
        if(e[i].die==0)
        {
            e[i].rectVillain.y=(4+2*(abs(e[i].villainPosition.x-playerPosition->x)>150)+(e[i].villainPosition.x<playerPosition->x))*e[i].rectVillain.h;
            evilBehavior(&e[i].rectVillain,&e[i].villainPosition,*playerPosition,rectImage,Assets,i);

            if(touch(*playerPosition,e[i].villainPosition,rectImage,Solution) && z==6 && !(!Img && contshot<7))
                {
                    State->Life++;
                    Mix_PlayChannel(7,Assets->Death,0);
                    if(State->faltas[1]!='9')
                        State->faltas[1]++;
                    else
                    {
                        State->faltas[0]++;
                        State->faltas[1]='0';
                        if(State->faltas[0]>'9')
                            State->faltas[0]='0';
                    }
                    playerPosition->y=0;
                    playerPosition->x=0;
                    *isRunning=false;
                    break;
                }
            SDL_RenderCopy(renderTarget,e[i].villain,&e[i].rectVillain,&e[i].villainPosition);
        }
        else if(e[i].banDie<24)
        {
            e[i].rectVillain.y=(8+(e[i].villainPosition.x<playerPosition->x))*e[i].rectVillain.h;
            e[i].die++;
            SDL_RenderCopy(renderTarget,e[i].villain,&e[i].rectVillain,&e[i].villainPosition);
        }
    }
}
void movimientosProtagonista(SDL_Rect *playerPosition,int velx,int vely,bool *salto,float *frameTime,float deltaTime,int *z,int *contshot,enemigo *e,bool active,Status *dir,bool *draw,SDL_Rect *rectImage,int direccion,int *shot)
{
     playerPosition->x+=(Img ? velx : velx*2);
    if(playerPosition->x<30)
        playerPosition->x=30;
    if(playerPosition->x >550)

        playerPosition->x=550;
    playerPosition->y+=(Img ? vely : 3*vely/2);
    *salto=(playerPosition->y>=375);
    if(*salto)
        playerPosition->y=375;

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

static void GameRun(SDL_Renderer *Render, game_state *State, game_assets *Assets, game_input *Input,bool restart)
{
    Solution.h=80;
    Solution.w=80;
    SDL_Rect qwerty;
    if(restart)
    {
        Fuego.trampaPosition.x=1200;
        for(int i=0;i<15;i++)
        {
            b[i].bulletView=false;
            b2[i].bulletView=false;
        }
        while(State->Cursor>0)
        {
            State->CMDText[State->Cursor - 1] = 0;
            State->Cursor--;
        }
        State->GameOn=false;
        State->faltas[0]='0';
        State->faltas[1]='0';
        State->kills=0;
        State->Life=0;
        State->Player.X=30;
        State->Player.Y=0;
        Assets->Map.Quads.x=0;
        Assets->Map.Tex=Assets->Map1.Tex;
        Assets->Clouds.Quads.x=0;
        Assets->Clouds.Tex=Assets->Clouds1.Tex;
        for(int i=0;i<NUM_ENEMIGOS;i++)
        {
            e[i].villain=Assets->Monster.Tex;
            e[i].villainPosition.x=100+(100+rand()%30)*i+30;
            e[i].villainPosition.w=100;
            e[i].villainPosition.h=100;
            e[i].rectVillain.x=e[i].rectVillain.y=0;
            e[i].rectVillain.w=Assets->Monster.Quads.w=40;
            e[i].rectVillain.h=Assets->Monster.Quads.h=40;
            e[i].villainPosition.y=375-e[i].rectVillain.h;
            e[i].die=0;
            e[i].banDie=0;
        }
        return;
    }
    Controller = GetGameController(Input, 0);
    SDL_Rect player = {0};
    player.x = State->Player.X;
    player.y = State->Player.Y;
    player.w = player.h = 60;
    if(Controller->Start.Down)
    {
        State->CurrentState = GAME_MENU;
    }

    if(Controller->MoveRight.Down)
    {
        active=true;
          if(State->Player.X<550)
            velX=11;
          else
          {
             if(Assets->Map.Quads.x<1412)
            {
                    if(State->premios[1]==1)
                        Assets->Llama.Quads.x-=(Img ? 11 : 22);
                   Assets->Map.Quads.x+=(Img ? 11 : 22);
                   Fuego.trampaPosition.x-=(Img ? 11 : 22);
                   Assets->Clouds.Quads.x+=(Img ? 11 : 22)/2;
                for(int i=0;i<15;i++)
                {
                    if(b[i].bulletView)
                    {
                        b[i].bulletPosition.x-=(Img ? 11 : 22);
                    }
                    if(b2[i].bulletView)
                    {
                        b2[i].bulletPosition.x-=(Img ? 11 : 22);
                    }
                }
                for(int i=0;i<NUM_ENEMIGOS;i++)
                {
                    e[i].villainPosition.x-=(Img ? 11 : 22);
                }
            }

          }
            dir=WALK;
            direccion=0;
            if(Controller->MoveUp.Down && salto)
            {
                jump=5;
                velY=-jumpSpeed;
                salto=false;
                z2=0;
                Mix_PlayChannel(3,Assets->Jump,0);
            }

        }
        else if(Controller->MoveLeft.Down)
        {
            active=true;
            if(State->Player.X>30)
                velX=-11;
            else
            {
                if(Assets->Map.Quads.x>0)
                {
                        if(State->premios[1]==1)
                            Assets->Llama.Quads.x+=(Img ? 11 : 22);
                       Assets->Map.Quads.x+=(Img ? -11 : -22);
                       Fuego.trampaPosition.x+=(Img ? 11 : 22);
                       Assets->Clouds.Quads.x+=(Img ? -11 : -22)/2;
                    for(int i=0;i<15;i++)
                    {
                        if(b[i].bulletView)
                        {
                            b[i].bulletPosition.x-=(Img ? -11 : -22);
                        }
                        if(b2[i].bulletView)
                        {
                            b2[i].bulletPosition.x-=(Img ? -11 : -22);
                        }
                    }
                    for(int i=0;i<NUM_ENEMIGOS;i++)
                         e[i].villainPosition.x-=(Img ? -11 : -22);
                }

            }
            dir=WALK;
            direccion=1;
            if(Controller->MoveUp.Down&& salto)
            {
                jump=5;
                velY=-jumpSpeed;
                salto=false;
                z2=0;
                Mix_PlayChannel(3,Assets->Jump,0);
            }
        }
        else
        {
            velX=0;
            active=false;
        }
        if(Controller->MoveDown.Down)
        {
            if(!Img)
            {
                if(!Controller->MoveRight.Down && !Controller->MoveLeft.Down)
                {
                    dir=DOWN;
                    active=true;
                    if(!Img)
                        State->kame++;
                }
            }
            else
            {
                dir=DOWN;
                active=true;
                if(!Img)
                    State->kame++;
            }

        }
        else if(Controller->MoveUp.Down && salto)
        {
            jump=2;
            velY=-jumpSpeed;
            salto=false;
            active=true;
            z2=0;
            Mix_PlayChannel(3,Assets->Jump,0);
        }
        draw=true;
        if(!salto)
        {
            velY+=gravity;
               z3=z2++/3;
               dir=JUMP;
        }
        else
        {
            velY=0;
            z2=0;
        }
        if(!Controller->MoveDown.Down && Controller->MoveDown.Count == 0)
        {
            dir = WALK;
            active=true;
            Mix_PlayChannel(4,Assets->ChargeEnd,0);
        }
        if(Controller->ActionUp.Count == 0)
        {
            if(!Controller->ActionUp.Down)
                shootUp = true;
            else
            {
                if(shootUp && contshot >= 7)
                {
                    shoot = 1;
                    contshot = 0;
                    if(dir != DOWN)
                    {
                        if(Img)
                        {
                            Mix_PlayChannel(0, Assets->Shoot, 0);
                            new_bala(b+noBalas, player, &noBalas, &shootUp, direccion, 1, Assets->BulletOne.Quads, Assets->BulletTwo.Tex, Assets->BulletOne.Tex, Assets->BulletOne.Quads);
                        }
                        else
                        {
                            Mix_PlayChannel(0, Assets->Punch, 0);
                            for(int i=0;i<NUM_ENEMIGOS;i++)
                            {
                                if(e[i].die==0 && touch(player,e[i].villainPosition,Current.Quads,Solution))
                                {
                                    e[i].die=1;
                                    State->kills++;
                                    State->Current.Score+=650-rand()%100;
                                    if(State->premios[1]==0)
                                    {
                                        rnd=rand()%40;
                                        if(rnd==17)
                                        {
                                            State->premios[1]=1;
                                            Assets->Llama.Quads.x=e[i].villainPosition.x;
                                            Assets->Llama.Quads.y=300;
                                        }
                                    }
                                    break;
                                }
                            }
                        }
                    }
                    else
                    {
                        if(!Img && State->kame>=50 && velY==0)
                            {
                                Mix_PlayChannel(6,Assets->Kame_Sound,0);
                                new_bala(b+noBalas, player, &noBalas, &shootUp, direccion, 0, Assets->BulletTwo.Quads, Assets->BulletTwo.Tex, Assets->BulletOne.Tex, Assets->BulletOne.Quads);
                                State->kame-=50;
                            }
                    }
                }
            }
        }
        movimientosProtagonista(&player,velX,velY,&salto,&frametime,State->dtInput,&z,&contshot,e,active,&dir,&draw,&Current.Quads,direccion,&shoot);
        State->Player.X = player.x;
        State->Player.Y = player.y;

        for(int i=0;i<NUM_ENEMIGOS;i++)
            e[i].rectVillain.y=2*e[i].rectVillain.h;

        SDL_RenderClear(Render);
        SDL_RenderCopy(Render,Assets->Clouds.Tex,&Assets->Clouds.Quads,NULL);
        SDL_RenderCopy(Render,Assets->Map.Tex,&Assets->Map.Quads,NULL);

        for(int i=0;i<10;i++)
        {
            if(b[i].bulletView)
            {
                if(Img)
                {
                    SDL_RenderCopy(Render,b[i].bullet,NULL,&b[i].bulletPosition);
                    b[i].bulletPosition.x+=16*b[i].bulletDireccion;
                    b[i].bulletCont++;
                    if(b[i].bulletCont>31)
                        b[i].bulletView=false;
                    else
                    {
                        if(b[i].bulletView)
                        {
                            for(int j=0;j<NUM_ENEMIGOS;j++)
                            {

                                if(!e[j].die)
                                 {
                                    e[j].die=touch(e[j].villainPosition,b[i].bulletPosition,Solution,b[i].rectBullet);
                                    if(e[j].die)
                                    {
                                        Mix_PlayChannel(2,Assets->Die,0);
                                        State->Current.Score+=600-abs((int)e[j].villainPosition.x - (int)State->Player.X);
                                        State->kills++;
                                        if(State->premios[1]==0)
                                        {
                                            rnd=rand()%40;
                                            if(rnd==17)
                                            {
                                                State->premios[1]=1;
                                                Assets->Llama.Quads.x=e[j].villainPosition.x;
                                                Assets->Llama.Quads.y=300;
                                            }
                                        }
                                        b[i].bulletView=false;
                                        break;
                                    }
                                 }
                            }
                        }
                    }
                }
                else
                {
                    Assets->Kamehame.Quads.x=(b[i].bulletCont%6)*Assets->Kamehame.Quads.w;
                    Assets->Kamehame.Quads.y=(b[i].bulletDireccion==1 ? 1 : 0)*Assets->Kamehame.Quads.h;
                    qwerty.x=b[i].bulletPosition.x;
                    qwerty.y=b[i].bulletPosition.y-15;
                    qwerty.w=Assets->Kamehame.Quads.w;
                    qwerty.h=Assets->Kamehame.Quads.h;
                    SDL_RenderCopy(Render,Assets->Kamehame.Tex,&(Assets->Kamehame.Quads),&qwerty);
                    b[i].bulletPosition.x+=16*b[i].bulletDireccion;
                    b[i].bulletCont++;
                    if(b[i].bulletCont>31)
                        b[i].bulletView=false;
                    else
                    {
                        if(b[i].bulletView)
                        {
                            for(int j=0;j<NUM_ENEMIGOS;j++)
                            {

                                if(!e[j].die)
                                 {
                                    e[j].die=touch(e[j].villainPosition,b[i].bulletPosition,Solution,Assets->Kamehame.Quads);
                                    if(e[j].die)
                                    {
                                        Mix_PlayChannel(2,Assets->Die,0);
                                        State->Current.Score+=600-abs((int)e[j].villainPosition.x - (int)State->Player.X);
                                        State->kills++;
                                        if(State->premios[1]==0)
                                        {
                                            rnd=rand()%40;
                                            if(rnd==17)
                                            {
                                                State->premios[1]=1;
                                                Assets->Llama.Quads.x=e[j].villainPosition.x;
                                                Assets->Llama.Quads.y=300;
                                            }
                                        }
                                        b[i].bulletView=false;
                                        break;
                                    }
                                 }
                            }
                        }
                    }
                }

            }

        }
        for(int i=0;i<15;i++)
        {
            if(b2[i].bulletView)
            {

                SDL_RenderCopy(Render,b2[i].bullet,NULL,&b2[i].bulletPosition);
                b2[i].bulletPosition.x+=10*b2[i].bulletDireccion;
                b2[i].bulletCont++;
                if(b2[i].bulletCont>61)
                    b2[i].bulletView=false;
                else
                {
                    if(b2[i].bulletView)
                    {
                        bulletImpact=(touch(player,b2[i].bulletPosition,player,b2[i].rectBullet)&& z==6 && !(!Img && contshot<7));
                        if(bulletImpact)
                        {
                            State->Life++;
                            Mix_PlayChannel(7,Assets->Death,0);
                            if(State->faltas[1]!='9')
                                State->faltas[1]++;
                            else
                            {
                                State->faltas[0]++;
                                State->faltas[1]='0';
                                if(State->faltas[0]>'9')
                                    State->faltas[0]='0';
                            }
                            player.x=0;
                            player.y=0;
                            b2[i].bulletView=false;
                            break;
                        }


                    }
                }
            }


        }
        imprimirVillano(e,&player,Current.Quads,z,&Running,Render,State,contshot,Assets);
        State->Player.Y=player.y;
        State->Player.X=player.x;
        Fuego.rectTrampa.x=(z/2)*Assets->Fire.Quads.w;

        qwerty.x=State->Player.X;
        qwerty.y=State->Player.Y;
        qwerty.w=32;
        qwerty.h=50;
        if(touch(qwerty,Fuego.trampaPosition,player,qwerty) && !(Img  && dir==DOWN))
        {

            State->Life++;
            Mix_PlayChannel(7,Assets->Burn,0);
            if(State->faltas[1]!='9')
                State->faltas[1]++;
            else
            {
                State->faltas[0]++;
                State->faltas[1]='0';
                if(State->faltas[0]>'9')
                    State->faltas[0]='0';
            }
            State->Player.X=0;
            State->Player.Y=0;
        }
        SDL_RenderCopy(Render,Current.Tex,&Current.Quads,&player);
         SDL_RenderCopy(Render,Fuego.trampaTex,&Fuego.rectTrampa,&Fuego.trampaPosition);

        SDL_Color TextColor = {0x00, 0x00, 0x00};
        SDL_Surface *TextSurface = TTF_RenderText_Blended(Font2, faltas, TextColor);
        SDL_Texture *TextTexture = SDL_CreateTextureFromSurface(Render, TextSurface);
        SDL_Rect Rect = {10, 20, 170, 32};
        SDL_RenderCopy(Render, TextTexture, 0, &Rect);
        SDL_FreeSurface(TextSurface);
        SDL_DestroyTexture(TextTexture);

        SDL_Surface *TextSurface2 = TTF_RenderText_Blended(Font2, State->faltas, TextColor);
        SDL_Texture *TextTexture2 = SDL_CreateTextureFromSurface(Render, TextSurface2);
        SDL_Rect Rect2 = {170, 20, 30, 32};
        SDL_RenderCopy(Render, TextTexture2, 0, &Rect2);
        SDL_FreeSurface(TextSurface2);
        SDL_DestroyTexture(TextTexture2);

        SDL_Surface *TextSurface3 = TTF_RenderText_Blended(Font2, score, TextColor);
        SDL_Texture *TextTexture3 = SDL_CreateTextureFromSurface(Render, TextSurface3);
        SDL_Rect Rect3 = {260, 20, 120, 32};
        SDL_RenderCopy(Render, TextTexture3, 0, &Rect3);
        SDL_FreeSurface(TextSurface3);
        SDL_DestroyTexture(TextTexture3);

        temporal=State->Current.Score;
        cantSco[4]=temporal%10+'0';
        temporal/=10;
        cantSco[3]=temporal%10+'0';
        temporal/=10;
        cantSco[2]=temporal%10+'0';
        temporal/=10;
        cantSco[1]=temporal%10+'0';
        temporal/=10;
        cantSco[0]=temporal%10+'0';

        SDL_Surface *TextSurface4 = TTF_RenderText_Blended(Font2, cantSco, TextColor);
        SDL_Texture *TextTexture4 = SDL_CreateTextureFromSurface(Render, TextSurface4);
        SDL_Rect Rect4 = {380, 20, 75, 32};
        SDL_RenderCopy(Render, TextTexture4, 0, &Rect4);
        SDL_FreeSurface(TextSurface4);
        SDL_DestroyTexture(TextTexture4);

        if(!Img)
        {
            SDL_RenderCopy(Render, Assets->Pila.Tex, 0, &Assets->Pila.Quads);
            Assets->Energia.Quads.w =2*(State->kame<=50 ? State->kame : 50);
            SDL_RenderCopy(Render,Assets->Energia.Tex,0,&Assets->Energia.Quads);
        }
        if(State->kills==NUM_ENEMIGOS)
        {
            SDL_RenderCopy(Render,Assets->Go.Tex,0,&(Assets->Go.Quads));
        }
        if(State->Life>10)
        {
            State->CurrentState=GAME_END;
            State->GameOn=false;
            OrderRecords(State);
            SDL_strlcpy(State->Current.Name, "", 32);
        }
        if(State->kills==NUM_ENEMIGOS && State->premios[0]==0 && Assets->Map.Quads.x>=1200)
        {
             State->premios[0]=1;
             Assets->Gallo.Quads.x=200;
             Assets->Gallo.Quads.y=200;
             Mix_PlayChannel(7,Assets->Coin,0);
        }
        if(State->premios[0]==1 && Assets->Map.Quads.x==0)
        {
            SDL_RenderCopy(Render,Assets->Gallo.Tex,NULL,&Assets->Gallo.Quads);
            if(touch(qwerty,Assets->Gallo.Quads,player,Assets->Gallo.Quads))
            {
                State->premios[0]=2;
                State->Current.Score+=1982;
                Mix_PlayChannel(7,Assets->Sing,0);
            }
        }
        if(State->premios[1]==1)
        {
            SDL_RenderCopy(Render,Assets->Llama.Tex,NULL,&Assets->Llama.Quads);
            if(touch(qwerty,Assets->Llama.Quads,player,Assets->Llama.Quads))
            {
                State->premios[1]=0;
                if(State->Life>0)
                {
                    Mix_PlayChannel(7,Assets->OneUp,0);
                    State->Life--;
                    if(State->faltas[1]!='0')
                        State->faltas[1]--;
                    else
                    {
                        State->faltas[0]--;
                        State->faltas[1]='9';
                        if(State->faltas[0]<'0')
                            State->faltas[0]='0';
                    }
                }
            }
        }
        if(Assets->Map.Quads.x>=1419 && State->Player.X==550 && State->kills==NUM_ENEMIGOS)
        {
            State->CurrentState=GAME_PLAY2;
        }
}

#define _GAME_RUN_H_
#endif
