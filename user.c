#include <stdio.h>
#include <string.h>
#include "user.h"

void register_user()
{
    char username[50], password[50];

    printf("\nEnter Username: ");
    scanf("%s", username);

    printf("Enter Password: ");
    scanf("%s", password);

    FILE *fp = fopen("users.txt", "a");
    fprintf(fp, "%s %s\n", username, password);
    fclose(fp);

    printf("\nRegistration Successful!\n");
}

int login_user(char current_user[])
{
    char username[50], password[50];
    char u[50], p[50];

    printf("\nEnter Username: ");
    scanf("%s", username);

    printf("Enter Password: ");
    scanf("%s", password);

    FILE *fp = fopen("users.txt", "r");

    while (fscanf(fp, "%s %s", u, p) != EOF)
    {
        if (strcmp(username, u) == 0 && strcmp(password, p) == 0)
        {
            strcpy(current_user, username);
            printf("\nLogin Successful!\n");
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    printf("\nInvalid Login!\n");
    return 0;
}