#include <stdio.h>
#include <stdlib.h>
#include "hospital.h"

int addPatient(Patient patients[], int count)
{
    if (count >= MAX_PATIENTS)
    {
        printf("The patient list is full.\n");
        return count;
    }

    patients[count].id = count + 1;

    printf("Enter patient name: ");
    getchar();
    fgets(patients[count].name, 50, stdin);

    printf("Enter age: ");
    scanf("%d", &patients[count].age);

    printf("Enter condition: ");
    getchar();
    fgets(patients[count].condition, 50, stdin);

    count = count + 1;
    printf("Patient added successfully.\n");
    return count;
}

void viewPatients(Patient patients[], int count)
{
    int i;

    if (count == 0)
    {
        printf("There are no patients to show.\n");
        return;
    }

    printf("\n       List of Patients         \n");
    for (i = 0; i < count; i++)
    {
        printf("ID        : %d\n", patients[i].id);
        printf("Name      : %s\n", patients[i].name);
        printf("Age       : %d\n", patients[i].age);
        printf("Condition : %s\n", patients[i].condition);
        printf("                               \n");
    }
}

void searchPatient(Patient patients[], int count)
{
    int id;
    int i;

    printf("Enter patient ID to search: ");
    scanf("%d", &id);

    for (i = 0; i < count; i++)
    {
        if (patients[i].id == id)
        {
            printf("ID        : %d\n", patients[i].id);
            printf("Name      : %s\n", patients[i].name);
            printf("Age       : %d\n", patients[i].age);
            printf("Condition : %s\n", patients[i].condition);
            return;
        }
    }

    printf("No patient found with ID %d.\n", id);
}

int deletePatient(Patient patients[], int count)
{
    int id;
    int i;
    int j;

    printf("Enter patient ID to delete: ");
    scanf("%d", &id);

    for (i = 0; i < count; i++)
    {
        if (patients[i].id == id)
        {
            for (j = i; j < count - 1; j++)
            {
                patients[j] = patients[j + 1];
            }
            count = count - 1;
            printf("Patient deleted successfully.\n");
            return count;
        }
    }

    printf("No patient found with ID %d.\n", id);
    return count;
}

void savePatients(Patient patients[], int count)
{
    FILE *fp;
    int i;

    fp = fopen("patients.txt", "w");
    if (fp == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    fprintf(fp, "%d\n", count);
    for (i = 0; i < count; i++)
    {
        fprintf(fp, "%d\n", patients[i].id);
        fprintf(fp, "%s\n", patients[i].name);
        fprintf(fp, "%d\n", patients[i].age);
        fprintf(fp, "%s\n", patients[i].condition);
    }

    fclose(fp);
}

void loadPatients(Patient patients[], int *count)
{
    FILE *fp;
    int i;

    fp = fopen("patients.txt", "r");
    if (fp == NULL)
    {
        *count = 0;
        return;
    }

    fscanf(fp, "%d", count);
    for (i = 0; i < *count; i++)
    {
        fscanf(fp, "%d", &patients[i].id);
        fscanf(fp, " %s", patients[i].name);
        fscanf(fp, "%d", &patients[i].age);
        fscanf(fp, " %s", patients[i].condition);
    }

    fclose(fp);
}