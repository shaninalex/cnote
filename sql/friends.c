#include <sqlite3.h>
#include <stdio.h>

int error(sqlite3 *db, const char *errorText);

int main(void) {
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open("friends.db", &db);
    if (rc != SQLITE_OK) {
        return error(db, "Cannot open database");
    }

    char *sql =
        "CREATE TABLE IF NOT EXISTS Friends(Id INTEGER PRIMARY KEY, Name TEXT);"
        "INSERT INTO Friends(Name) VALUES ('Tom');"
        "INSERT INTO Friends(Name) VALUES ('Rebecca');"
        "INSERT INTO Friends(Name) VALUES ('Jim');"
        "INSERT INTO Friends(Name) VALUES ('Roger');"
        "INSERT INTO Friends(Name) VALUES ('Robert');";

    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to create table\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        fprintf(stdout, "Table Friends created successfully\n");
    }

    int last_id = sqlite3_last_insert_rowid(db);
    printf("The last Id of the inserted row is %d\n", last_id);

    sqlite3_close(db);
    return 0;
}

int error(sqlite3 *db, const char *errorText) {
    fprintf(stderr, "%s: %s\n", errorText, sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
}