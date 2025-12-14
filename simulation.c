#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char action[100];
    struct Node* prev;
    struct Node* next;
} Node;

Node* head = NULL;
Node* current = NULL;

// Create a new node
Node* createNode(char* action) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    strcpy(newNode->action, action);
    newNode->prev = newNode->next = NULL;
    return newNode;
}

// Add new action (like typing or deleting)
void addAction(char* action) {
    Node* newNode = createNode(action);

    // If current is not the last, delete forward history
    if (current && current->next) {
        Node* temp = current->next;
        while (temp) {
            Node* del = temp;
            temp = temp->next;
            free(del);
        }
        current->next = NULL;
    }

    if (current == NULL) {       
        head = newNode;
        current = newNode;
    } else {
        current->next = newNode;
        newNode->prev = current;
        current = newNode;
    }

    printf("Action Added: %s\n", action);
}

// Undo operation
void undo() {
    if (current && current->prev) {
        current = current->prev;
        printf("Undo: Now at '%s'\n", current->action);
    } else {
        printf("Undo not possible!\n");
    }
}

// Redo operation
void redo() {
    if (current && current->next) {
        current = current->next;
        printf("Redo: Now at '%s'\n", current->action);
    } else {
        printf("Redo not possible!\n");
    }
}

// Display all actions (for debugging)
void displayHistory() {
    Node* temp = head;
    printf("\nHistory: ");
    while (temp) {
        if (temp == current)
            printf(" [%s] ", temp->action);  // mark current
        else
            printf(" %s ", temp->action);
        temp = temp->next;
    }
    printf("\n");
}

int main() {
    addAction("Type: Hello");
    addAction("Type: World");
    addAction("Delete: 'World'");
    displayHistory();

    undo();
    undo();
    displayHistory();

    redo();
    displayHistory();

    addAction("Type: ChatGPT");  // new action clears redo history
    displayHistory();

    redo(); // should fail
    return 0;
}