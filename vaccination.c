#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[50];
    int age;
    int dose1;
    int dose2;
} Citizen;

typedef struct {
    Citizen *data;
    int size;
    int capacity;
} List;

void ensure_capacity(List *list) {
    if (list->size >= list->capacity) {
        int newCap = list->capacity * 2;
        Citizen *tmp = (Citizen *)realloc(list->data, sizeof(Citizen) * newCap);

        if (tmp == NULL) {
            printf("Memory allocation failed!\n");
            exit(1);
        }
        list->data = tmp;
        list->capacity = newCap;
    }
}

void init_list(List *list) {
    list->capacity = 5;
    list->size = 0;
    list->data = (Citizen *)malloc(sizeof(Citizen) * list->capacity);
}

void register_citizen(List *list) {
    ensure_capacity(list);
    Citizen c;

    printf("Enter name: ");
    scanf("%s", c.name);

    printf("Enter age: ");
    scanf("%d", &c.age);

    c.dose1 = 0;
    c.dose2 = 0;

    list->data[list->size++] = c;

    printf("Citizen registered successfully!\n");
}

void list_citizens(List *list) {
    if (list->size == 0) {
        printf("No citizens registered.\n");
        return;
    }

    for (int i = 0; i < list->size; i++) {
        printf("%d. %s | Age: %d | Dose1: %d | Dose2: %d\n",
               i + 1,
               list->data[i].name,
               list->data[i].age,
               list->data[i].dose1,
               list->data[i].dose2);
    }
}

void administer_dose(List *list, int *stock) {
    if (*stock <= 0) {
        printf("No stock available!\n");
        return;
    }

    list_citizens(list);
    printf("Enter citizen number to administer dose: ");
    int n;
    scanf("%d", &n);

    if (n < 1 || n > list->size) {
        printf("Invalid citizen number!\n");
        return;
    }

    Citizen *c = &list->data[n - 1];

    if (c->dose1 == 0) {
        c->dose1 = 1;
        (*stock)--;
        printf("Dose 1 administered!\n");
    }
    else if (c->dose2 == 0) {
        c->dose2 = 1;
        (*stock)--;
        printf("Dose 2 administered!\n");
    }
    else {
        printf("Citizen already fully vaccinated!\n");
    }
}

void manage_stock(int *stock) {
    int add;
    printf("Enter stock to add: ");
    scanf("%d", &add);
    *stock += add;
    printf("Stock updated! Total stock: %d\n", *stock);
}

void pending_second_dose(List *list) {
    printf("\n--- Citizens Pending Dose 2 ---\n");

    int found = 0;

    for (int i = 0; i < list->size; i++) {
        if (list->data[i].dose1 == 1 && list->data[i].dose2 == 0) {
            printf("%s | Age: %d\n", list->data[i].name, list->data[i].age);
            found = 1;
        }
    }

    if (!found) {
        printf("No pending second dose.\n");
    }
}

void daily_summary(List *list) {
    int dose1 = 0, dose2 = 0;

    for (int i = 0; i < list->size; i++) {
        dose1 += list->data[i].dose1;
        dose2 += list->data[i].dose2;
    }

    printf("\n--- Daily Summary ---\n");
    printf("Dose 1 administered: %d\n", dose1);
    printf("Dose 2 administered: %d\n", dose2);
}

void save_citizens(List *list) {
    FILE *f = fopen("citizens.txt", "w");
    for (int i = 0; i < list->size; i++) {
        fprintf(f, "%s %d %d %d\n",
                list->data[i].name,
                list->data[i].age,
                list->data[i].dose1,
                list->data[i].dose2);
    }
    fclose(f);
}

void load_citizens(List *list) {
    FILE *f = fopen("citizens.txt", "r");
    if (!f) return;

    while (!feof(f)) {
        Citizen c;
        if (fscanf(f, "%s %d %d %d", c.name, &c.age, &c.dose1, &c.dose2) == 4) {
            ensure_capacity(list);
            list->data[list->size++] = c;
        }
    }
    fclose(f);
}

void save_stock(int stock) {
    FILE *f = fopen("stock.txt", "w");
    fprintf(f, "%d", stock);
    fclose(f);
}

void load_stock(int *stock) {
    FILE *f = fopen("stock.txt", "r");
    if (!f) return;
    fscanf(f, "%d", stock);
    fclose(f);
}

void free_list(List *list) {
    free(list->data);
}

int main() {
    List list;
    int stock = 0;
    int running = 1;
    int opt;

    init_list(&list);
    load_citizens(&list);
    load_stock(&stock);

    while (running) {
        printf("\n--- MENU ---\n");
        printf("1. Register Citizen\n");
        printf("2. List Citizens\n");
        printf("3. Administer Dose\n");
        printf("4. Manage Stock\n");
        printf("5. Daily Summary\n");
        printf("6. Pending Second Dose\n");
        printf("7. Save & Exit\n");

        printf("Enter option: ");
        scanf("%d", &opt);

        switch (opt) {
            case 1: register_citizen(&list); break;
            case 2: list_citizens(&list); break;
            case 3: administer_dose(&list, &stock); break;
            case 4: manage_stock(&stock); break;
            case 5: daily_summary(&list); break;
            case 6: pending_second_dose(&list); break;
            case 7:
                save_citizens(&list);
                save_stock(stock);
                printf("Data saved. Exiting...\n");
                running = 0;
                break;
            default:
                printf("Invalid option!\n");
        }
    }

    free_list(&list);
    return 0;
}