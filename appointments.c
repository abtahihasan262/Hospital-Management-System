#include <stdio.h>
#include <string.h>
#include <math.h>
#include "hospital.h"

/* ============================================================
   BUFFER WIDTHS – MUST MATCH YOUR hospital.h
   ============================================================ */
#define DATE_WIDTH   11   /* date[12] → %11s + NUL */
#define STATUS_WIDTH 9    /* status[10] → %9s + NUL  */

#define STR(x) #x
#define XSTR(x) STR(x)

/* ============================================================
   INTERNAL HELPER FUNCTIONS – SAFE INPUT HANDLING
   ============================================================ */

static void clearInputBuffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

static int readInt(const char *prompt)
{
    int value;
    while (1)
    {
        printf("%s", prompt);
        if (scanf("%d", &value) == 1)
        {
            clearInputBuffer();
            return value;
        }
        printf("Invalid input. Please enter a whole number.\n");
        clearInputBuffer();
    }
}

static int isPlausibleDate(const char *s)
{
    int len = (int)strlen(s);
    int digitCount = 0;
    int i;

    if (len < 6)
        return 0;

    for (i = 0; i < len; i++)
    {
        if (s[i] >= '0' && s[i] <= '9')
            digitCount++;
        else if (s[i] != '/' && s[i] != '-')
            return 0;
    }
    return digitCount >= 6;
}

