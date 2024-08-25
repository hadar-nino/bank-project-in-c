#ifndef Employee_H
#define Employee_H

typedef struct {
    int employeeID;
    int branchID;
    char name[50];
    char position[50];
} Employee;
void createEmployee(Employee* employee);
void showEmployee(const Employee* employee);
#endif // !Employee_H
