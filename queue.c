#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (!q) {
        return NULL;
    }
    q->tail = q->head = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q)
        return;
    list_ele_t *c, *n;
    c = q->head;
    while (c != NULL) {
        n = c->next;
        free(c->value);
        free(c);
        c = n;
    }
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    list_ele_t *newh;
    int value_size;

    if (!q || !s) {
        return false;
    }

    newh = malloc(sizeof(list_ele_t));
    if (newh == NULL) {
        return false;
    }
    value_size = strlen(s);
    char *value_new = malloc(sizeof(char) * (value_size + 1));
    if (!value_new) {
        free(newh);
        return false;
    }
    strncpy(value_new, s, value_size);
    value_new[value_size] = '\0';

    newh->value = value_new;
    if (!q->head) {
        q->head = q->tail = newh;
        newh->next = NULL;
    } else {
        newh->next = q->head;
        q->head = newh;
    }
    q->size += 1;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    list_ele_t *newt;
    int value_size;

    if (!q || !s) {
        return false;
    }
    newt = malloc(sizeof(list_ele_t));
    if (newt == NULL) {
        return false;
    }
    value_size = strlen(s);
    char *value_new = malloc(sizeof(char) * (value_size + 1));
    if (!value_new) {
        free(newt);
        return false;
    }
    strncpy(value_new, s, value_size);
    value_new[value_size] = '\0';

    newt->value = value_new;
    newt->next = NULL;

    if (!q->tail) {
        q->head = q->tail = newt;
    } else {
        q->tail->next = newt;
        q->tail = newt;
    }

    q->size += 1;

    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    list_ele_t *tmph;
    if (!q || !q->head) {
        return false;
    }
    tmph = q->head;
    if (sp != NULL) {
        memset(sp, 0, bufsize);
        strncpy(sp, tmph->value, bufsize - 1);
    }

    q->head = q->head->next;
    free(tmph->value);
    free(tmph);
    q->size -= 1;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (!q) {
        return 0;
    }
    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    list_ele_t *p, *c, *n;
    if (q == NULL || q->head == NULL) {
        return;
    }
    p = q->head;
    c = p->next;
    q->tail = p;
    while (c != NULL) {
        n = c->next;
        c->next = p;
        p = c;
        c = n;
    }
    q->tail->next = NULL;
    q->head = p;
}


list_ele_t *q_merge(list_ele_t *left, list_ele_t *right)
{
    list_ele_t tmp_head;
    list_ele_t *head;
    head = &tmp_head;

    while (left && right) {
        if (strcasecmp(left->value, right->value) <= 0) {
            head->next = left;
            left = left->next;
        } else {
            head->next = right;
            right = right->next;
        }
        head = head->next;
    }

    if (left) {
        head->next = left;
    }

    if (right) {
        head->next = right;
    }

    return tmp_head.next;
}

list_ele_t *q_merge_sort(list_ele_t *head)
{
    if (head == NULL || head->next == NULL) {
        return head;
    }

    // find middle
    list_ele_t *slow;
    list_ele_t *fast;
    slow = head;
    fast = slow->next;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    // split list
    fast = slow->next;
    slow->next = NULL;
    list_ele_t *left = q_merge_sort(head);
    list_ele_t *right = q_merge_sort(fast);

    return q_merge(left, right);
}


/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q || !q->head) {
        return;
    }
    q->head = q_merge_sort(q->head);
}
