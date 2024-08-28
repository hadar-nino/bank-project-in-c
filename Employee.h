#ifndef Employee_H
#define Employee_H

typedef struct {
    int employeeID;
    int branchID;
    char name[50];
    int age;
} Employee;
void createEmployee(Employee* employee);
void showEmployee(Employee* employee);
#endif // !Employee_H
