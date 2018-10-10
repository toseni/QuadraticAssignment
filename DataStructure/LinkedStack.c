//
// Created by tomas on 18.10.7.
//

#include <stddef.h>
#include <stdlib.h>
#include "DataStructure.h"

void init(stack_node* head)
{
    head = NULL;
}

stack_node* push(stack_node* head, stack_data data)
{
    stack_node* tmp = (stack_node*)malloc(sizeof(stack_node));
    if(tmp == NULL)
    {
        exit(0);
    }

    tmp->data = data;
    tmp->next = head;
    head = tmp;
    return head;
}

stack_node* pop(stack_node *head, stack_data *element)
{
    stack_node* tmp = head;
    *element = head->data;
    head = head->next;
    free(tmp);
    return head;
}

int isEmpty(stack_node *head)
{
    return head == NULL ? 1 : 0;
}