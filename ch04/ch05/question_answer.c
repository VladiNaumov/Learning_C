#include <stdio.h>

// Определение структуры для вопроса
typedef struct {
    char questionText[100];  // Текст вопроса
    char options[3][50];     // Варианты ответов
    int correctAnswer;       // Номер правильного ответа (1, 2 или 3)
} Question;

// Функция вывода вопроса и вариантов ответа на консоль
void printQuestion(Question q) {
    printf("%s\n", q.questionText);
    for (int i = 0; i < 3; i++) {
        printf("%d) %s\n", i + 1, q.options[i]);
    }
}

// Функция получения ответа пользователя
int getUserAnswer() {
    int answer;
    printf("Выберите правильный вариант (1, 2 или 3): ");
    scanf("%d", &answer);
    return answer;
}

// Функция проверки правильности ответа
void checkAnswer(Question q, int answer) {
    if (answer == q.correctAnswer) {
        printf("Верно\n");
    } else {
        printf("Неправильно\n");
    }
}

// Главная функция программы
int main() {
    // Определение вопросов с использованием структуры
    Question questions[3] = {
        {"Сколько будет три плюс три?", {"2", "6", "10"}, 2},
        {"Сколько будет пять минус два?", {"3", "2", "7"}, 1},
        {"Сколько будет два умножить на три?", {"6", "5", "4"}, 1}
    };

    for (int i = 0; i < 3; i++) {     // Цикл по трём вопросам
        printQuestion(questions[i]);  // Выводим вопрос и варианты ответа
        int answer = getUserAnswer(); // Получаем ответ от пользователя
        checkAnswer(questions[i], answer); // Проверяем ответ
        printf("\n");                 // Печатаем пустую строку для разделения вопросов
    }

    return 0;
}


/*
Структура Question: Структура Question содержит текст вопроса (questionText), массив вариантов ответа (options) и номер правильного ответа (correctAnswer). Это позволяет нам группировать связанные данные вместе и облегчить управление вопросами.

Массив questions[3]: В главной функции мы создаем массив questions, который содержит три вопроса. Каждый вопрос представляет собой экземпляр структуры Question.

Функции printQuestion(), checkAnswer(): Эти функции теперь принимают структуру Question в качестве параметра, что делает код более гибким и легко расширяемым.
*/