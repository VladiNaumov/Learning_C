#include <stdio.h>
#include <conio.h>

int main()
{
    char mas[10][21];
    int i;

    // simvol @
    int x = 10, y = 6;

    //nagatie klavichi
    char key;

    int ox, oy;

    // apple
    int ax = 4, ay = 3;

    rand(time(NULL));

    do
    {

    sprintf(mas[0], "####################");

    for (i = 1; i < 9; ++i)
        sprintf(mas[i], "#                  #");

    sprintf(mas[9], "####################");

    mas[y][x] = '@';
    mas[ax][ay] = 'a';

    // ochistka ekrana konsoli
    system("cls");

    for (i = 0; i < 10; ++i) {
        printf("%s\n", mas[i]);
    }

    key = getch();

    ox = x;
    oy = y;

    if (key == 'w') y--;
    if (key == 's') y++;
    if (key == 'a') x--;
    if (key == 'd') x++;

    //ogranichivaem peredvigenie za steni
    if (mas[y][x] == '@')
    {
        x = ox;
        y = oy;
    }
    if((x == ax) && (y == ay))
    {
        ax = rand() * 1.0 / RAND_MAX * 18 + 1;
        ay = rand() * 1.0 / RAND_MAX * 8 + 1;
    }

    } while (key != 'e');

    //system("");

    return 0;


}
