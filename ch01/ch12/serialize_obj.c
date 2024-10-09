// Пример сериализации данных структуры в файл на языке C для Windows:

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Определяем структуру данных
typedef struct {
    int id;
    char name[50];
    float value;
} Data;

void serialize(const char *filename, Data *data) {
    FILE *file = fopen(filename, "wb");  // Открываем файл для записи в бинарном формате
    if (file == NULL) {
        perror("Ошибка открытия файла");
        return;
    }

    // Пишем структуру в файл
    fwrite(data, sizeof(Data), 1, file);
    fclose(file);
}

void deserialize(const char *filename, Data *data) {
    FILE *file = fopen(filename, "rb");  // Открываем файл для чтения в бинарном формате
    if (file == NULL) {
        perror("Ошибка открытия файла");
        return;
    }

    // Читаем структуру из файла
    fread(data, sizeof(Data), 1, file);
    fclose(file);
}

int main() {
    Data originalData = {1, "Пример", 99.99}; // Исходная структура данных
    const char *filename = "data.bin";

    // Сериализация структуры в файл
    serialize(filename, &originalData);

    // Чтение структуры из файла
    Data loadedData;
    deserialize(filename, &loadedData);

    // Проверка данных
    printf("ID: %d\n", loadedData.id);
    printf("Name: %s\n", loadedData.name);
    printf("Value: %.2f\n", loadedData.value);

    return 0;
}


/*
 * Что здесь происходит:
    serialize() — сериализует структуру Data в бинарный файл.
    deserialize() — читает структуру из файла и восстанавливает данные.
    Программа создает файл data.bin, записывает туда структуру и затем загружает её обратно, выводя данные на экран.
    Этот пример наглядно показывает процесс записи и чтения структур в файл.
 * */