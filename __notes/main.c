#include <stdio.h>
#include <time.h>
#include <string.h>
#include <sqlite3.h>

typedef struct Note
{
    int id;
    time_t created_at;
    char title[40];
} T_Note;

void test()
{
    T_Note testNote;
    testNote.id = 1;
    strcpy(testNote.title, "This is test note");
    testNote.created_at = time(NULL); // Assign current time

    printf("id: %d:\ntitle: %s\ncreated: %s",
           testNote.id,
           testNote.title,
           ctime(&testNote.created_at)); // Convert time_t to string
}

int main()
{
    test();
    return 0;
}