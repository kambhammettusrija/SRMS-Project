#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STUD_FILE "students.txt"
#define CRE_FILE  "credentials.txt"
#define MAX_ATTEMPTS 3

char currentUser[50];
char currentRole[20];

int login() {
    char u[50], p[50], r[20];
    char inUser[50], inPass[50];
    int attempt = 0;

    printf("WELCOME TO THE STUDENT MANAGEMENT SYSTEM\n");

    while (attempt < MAX_ATTEMPTS) {
        printf("\n--- Login Attempt %d of %d ---\n", attempt + 1, MAX_ATTEMPTS);
        printf("USERNAME: ");
        scanf("%49s", inUser); 
        printf("PASSWORD: ");
        scanf("%49s", inPass); 

        FILE *fp = fopen(CRE_FILE, "r");
        if (!fp) {
            printf("Credential file missing! Cannot proceed.\n");
            return 0;
        }

        int loggedIn = 0;
        fseek(fp, 0, SEEK_SET); 
        
        while (fscanf(fp, "%49s %49s %19s", u, p, r) == 3) {
            if (strcmp(inUser, u) == 0 && strcmp(inPass, p) == 0) {
                strncpy(currentUser, u, sizeof(currentUser) - 1);
                currentUser[sizeof(currentUser) - 1] = '\0';
                strncpy(currentRole, r, sizeof(currentRole) - 1);
                currentRole[sizeof(currentRole) - 1] = '\0';
                fclose(fp);
                return 1;
            }
        }

        fclose(fp);
        printf("Invalid username or password.\n");
        attempt++;
    }

    printf("\nMaximum login attempts exceeded. Exiting.\n");
    return 0;
}

void searchStudent() {
    int find, roll;
    char name[50];
    float mark;

    printf("Enter roll to search: ");
    scanf("%d", &find);

    FILE *fp = fopen(STUD_FILE, "r");
    if (!fp) {
        printf("No student file!\n");
        return;
    }

    int found = 0;
    while (fscanf(fp, "%d %49s %f", &roll, name, &mark) == 3) {
        if (roll == find) {
            printf("\n--- Student Details ---\n");
            printf("Roll: %d\n", roll);
            printf("Name: %s\n", name);
            printf("Mark: %.2f\n", mark);
            printf("-----------------------\n");
            found = 1;
            break; 
        }
    }
    fclose(fp);
    if (!found) {
        printf("Student with roll %d not found!\n", find);
    }
}

void addStudent() {
    int roll;
    char name[50];
    float mark;

    printf("Roll: ");
    if (scanf("%d", &roll) != 1) { return; } 
    printf("Name: ");
    scanf(" %49[^\n]", name); 
    printf("Mark: ");
    if (scanf("%f", &mark) != 1) { return; }

    FILE *fp = fopen(STUD_FILE, "a");
    fprintf(fp, "%d %s %.2f\n", roll, name, mark);
    fclose(fp);

    printf("Student added!\n");
}

void displayStudents() {
    FILE *fp = fopen(STUD_FILE, "r");
    if (!fp) {
        printf("No student file!\n");
        return;
    }

    int roll;
    char name[50];
    float mark;

    printf("\n--- All Students ---\n");
    printf("Roll\tName\tMark\n");
    printf("----\t----\t----\n");
    while (fscanf(fp, "%d %49s %f", &roll, name, &mark) == 3) { 
        printf("%d\t%s\t%.2f\n", roll, name, mark);
    }
    printf("--------------------\n");

    fclose(fp);
}

void deleteStudent() {
    int delRoll;
    printf("Enter roll to delete: ");
    if (scanf("%d", &delRoll) != 1) { printf("Invalid input.\n"); return; }

    FILE *fp = fopen(STUD_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp) { return; }

    int roll;
    char name[50];
    float mark;
    int found = 0;

    while (fscanf(fp, "%d %49s %f", &roll, name, &mark) == 3) {
        if (roll != delRoll) {
            fprintf(temp, "%d %s %.2f\n", roll, name, mark);
        } else {
            found = 1;
        }
    }

    fclose(fp);
    fclose(temp);

    remove(STUD_FILE);
    rename("temp.txt", STUD_FILE);

    if (found) printf("Student deleted!\n");
    else printf("Roll not found!\n");
}

void updateStudent() {
    int updateRoll;
    printf("Enter roll to update: ");
    if (scanf("%d", &updateRoll) != 1) { printf("Invalid input.\n"); return; }

    FILE *fp = fopen(STUD_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp) { return; }

    int roll;
    char name[50];
    float mark;
    int found = 0;

    while (fscanf(fp, "%d %49s %f", &roll, name, &mark) == 3) {
        if (roll == updateRoll) {
            found = 1;
            char newName[50];
            float newMark;

            printf("New Name: ");
            scanf(" %49[^\n]", newName);
            printf("New Mark: ");
            if (scanf("%f", &newMark) != 1) { return; }

            fprintf(temp, "%d %s %.2f\n", roll, newName, newMark);
        } else {
            fprintf(temp, "%d %s %.2f\n", roll, name, mark);
        }
    }

    fclose(fp);
    fclose(temp);

    remove(STUD_FILE);
    rename("temp.txt", STUD_FILE);

    if (found) printf("Student updated!\n");
    else printf("Roll not found!\n");
}

void adminMenu() {
    int c;
    while (1) {
        printf("\nADMIN MENU\n");
        printf("1.Add Student\n2.Display All Students\n3.Search Student by Roll\n4.Update Student\n5.Delete Student\n6.Logout\n");
        if (scanf("%d",&c) != 1) { c = 0; }
        
        if(c==1)addStudent();
        else if(c==2)displayStudents();
        else if(c==3)searchStudent();
        else if(c==4)updateStudent();
        else if(c==5)deleteStudent();
        else return;
    }
}

void staffMenu() {
    int c;
    while (1) {
        printf("\nSTAFF MENU\n");
        printf("1.Add Student\n2.Display All Students\n3.Search Student by Roll\n4.Update Student\n5.Logout\n");
        if (scanf("%d",&c) != 1) { c = 0; }
        
        if(c==1)addStudent();
        else if(c==2)displayStudents();
        else if(c==3)searchStudent();
        else if(c==4)updateStudent();
        else return;
    }
}

void guestMenu() {
    int c;
    while (1) {
        printf("\nGUEST MENU\n");
        printf("1.Display All Students\n2.Search Student by Roll\n3.Logout\n");
        if (scanf("%d",&c) != 1) { c = 0; }
        
        if(c==1)displayStudents();
        else if(c==2)searchStudent();
        else return;
    }
}

int main() {
    if (!login()) {
        return 0;
    }

    printf("\nSuccessfully logged in as: %s (%s)\n", currentUser, currentRole);

    if (strcmp(currentRole,"admin")==0) adminMenu();
    else if (strcmp(currentRole,"staff")==0) staffMenu();
    else if (strcmp(currentRole,"guest")==0) guestMenu();
    else printf("Unknown role. Exiting.\n");

    printf("Logged out. Goodbye!\n");
    return 0;
}
