#include<stdio.h>
#include<stdlib.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
//把数组下标转换成对应的坐标
#define PosX(w,n) ((n-1)%w + 1)
#define PosY(w,n) ((n-1)/w +1)
//把坐标转换成对应的下标
#define Nmbxy(w,x,y) ((y-1)*(w) +x)

#if defined (__WIN32)
#include<windows.h>
#endif

void LoadImage();
int MineNmb(int x, int y); //判断周围有多个雷
int CanSert(int x,int y); //判断是否可以埋雷 保证周围最多只能出现3个雷
void InitMine(int NmbMine,int Firstx,int Firsty);//初始化并埋雷
void Destroy();//退出程序清理工作
void Darw();//刷新
void DarwMine();//点到雷是刷新
int runl(int x,int y); //左键
int runr(int x,int y);//右键
void Start(); //开始游戏
void showmsg(int m);

int IsFirstClick = 1;
int quit = 0;
unsigned int  msize= 0x00100010; //int 型 0x10 X 0x10矩阵
SDL_Rect farme={0,10};//主要位置
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
    printf("游戏设置--矩阵大小\n");
    printf("输入一个16进制整形数(如000A000A)\n前4位表示宽度后四位表示高度:");
    scanf("%x",&msize);
    farme.w = 30 * (msize>>16),farme.h=30*(msize & 0x0000FFFF);

    if(farme.w/30< 5 || farme.w/30 > 30 || farme.h/30 <5 || farme.h/30 > 30)
    {
        printf( "无效输入,将使用默认值初始化。\n");
        msize = 0x00100010;
        farme.w = 30 * (msize>>16),farme.h=30*(msize & 0x0000ffff);
    }

    printf("创建%d X %d的矩阵\n",farme.w/30 ,farme.h/30);
    MainWindow = SDL_CreateWindow("My qq:2570667204",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          farme.w+farme.x,farme.h+farme.y,
                                          0);

    MainWinSurface = SDL_GetWindowSurface(MainWindow);

    int w = msize>>16,h = msize - (msize>>16<<16);

    InitMine(w*h/8,0,0);
    Darw();
    while(!quit)
    {
        InitMine(w*h/8,0,0);
        Start();
    }

    Destroy();

    return 0;
}

void LoadImage()
{
     IMGNo = IMG_Load("image/no.png");
     if(IMGFlag == NULL) printf("no.png丢失\n");
     IMGWhite = IMG_Load("image/white.png");
     if(IMGWhite == NULL) printf("white.png丢失\n");
     IMGFlag = IMG_Load("image/flag.png");
     if(IMGFlag == NULL) printf("flag.png丢失\n");
     IMGFlagc = IMG_Load("image/flagc.png");
     if(IMGFlagc == NULL) printf("flagc.png丢失\n");
     IMGUnKnew = IMG_Load("image/Unknew.png");
     if(IMGUnKnew == NULL)printf("Unknew.png丢失\n");
     IMGMine = IMG_Load("image/mine.png");
     if(IMGMine == NULL)printf("mine.png丢失\n");
     IMGMiner = IMG_Load("image/miner.png");
     if(IMGMiner == NULL)printf("miner.png丢失\n");
     IMG1 = IMG_Load("image/1.png");
     if(IMG1 == NULL)printf("1.png丢失\n");
     IMG2 = IMG_Load("image/2.png");
     if(IMG2 == NULL)printf("2.png丢失\n");
     IMG3 = IMG_Load("image/3.png");
     if(IMG3 == NULL)printf("3.png丢失\n");


}

