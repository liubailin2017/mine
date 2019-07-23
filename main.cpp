#include<stdio.h>
#include<stdlib.h>
#include<SDL.h>
#include<SDL_image.h>
#define PosX(w,n) ((n-1)%w + 1)
#define PosY(w,n) ((n-1)/w +1)
#define Nmbxy(w,x,y) ((y-1)*(w) +x)

#if defined (__WIN32)
#include<windows.h>
#endif

void LoadImage();
int MineNmb(int x, int y);
int CanSert(int x,int y); 
void InitMine(int NmbMine,int Firstx,int Firsty);
void Destroy();
void Darw();
void DarwMine();
int runl(int x,int y);
int runr(int x,int y);
void Start();
void showmsg(int m);

int IsFirstClick = 1;
int quit = 0;
int  msizew= 10;
int msizeh = 10;
SDL_Rect farme={0,10};
SDL_Window *MainWindow = NULL;
SDL_Surface *MainWinSurface = NULL;
SDL_Surface *IMGNo;
SDL_Surface *IMGWhite;
SDL_Surface *IMGFlag;
SDL_Surface *IMGFlagc;
SDL_Surface *IMGUnKnew;
SDL_Surface *IMGMine;
SDL_Surface *IMGMiner;
SDL_Surface *IMG1;
SDL_Surface *IMG2;
SDL_Surface *IMG3;

typedef char BIT;

BIT *IsMine = NULL;
BIT *IsDo = NULL;
BIT *IsFlag = NULL;

int main(int argc ,char *argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    LoadImage();
    printf("\n\n\n\n");
    printf("input:");
 
    farme.w = 30 * msizew,farme.h=30*msizeh;
 
    MainWindow = SDL_CreateWindow("My qq:2570667204",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          farme.w+farme.x,farme.h+farme.y,
                                          0);

    MainWinSurface = SDL_GetWindowSurface(MainWindow);
 
    InitMine(msizew*msizeh/8,0,0);
    Darw();
    while(!quit)
    {
        InitMine(msizew*msizeh/8,0,0);
        Start();
    }

    Destroy();

    return 0;
}

void LoadImage()
{
     IMGNo = IMG_Load("image/no.png");
     if(IMGFlag == NULL) printf("not find no.png,");
     IMGWhite = IMG_Load("image/white.png");
     if(IMGWhite == NULL) printf("white.png");
     IMGFlag = IMG_Load("image/flag.png");
     if(IMGFlag == NULL) printf("flag.png");
     IMGFlagc = IMG_Load("image/flagc.png");
     if(IMGFlagc == NULL) printf("flagc.png");
     IMGUnKnew = IMG_Load("image/Unknew.png");
     if(IMGUnKnew == NULL)printf("Unknew.png");
     IMGMine = IMG_Load("image/mine.png");
     if(IMGMine == NULL)printf("mine.png");
     IMGMiner = IMG_Load("image/miner.png");
     if(IMGMiner == NULL)printf("miner.png");
     IMG1 = IMG_Load("image/1.png");
     if(IMG1 == NULL)printf("1.png");
     IMG2 = IMG_Load("image/2.png");
     if(IMG2 == NULL)printf("2.png");
     IMG3 = IMG_Load("image/3.png");
     if(IMG3 == NULL)printf("3.png");
     printf("\n");
}

int MineNmb(int x, int y) 
{
    int sum = 0; 
    if(x+1 <= msizew && IsMine[Nmbxy(msizew,x+1,y)] == 1) sum ++;
    if(1 <= x-1 && IsMine[Nmbxy(msizew,x-1,y)] == 1) sum ++;
    if(y-1 >= 1 && IsMine[Nmbxy(msizew,x,y-1)] == 1) sum ++;
    if(y+1 <= msizeh && IsMine[Nmbxy(msizew,x,y+1)] == 1) sum ++;
    if(y-1 >= 1 && x-1 >= 1 && IsMine[Nmbxy(msizew,x-1,y-1)] == 1) sum ++;
    if(msizew >= x+1 && y-1 >= 1 && IsMine[Nmbxy(msizew,x+1,y-1)] == 1) sum ++;
    if(x-1 >= 1 && y+1 <= msizeh && IsMine[Nmbxy(msizew,x-1,y+1)] == 1) sum ++;
    if(x+1 <= msizew && y+1 <= msizeh && IsMine[Nmbxy(msizew,x+1,y+1)] == 1) sum ++;
    return sum;
}

