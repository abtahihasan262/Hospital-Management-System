#include <stdio.h>
#include <string.h>
#include "hospital.h"

/* ----- Constants for safe string widths ----- */
#define DATE_WIDTH   11      /* date[12] fits "YYYY-MM-DD" + NUL */
#define STATUS_WIDTH 9       /* status[10] fits "Cancelled" + NUL */
#define STR(x) #x
#define XSTR(x) STR(x)

/* ----- Helper: clear input buffer after invalid input ----- */
static void clearInput(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

/* ----- Helper: read an integer, reprompt on invalid input ----- */
static int readInt(const char *prompt)
{
    int val;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &val) == 1) {
            clearInput();
            return val;
        }
        printf("Invalid input. Please enter a whole number.\n");
        clearInput();
    }
}

/* ============================================================
   BOOK APPOINTMENT
   ============================================================ */
int bookAppointment(Appointment appts[], int apptCount,
                    Doctor doctors[], int docCount,
                    Patient patients[], int patCount)
{
    if (apptCount >= MAX_APPOINTMENTS) {
        printf("The appointment list is full.\n");
        return apptCount;
    }

    Appointment a;
    int i, docIdx = -1, patFound = 0;

    /* Show available doctors and all patients */
    printf("\n--- Available Doctors ---\n");
    for (i = 0; i < docCount; i++)
        if (doctors[i].is_available)
            printf("ID: %d, Name: %s\n", doctors[i].id, doctors[i].name);

    printf("\n--- All Patients ---\n");
    for (i = 0; i < patCount; i++)
        printf("ID: %d, Name: %s\n", patients[i].id, patients[i].name);

    a.appt_id = readInt("\nEnter appointment ID: ");

    /* Duplicate ID check */
    for (i = 0; i < apptCount; i++)
        if (appts[i].appt_id == a.appt_id) {
            printf("Appointment ID already exists.\n");
            return apptCount;
        }

    a.doctor_id = readInt("Enter doctor ID: ");

    for (i = 0; i < docCount; i++)
        if (doctors[i].id == a.doctor_id) { docIdx = i; break; }

    if (docIdx == -1) {
        printf("Doctor not found.\n");
        return apptCount;
    }
    if (!doctors[docIdx].is_available) {
        printf("Doctor is currently on leave.\n");
        return apptCount;
    }

    a.patient_id = readInt("Enter patient ID: ");

    for (i = 0; i < patCount; i++)
        if (patients[i].id == a.patient_id) { patFound = 1; break; }

    if (!patFound) {
        printf("Patient not found.\n");
        return apptCount;
    }

    /* Date with bounded read and basic validation */
    printf("Enter appointment date (e.g., DD/MM/YYYY): ");
    while (1) {
        if (scanf("%" XSTR(DATE_WIDTH) "s", a.date) != 1 || strlen(a.date) < 6) {
            printf("Invalid date. Re-enter: ");
            clearInput();
            continue;
        }
        clearInput();
        break;
    }

    strcpy(a.status, "Booked");
    appts[apptCount] = a;
    printf("Appointment booked successfully.\n");
    return apptCount + 1;
}

/* ============================================================
   VIEW APPOINTMENTS – Terminal Output
   ============================================================ */
void viewAppointments(Appointment appts[], int apptCount,
                      Doctor doctors[], int docCount,
                      Patient patients[], int patCount)
{
    if (apptCount == 0) {
        printf("There are no appointments to show.\n");
        return;
    }

    printf("\n--------- List of Appointments ---------\n");
    for (int i = 0; i < apptCount; i++) {
        printf("Appointment ID   : %d\n", appts[i].appt_id);

        int found = 0;
        for (int j = 0; j < docCount; j++)
            if (doctors[j].id == appts[i].doctor_id) {
                printf("Doctor Name      : %s\n", doctors[j].name);
                found = 1;
                break;
            }
        if (!found) printf("Doctor Name      : Unknown\n");

        printf("Doctor ID        : %d\n", appts[i].doctor_id);

        found = 0;
        for (int j = 0; j < patCount; j++)
            if (patients[j].id == appts[i].patient_id) {
                printf("Patient Name     : %s\n", patients[j].name);
                found = 1;
                break;
            }
        if (!found) printf("Patient Name     : Unknown\n");

        printf("Patient ID       : %d\n", appts[i].patient_id);
        printf("Date             : %s\n", appts[i].date);
        printf("Status           : %s\n", appts[i].status);
        printf("----------------------------------------\n");
    }
}

/* ============================================================
   CANCEL APPOINTMENT
   ============================================================ */
void cancelAppointment(Appointment appts[], int apptCount)
{
    if (apptCount == 0) {
        printf("There are no appointments.\n");
        return;
    }

    int id = readInt("Enter appointment ID: ");

    for (int i = 0; i < apptCount; i++) {
        if (appts[i].appt_id == id) {
            if (strcmp(appts[i].status, "Cancelled") == 0) {
                printf("Appointment is already cancelled.\n");
                return;
            }
            strcpy(appts[i].status, "Cancelled");
            printf("Appointment cancelled successfully.\n");
            return;
        }
    }
    printf("Appointment not found.\n");
}

