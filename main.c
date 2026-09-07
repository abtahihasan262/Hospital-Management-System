#include <stdio.h>
#include <string.h>
#include "hospital.h"

void showSummary(Doctor doctors[], int docCount, Patient patients[], int patCount, Appointment appts[], int apptCount)
{
    int i;
    int available;
    int scheduled;
    int cancelled;

    available = 0;
    scheduled = 0;
    cancelled = 0;

    for (i = 0; i < docCount; i++)
    {
        if (doctors[i].is_available == 1)
        {
            available = available + 1;
        }
    }

    for (i = 0; i < apptCount; i++)
    {
        if (strcmp(appts[i].status, "Booked") == 0)
        {
            scheduled = scheduled + 1;
        }
        if (strcmp(appts[i].status, "Cancelled") == 0)
        {
            cancelled = cancelled + 1;
        }
    }

    printf("\n           SUMMARY            \n");
    printf("Total Doctors     : %d\n", docCount);
    printf("Available         : %d\n", available);
    printf("On Leave          : %d\n", docCount - available);
    printf("Total Patients    : %d\n", patCount);
    printf("Total Appointments: %d\n", apptCount);
    printf("Booked            : %d\n", scheduled);
    printf("Cancelled         : %d\n", cancelled);
    printf("\n");
}

int main()
{
    Doctor doctors[MAX_DOCTORS];
    Patient patients[MAX_PATIENTS];
    Appointment appts[MAX_APPOINTMENTS];

    int docCount = 0;
    int patCount = 0;
    int apptCount = 0;
    int choice = 0;


    loadDoctors(doctors, &docCount);
    loadPatients(patients, &patCount);
    loadAppointments(appts, &apptCount);

    printf("                                          \n");
    printf("        HOSPITAL MANAGEMENT SYSTEM        \n");
    printf("                                          \n");
    printf("Doctors: %d  Patients: %d  Appointments: %d\n", docCount, patCount, apptCount);

    while (1)
    {
        printf("\n         MAIN MENU            \n");
        printf("1.  Add Doctor\n");
        printf("2.  View All Doctors\n");
        printf("3.  Search Doctor\n");
        printf("4.  Update Doctor\n");
        printf("5.  Delete Doctor\n");
        printf("6.  Add Patient\n");
        printf("7.  View All Patients\n");
        printf("8.  Search Patient\n");
        printf("9.  Delete Patient\n");
        printf("10. Book Appointment\n");
        printf("11. View All Appointments\n");
        printf("12. Cancel Appointment\n");
        printf("13. Summary\n");
        printf("14. Save and Exit\n");
        printf("\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1)
        {
            docCount = addDoctor(doctors, docCount);
        }
        else if (choice == 2)
        {
            viewDoctors(doctors, docCount);
        }
        else if (choice == 3)
        {
            searchDoctor(doctors, docCount);
        }
        else if (choice == 4)
        {
            updateDoctor(doctors, docCount);
        }
        else if (choice == 5)
        {
            docCount = deleteDoctor(doctors, docCount);
        }
        else if (choice == 6)
        {
            patCount = addPatient(patients, patCount);
        }
        else if (choice == 7)
        {
            viewPatients(patients, patCount);
        }
        else if (choice == 8)
        {
            searchPatient(patients, patCount);
        }
        else if (choice == 9)
        {
            patCount = deletePatient(patients, patCount);
        }
        else if (choice == 10)
        {
            apptCount = bookAppointment(appts, apptCount, doctors, docCount, patients, patCount);
        }
        else if (choice == 11)
        {
            viewAppointments(appts, apptCount, doctors, docCount, patients, patCount);
        }
        else if (choice == 12)
        {
            cancelAppointment(appts, apptCount);
        }
        else if (choice == 13)
        {
            showSummary(doctors, docCount, patients, patCount, appts, apptCount);
        }
        else if (choice == 14)
        {
            saveDoctors(doctors, docCount);
            savePatients(patients, patCount);
            saveAppointments(appts, apptCount);
            printf("Data saved. Goodbye!\n");
            break;
        }
        else
        {
            printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}