int CanSert(int x,int y) 
{ 
    if(msizew >= x+1 && MineNmb(x+1,y) >= 3) return 0;
    if(1 <= x-1 && MineNmb(x-1,y) >= 3) return 0;
    if(y-1 >= 1 && MineNmb(x,y-1) >= 3) return 0;
    if(y+1 <= msizeh && MineNmb(x,y+1) >= 3) return 0;
    if(y-1 >= 1 && x-1 >= 1 && MineNmb(x-1,y-1) >= 3) return 0;
    if(msizew >= x+1 && y-1 >= 1 && MineNmb(x+1,y-1) >= 3) return 0;
    if(x-1 >= 1 && y+1 <= msizeh && MineNmb(x-1,y+1) >= 3) return 0;
    if(x+1 <= msizew && y+1 <= msizeh && MineNmb(x+1,y+1) >= 3) return 0;
 return 1;
}

void InitMine(int NmbMine,int Firstx,int Firsty)
{
    if(!IsMine) IsMine =(char *)malloc((msizew) * (msizeh)+1);
    if(!IsDo) IsDo =(char *)malloc((msizew) * (msizeh)+1);
    if(!IsFlag) IsFlag =(char *)malloc((msizew) * (msizeh+1));
    int i = 0,j = 0;
   
    for(i = 1; i <= msizeh; i++)
        for(j = 0; j <= msizew; j++)
        {
            IsMine[Nmbxy(msizew,j,i)] = 0;
            IsDo[Nmbxy(msizew,j,i)] = 0;
            IsFlag[Nmbxy(msizew,j,i)] = 0;
        }
    int t =0;
        while(NmbMine)
        {
             if(!(rand()%(msizew*msizeh)/NmbMine)
                && CanSert(PosX(msizew,t),PosY(msizew,t))
                && !IsMine[t]
                && !(PosX(msizew,t)==Firstx && PosY(msizew,t)==Firsty))
            {
              IsMine[t] = 1;
              NmbMine--;
            }
        t = (t + 1) % (msizew *msizeh + 1);
        }
        printf("InitMine.outmap:\n");
        for(i = 0; i <= msizeh; i++)
        {
            for(j = 0; j <= msizew; j++)
            {
                if(j == 0 )
                    printf("%d ",i%10);
                else if(i == 0)
                    printf("%d ",j%10);
                else
                {
                    printf("%d ",IsMine[(i-1)*msizew +j]);
                }
            }
        printf("\n");
        }
}

void Destroy()
{
    free(IsMine);
    free(IsDo);
    free(IsFlag);
    SDL_FreeSurface(IMGNo);
    SDL_FreeSurface(IMGWhite);
    SDL_FreeSurface(IMGFlag);
    SDL_FreeSurface(IMGFlagc);
    SDL_FreeSurface(IMGUnKnew);
    SDL_FreeSurface(IMGMine);
    SDL_FreeSurface(IMGMiner);
    SDL_FreeSurface(IMG1);
    SDL_FreeSurface(IMG2);
    SDL_FreeSurface(IMG3);
}

void Darw()
{
    SDL_Rect pos= {0,0,30,30};
     
    int i,j;
    for(i = 0; i<msizeh ;i++) {
        for(j = 0; j < msizew; j++)
        {
            pos.x = j * 30 + farme.x;
            pos.y = i * 30 + farme.y;
            if(IsDo[Nmbxy(msizew,j+1,i+1)] == 1)
            {
                SDL_BlitSurface(IMGWhite,NULL,MainWinSurface,&pos);
                switch(MineNmb(j+1,i+1))
                {
                    case 1 :
                            SDL_BlitSurface(IMG1,NULL,MainWinSurface,&pos);
                            break;
                    case 2 :
                            SDL_BlitSurface(IMG2,NULL,MainWinSurface,&pos);
                            break;
                    case 3 :
                            SDL_BlitSurface(IMG3,NULL,MainWinSurface,&pos);
                            break;
                }
            }else if(IsFlag[Nmbxy(msizew,j+1,i+1)]== 1)
                    SDL_BlitSurface(IMGFlag,NULL,MainWinSurface,&pos);
            else if(IsFlag[Nmbxy(msizew,j+1,i+1)] == 2)
                    SDL_BlitSurface(IMGUnKnew,NULL,MainWinSurface,&pos);
            else
                    SDL_BlitSurface(IMGNo,NULL,MainWinSurface,&pos);
        }
    }
    SDL_UpdateWindowSurface(MainWindow);
}