int MineNmb(int x, int y) //判断周围有多个雷
{
    int sum = 0;
    int w = msize>>16,h = msize - (msize>>16<<16);
    if(x+1 <= w && IsMine[Nmbxy(w,x+1,y)] == 1) sum ++;//右
    if(1 <= x-1 && IsMine[Nmbxy(w,x-1,y)] == 1) sum ++; // 左
    if(y-1 >= 1 && IsMine[Nmbxy(w,x,y-1)] == 1) sum ++;//上
    if(y+1 <= h && IsMine[Nmbxy(w,x,y+1)] == 1) sum ++;//下
    if(y-1 >= 1 && x-1 >= 1 && IsMine[Nmbxy(w,x-1,y-1)] == 1) sum ++; //左上
    if(w >= x+1 && y-1 >= 1 && IsMine[Nmbxy(w,x+1,y-1)] == 1) sum ++;//右上
    if(x-1 >= 1 && y+1 <= h && IsMine[Nmbxy(w,x-1,y+1)] == 1) sum ++;//左下
    if(x+1 <= w && y+1 <= h && IsMine[Nmbxy(w,x+1,y+1)] == 1) sum ++; //右下
    return sum;
}

int CanSert(int x,int y) //判断是否可以埋雷 保证周围最多只能出现3个雷
{
    int w = msize>>16,h = msize - (msize>>16<<16);
    if(w >= x+1 && MineNmb(x+1,y) >= 3) return 0;//右
    if(1 <= x-1 && MineNmb(x-1,y) >= 3) return 0; // 左
    if(y-1 >= 1 && MineNmb(x,y-1) >= 3) return 0;//上
    if(y+1 <= h && MineNmb(x,y+1) >= 3) return 0;//下
    if(y-1 >= 1 && x-1 >= 1 && MineNmb(x-1,y-1) >= 3) return 0;//左上
    if(w >= x+1 && y-1 >= 1 && MineNmb(x+1,y-1) >= 3) return 0;//右上
    if(x-1 >= 1 && y+1 <= h && MineNmb(x-1,y+1) >= 3) return 0;//左下
    if(x+1 <= w && y+1 <= h && MineNmb(x+1,y+1) >= 3) return 0; //右下

 return 1;
}

void InitMine(int NmbMine,int Firstx,int Firsty)//初始化并埋雷
{
    /*如果没有分配空间就分配空间*/
    if(!IsMine) IsMine =(char *)malloc((msize>>16) * (msize - (msize>>16<<16))+1);
    if(!IsDo) IsDo =(char *)malloc((msize>>16) * (msize - (msize>>16<<16))+1);
    if(!IsFlag) IsFlag =(char *)malloc((msize>>16) * (msize - (msize>>16<<16))+1);
    int i = 0,j = 0;
    int w = msize>>16,h = msize - (msize>>16<<16);//矩阵宽和高
    for(i = 1; i <= h; i++)
        for(j = 0; j <= w; j++)
        {
            IsMine[Nmbxy(w,j,i)] = 0;
            IsDo[Nmbxy(w,j,i)] = 0;
            IsFlag[Nmbxy(w,j,i)] = 0;
        }
    int t =0;
        while(NmbMine)
        {
             if(!(rand()%(w*h)/NmbMine)
                && CanSert(PosX(w,t),PosY(w,t))
                && !IsMine[t]
                && !(PosX(w,t)==Firstx && PosY(w,t)==Firsty))
            {
              IsMine[t] = 1;
              NmbMine--;
            }
        t = (t + 1) % (w * h + 1);
        }
        printf("InitMine.outmap:\n");
        for(i = 0; i <= h; i++)
        {
            for(j = 0; j <= w; j++)
            {
                if(j == 0 )
                    printf("%d ",i%10);
                else if(i == 0)
                    printf("%d ",j%10);
                else
                {
                    printf("%d ",IsMine[(i-1)*w +j]);
                }
            }
        printf("\n");
        }
}

void Destroy()//退出程序清理工作
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

void Darw()//刷新
{
    SDL_Rect pos= {0,0,30,30};
    int w = msize>>16,h = msize - (msize>>16<<16);
    int i,j;
    for(i = 0; i<h ;i++)
        for(j = 0; j < w; j++)
        {
            pos.x = j * 30 + farme.x;
            pos.y = i * 30 + farme.y;
            if(IsDo[Nmbxy(w,j+1,i+1)] == 1)
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
            }

            else
                if(IsFlag[Nmbxy(w,j+1,i+1)]== 1)
                        SDL_BlitSurface(IMGFlag,NULL,MainWinSurface,&pos);
                else if(IsFlag[Nmbxy(w,j+1,i+1)] == 2)
                        SDL_BlitSurface(IMGUnKnew,NULL,MainWinSurface,&pos);
                else
                        SDL_BlitSurface(IMGNo,NULL,MainWinSurface,&pos);
        }
        SDL_UpdateWindowSurface(MainWindow);
}

