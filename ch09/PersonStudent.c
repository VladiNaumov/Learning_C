// File name: ExtremeC_examples_chapter8_1.c
// Description: Nesting structure variables

#include <stdio.h>

typedef struct {
  char first_name[32];
  char last_name[32];
  unsigned int birth_year;
} person_t;

typedef struct {
  person_t person;
  char student_number[16]; // Extra attribute
  unsigned int passed_credits; // Extra attribute
} student_t;

int main(int argc, char** argv) {
  student_t s;
  student_t* s_ptr = &s;
  person_t* p_ptr = (person_t*)&s;
  printf("Student pointer points to %p\n", (void*)s_ptr);
  printf("Person pointer points to %p\n", (void*)p_ptr);
  return 0;  
}


/*
Мы ожидаем,что s_ptr и p_ptr указывают	на	один и тот же адрес	в памяти.
Действительно,	они	указывают	на	один и тот	же	адрес.
Обратите внимание:
данный вывод может меняться с каждым запуском,но суть в	том, что указатели	всегда
ссылаются на один участок памяти. Это значит, что структурная переменная типа
student_t в	действительности наследует	структуру	person_t в своей схеме
размещения.	Отсюда	следует, что поведенческие	функции	класса	Person	можно
вызывать с помощью	указателя на объект	student.
Иными словами, поведенческие функции класса	Person могут использоваться	объектами student.
*/