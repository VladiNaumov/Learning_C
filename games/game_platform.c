//Пишем простую игру платформер

#include <stdio.h>
#include <stdio.h>
#include <math.h>
#include <windows.h>

#define mapWidth 80
#define mapHeight 25



typedef struct SObject{
    float x,y;
    float width;
    float heigh;
    float vertSpeed;

}TObject;

char map[mapHeight][mapWidth + 1];
TObject mario;

void ClearMap()
{
    for (int i = 0; i < mapWidth; i++)
        map[0][i] = '.';
    map[0][mapWidth]='\0';
    for (int j = 1; j < mapHeight; j++)
        sprintf(map[j], map[0]);
}

void ShowMap()
{
    map[mapHeight - 1][mapWidth - 1] ='\0';
    for (int j = 0; j < mapHeight; j++)
        printf("%s", map[j]);
}

void SetObjectPos(TObject *obj, float xPos, float yPos)
{
    obj ->x = xPos;
    obj ->y = yPos;
}

void InitObject(TObject *obj, float xPos, float yPos, float oWidth, float oHeight )
{
    SetObjectPos(obj, xPos, yPos);
    obj->width = oWidth;
    obj->heigh = oHeight;
    obj->vertSpeed = 0;
}

void VertMoveObject(TObject *obj)
{
    obj->vertSpeed += 0.05;
    SetObjectPos(obj, obj->x, obj->y + obj->vertSpeed);
}

void PutObjectOnMap(TObject obj)
{
    int ix = (int)round(obj.x);
    int iy = (int)round(obj.y);
    int iWidth = (int)round(obj.width);
    int iHeight = (int)round(obj.heigh);

    for (int i = ix; i < (ix + iWidth); i++)
        for (int j = iy; j < (iy + iHeight); j++)
               map[j][i] = '@';
}

void setCur(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int main()
{
    InitObject(&mario, 39, 10, 3, 3);

    do
    {
        ClearMap();
        VertMoveObject(&mario);
        PutObjectOnMap(mario);
        setCur(0, 0);
        ShowMap();
        Sleep(1000);


    }while (GetKeyState(VK_SPACE) >= 0);

     return 0;
}
