#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../data/budget.h"
#include "../common/file_helper.h"

#define MAX_LINE_SIZE 256
#define FILE_PATH "budgets.csv"

static Budget *get_budgets_from_file(FILE *file);
static Budget get_budget_from_line(char *line);

Budget *get_budgets(int *num_budgets)
{
    FILE *file = fopen("budgets.csv", "r");
    if (file == NULL)
    {
        printf("Error opening the file.\n");
        exit(0);
    }

    Budget *budgets = get_budgets_from_file(file);

    *num_budgets = count_lines(file) - 1; // -1 because of the header

    fclose(file);
    return budgets;
}

Budget get_budget_by_id(int id)
{
    FILE *file = fopen(FILE_PATH, "r");
    if (file == NULL)
    {
        printf("Error opening the file.\n");
        exit(0);
    }

    Budget *budgets = get_budgets_from_file(file);

    Budget budget;
    for (int i = 0; i < sizeof(budgets); i++)
    {
        if (atoi(budgets[i].id) == id)
        {
            budget = budgets[i];
        }
    }

    fclose(file);
    return budget;
}

void add_budget(Budget budget)
{
    FILE *file = fopen(FILE_PATH, "a+");
    if (file == NULL)
    {
        printf("Error opening the file.\n");
        exit(0);
    }

    fseek(file, 0, SEEK_END);

    long size = ftell(file);
    if (size == 0)
    {
        fprintf(file, "id,name,amount,account_name\n");
    }

    fprintf(file, "%s,%s,%.2f,%s\n", budget.id, budget.name, budget.amount, budget.account_name);

    fclose(file);
}

static Budget *get_budgets_from_file(FILE *file)
{
    Budget *budgets = malloc(sizeof(Budget) * 100);
    if (budgets == NULL)
    {
        printf("Error allocating memory.\n");
        exit(0);
    }

    char line[MAX_LINE_SIZE];
    int i = 0;
    while (fgets(line, sizeof(line), file))
    {
        if (i == 0)
        {
            i++;
            continue;
        }

        Budget budget = get_budget_from_line(line);
        budgets[i - 1] = budget;
        i++;
    }

    return budgets;
}

static Budget get_budget_from_line(char *line)
{
    Budget budget;
    int token_index = 0;
    char *token = strtok(line, ",");

    while (token != NULL)
    {
        switch (token_index)
        {
        case 0:
            strcpy(budget.id, token);
            break;
        case 1:
            strcpy(budget.name, token);
            break;
        case 2:
            budget.amount = atof(token);
            break;
        case 3:
            strcpy(budget.account_name, token);
            break;
        default:
            break;
        }
        token_index++;
        token = strtok(NULL, ",");
    }
    return budget;
}