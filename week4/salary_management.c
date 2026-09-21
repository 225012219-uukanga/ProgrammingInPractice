#include <stdio.h>

int main() {
    float salaries[50];
    float total = 0;
    float highest = 0;
    float lowest = 0;
    float average;
    float searchValue;
    int found = 0;

   
    for (int i = 0; i < 50; i++) {
        printf("Enter salary for employee %d: ", i + 1);
        scanf("%f", &salaries[i]);

        total = total + salaries[i];

        if (i == 0) {
            highest = salaries[i];
            lowest = salaries[i];
        }
        if (salaries[i] > highest) {
            highest = salaries[i];
        }
        if (salaries[i] < lowest) {
            lowest = salaries[i];
        }
    }

    average = total / 50;

    
    printf("\n--- Employee Salaries ---\n");
    for (int i = 0; i < 50; i++) {
        printf("Employee %d: %.2f\n", i + 1, salaries[i]);
    }

    
    printf("\n--- Salary Report ---\n");
    printf("Total salary expenditure: %.2f\n", total);
    printf("Average salary: %.2f\n", average);
    printf("Highest salary: %.2f\n", highest);
    printf("Lowest salary: %.2f\n", lowest);

    // Search for a particular salary
    printf("\nEnter a salary to search for: ");
    scanf("%f", &searchValue);

    for (int i = 0; i < 50; i++) {
        if (salaries[i] == searchValue) {
            found = 1;
            printf("Salary %.2f found at employee position %d\n", searchValue, i + 1);
            break;
        }
    }

    if (!found) {
        printf("Salary %.2f not found in the records.\n", searchValue);
    }

    return 0;
}