/* программы на C для работы с PostgreSQL, реализующий полные CRUD (Create, Read, Update, Delete) операции с использованием libpq */

#include <stdio.h>
#include <stdlib.h>
#include <libpq-fe.h>

void check_connection(PGconn *conn) {
    if (PQstatus(conn) != CONNECTION_OK) {
        printf("Connection failed: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }
}

void create_record(PGconn *conn) {
    const char *query = "INSERT INTO test_table (name) VALUES ('John Doe')";
    PGresult *res = PQexec(conn, query);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        printf("INSERT failed: %s\n", PQerrorMessage(conn));
    } else {
        printf("Record created successfully\n");
    }

    PQclear(res);
}

void read_records(PGconn *conn) {
    const char *query = "SELECT id, name FROM test_table";
    PGresult *res = PQexec(conn, query);

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("SELECT failed: %s\n", PQerrorMessage(conn));
    } else {
        int rows = PQntuples(res);
        for (int i = 0; i < rows; i++) {
            printf("ID: %s, Name: %s\n", PQgetvalue(res, i, 0), PQgetvalue(res, i, 1));
        }
    }

    PQclear(res);
}

void update_record(PGconn *conn, int id, const char *new_name) {
    char query[256];
    snprintf(query, sizeof(query), "UPDATE test_table SET name = '%s' WHERE id = %d", new_name, id);
    PGresult *res = PQexec(conn, query);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        printf("UPDATE failed: %s\n", PQerrorMessage(conn));
    } else {
        printf("Record updated successfully\n");
    }

    PQclear(res);
}

void delete_record(PGconn *conn, int id) {
    char query[128];
    snprintf(query, sizeof(query), "DELETE FROM test_table WHERE id = %d", id);
    PGresult *res = PQexec(conn, query);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        printf("DELETE failed: %s\n", PQerrorMessage(conn));
    } else {
        printf("Record deleted successfully\n");
    }

    PQclear(res);
}

int main() {
    PGconn *conn = PQconnectdb("host=localhost dbname=testdb user=testuser password=testpassword");

    check_connection(conn);

    // Выполняем CRUD операции
    create_record(conn);
    read_records(conn);
    update_record(conn, 1, "Jane Doe");
    read_records(conn);
    delete_record(conn, 1);
    read_records(conn);

    PQfinish(conn);

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
    id SERIAL PRIMARY KEY,
    name VARCHAR(100)
);

Компиляция:
gcc -o pg_crud pg_crud.c -lpq
 *
 * */