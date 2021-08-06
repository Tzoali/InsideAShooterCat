#if !defined(_GAME_H_)

TTF_Font *Font;
TTF_Font *Font2;
enum Status {WALK,DOWN,JUMP,NOTHING};
enum EvilStatus{NOTHING2,JUMP2,ATTACK,SHOOT};

int Img = 0;

union vector2
{
    struct
    {
        float X;
        float Y;
    };
    float E[2];
};

struct game_button
{
    bool32 Down;
    int32 Count;
};

struct game_controller
{
    union
    {
        game_button Buttons[10];
        struct
        {
            game_button MoveUp;
            game_button MoveLeft;
            game_button MoveRight;
            game_button MoveDown;

            game_button ActionDown;
            game_button ActionUp;
            game_button ActionRight;
            game_button ActionLeft;

            game_button Start;
        };
    };
};

struct game_input
{
    struct
    {
        int MouseX;
        int MouseY;
        union
        {
            game_button Mouse[2];
            struct
            {
                game_button MouseLeft;
                game_button MouseRight;
            };
        };
    };

    game_controller Controller[2];
};

inline game_controller *
GetGameController(game_input *Input, int Index)
{
    game_controller *Result = &Input->Controller[Index];
    return(Result);
}


enum game_exec_state {GAME_MENU, GAME_PLAY, GAME_FRONT, GAME_RECORDS,GAME_END,GAME_WIN,GAME_INSTRUCTIONS,GAME_PLAY2,GAME_MENU2,GAME_RECORDS2,GAME_INSTRUCTIONS2,GAME_PLAY3,GAME_MENU3,GAME_RECORDS3,GAME_INSTRUCTIONS3};

struct record
{
    char Name[32];
    int Score;
};

struct game_state
{
    char CMDText[256];
    int32 Cursor;
    vector2 Player;
    game_exec_state CurrentState;
    bool32 GameOn;
    char faltas[5];
    int kills;
    int kame;
    int premios[5];

    record Records[10];
    record Current;
    record WinScore;

    int Life;
    float dtInput;
};

void OrderRecords(game_state *State)
{
    record Temp = {0};

    int I = 0;
    for(I = 0; I < 10; ++I)
    {
        if(State->Current.Score >= State->Records[I].Score)
        {
            break;
        }
    }

    if(I < 10)
    {
        for(int Index = I; Index < 10; ++Index)
        {
            Temp = State->Records[Index];
            State->Records[Index] = State->Current;
            State->Current = Temp;
        }
    }


}

#define _GAME_H_
#endif