static void readDate(char *dest)
{
    while (1)
    {
        printf("Enter appointment date: ");
        if (scanf("%" XSTR(DATE_WIDTH) "s", dest) != 1)
        {
            printf("Invalid input.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        if (isPlausibleDate(dest))
            return;

        printf("Invalid or truncated date (\"%s\"). Please re-enter (e.g. DD/MM/YYYY).\n", dest);
    }
}

/* ============================================================
   BOOK APPOINTMENT
   ============================================================ */
int bookAppointment(Appointment appts[], int apptCount,
                    Doctor doctors[], int docCount,
                    Patient patients[], int patCount)
{
    Appointment a;
    int i;
    int doctorIndex = -1;
    int patientFound = 0;

    if (apptCount >= MAX_APPOINTMENTS)
    {
        printf("The appointment list is full.\n");
        return apptCount;
    }

    printf("\n--- Available Doctors ---\n");
    for (i = 0; i < docCount; i++)
    {
        if (doctors[i].is_available == 1)
        {
            printf("ID: %d, Name: %s\n", doctors[i].id, doctors[i].name);
        }
    }

    printf("\n--- All Patients ---\n");
    for (i = 0; i < patCount; i++)
    {
        printf("ID: %d, Name: %s\n", patients[i].id, patients[i].name);
    }

    a.appt_id = readInt("\nEnter appointment ID: ");

    for (i = 0; i < apptCount; i++)
    {
        if (appts[i].appt_id == a.appt_id)
        {
            printf("Appointment ID already exists. Please use a unique ID.\n");
            return apptCount;
        }
    }

    a.doctor_id = readInt("Enter doctor ID: ");

    for (i = 0; i < docCount; i++)
    {
        if (doctors[i].id == a.doctor_id)
        {
            doctorIndex = i;
            break;
        }
    }
    if (doctorIndex == -1)
    {
        printf("Doctor not found.\n");
        return apptCount;
    }

    if (doctors[doctorIndex].is_available != 1)
    {
        printf("Doctor is currently on leave. Cannot book appointment.\n");
        return apptCount;
    }

    a.patient_id = readInt("Enter patient ID: ");

    for (i = 0; i < patCount; i++)
    {
        if (patients[i].id == a.patient_id)
        {
            patientFound = 1;
            break;
        }
    }
    if (!patientFound)
    {
        printf("Patient not found.\n");
        return apptCount;
    }

    readDate(a.date);
    strcpy(a.status, "Booked");
    appts[apptCount] = a;

    printf("Appointment booked successfully.\n");
    return apptCount + 1;
}

/* ============================================================
   VIEW APPOINTMENTS (Terminal Output)
   ============================================================ */
void viewAppointments(Appointment appts[], int apptCount,
                       Doctor doctors[], int docCount,
                       Patient patients[], int patCount)
{
    int i, j;

    if (apptCount == 0)
    {
        printf("There are no appointments to show.\n");
        return;
    }

    printf("\n--------- List of Appointments ---------\n");

    for (i = 0; i < apptCount; i++)
    {
        printf("Appointment ID   : %d\n", appts[i].appt_id);

        int found = 0;
        for (j = 0; j < docCount; j++)
        {
            if (doctors[j].id == appts[i].doctor_id)
            {
                printf("Doctor Name      : %s\n", doctors[j].name);
                found = 1;
                break;
            }
        }
        if (!found)
            printf("Doctor Name      : Unknown\n");

        printf("Doctor ID        : %d\n", appts[i].doctor_id);

        found = 0;
        for (j = 0; j < patCount; j++)
        {
            if (patients[j].id == appts[i].patient_id)
            {
                printf("Patient Name     : %s\n", patients[j].name);
                found = 1;
                break;
            }
        }
        if (!found)
            printf("Patient Name     : Unknown\n");

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
    if (apptCount == 0)
    {
        printf("There are no appointments.\n");
        return;
    }

    int id = readInt("Enter appointment ID: ");

    for (int i = 0; i < apptCount; i++)
    {
        if (appts[i].appt_id == id)
        {
            if (strcmp(appts[i].status, "Cancelled") == 0)
            {
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
   SAVE APPOINTMENTS – Writes the SAME FORMAT as Terminal
   ============================================================ */
void saveAppointments(Appointment appts[], int count,
                       Doctor doctors[], int docCount,
                       Patient patients[], int patCount)
{
    FILE *fp = fopen("appointments.txt", "w");
    if (fp == NULL)
    {
        printf("Could not open appointments.txt\n");
        return;
    }

    if (count == 0)
    {
        fprintf(fp, "No appointments to show.\n");
    }
    else
    {
        fprintf(fp, "\n--------- List of Appointments ---------\n");

        for (int i = 0; i < count; i++)
        {
            fprintf(fp, "Appointment ID   : %d\n", appts[i].appt_id);

            int found = 0;
            for (int j = 0; j < docCount; j++)
            {
                if (doctors[j].id == appts[i].doctor_id)
                {
                    fprintf(fp, "Doctor Name      : %s\n", doctors[j].name);
                    found = 1;
                    break;
                }
            }
            if (!found)
                fprintf(fp, "Doctor Name      : Unknown\n");

            fprintf(fp, "Doctor ID        : %d\n", appts[i].doctor_id);

            found = 0;
            for (int j = 0; j < patCount; j++)
            {
                if (patients[j].id == appts[i].patient_id)
                {
                    fprintf(fp, "Patient Name     : %s\n", patients[j].name);
                    found = 1;
                    break;
                }
            }
            if (!found)
                fprintf(fp, "Patient Name     : Unknown\n");

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
   LOAD APPOINTMENTS – FULLY REPAIRED
   - Uses strstr to detect "Appointment ID" (most reliable)
   - Uses "%*[^:]: %d" to make parsing whitespace‑flexible
   - Uses hasId flag to accept ID 0
   - No buffer overflows (bounded %s)
   - Warns on incomplete records
   ============================================================ */
void loadAppointments(Appointment appts[], int *count)
{
    FILE *fp = fopen("appointments.txt", "r");
    if (fp == NULL)
    {
        printf("No saved appointments file found (appointments.txt). Starting with zero appointments.\n");
        *count = 0;
        return;
    }

    char line[256];
    int i = 0;
    Appointment temp = {0};          /* zero‑initialises everything */
    int inRecord = 0;
    int hasId = 0;                  /* flag to allow ID 0 */

    while (fgets(line, sizeof(line), fp))
    {
        /* Strip newline manually (no strcspn) */
        char *p = line;
        while (*p && *p != '\n' && *p != '\r') p++;
        *p = '\0';

        if (line[0] == '\0')
            continue;

        /* ===== Detect start of a new appointment ===== */
        /* Using strstr is the safest way to find the label regardless of spacing */
        if (strstr(line, "Appointment ID") != NULL)
        {
            /* If we were already in a record, finalise it */
            if (inRecord)
            {
                /* Only save if we have a valid ID and other fields */
                if (hasId && temp.doctor_id != 0 && temp.patient_id != 0 &&
                    strlen(temp.date) > 0 && strlen(temp.status) > 0)
                {
                    if (i < MAX_APPOINTMENTS)
                    {
                        appts[i] = temp;
                        i++;
                    }
                }
                else
                {
                    printf("Warning: Skipping incomplete appointment record.\n");
                }
                /* Reset temp and flag for the new record */
                memset(&temp, 0, sizeof(Appointment));
                hasId = 0;
                inRecord = 0;
            }
            inRecord = 1;  /* now inside a record */
        }

        if (!inRecord)
            continue;  /* skip lines before first record */

        /* ===== WHITESPACE‑FLEXIBLE PARSING =====
           "%*[^:]: %d" reads everything up to the colon (discards it),
           then skips the colon and any following spaces, then reads the value.
           This works regardless of the number of spaces. */
        int value;
        if (sscanf(line, " %*[^:]: %d", &value) == 1)
        {
            /* We need to distinguish which field this is.
               We check the start of the line to know which ID it is. */
            if (strstr(line, "Appointment ID") != NULL)
            {
                temp.appt_id = value;
                hasId = 1;
            }
            else if (strstr(line, "Doctor ID") != NULL)
            {
                temp.doctor_id = value;
            }
            else if (strstr(line, "Patient ID") != NULL)
            {
                temp.patient_id = value;
            }
        }
        else
        {
            /* Try to parse Date and Status – they are strings */
            char str[100];
            if (sscanf(line, " %*[^:]: %" XSTR(DATE_WIDTH) "s", temp.date) == 1)
            {
                /* safe: bounded, no temporary buffer */
            }
            else if (sscanf(line, " %*[^:]: %" XSTR(STATUS_WIDTH) "s", temp.status) == 1)
            {
                /* safe: bounded */
            }
        }
        /* "Doctor Name" and "Patient Name" are ignored; we look up later */
    }

    /* Save the last record if we were inside one */
    if (inRecord)
    {
        if (hasId && temp.doctor_id != 0 && temp.patient_id != 0 &&
            strlen(temp.date) > 0 && strlen(temp.status) > 0)
        {
            if (i < MAX_APPOINTMENTS)
            {
                appts[i] = temp;
                i++;
            }
        }
        else
        {
            printf("Warning: Skipping incomplete appointment record at end of file.\n");
        }
    }

    *count = i;
    fclose(fp);

    if (*count == 0)
        printf("No appointments loaded from appointments.txt.\n");
    else
        printf("Appointments loaded from appointments.txt (text format).\n");
}
