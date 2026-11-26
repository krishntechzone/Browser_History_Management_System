#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_URL 200

// Doubly Linked List Node
typedef struct Node {
    char url[MAX_URL];
    struct Node *prev;
    struct Node *next;
} Node;

// Stack structure
typedef struct Stack {
    Node *top;
} Stack;

// Browser structure
typedef struct Browser {
    Node *head;
    Node *current;
    Stack backStack;
    Stack forwardStack;
} Browser;


// Initialize stack
void initializeStack(Stack *stack) {
    stack->top = NULL;
}


// Push node into stack
void push(Stack *stack, Node *node) {
    Node *newNode = (Node *)malloc(sizeof(Node));

    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    strcpy(newNode->url, node->url);
    newNode->prev = stack->top;
    newNode->next = NULL;

    stack->top = newNode;
}


// Pop node from stack
Node *pop(Stack *stack) {
    if (stack->top == NULL) {
        return NULL;
    }

    Node *temp = stack->top;
    stack->top = temp->prev;

    Node *result = (Node *)malloc(sizeof(Node));

    if (result == NULL) {
        printf("Memory allocation failed!\n");
        return NULL;
    }

    strcpy(result->url, temp->url);

    free(temp);

    result->prev = NULL;
    result->next = NULL;

    return result;
}


// Check whether stack is empty
int isEmpty(Stack *stack) {
    return stack->top == NULL;
}


// Clear stack
void clearStack(Stack *stack) {
    while (!isEmpty(stack)) {
        Node *temp = stack->top;
        stack->top = temp->prev;
        free(temp);
    }
}


// Initialize browser
void initializeBrowser(Browser *browser) {
    browser->head = NULL;
    browser->current = NULL;

    initializeStack(&browser->backStack);
    initializeStack(&browser->forwardStack);
}


// Create new history node
Node *createNode(char url[]) {
    Node *newNode = (Node *)malloc(sizeof(Node));

    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        return NULL;
    }

    strcpy(newNode->url, url);
    newNode->prev = NULL;
    newNode->next = NULL;

    return newNode;
}


// Visit new webpage
void visitPage(Browser *browser, char url[]) {

    Node *newNode = createNode(url);

    if (newNode == NULL) {
        return;
    }

    if (browser->current == NULL) {
        browser->head = newNode;
        browser->current = newNode;
    }
    else {
        // Push current page into back stack
        push(&browser->backStack, browser->current);

        // Remove forward history
        clearStack(&browser->forwardStack);

        // Connect new node
        newNode->prev = browser->current;
        browser->current->next = newNode;

        browser->current = newNode;
    }

    printf("\nWebsite visited successfully!\n");
}


// Go back
void goBack(Browser *browser) {

    if (isEmpty(&browser->backStack)) {
        printf("\nNo previous page available!\n");
        return;
    }

    push(&browser->forwardStack, browser->current);

    Node *previousPage = pop(&browser->backStack);

    if (previousPage != NULL) {
        browser->current = previousPage;
        printf("\nMoved back successfully!\n");
    }
}


// Go forward
void goForward(Browser *browser) {

    if (isEmpty(&browser->forwardStack)) {
        printf("\nNo forward page available!\n");
        return;
    }

    push(&browser->backStack, browser->current);

    Node *nextPage = pop(&browser->forwardStack);

    if (nextPage != NULL) {
        browser->current = nextPage;
        printf("\nMoved forward successfully!\n");
    }
}


// Display current webpage
void displayCurrentPage(Browser *browser) {

    if (browser->current == NULL) {
        printf("\nNo webpage visited yet!\n");
        return;
    }

    printf("\nCurrent Page: %s\n", browser->current->url);
}


// Display complete browsing history
void displayHistory(Browser *browser) {

    if (browser->head == NULL) {
        printf("\nBrowsing history is empty!\n");
        return;
    }

    Node *temp = browser->head;

    printf("\n========== BROWSING HISTORY ==========\n");

    while (temp != NULL) {

        if (temp == browser->current) {
            printf("-> %s  [CURRENT PAGE]\n", temp->url);
        }
        else {
            printf("   %s\n", temp->url);
        }

        temp = temp->next;
    }

    printf("====================================\n");
}


// Clear browsing history
void clearHistory(Browser *browser) {

    Node *temp = browser->head;

    while (temp != NULL) {
        Node *nextNode = temp->next;
        free(temp);
        temp = nextNode;
    }

    browser->head = NULL;
    browser->current = NULL;

    clearStack(&browser->backStack);
    clearStack(&browser->forwardStack);

    printf("\nBrowsing history cleared successfully!\n");
}


// Free all memory
void cleanupBrowser(Browser *browser) {
    clearHistory(browser);
}


// Main function
int main() {

    Browser browser;
    initializeBrowser(&browser);

    int choice;
    char url[MAX_URL];

    do {

        printf("\n");
        printf("====================================\n");
        printf("     BROWSER HISTORY MANAGEMENT      \n");
        printf("====================================\n");

        printf("1. Visit New Website\n");
        printf("2. Go Back\n");
        printf("3. Go Forward\n");
        printf("4. Display Current Page\n");
        printf("5. Display Browsing History\n");
        printf("6. Clear Browsing History\n");
        printf("7. Exit\n");

        printf("------------------------------------\n");
        printf("Enter your choice: ");

        scanf("%d", &choice);
        getchar();

        switch (choice) {

            case 1:
                printf("Enter website URL: ");
                fgets(url, MAX_URL, stdin);

                url[strcspn(url, "\n")] = '\0';

                visitPage(&browser, url);
                break;

            case 2:
                goBack(&browser);
                break;

            case 3:
                goForward(&browser);
                break;

            case 4:
                displayCurrentPage(&browser);
                break;

            case 5:
                displayHistory(&browser);
                break;

            case 6:
                clearHistory(&browser);
                break;

            case 7:
                cleanupBrowser(&browser);
                printf("\nThank you for using Browser History System!\n");
                break;

            default:
                printf("\nInvalid choice! Please try again.\n");
        }

    } while (choice != 7);

    return 0;
}