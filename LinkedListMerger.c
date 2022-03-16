#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MEMORY_ALLOCATION_ERROR 2

struct Node {
    bool isHole;
    int processID;
    int base;
    int limit;
    struct Node *next;
};

typedef struct Node *Link;

void printHole(Link head, int counter) {
    printf("Node %d: H (Hole), base = %d, limit = %d\n",
           counter, head->base, head->limit);
}

void printProcess(Link head, int counter) {
    printf("Node %d: P%d, base = %d, limit = %d\n",
           counter, head->processID, head->base, head->limit);
}

void printMemory(Link head) {

    if (!head) {
        printf("\n");
        return;
    }

    int counter = 1;
    while (head) {
        if (head->isHole) {
            printHole(head, counter);

        } else {
            printProcess(head, counter);
        }

        counter++;
        head = head->next;
    }
}

Link createMemoryNode(bool isHole, int processID, int base, int limit) {

    Link newLink = (Link) malloc(sizeof(struct Node));

    if (newLink == NULL) {
        perror("Cannot allocate memory with a null pointer.");
        exit(MEMORY_ALLOCATION_ERROR);
    }

    newLink->isHole = isHole;
    newLink->processID = processID;
    newLink->base = base;
    newLink->limit = limit;
    newLink->next = NULL;

    return newLink;
}

Link createNodeWithNext(bool isHole, int processID, int base, int limit, Link nextNode) {

    Link link = createMemoryNode(isHole, processID, base, limit);
    link->next = nextNode;

    return link;
}

void push(Link *head, bool isHole, int processID, int base, int limit) {

    Link newHead = createNodeWithNext(isHole, processID, base, limit, *head);
    *head = newHead;
}

void mergeFreeBlocks(Link *head) {

    if (!(*head) || !(*head)->next) {
        return;
    }

    if ((*head)->isHole && (*head)->next->isHole) {

        Link nodeToMerge = (*head)->next;
        (*head)->limit = ((*head)->limit) + (nodeToMerge->limit);
        (*head)->next = nodeToMerge->next;
        free(nodeToMerge);
        return mergeFreeBlocks(head);
    }

    return mergeFreeBlocks(&(*head)->next);
}

Link getTail(Link head) {

    if (!head) {
        return NULL;
    }

    if (!head->next) {
        return head;
    }

    return getTail(head->next);
}

void calculateNewBase(Link *head) {

    if (!(*head)->next) {
        return;
    }

    (*head)->next->base = (*head)->base + (*head)->limit;

    return calculateNewBase(&(*head)->next);
}

void shiftFreeCells(Link *head) {

    if (!(*head) || !(*head)->next) {
        return;
    }

    if ((*head)->isHole) {

        mergeFreeBlocks(head);
        Link currentTail = getTail(*head);
        Link newTail = (*head);
        currentTail->next = newTail;

        *head = (*head)->next;
        (*head)->base = newTail->base;

        newTail->next = NULL;
        return shiftFreeCells((&(*head)->next));
    }

    return shiftFreeCells(&(*head)->next);
}

void compaction(Link *head) {
    shiftFreeCells(head);
    calculateNewBase(head);
}

