/*
 * Code for basic C skills diagnostic.
 * Developed for courses 15-213/18-213/15-513 by R. E. Bryant, 2017
 * Modified to store strings, 2018
 */

/*
 * This program implements a queue supporting both FIFO and LIFO
 * operations.
 *
 * It uses a singly-linked list to represent the set of queue elements
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

#define Q_RETURN_IF_NULL(ptr, retval) \
    do {                              \
        if (ptr == NULL)              \
            return retval;            \
    } while (0)

/*
  Create empty queue.
  Return NULL if could not allocate space.
*/
queue_t *q_new()
{
    queue_t *q = (queue_t *) malloc(sizeof(queue_t));

    if (q != NULL) {
        q->head = NULL;
        q->indirect_tail = &q->head;
        q->size = 0;
    }
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (q != NULL) {
        list_ele_t *next;

        while (q->head != NULL) {
            next = q->head->next;
            free(q->head->value);
            free(q->head);
            q->head = next;
        }
        free(q);
    }
}

list_ele_t *q_insert_prologue(queue_t *q, char *s)
{
    Q_RETURN_IF_NULL(q, NULL);

    list_ele_t *new_node = (list_ele_t *) malloc(sizeof(list_ele_t));
    size_t length;

    Q_RETURN_IF_NULL(new_node, NULL);

    length = strlen(s) + 1;  // +1 for NULL byte
    new_node->value = (char *) malloc(length * sizeof(char));
    if (new_node->value != NULL) {
        memcpy(new_node->value, s, length);
        return new_node;
    } else {
        free(new_node);
        return NULL;
    }
}

/*
  Attempt to insert element at head of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
  Argument s points to the string to be stored.
  The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    list_ele_t *new_node;

    Q_RETURN_IF_NULL((new_node = q_insert_prologue(q, s)), false);
    new_node->next = q->head;
    q->head = new_node;
    if (q->size == 0) {
        q->indirect_tail = &new_node->next;
    }
    ++q->size;
    return true;
}


/*
  Attempt to insert element at tail of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
  Argument s points to the string to be stored.
  The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    list_ele_t *new_node;

    Q_RETURN_IF_NULL((new_node = q_insert_prologue(q, s)), false);
    new_node->next = NULL;
    *(q->indirect_tail) = new_node;
    q->indirect_tail = &new_node->next;
    ++q->size;
    return true;
}

/*
  Attempt to remove element from head of queue.
  Return true if successful.
  Return false if queue is NULL or empty.
  If sp is non-NULL and an element is removed, copy the removed string to *sp
  (up to a maximum of bufsize-1 characters, plus a null terminator.)
  The space used by the list element and the string should be freed.
*/
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (q != NULL && q->size != 0) {
        list_ele_t *ptr;

        ptr = q->head;
        q->head = q->head->next;
        if (--q->size == 0) {
            q->indirect_tail = &q->head;
        }

        if (sp != NULL) {
            strncpy(sp, ptr->value, bufsize - 1);
            sp[bufsize - 1] = '\0';
        }
        free(ptr->value);
        free(ptr);

        return true;
    } else {
        return false;
    }
}

/*
  Return number of elements in queue.
  Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    return (q == NULL) ? 0 : q->size;
}

/*
  Reverse elements in queue
  No effect if q is NULL or empty
  This function should not allocate or free any list elements
  (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
  It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (q != NULL && q->size > 1) {
        list_ele_t *prev = q->head, *next;

        q->indirect_tail = &prev->next;
        q->head = q->head->next;
        *q->indirect_tail = NULL;
        while (q->head != NULL) {
            next = q->head->next;
            q->head->next = prev;
            prev = q->head;
            q->head = next;
        }
        q->head = prev;
    }
}
