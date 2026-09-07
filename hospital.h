#define MAX_DOCTORS 50
#define MAX_PATIENTS 100
#define MAX_APPOINTMENTS 100

typedef struct 
{
    int id;
    char name[50];
    char specialization[30];
    int is_available;
} Doctor;

typedef struct 
{
    int id;
    char name[50];
    int age;
    char condition[100];
} Patient;

typedef struct 
{
    int appt_id;
    int doctor_id;
    int patient_id;
    char date[12];
    char status[15];
} Appointment;

int  addDoctor(Doctor doctors[], int count);
void viewDoctors(Doctor doctors[], int count);
void searchDoctor(Doctor doctors[], int count);
void updateDoctor(Doctor doctors[], int count);
int  deleteDoctor(Doctor doctors[], int count);
void saveDoctors(Doctor doctors[], int count);
void loadDoctors(Doctor doctors[], int *count);

int addPatient(Patient patients[], int count);
void viewPatients(Patient patients[], int count);
void searchPatient(Patient patients[], int count);
int deletePatient(Patient patients[], int count);
void savePatients(Patient patients[], int count);
void loadPatients(Patient patients[], int *count);

int bookAppointment(Appointment appts[], int apptCount, Doctor doctors[], int docCount, Patient patients[], int patCount);
void viewAppointments(Appointment appts[], int apptCount, Doctor doctors[], int docCount, Patient patients[], int patCount);
void cancelAppointment(Appointment appts[], int apptCount);
void saveAppointments(Appointment appts[], int count,
                      Doctor doctors[], int docCount,
                      Patient patients[], int patCount);
void loadAppointments(Appointment appts[], int *count);
void loadAppointments(Appointment appts[], int *count);

void showSummary(Doctor doctors[], int docCount, Patient patients[], int patCount, Appointment appts[], int apptCount);
