#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define FILE_PATH "todos.txt"
#define STR_LENGTH 100

typedef struct node{
    struct node* next;
    char content[STR_LENGTH];
    int completed;
} node;

typedef struct {
    node* last;
    node* first;
    int length;
} todoList;

void add(todoList* list, node* toAdd) {
    if (list->first == NULL) 
        list->first = toAdd;
    else 
        list->last->next = toAdd;
    list->last = toAdd;
    list->length++;
}

node *get(const todoList* list, int order) {
    node* current = list->first; 
    while (--order > 0 && current != NULL) {
        current = current->next;
    }
    return current;
}

void mark(todoList* list, int order) {
    get(list, order)->completed = 1;
}

void delete(todoList* list, int order) {
    node* current = list->first;
    node* prev = NULL;

    for (int i = 1; i < order; i++) {
        prev = current;
        current = current->next;
    }

    if (prev == NULL) {
        list->first = current->next;
    } else {
        prev->next = current->next;
    }

    if (current == list->last) {
        list->last = prev;
    }

    free(current);
    list->length--;
}

void printTodos(todoList* list) {
    int order = 1;
    node* current = list->first;
    while (current != NULL) {
        printf("%d-) %s %s\n", order++, current->completed ? "[X]" : "[ ]", current->content);
        current = current->next;
    }
}

void writeListToFile(todoList* list, char* filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("An error occured while writing the file.\n");
        return;
    }
    node* root = list->first;
    while (root != NULL) {
        fprintf(fp, "%d %s\n", root->completed, root->content);
        root = root->next;
    }
    fclose(fp);
}

void readListFromFile(todoList* list, char* filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("An error occured while reading the file.\n");
        return;
    }
    int completed;
    char content[STR_LENGTH];
    while (fscanf(fp, "%d %[^\n]", &completed, content) != EOF) {
        node* newNode = malloc(sizeof(node));
        if (newNode == NULL) {
            printf("Memory allocation failed.\n");
            fclose(fp);
            return;
        }
        newNode->completed = completed;
        strcpy(newNode->content, content);
        newNode->next = NULL;
        add(list, newNode);
    }
    fclose(fp);
}

int main() {
    todoList list = {NULL, NULL, 0};
    readListFromFile(&list, FILE_PATH);
    bool run = true;
    int order;
    while (run) {
        char* choice[STR_LENGTH];
        system("clear");
        printf("----------------------------------[TODOS]----------------------------------\n");
        printTodos(&list);
        printf("---------------------------------------------------------------------------\n");
        printf("___________________________________\n");
        printf("[1] Add new todo.\n");
        printf("[2] Delete todo.\n");
        printf("[3] Mark the todo as completed.\n");
        printf("[4] Quit\n");
        printf("___________________________________\n");
        printf("Choice: ");
        scanf("%s", choice);
        getchar();

        if (strcmp(choice, "1") == 0) {
            char content[STR_LENGTH];
            printf("Write the content of the todo: ");
            fgets(content, STR_LENGTH, stdin);
            content[strcspn(content, "\n")] = 0; // remove newline character

            node* todo = malloc(sizeof(node));
            if (todo == NULL) {
                printf("Memory allocation failed.\n");
                break;
            }
            todo->completed = 0;
            strcpy(todo->content, content);
            todo->next = NULL;
            add(&list, todo);
        } else if (strcmp(choice, "2") == 0) {
            printf("Order: ");
            scanf("%d", &order);
            if (order < 1 || order > list.length) {
                getchar();
                printf("Invalid order. Please try again.\n");
                getchar();
            } else
                delete(&list, order);
        } else if (strcmp(choice, "3") == 0) {
            printf("Order: ");
            scanf("%d", &order);
            if (order < 1 || order > list.length) {
                getchar();
                printf("Invalid order. Please try again.\n");
                getchar();
            } else
                mark(&list, order);
        } else if (strcmp(choice, "4") == 0) {
            writeListToFile(&list, FILE_PATH);
            run = false;
        }
        else {
            printf("Invalid choice. Please try again.\n");
            getchar();
        }
    }
    return 0;
}