void DarwMine()
{
        SDL_Rect pos= {0,0,30,30};
      
        int i,j;
        for(i = 0; i<msizeh ;i++)
            for(j = 0; j < msizew; j++)
        {
            pos.x = j * 30 + farme.x;
            pos.y = i * 30 + farme.y;

            if(IsFlag[Nmbxy(msizew,j+1,i+1)]==1 && IsMine[Nmbxy(msizew,j+1,i+1)] == 1 )
                SDL_BlitSurface(IMGMiner,NULL,MainWinSurface,&pos);
            else if(IsFlag[Nmbxy(msizew,j+1,i+1)]==1 && IsMine[Nmbxy(msizew,j+1,i+1)] != 1 )
                SDL_BlitSurface(IMGFlagc,NULL,MainWinSurface,&pos);
            else if(IsMine[Nmbxy(msizew,j+1,i+1)] == 1)
                SDL_BlitSurface(IMGMine,NULL,MainWinSurface,&pos);
        }
        SDL_UpdateWindowSurface(MainWindow);
}

int runl(int x,int y)
{
  
    if(IsFirstClick)
    {
        InitMine(msizew*msizeh/8,x,y);
        IsFirstClick = 0;
    }

    if(IsMine[Nmbxy(msizew,x,y)])
    {
        return 1;
    }
    else
    {
        if(!IsDo[Nmbxy(msizew,x,y)] && MineNmb(x,y) == 0)
        {
            IsDo[Nmbxy(msizew,x,y)] = 1;
            IsFlag[Nmbxy(msizew,x,y)]=0;

            if(msizew >= x+1 ) runl(x+1,y);
            if(1 <= x-1 ) runl(x-1,y); 
            if(y-1 >= 1 ) runl(x,y-1);
            if(y+1 <= msizeh ) runl(x,y+1);
            if(y-1 >= 1 && x-1 >= 1 ) runl(x-1,y-1);
            if(msizew >= x+1 && y-1 >= 1 ) runl(x+1,y-1);
            if(x-1 >= 1 && y+1 <= msizeh ) runl(x-1,y+1);
            if(x+1 <= msizew && y+1 <= msizeh ) runl(x+1,y+1);
        }
        else
        {
            IsDo[Nmbxy(msizew,x,y)] = 1;
            IsFlag[Nmbxy(msizew,x,y)]=0;
        }
    return 0;
    }
}

int runr(int x,int y)
{ 
    int i,j;
    int iswin = 1;
    if(IsDo[Nmbxy(msizew,x,y)]==0)
    {
        IsFlag[Nmbxy(msizew,x,y)] ++;
        IsFlag[Nmbxy(msizew,x,y)] = IsFlag[Nmbxy(msizew,x,y)] %3;
    }
    printf("runr.msg:\n");
    for(i = 0; i<msizeh ;i++){
        for(j = 0; j < msizew; j++)
        {
            printf("%d,%d ",IsFlag[Nmbxy(msizew,j+1,i+1)] ,IsMine[Nmbxy(msizew,j+1,i+1)]);
            if(IsFlag[Nmbxy(msizew,j+1,i+1)] != IsMine[Nmbxy(msizew,j+1,i+1)]) iswin = 0;
        }
        printf("\n");
    }
    return iswin;
}

int WinGetch()
{
    SDL_Event event;
    int exit = 0;
    while(!exit)
    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_MOUSEBUTTONDOWN)exit = 1 ;
        if(event.type == SDL_QUIT) {exit = 1,quit = 1;}
    }
return 1;
}

void Start()
{
    int gameover = 0;
    SDL_Event event;
    while(!gameover && !quit)
    {
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_MOUSEBUTTONDOWN :
                if(event.button.button == SDL_BUTTON_LEFT)
                {
                    if(runl((event.button.x-farme.x)/30+1
                            ,(event.button.y-farme.y)/30+1))
                    {
                        DarwMine();
                        gameover = 1;
                        IsFirstClick = 1;
                        showmsg(1);
                    }
                    else
                        Darw();
                }
                else if(event.button.button == SDL_BUTTON_RIGHT)
                {
                    if(runr((event.button.x-farme.x)/30+1,(event.button.y-farme.y)/30+1) == 1)
                    {

                        DarwMine();
                        gameover = 1;
                        IsFirstClick = 1;
                        showmsg(0);
                    }
                    Darw();
                }
                break;

                case SDL_QUIT :
                quit = 1;
                break;
            }
        }
    }
}

void showmsg(int m)
{
    if(m == 0)
    {
#if defined (__WIN32)
        MessageBox(0,"Win ","note",0);
#endif
    }
    else if(m ==1)
    {
#if defined (__WIN32)
          MessageBox(0,"failure","note",0);
#endif
    }
}
