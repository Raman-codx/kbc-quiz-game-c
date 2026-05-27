#include <stdio.h>
#include <string.h>
#include "history.h"

void save_history(char username[], int money)
{
    FILE *fp = fopen("history.txt", "a");
    fprintf(fp, "%s %d\n", username, money);
    fclose(fp);
}

void show_history(char username[])
{
    FILE *fp = fopen("history.txt", "r");

    char user[50];
    int money;

    printf("\nPrevious Game History:\n");

    while (fscanf(fp, "%s %d", user, &money) != EOF)
    {
        if (strcmp(user, username) == 0)
        {
            printf("You won: Rs %d\n", money);
        }
    }

    fclose(fp);
}