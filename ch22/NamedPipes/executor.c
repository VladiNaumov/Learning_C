// executor.c
#include <stdio.h>
#include <windows.h>

int main() {
    HANDLE hPipeNumbers, hPipeOperation;  // Дескрипторы именованных каналов
    DWORD dwRead;                         // Количество прочитанных байтов
    int numbers[2];                       // Числа, полученные из канала
    char operation;                       // Операция, полученная из канала
    int result;                           // Результат вычислений

    // Открытие именованных каналов для получения чисел и операции
    hPipeNumbers = CreateNamedPipe(
        "\\\\.\\pipe\\NumbersPipe",      // Имя именованного канала для чисел
        PIPE_ACCESS_INBOUND,            // Разрешение на входные данные
        PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,  // Тип и режим канала
        1,                               // Количество экземпляров канала
        1024,                            // Размер входного буфера
        1024,                            // Размер выходного буфера
        0,                               // Время ожидания (бесконечное)
        NULL                             // Атрибуты безопасности по умолчанию
    );

    hPipeOperation = CreateNamedPipe(
        "\\\\.\\pipe\\OperatorPipe",     // Имя именованного канала для операции
        PIPE_ACCESS_INBOUND,            // Разрешение на входные данные
        PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,  // Тип и режим канала
        1,                               // Количество экземпляров канала
        1024,                            // Размер входного буфера
        1024,                            // Размер выходного буфера
        0,                               // Время ожидания (бесконечное)
        NULL                             // Атрибуты безопасности по умолчанию
    );

    if (hPipeNumbers == INVALID_HANDLE_VALUE || hPipeOperation == INVALID_HANDLE_VALUE) {
        printf("Could not create pipe.\n");
        return 1;
    }

    // Ожидание подключения каналов и чтение данных
    printf("Waiting for numbers...\n");
    ConnectNamedPipe(hPipeNumbers, NULL);
    ReadFile(hPipeNumbers, numbers, sizeof(numbers), &dwRead, NULL);
    CloseHandle(hPipeNumbers);

    printf("Waiting for operation...\n");
    ConnectNamedPipe(hPipeOperation, NULL);
    ReadFile(hPipeOperation, &operation, sizeof(operation), &dwRead, NULL);
    CloseHandle(hPipeOperation);

    // Выполнение операции в зависимости от полученного символа
    switch (operation) {
        case '+':
            result = numbers[0] + numbers[1];
            break;
        case '-':
            result = numbers[0] - numbers[1];
            break;
        case '*':
            result = numbers[0] * numbers[1];
            break;
        default:
            printf("Unknown operation.\n");
            return 1;
    }

    // Вывод результата
    printf("Result: %d\n", result);

    return 0;
}
