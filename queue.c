#include "queue.h"

// int pop(List_T queue)
// {       
//         int x = 6;
//         queue = List_pop(queue, (void **)&x);

//         fprintf(stderr, "Inside pop: %d\n", x);
//         return x;
// }

int pop(List_T *queue) {
        int x;
        void *element_ptr;
        
        if (List_length(*queue) == 0) {
                return -1;
        }
        
        *queue = List_pop(*queue, &element_ptr);
        if (element_ptr != NULL) {
            x = *(int *)element_ptr;
            return x;
        }
        // Handle error case
        return -1;
}

void push(List_T *queue, int *x)
{
        List_T new_element = List_list((void *)x, NULL);
        *queue = List_append(*queue, new_element);
}

int main()
{       
        int a = 1;
        int b = 2;
        int c = 3;
        int d = 10;
        int e = 45;

        List_T queue = List_list((void *)&a, (void *)&b, (void *)&c, NULL);
        int first = pop(&queue);
        fprintf(stderr, "Final answer: %d\n", first);

        int second = pop(&queue);
        fprintf(stderr, "Final answer: %d\n", second);

        int third = pop(&queue);
        fprintf(stderr, "Final answer: %d\n", third);

        int fourth = pop(&queue);
        fprintf(stderr, "Final answer: %d\n", fourth);

        push(&queue, (void *)&d);
        push(&queue, (void *)&e);

        int fifth = pop(&queue);
        fprintf(stderr, "Final answer: %d\n", fifth);

        int sixth = pop(&queue);
        fprintf(stderr, "Final answer: %d\n", sixth);

        List_free(&queue);
}
