/* кода на языке C, реализующий CRUD операции для MySQL с использованием библиотеки MySQL C API:  */

#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>

void finish_with_error(MYSQL *con) {
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}

void create_record(MYSQL *con) {
    if (mysql_query(con, "INSERT INTO test_table(name) VALUES('John Doe')")) {
        finish_with_error(con);
    }
    printf("Record created successfully\n");
}

void read_records(MYSQL *con) {
    if (mysql_query(con, "SELECT id, name FROM test_table")) {
        finish_with_error(con);
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL) {
        finish_with_error(con);
    }

    int num_fields = mysql_num_fields(result);
    MYSQL_ROW row;

    while ((row = mysql_fetch_row(result))) {
        for (int i = 0; i < num_fields; i++) {
            printf("%s ", row[i] ? row[i] : "NULL");
        }
        printf("\n");
    }

    mysql_free_result(result);
}

void update_record(MYSQL *con, int id, const char *new_name) {
    char query[256];
    snprintf(query, sizeof(query), "UPDATE test_table SET name='%s' WHERE id=%d", new_name, id);

    if (mysql_query(con, query)) {
        finish_with_error(con);
    }
    printf("Record updated successfully\n");
}

void delete_record(MYSQL *con, int id) {
    char query[128];
    snprintf(query, sizeof(query), "DELETE FROM test_table WHERE id=%d", id);

    if (mysql_query(con, query)) {
        finish_with_error(con);
    }
    printf("Record deleted successfully\n");
}

int main() {
    MYSQL *con = mysql_init(NULL);

    if (con == NULL) {
        fprintf(stderr, "mysql_init() failed\n");
        exit(1);
    }

    if (mysql_real_connect(con, "localhost", "testuser", "testpassword", "testdb", 0, NULL, 0) == NULL) {
        finish_with_error(con);
    }

    // Выполняем CRUD операции
    create_record(con);
    read_records(con);
    update_record(con, 1, "Jane Doe");
    read_records(con);
    delete_record(con, 1);
    read_records(con);

    mysql_close(con);

    return 0;
}

/*

Создание записи (Create): Функция create_record() добавляет новую запись в таблицу.
Чтение записей (Read): Функция read_records() выводит все записи из таблицы.
Обновление записи (Update): Функция update_record() обновляет запись по ID.
Удаление записи (Delete): Функция delete_record() удаляет запись по ID.
Таблица базы данных:
Этот пример предполагает, что у вас есть таблица test_table с полями id (целочисленный, автоинкремент) и name (строка).

SQL-запрос для создания таблицы:

sql
Kopioi koodi
CREATE TABLE test_table (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100)
);

Компиляция:
gcc -o mysql_crud mysql_crud.c `mysql_config --cflags --libs`

Зависимости: Убедитесь, что у вас установлены MySQL и библиотека libmysqlclient.
 *
 * */