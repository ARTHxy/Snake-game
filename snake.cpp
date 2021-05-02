#include <iostream>
#include <Windows.h>
#include <vector>
#include <conio.h>
using namespace std;

enum dir
{
    Up,
    Down,
    Left,
    Right,
    Awaiting
};

int main()
{
    int dir = Awaiting;
    //MAP
    const int h = 20; // HEIGHT
    const int w = 30; // WIDTH
    //MAP - WHEN CHANGING DIMENSIONS OF MAP CONSIDER CHANGING WINDOW SIZE IN SetWindowPos(); (25 LINE)
    CHAR_INFO map[h][w];
    SetWindowTextA(GetConsoleWindow(), "Snake");
    SetWindowPos(GetConsoleWindow(), HWND_TOP, 780, 300, 273, 319, SWP_SHOWWINDOW);
    COORD endCorner; endCorner.X = w; endCorner.Y = h;
    COORD beginCorner; beginCorner.X = 0; beginCorner.Y = 0;
    SMALL_RECT wtf; wtf.Bottom = h; wtf.Top = 0; wtf.Left = 0; wtf.Right = w;
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            if (y == 0 || y == h-1 || x == 0 || x == w-1) map[y][x].Attributes = BACKGROUND_INTENSITY | FOREGROUND_INTENSITY;
            else map[y][x].Attributes = 0x0000 | 0x0000;
            map[y][x].Char.UnicodeChar = ' ';
        }
    }
    srand(time(NULL));
    vector<COORD> snake(1);
    snake[0].Y = 0; snake[0].X = 0;
    //RANDOM SPAWN FOR SNAKE
    while (snake[0].Y == 0 || snake[0].X == 0)
    {
        snake[0].Y = (rand() % (h - 2)) + 1;
        snake[0].X = (rand() % (w - 2)) + 1;
    }

    system("cls");
    HANDLE doorhandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO CurInfo; CurInfo.bVisible = false; CurInfo.dwSize = 100;
    SetConsoleCursorInfo(doorhandle, &CurInfo);
    char input;
    COORD food;
    food.Y = 0; food.X = 0;
    COORD tailbuffer; bool newtail = false;
    bool play = true;
    while (play)
    {
        if (snake.size() > 1)
        {
            for (int i = snake.size() - 1; i > 0; i--)
            {
                snake[i] = snake[i - 1];
            }
        }
        if (newtail)
        {
            snake.push_back(tailbuffer);
            newtail = false;
        }
        WriteConsoleOutput(
            doorhandle,
            (const CHAR_INFO*)map,
            endCorner,
            beginCorner,
            &wtf
        );
        while ( _kbhit() )
        {
            input = _getch();
            if ((input == 68 || input == 100) && dir != Left)//RIGHT
            {
                dir = Right;
                break;
            }
            else if ((input == 65 || input == 97) && dir != Right)//LEFT
            {
                dir = Left;
                break;
            }
            else if ((input == 119 || input == 87) && dir != Down)//UP
            {
                dir = Up;
                break;
            }
            else if ((input == 83 || input == 115) && dir != Up)//DOWN
            {
                dir = Down;
                break;
            }
        //CONTINUOUS MOVEMENT AND BORDER DETECTION
        }
        if (dir == Up)
        {
            snake[0].Y -= 1;
            if (snake[0].Y < 1) break;
        }
        else if (dir == Down)
        {
            snake[0].Y += 1;
            if (snake[0].Y >= h - 1) break;
        }
        else if (dir == Left)
        {
            snake[0].X -= 1;
            if (snake[0].X < 1) break;
        }
        else if (dir == Right)
        {
            snake[0].X += 1;
            if (snake[0].X >= w - 1) break;
        }
        //FOOD
        while (food.Y == 0 || food.X == 0)
        {
            food.Y = (rand() % (h - 2)) + 1;
            food.X = (rand() % (w - 2)) + 1;
            //CHECK FOR FOOD INSIDE SNAKE TAIL
            for (int i = 0; i < snake.size(); i++)
            {
                if (food.Y == snake[i].Y && food.X == snake[i].X)
                {
                    food.Y = 0; food.X = 0;
                    break;
                }
            }
        }
        SetConsoleCursorPosition(doorhandle, food);
        cout << "\x1B[91m" << char(219) << "\x1B[0m";
        //EATING FOOD
        if (snake[0].Y == food.Y && snake[0].X == food.X)
        {
            tailbuffer = snake[snake.size() - 1];
            food.Y = 0; food.X = 0;
            newtail = true;
        }
        //SNAKE DRAW
        for (int i=0;i<snake.size(); i++)
        {
            SetConsoleCursorPosition(doorhandle, snake[i]);
            if (i != 0) cout << "\x1B[32;2m" << char(219) << "\x1B[0m";
            else cout << "\x1B[92;1m" << char(219) << "\x1B[0m";
        }
        //SNAKE TAIL HIT DETECTION
        for (int i = 1; i < snake.size(); i++)
        {
            if (snake[0].Y == snake[i].Y && snake[0].X == snake[i].X)
            {
                play = false;
                break;
            }
        }
        Sleep(120);
    }
    system("cls");
    if (snake.size() == 1)
    {
        cout << "Your score is 1 point :(\n";
    }
    else
    {
        cout << "Your score is: " << snake.size() << " points!\n";
    }
    system("pause");
}