void freeLinkedList(struct Node *head) {
    struct Node *temp;

    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {
    printf("\nlist starting and ending in Hole\n");
    printf("--------------------------------\n");
    Link node = createNodeWithNext(true, 0, 26, 6, NULL);
    Link node2 = createNodeWithNext(false, 3, 16, 10, node);
    Link node3 = createNodeWithNext(true, 0, 15, 1, node2);
    Link node4 = createNodeWithNext(true, 0, 11, 4, node3);
    Link node5 = createNodeWithNext(true, 0, 7, 4, node4);
    Link node6 = createNodeWithNext(false, 17, 6, 1, node5);
    Link head = createNodeWithNext(true, 1, 0, 6, node6);

    printMemory(head);
    printf("\n");
    compaction(&head);
    printMemory(head);
    printf("--------------------------------\n");

    printf("\nlist starting with process and ending in Hole\n");
    printf("--------------------------------\n");
    Link testNode = createNodeWithNext(true, 0, 26, 6, NULL);
    Link testNode2 = createNodeWithNext(false, 3, 16, 10, testNode);
    Link testNode3 = createNodeWithNext(true, 0, 7, 9, testNode2);
    Link testNode4 = createNodeWithNext(false, 2, 6, 1, testNode3);
    Link testHead = createNodeWithNext(false, 1, 0, 6, testNode4);

    printMemory(testHead);
    printf("\n");
    compaction(&testHead);
    printMemory(testHead);
    printf("--------------------------------\n");

    printf("\nOne process test\n");
    printf("----------------------------------\n");
    head = NULL;
    push(&head, false, 1, 0, 10);
    printMemory(head);
    printf("\n");
    compaction(&head);
    printMemory(head);
    printf("----------------------------------\n");


    printf("\ntwo hole test\n");
    printf("----------------------------------\n");
    head = NULL;
    push(&head, true, 0, 10, 12);
    push(&head, true, 0, 0, 10);
    printMemory(head);
    printf("\n");
    compaction(&head);
    printMemory(head);
    printf("----------------------------------\n");


    printf("\nlist starting with hole test\n");
    printf("----------------------------------\n");
    head = NULL;
    push(&head, true, 0, 47, 2);
    push(&head, false, 5, 46, 1);
    push(&head, false, 4, 37, 9);
    push(&head, true, 0, 35, 2);
    push(&head, true, 0, 34, 1);
    push(&head, true, 0, 28, 6);
    push(&head, false, 3, 18, 10);
    push(&head, true, 0, 17, 1);
    push(&head, false, 2, 16, 1);
    push(&head, false, 1, 10, 6);
    push(&head, true, 0, 0, 10);
    printMemory(head);
    printf("\n");
    compaction(&head);
    printMemory(head);
    printf("----------------------------------\n");


    printf("\nlist starting with process test\n");
    printf("----------------------------------\n");
    head = NULL;
    push(&head, true, 0, 47, 2);
    push(&head, false, 6, 46, 1);
    push(&head, false, 5, 37, 9);
    push(&head, true, 0, 35, 2);
    push(&head, true, 0, 34, 1);
    push(&head, true, 0, 28, 6);
    push(&head, false, 4, 18, 10);
    push(&head, true, 0, 17, 1);
    push(&head, false, 3, 16, 1);
    push(&head, false, 2, 10, 6);
    push(&head, false, 1, 0, 10);
    printMemory(head);
    printf("\n");
    compaction(&head);
    printMemory(head);
    printf("----------------------------------\n");

    printf("\nmergeFreeBlocks TESTS BEGIN HERE\n");
    printf("----------------------------------\n");
    printf("empty list test\n");
    head = NULL;
    printMemory(head);
    printf("\n");
    mergeFreeBlocks(&head);
    printMemory(head);
    printf("----------------------------------\n");


    printf("\ntwo process test\n");
    printf("----------------------------------\n");
    head = NULL;
    push(&head, false, 2, 10, 12);
    push(&head, false, 1, 0, 10);
    printMemory(head);
    printf("\n");
    mergeFreeBlocks(&head);
    printMemory(head);
    printf("----------------------------------\n");

    printf("\ntwo hole test\n");
    printf("----------------------------------\n");
    head = NULL;
    push(&head, true, 0, 10, 12);
    push(&head, true, 0, 0, 10);
    printMemory(head);
    printf("\n");
    mergeFreeBlocks(&head);
    printMemory(head);
    printf("----------------------------------\n");


    printf("\nlist starting with hole test\n");
    printf("----------------------------------\n");
    head = NULL;
    push(&head, true, 0, 49, 4);
    push(&head, true, 0, 47, 2);
    push(&head, false, 5, 46, 1);
    push(&head, false, 4, 37, 9);
    push(&head, true, 0, 35, 2);
    push(&head, true, 0, 34, 1);
    push(&head, true, 0, 28, 6);
    push(&head, false, 3, 18, 10);
    push(&head, true, 0, 17, 1);
    push(&head, false, 2, 16, 1);
    push(&head, false, 1, 10, 6);
    push(&head, true, 0, 0, 10);
    printMemory(head);
    printf("\n");
    mergeFreeBlocks(&head);
    printMemory(head);
    printf("----------------------------------\n");


    printf("\nlist starting with process test\n");
    printf("----------------------------------\n");
    head = NULL;
    push(&head, true, 0, 49, 4);
    push(&head, true, 0, 47, 2);
    push(&head, false, 6, 46, 1);
    push(&head, false, 5, 37, 9);
    push(&head, true, 0, 35, 2);
    push(&head, true, 0, 34, 1);
    push(&head, true, 0, 28, 6);
    push(&head, false, 4, 18, 10);
    push(&head, true, 0, 17, 1);
    push(&head, false, 3, 16, 1);
    push(&head, false, 2, 10, 6);
    push(&head, false, 1, 0, 10);
    printMemory(head);
    printf("\n");
    mergeFreeBlocks(&head);
    printMemory(head);
    printf("----------------------------------\n");

    freeLinkedList(head);
    freeLinkedList(testHead);

    return 0;
}
