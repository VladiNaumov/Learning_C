#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"

// Функция для чтения JSON файла в строку
char* read_file(const char* filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Cannot open file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *data = (char*)malloc(length + 1);
    if (!data) {
        perror("Cannot allocate memory");
        fclose(file);
        return NULL;
    }

    fread(data, 1, length, file);
    fclose(file);

    data[length] = '\0'; // null-terminate the string
    return data;
}

int main() {
    // Пример JSON файла
    const char *filename = "example.json";

    // Чтение файла
    char *json_data = read_file(filename);
    if (json_data == NULL) {
        return 1;
    }

    // Парсинг JSON
    cJSON *json = cJSON_Parse(json_data);
    if (json == NULL) {
        printf("Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        free(json_data);
        return 1;
    }

    // Получение данных из JSON
    const cJSON *name = cJSON_GetObjectItemCaseSensitive(json, "name");
    if (cJSON_IsString(name) && (name->valuestring != NULL)) {
        printf("Name: %s\n", name->valuestring);
    }

    const cJSON *age = cJSON_GetObjectItemCaseSensitive(json, "age");
    if (cJSON_IsNumber(age)) {
        printf("Age: %d\n", age->valueint);
    }

    // Очистка
    cJSON_Delete(json);
    free(json_data);

    return 0;
}

/*
Объяснение кода:

read_file: Эта функция открывает файл, считывает его содержимое в строку и возвращает указатель на эту строку.
cJSON_Parse: Парсит строку в объект JSON.
cJSON_GetObjectItemCaseSensitive: Извлекает элементы из JSON по их ключам.
cJSON_Delete: Освобождает память, занятую объектом JSON.
free: Освобождает память, выделенную для строки.
*/