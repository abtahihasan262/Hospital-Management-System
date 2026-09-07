#include <stdio.h>
#include "hospital.h"

/* Search the array for a doctor with this ID.
   Returns the index if found, or -1 if not. */
int findDoctorIndex(Doctor doctors[], int count, int id) {
    int i;
    for (i = 0; i < count; i++) {
        if (doctors[i].id == id) {
            return i;
        }
    }
    return -1;
}

/* Add a doctor. Returns the new count. */
int addDoctor(Doctor doctors[], int count) {
    Doctor d;

    if (count >= MAX_DOCTORS) {          /* array is full */
        printf("The doctor list is full. Cannot add more doctors.\n");
        return count;
    }

    printf("Enter doctor ID: ");
    scanf("%d", &d.id);
    getchar();   /* remove the newline left by scanf so gets works */

    if (findDoctorIndex(doctors, count, d.id) != -1) {   /* no duplicate IDs */
        printf("A doctor with ID %d already exists.\n", d.id);
        return count;
    }

    /* gets reads a whole line, so names with spaces work */
    printf("Enter doctor name: ");
    fgets(d.name,50, stdin);
    printf("Enter specialization: ");
    fgets(d.specialization, 30, stdin);

    d.is_available = 1;
    doctors[count] = d;
    count = count + 1;

    printf("Doctor added successfully.\n");
    return count; 
}

/* Show all doctors. */
void viewDoctors(Doctor doctors[], int count) {
    int i;

    if (count == 0) {
        printf("There are no doctors to show.\n");
        return;
    }

    printf("\n--------- List of Doctors ---------\n");
    for (i = 0; i < count; i++) {
        printf("ID             : %d\n", doctors[i].id);
        printf("Name           : %s\n", doctors[i].name);
        printf("Specialization : %s\n", doctors[i].specialization);
        if (doctors[i].is_available == 1) {
            printf("Available      : Yes\n");
        } else {
            printf("Available      : No\n");
        }
        printf("----------------------------------\n");
    }
}

/* Find one doctor by ID and print their details. */
void searchDoctor(Doctor doctors[], int count) {
    int id;
    int index;

    printf("Enter doctor ID to search: ");
    scanf("%d", &id);

    index = findDoctorIndex(doctors, count, id);
    if (index == -1) {
        printf("No doctor found with ID %d.\n", id);
        return;
    }

    printf("\n--------- Doctor Found ---------\n");
    printf("ID             : %d\n", doctors[index].id);
    printf("Name           : %s\n", doctors[index].name);
    printf("Specialization : %s\n", doctors[index].specialization);
    if (doctors[index].is_available == 1) {
        printf("Available      : Yes\n");
    } else {
        printf("Available      : No\n");
    }
    printf("-------------------------------\n");
}

/* Update a doctor's name, specialization, or availability. */
void updateDoctor(Doctor doctors[], int count) {
    int id;
    int index;
    int choice;
    int avail;

    printf("Enter doctor ID to update: ");
    scanf("%d", &id);

    index = findDoctorIndex(doctors, count, id);
    if (index == -1) {
        printf("No doctor found with ID %d.\n", id);
        return;
    }

    printf("What would you like to update for %s?\n", doctors[index].name);
    printf("  1. Name\n");
    printf("  2. Specialization\n");
    printf("  3. Availability\n");
    printf("Enter choice: ");
    scanf("%d", &choice);
    getchar();   /* remove the newline left by scanf so gets works */

    if (choice == 1) {
        printf("Enter new name: ");
        fgets(doctors[index].name, 50, stdin);
        printf("Name updated.\n");
    } else if (choice == 2) {
        printf("Enter new specialization: ");
        fgets(doctors[index].specialization, 30, stdin);
        printf("Specialization updated.\n");
    } else if (choice == 3) {
        printf("Is the doctor available? (1 = Yes, 0 = No): ");
        scanf("%d", &avail);
        if (avail == 1) {
            doctors[index].is_available = 1;
        } else {
            doctors[index].is_available = 0;
        }
        printf("Availability updated.\n");
    } else {
        printf("Invalid choice. No change was made.\n");
    }
}

/* Delete a doctor by ID, shifting the later doctors left. Returns the new count. */
int deleteDoctor(Doctor doctors[], int count) {
    int id;
    int index;
    int i;

    printf("Enter doctor ID to delete: ");
    scanf("%d", &id);

    index = findDoctorIndex(doctors, count, id);
    if (index == -1) {
        printf("No doctor found with ID %d.\n", id);
        return count;
    }

    /* move every later doctor one place to the left to fill the gap */
    for (i = index; i < count - 1; i++) {
        doctors[i] = doctors[i + 1];
    }

    count = count - 1;
    printf("Doctor with ID %d deleted successfully.\n", id);
    return count;
}

/* Save all doctors to doctors.txt: count first, then each field on its own line. */
void saveDoctors(Doctor doctors[], int count) {
    FILE *fp;
    int i;

    fp = fopen("doctors.txt", "w");
    if (fp == NULL) {
        printf("Error: could not open doctors.txt to save.\n");
        return;
    }

    fprintf(fp, "%d\n", count);
    for (i = 0; i < count; i++) {
        fprintf(fp, "%d\n", doctors[i].id);
        fprintf(fp, "%s\n", doctors[i].name);
        fprintf(fp, "%s\n", doctors[i].specialization);
        fprintf(fp, "%d\n", doctors[i].is_available);
    }

    fclose(fp);
}

/* Load doctors from doctors.txt. If the file does not exist yet, start empty.
   count is a pointer so the new value goes back to main. */
void loadDoctors(Doctor doctors[], int *count) {
    FILE *fp;
    int i;

    fp = fopen("doctors.txt", "r");
    if (fp == NULL) {
        *count = 0;
        return;
    }

    fscanf(fp, "%d", count);
    for (i = 0; i < *count; i++) {
        fscanf(fp, "%d", &doctors[i].id);
        fscanf(fp, " %49[^\n]", doctors[i].name);   /* whole line, spaces ok */
        fscanf(fp, " %29[^\n]", doctors[i].specialization);
        fscanf(fp, "%d", &doctors[i].is_available);
    }

    fclose(fp);
}