#include <stdio.h>

// Определение структуры Rectangle
typedef struct {
    float width;      // Ширина
    float height;     // Высота
    float area;       // Площадь
    float perimeter;  // Периметр
} Rectangle;

// Функция для создания прямоугольника и вычисления его свойств
Rectangle create_rectangle(Rectangle rect, float width, float height) {
    rect.width = width;            // Устанавливаем ширину
    rect.height = height;          // Устанавливаем высоту
    rect.area = width * height;    // Вычисляем площадь
    rect.perimeter = 2 * (width + height); // Вычисляем периметр
    return rect;                   // Возвращаем заполненную структуру
}

// Функция для вывода свойств прямоугольника
void print_rectangle(Rectangle rect) {
    printf("Rectangle:\n");
    printf("Width: %.2f\n", rect.width);
    printf("Height: %.2f\n", rect.height);
    printf("Area: %.2f\n", rect.area);
    printf("Perimeter: %.2f\n", rect.perimeter);
}

int main() {

     Rectangle rec;
    // Создание прямоугольника через функцию create_rectangle
    Rectangle myRect = create_rectangle(rec, 5.0, 3.0);

    // Вывод свойств прямоугольника на экран
    print_rectangle(myRect);

    return 0;
}
