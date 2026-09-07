#include <stdio.h>
#include <string.h>
#include <math.h>
#include "hospital.h"
/* Purpose: Book a new appointment */
int bookAppointment(Appointment appts[], int apptCount,
Doctor doctors[], int docCount,
Patient patients[], int patCount)
{
Appointment a;
int i;
int doctorFound = 0;
int patientFound = 0;
/* Purpose: Check whether the appointment array is full */
if (apptCount >= MAX_APPOINTMENTS)
{
printf("The appointment list is full.\n");
return apptCount;
}
/* Purpose: Take appointment information from the user */
printf("Enter appointment ID: ");
scanf("%d", &a.appt_id);
printf("Enter doctor ID: ");
scanf("%d", &a.doctor_id);
/* Purpose: Check whether the entered doctor exists */
for (i = 0; i < docCount; i++)
{
if (doctors[i].id == a.doctor_id)
{

doctorFound = 1;
break;
}
}
/* Purpose: Stop booking if doctor does not exist */
if (doctorFound == 0)
{
printf("Doctor not found.\n");
return apptCount;
}
printf("Enter patient ID: ");
scanf("%d", &a.patient_id);

/* Purpose: Check whether the entered patient exists */
for (i = 0; i < patCount; i++)
{
if (patients[i].id == a.patient_id)
{
patientFound = 1;
break;
}
}
/* Purpose: Stop booking if patient does not exist */
if (patientFound == 0)
{
printf("Patient not found.\n");
return apptCount;
}

/* Purpose: Take the appointment date */
printf("Enter appointment date: ");
scanf("%11s", a.date);
/* Purpose: Set the appointment status as booked */
strcpy(a.status, "Booked");
/* Purpose: Store the appointment in the array */
appts[apptCount] = a;
printf("Appointment booked successfully.\n");
/* Purpose: Increase the appointment count */
return apptCount + 1;
}
/* Purpose: Display all booked appointments */
void viewAppointments(Appointment appts[], int apptCount,
Doctor doctors[], int docCount,
Patient patients[], int patCount)
{
int i;
int j;
/* Purpose: Check whether there are any appointments */
if (apptCount == 0)
{
printf("There are no appointments to show.\n");
return;
}
printf("\n========== List of Appointments ==========\n");
/* Purpose: Display each appointment */
for (i = 0; i < apptCount; i++)
{
printf("Appointment ID: %d\n",

appts[i].appt_id);
printf("Doctor ID: %d\n",
appts[i].doctor_id);
/* Purpose: Find and display the doctor's name */
for (j = 0; j < docCount; j++)
{
if (doctors[j].id == appts[i].doctor_id)
{
printf("Doctor Name: %s\n",
doctors[j].name);
break;
}
}
printf("Patient ID: %d\n",
appts[i].patient_id);
/* Purpose: Find and display the patient's name */
for (j = 0; j < patCount; j++)
{
if (patients[j].id == appts[i].patient_id)
{
printf("Patient Name: %s\n",
patients[j].name);
break;
}
}
printf("Date: %s\n",
appts[i].date);
printf("Status: %s\n",
appts[i].status);

printf("-----------------------------\n");
}
}
/* Purpose: Cancel an existing appointment */
void cancelAppointment(Appointment appts[], int apptCount)
{
int id;
int i;
/* Purpose: Check whether there are appointments */
if (apptCount == 0)
{
printf("There are no appointments.\n");
return;
}
/* Purpose: Get the appointment ID to cancel */
printf("Enter appointment ID: ");
scanf("%d", &id);
/* Purpose: Search for the appointment */
for (i = 0; i < apptCount; i++)
{
if (appts[i].appt_id == id)
{
/* Purpose: Change the appointment status */
strcpy(appts[i].status, "Cancelled");
printf("Appointment cancelled successfully.\n");
return;
}
}

/* Purpose: Tell the user if the appointment was not found */
printf("Appointment not found.\n");
}
/* Purpose: Save appointment data into a file */
void saveAppointments(Appointment appts[], int count)
{
FILE *fp;
/* Purpose: Open the file for writing */
fp = fopen("appointments.dat", "wb");
if (fp == NULL)
{
printf("Could not open appointments.dat\n");
return;
}
/* Purpose: Save the number of appointments */
fwrite(&count, sizeof(int), 1, fp);
/* Purpose: Save all appointment information */
fwrite(appts, sizeof(Appointment), count, fp);
fclose(fp);
}
/* Purpose: Load appointment data from a file */
void loadAppointments(Appointment appts[], int *count)
{
FILE *fp;
/* Purpose: Open the file for reading */
fp = fopen("appointments.dat", "rb");
if (fp == NULL)
{

*count = 0;
return;
}
/* Purpose: Load the number of appointments */
fread(count, sizeof(int), 1, fp);
/* Purpose: Prevent the count from exceeding array size */
if (*count > MAX_APPOINTMENTS)
{
*count = MAX_APPOINTMENTS;
}
/* Purpose: Load all appointment information */
fread(appts, sizeof(Appointment), *count, fp);
fclose(fp);
}