void DarwMine()//点到雷是刷新
{
        SDL_Rect pos= {0,0,30,30};
        int w = msize>>16,h = msize - (msize>>16<<16);
        int i,j;
        for(i = 0; i<h ;i++)
            for(j = 0; j < w; j++)
        {
            pos.x = j * 30 + farme.x;
            pos.y = i * 30 + farme.y;

            if(IsFlag[Nmbxy(w,j+1,i+1)]==1 && IsMine[Nmbxy(w,j+1,i+1)] == 1 )
                SDL_BlitSurface(IMGMiner,NULL,MainWinSurface,&pos);
            else if(IsFlag[Nmbxy(w,j+1,i+1)]==1 && IsMine[Nmbxy(w,j+1,i+1)] != 1 )
                SDL_BlitSurface(IMGFlagc,NULL,MainWinSurface,&pos);
            else if(IsMine[Nmbxy(w,j+1,i+1)] == 1)
                SDL_BlitSurface(IMGMine,NULL,MainWinSurface,&pos);
        }
        SDL_UpdateWindowSurface(MainWindow);
}

int runl(int x,int y) //左键
{
    int w = msize>>16,h = msize - (msize>>16<<16);

    if(IsFirstClick)
    {
        InitMine(w*h/8,x,y);
        IsFirstClick = 0;
    }

    if(IsMine[Nmbxy(w,x,y)])
    {
        return 1;
    }
    else
    {
        if(!IsDo[Nmbxy(w,x,y)] && MineNmb(x,y) == 0)
        {
            IsDo[Nmbxy(w,x,y)] = 1;
            IsFlag[Nmbxy(w,x,y)]=0;

            if(w >= x+1 ) runl(x+1,y);//右
            if(1 <= x-1 ) runl(x-1,y); // 左
            if(y-1 >= 1 ) runl(x,y-1);//上
            if(y+1 <= h ) runl(x,y+1);//下
            if(y-1 >= 1 && x-1 >= 1 ) runl(x-1,y-1);//左上
            if(w >= x+1 && y-1 >= 1 ) runl(x+1,y-1);//右上
            if(x-1 >= 1 && y+1 <= h ) runl(x-1,y+1);//左下
            if(x+1 <= w && y+1 <= h ) runl(x+1,y+1); //右下
        }
        else
        {
            IsDo[Nmbxy(w,x,y)] = 1;
            IsFlag[Nmbxy(w,x,y)]=0;
        }
    return 0;
    }
}

int runr(int x,int y)//右键
{
    int w = msize>>16,h = msize - (msize>>16<<16);
    int i,j;
    int iswin = 1;
    if(IsDo[Nmbxy(w,x,y)]==0)
    {
        IsFlag[Nmbxy(w,x,y)] ++;
        IsFlag[Nmbxy(w,x,y)] = IsFlag[Nmbxy(w,x,y)] %3;
    }
    printf("runr.msg:\n");
    for(i = 0; i<h ;i++){
        for(j = 0; j < w; j++)
        {
            printf("%d,%d ",IsFlag[Nmbxy(w,j+1,i+1)] ,IsMine[Nmbxy(w,j+1,i+1)]);
            if(IsFlag[Nmbxy(w,j+1,i+1)] != IsMine[Nmbxy(w,j+1,i+1)]) iswin = 0;
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

void Start() //开始游戏
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
        MessageBox(0,"Win ","恭喜",0);
#endif
    }
    else if(m ==1)
    {
#if defined (__WIN32)
          MessageBox(0,"踩到雷了 ","失败",0);
#endif
    }
}