/* ============================================================
   SAVE – Writes the same formatted table as terminal
   ============================================================ */
void saveAppointments(Appointment appts[], int count,
                      Doctor doctors[], int docCount,
                      Patient patients[], int patCount)
{
    FILE *fp = fopen("appointments.txt", "w");
    if (!fp) {
        printf("Could not open appointments.txt\n");
        return;
    }

    if (count == 0) {
        fprintf(fp, "There are no appointments to show.\n");
    } else {
        /* Start with a newline so the file matches the terminal output */
        fprintf(fp, "\n--------- List of Appointments ---------\n");
        for (int i = 0; i < count; i++) {
            fprintf(fp, "Appointment ID   : %d\n", appts[i].appt_id);

            int found = 0;
            for (int j = 0; j < docCount; j++)
                if (doctors[j].id == appts[i].doctor_id) {
                    fprintf(fp, "Doctor Name      : %s\n", doctors[j].name);
                    found = 1;
                    break;
                }
            if (!found) fprintf(fp, "Doctor Name      : Unknown\n");

            fprintf(fp, "Doctor ID        : %d\n", appts[i].doctor_id);

            found = 0;
            for (int j = 0; j < patCount; j++)
                if (patients[j].id == appts[i].patient_id) {
                    fprintf(fp, "Patient Name     : %s\n", patients[j].name);
                    found = 1;
                    break;
                }
            if (!found) fprintf(fp, "Patient Name     : Unknown\n");

            fprintf(fp, "Patient ID       : %d\n", appts[i].patient_id);
            fprintf(fp, "Date             : %s\n", appts[i].date);
            fprintf(fp, "Status           : %s\n", appts[i].status);
            fprintf(fp, "----------------------------------------\n");
        }
    }

    fclose(fp);
    printf("Appointments saved to appointments.txt (text format).\n");
}

/* ============================================================
   LOAD – Parses the exact same format back into memory
   Uses only sscanf with exact label patterns – NO strstr, NO strcspn
   ============================================================ */
void loadAppointments(Appointment appts[], int *count)
{
    FILE *fp = fopen("appointments.txt", "r");
    if (!fp) {
        printf("No saved appointments file found.\n");
        *count = 0;
        return;
    }

    char line[256];
    int i = 0;
    Appointment temp = {0};
    int inRecord = 0;
    int hasId = 0;

    while (fgets(line, sizeof(line), fp)) {
        /* Manually strip newline (no strcspn) */
        char *p = line;
        while (*p && *p != '\n' && *p != '\r') p++;
        *p = '\0';

        if (line[0] == '\0') continue;

        /* Detect start of a new record using exact label pattern */
        int id;
        if (sscanf(line, "Appointment ID   : %d", &id) == 1) {
            /* Finalise previous record if any */
            if (inRecord) {
                if (hasId && temp.doctor_id != 0 && temp.patient_id != 0 &&
                    strlen(temp.date) > 0 && strlen(temp.status) > 0) {
                    if (i < MAX_APPOINTMENTS) {
                        appts[i] = temp;
                        i++;
                    }
                } else {
                    printf("Warning: Skipping incomplete record.\n");
                }
                memset(&temp, 0, sizeof(Appointment));
                hasId = 0;
            }
            temp.appt_id = id;
            hasId = 1;
            inRecord = 1;
            continue;
        }

        if (!inRecord) continue;

        /* Parse other fields using exact label patterns */
        int docId, patId;
        char dateStr[DATE_WIDTH + 1];
        char statusStr[STATUS_WIDTH + 1];

        if (sscanf(line, "Doctor ID        : %d", &docId) == 1)
            temp.doctor_id = docId;
        else if (sscanf(line, "Patient ID       : %d", &patId) == 1)
            temp.patient_id = patId;
        else if (sscanf(line, "Date             : %" XSTR(DATE_WIDTH) "s", dateStr) == 1)
            strcpy(temp.date, dateStr);
        else if (sscanf(line, "Status           : %" XSTR(STATUS_WIDTH) "s", statusStr) == 1)
            strcpy(temp.status, statusStr);
        /* "Doctor Name" and "Patient Name" are ignored – we look up later */
    }

    /* Finalise the last record */
    if (inRecord) {
        if (hasId && temp.doctor_id != 0 && temp.patient_id != 0 &&
            strlen(temp.date) > 0 && strlen(temp.status) > 0) {
            if (i < MAX_APPOINTMENTS) {
                appts[i] = temp;
                i++;
            }
        } else {
            printf("Warning: Skipping incomplete record at end of file.\n");
        }
    }

    *count = i;
    fclose(fp);
    printf("Loaded %d appointments from appointments.txt.\n", i);
}
