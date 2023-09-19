// list/list.c
// 
// Implementation for linked list.
//
// <Author>

#include <stdio.h>
#include <stdlib.h>
//#include <string.h>

#include "list.h"

list_t *list_alloc() {
    list_t *l = (list_t *)malloc(sizeof(list_t));
    if (l == NULL) {
        perror("Error allocating memory for the list");
        exit(EXIT_FAILURE);
    }
    l->head = NULL;
    return l;
}

// Function to free the memory occupied by the list
void list_free(list_t *l) {
    if (l == NULL) {
        return; // Nothing to free if the list is already NULL
    }

    node_t* current = l->head;
    while (current != NULL) {
        node_t* next = current->next;
        free(current);
        current = next;
    }

    free(l);
}

// Function to print the elements of the list
void list_print(list_t *l) {
    if (l == NULL || l->head == NULL) {
        printf("Empty List\n");
        return;
    }

    node_t* current = l->head;
    while (current != NULL) {
        printf("%d -> ", current->value);
        current = current->next;
    }
    printf("NULL\n");
}

// Function to get the length of the list
int list_length(list_t *l) {
    int length = 0;
    if (l == NULL) {
        return length;
    }

    node_t* current = l->head;
    while (current != NULL) {
        length++;
        current = current->next;
    }

    return length;
}

// Function to add an element to the back of the list
void list_add_to_back(list_t *l, elem value) {
    if (l == NULL) {
        return;
    }

    node_t* new_node = (node_t *)malloc(sizeof(node_t));
    if (new_node == NULL) {
        perror("Error allocating memory for a new node");
        exit(EXIT_FAILURE);
    }

    new_node->value = value;
    new_node->next = NULL;

    if (l->head == NULL) {
        l->head = new_node;
    } else {
        node_t* current = l->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
}

// Function to add an element to the front of the list
void list_add_to_front(list_t *l, elem value) {
    if (l == NULL) {
        return;
    }

    node_t* new_node = (node_t *)malloc(sizeof(node_t));
    if (new_node == NULL) {
        perror("Error allocating memory for a new node");
        exit(EXIT_FAILURE);
    }

    new_node->value = value;
    new_node->next = l->head;
    l->head = new_node;
}

// Function to add an element at a specific index in the list
void list_add_at_index(list_t *l, elem value, int index) {
    if (l == NULL || index < 0) {
        return;
    }

    if (index == 0) {
        list_add_to_front(l, value);
        return;
    }

    int current_index = 0;
    node_t* current = l->head;
    while (current != NULL && current_index < index - 1) {
        current = current->next;
        current_index++;
    }

    if (current == NULL) {
        return; // Index is out of bounds
    }

    node_t* new_node = (node_t *)malloc(sizeof(node_t));
    if (new_node == NULL) {
        perror("Error allocating memory for a new node");
        exit(EXIT_FAILURE);
    }

    new_node->value = value;
    new_node->next = current->next;
    current->next = new_node;
}

// Function to remove an element from the back of the list
elem list_remove_from_back(list_t *l) {
    if (l == NULL || l->head == NULL) {
        return -1;
    }

    if (l->head->next == NULL) {
        elem value = l->head->value;
        free(l->head);
        l->head = NULL;
        return value;
    }

    node_t* current = l->head;
    while (current->next->next != NULL) {
        current = current->next;
    }

    elem value = current->next->value;
    free(current->next);
    current->next = NULL;
    return value;
}

// Function to remove an element from the front of the list
elem list_remove_from_front(list_t *l) {
    if (l == NULL || l->head == NULL) {
        return -1;
    }

    elem value = l->head->value;
    node_t* temp = l->head;
    l->head = l->head->next;
    free(temp);
    return value;
}

// Function to remove an element at a specific index in the list
elem list_remove_at_index(list_t *l, int index) {
    if (l == NULL || index < 0 || l->head == NULL) {
        return -1;
    }

    if (index == 0) {
        return list_remove_from_front(l);
    }

    int current_index = 0;
    node_t* current = l->head;
    while (current->next != NULL && current_index < index - 1) {
        current = current->next;
        current_index++;
    }

    if (current->next == NULL) {
        return -1; // Index is out of bounds
    }

    elem value = current->next->value;
    node_t* temp = current->next;
    current->next = current->next->next;
    free(temp);
    return value;
}

// Function to check if an element is in the list
bool list_is_in(list_t *l, elem value) {
    if (l == NULL || l->head == NULL) {
        return false;
    }

    node_t* current = l->head;
    while (current != NULL) {
        if (current->value == value) {
            return true;
        }
        current = current->next;
    }

    return false;
}

// Function to get the element at a specific index in the list
elem list_get_elem_at(list_t *l, int index) {
    if (l == NULL || index < 0 || l->head == NULL) {
        return -1;
    }

    int current_index = 0;
    node_t* current = l->head;
    while (current != NULL && current_index < index) {
        current = current->next;
        current_index++;
    }

    if (current == NULL) {
        return -1; // Index is out of bounds
    }

    return current->value;
}

int list_get_index_of(list_t *l, elem value) {
    if (l == NULL || l->head == NULL) {
        return -1; // List is empty or NULL
    }

    int index = 0;
    node_t* current = l->head;

    while (current != NULL) {
        if (current->value == value) {
            return index; // Element found at this index
        }
        current = current->next;
        index++;
    }

    return -1; // Element not found in the list
}


