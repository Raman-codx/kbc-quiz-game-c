#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<time.h>
#include "user.h"
#include "history.h"

#define MAX_QUES_LEN 300
#define MAX_OPTION_LEN 300


const char* COLOR_END="\033[0m";
const char* RED="\033[1;31m";
const char* GREEN="\033[1;32m";
const char* YELLOW="\033[1;33m";
const char* BLUE="\033[1;34m";
const char* PINK="\033[1;35m";
const char* AQUA="\033[1;36m";

typedef struct 
{
    char text[MAX_QUES_LEN];
    char options[4][ MAX_OPTION_LEN];
    char correct_option;
    int timeout;  
    int prize_money;
}Question;

int read_questions(char* file_name, Question** questions);
void print_formatted_question(Question question);
int play_game(Question* questions,int no_of_questions);
int use_lifeline(Question* question, int *lifeline);

int main()
{
    srand(time(NULL));

    char current_user[50];
    int choice;

    printf("\n1. Register\n2. Login\nEnter choice: ");
    scanf("%d", &choice);
    getchar();   // clears buffer

    if (choice == 1)
    {
        register_user();
    }

    if (!login_user(current_user))
    {
        return 0;
    }

    printf("\n\n\t\t%sWelcome %s to KBC!!%s\n", PINK, current_user, COLOR_END);

    Question* questions;
    int no_of_questions = read_questions("questions.txt", &questions);

    int final_money = play_game(questions, no_of_questions);

    save_history(current_user, final_money);
    show_history(current_user);

    free(questions);

    return 0;
}

int play_game(Question* questions,int no_of_questions)
{
    int money_won=0;
    int lifeline[]={1,1};

    
    for(int i=0;i<no_of_questions;i++)
    {
        print_formatted_question(questions[i]);

        char input[10];

        time_t start_time = time(NULL);

        printf("\nYou have %d seconds to answer...\n", questions[i].timeout);
       scanf(" %c", &input[0]);
        char ch = toupper(input[0]);   

        time_t end_time = time(NULL);

        double time_taken = difftime(end_time, start_time);

if (time_taken > questions[i].timeout)
{
    printf("\n%sTime's Up!%s\n", RED, COLOR_END);
    break;
}
        if(ch=='L')
        {
         int value=use_lifeline(&questions[i],lifeline);
         if(value==1)
         {
            i--;
         }
        else if(value == 2)   // Skip
           {
             printf("\n%sQuestion Skipped! Moving to next question...%s\n", YELLOW, COLOR_END);
                  
            }
         else
         {
          i--;
         }
         continue;
        }
        if(ch==questions[i].correct_option)
        {
        printf("\n%scorrect!%s",GREEN,COLOR_END);
        money_won=questions[i].prize_money;
        printf("\n%sYou have won: Rs %d%s",BLUE,money_won,COLOR_END);
        }
        else
        {
        printf("\n%sWrong answer! correct answer is %c.%s",RED,questions[i].correct_option,COLOR_END);
        break;  
        }

    }
    printf("\n\n%sGame Over! Your total winnings are: Rs %d%s",BLUE,money_won,COLOR_END);

return money_won;   
}

int use_lifeline(Question* question, int *lifeline)
{
printf("\n\n%sAvailable Lifeline:%s",PINK,COLOR_END);
if(lifeline[0]) 
printf("\n%s1. Fifty-Fifty (50/50)%s",PINK,COLOR_END);
if(lifeline[1]) 
printf("\n%s2. Skip the Question%s",PINK,COLOR_END);
printf("\n%sChoose a Lifeline or 0 to return: %s",PINK,COLOR_END);

char input[10];
getchar();  // before fgets
fgets(input, sizeof(input), stdin);  
 char ch = toupper(input[0]);

 switch (ch)
 {
 case '1':
        if(lifeline[0])
        {
          lifeline[0]=0;
          int removed=0;
          while(removed<2)
          {
            int num=rand()%4;
            if((num + 'A') !=question->correct_option && question->options[num][0] !='\0')
            {
              question->options[num][0] = '\0';
              removed++;
            }
          }
          return 1;
         }
         break;
 case '2':
    if(lifeline[1])
    {
      lifeline[1]=0;
      return 2;
    }
    break;

    default:
      printf("\n%sReturning to the Question: %s",PINK,COLOR_END);
    
    break;
 }
 return 0;
}

void print_formatted_question(Question question)
{
    printf("\n\n%s%s%s",YELLOW,question.text,COLOR_END);
    for(int i=0;i<4;i++)
    {
      if(question.options[i][0]!='\0')
      {
       printf("%s%c. %s%s",AQUA,('A'+i),question.options[i],COLOR_END);
      }
    }
  
printf("\n%sHurry!! You have only %d Seconds to answer..%s",YELLOW,question.timeout,COLOR_END);
printf("\n%sEnter Your answer [A, B, C, or D] or L for lifeline:%s",GREEN,COLOR_END);

}

int read_questions(char* file_name, Question** questions)
{
    FILE *file =fopen(file_name,"r");
    if(file==NULL)
    {
        printf("\nUnable to open the questions bank. ");
        exit(0);
    }

    char str[MAX_QUES_LEN];
    int no_of_lines =0;
    while (fgets(str, MAX_QUES_LEN, file))
    {
        no_of_lines++;
    }
    int no_of_questions = no_of_lines / 8;
    *questions = (Question *) malloc(no_of_questions * sizeof(Question));

    rewind(file);


    for(int i=0;i< no_of_questions;i++)
    {
     fgets((*questions)[i].text,MAX_QUES_LEN, file);
        for(int j=0;j<4;j++)
        {
        fgets((*questions)[i].options[j],MAX_OPTION_LEN, file); 
        }
      char option[10];
      fgets(option, 10, file);
      (*questions)[i].correct_option=option[0];

     char timeout[10];
     fgets(timeout,10,file);
     (*questions)[i].timeout=atoi(timeout);

     char prize_money[10];
     fgets(prize_money,10,file);
     (*questions)[i].prize_money=atoi(prize_money);

    }
fclose(file);
return no_of_questions;
}
