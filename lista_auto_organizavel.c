#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct product
{   
    int serial;
    char name[64];
    float price;
}Product;

typedef struct node
{
    struct node *next;
    struct node *prev;
    Product *product;
    int count;
} Node;

typedef struct
{
    Node *begin;
    Node *end;
    int size;
    int total_cost;
} List;

Product *Product_create(int serial, const char *name, float price)
{
    Product *product = (Product*) calloc(1, sizeof(Product));
    product->serial = serial;
    product->price = price;
    strcpy(product->name, name);

    return product;
}

void Product_destroy(Product **product_ref)
{
    Product *product = *product_ref;
    free(product);
    *product_ref = NULL;
}

Node *Node_create(int serial, const char *name, float price)
{
    Node *node = (Node*) calloc(1,sizeof(Node));
    node->next = node->prev = NULL;
    node->count = 0;
    node->product = Product_create(serial, name, price);

    return node;
}

void Node_destroy(Node **node_ref) {
    Node *node = *node_ref;
    Product_destroy(&node->product);
    free(node);
    *node_ref = NULL;
}

List *List_create()
{
    List *list = (List*) calloc(1, sizeof(List));
    list->begin = list->end = NULL;
    list->size = 0;
    list->total_cost = 0;

    return list;
}

void List_destroy(List **L_ref) {
    List *L = *L_ref;

    Node *p = L->begin;
    Node *aux = NULL;

    while (p != NULL) {
        aux = p;
        p = p->next;
        Node_destroy(&aux);
    }
    free(L);

    *L_ref = NULL;
}

bool List_is_empty(List *list)
{
    return list->size == 0;
}

void List_add_last(List *list, int serial, const char *name, float price)
{
    Node *node = Node_create(serial,name,price);

    if (List_is_empty(list))
        list->begin = node;
    else
        list->end->next = node;

    node->prev = list->end;
    list->end = node;
    list->size++;

    printf("Product added\n\n");
}

void List_print(const List *list)
{
    Node *aux = list->begin;

    while (aux)
    {
        printf("%d-(%d) -> ",aux->product->serial, aux->count);
        aux = aux->next;
    }
    printf("NULL\nSize: %d",list->size);
}

void List_access(List *list, int serial)
{
    if (List_is_empty(list))
        printf("List is empty.\n\n");
    else
    {
        int cost = 1;
        Node *aux = list->begin;

        while (aux && aux->product->serial != serial) //buscando produto
        {
            aux = aux->next;
            cost++;
        }
           
        if (aux) //produto encontrado
        {
            aux->count++;
            list->total_cost += cost;

            if (list->begin != aux) //reorganizar lista
            {
                Node *q = aux->prev;

                while (q && q->count <= aux->count)
                {
                    if (q == list->begin)
                        list->begin = aux;
                    else
                        q->prev->next = aux;
                    
                    if (aux == list->end)
                        list->end = q;
                    else
                        aux->next->prev = q;

                    aux->prev = q->prev;
                    q->prev = aux;
                    q->next = aux->next;
                    aux->next = q;

                    q = aux->prev;
                }
            }

            //imprimindo produto
            printf("%d - %s - R$%.2f\n",aux->product->serial, aux->product->name, aux->product->price);
        }
        else
            printf("Product not found.\n\n");
    }
}

void List_output(const List *list)
{
    Node *aux = list->begin;

    printf("\nL = (");

    while (aux)
    {
        printf("%d",aux->product->serial);

        if (aux != list->end)
            printf(", ");
        
        aux = aux->next;
    }

    printf(")\n");
    printf("Total cost = %d\n",list->total_cost);
}
    
int main()
{
    char cmd[12];
    List *list = List_create();
    
    do{
        scanf("%s",cmd);

        if (strcmp(cmd,"add") == 0)
        {
            int serial;
            char name[64];
            float price;

            scanf("%d %s %f",&serial, name, &price);
            List_add_last(list,serial,name,price);

        }
        else
        {
            if (strcmp(cmd,"access") == 0)
            {
                int serial;
                scanf("%d",&serial);
                List_access(list,serial);
            }
        }
    } while (strcmp("para",cmd) != 0);
    
    List_output(list);
    List_destroy(&list);

    return 0;
}