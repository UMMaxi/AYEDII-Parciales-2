#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include <assert.h>

#include "pokerdeck.h"

struct s_pokerdck 
{
    struct s_node *first;
    unsigned int cantidad; // Cantidad de cartas que hay en el mazo
};

struct s_node 
{
    cardnum_t num;
    cardsuit_t suit;
    struct s_node *nextcard;
};

typedef struct s_node * node_t;

// Auxiliar dump functions
static void dump_suit(cardsuit_t suit) 
{
    printf("%s", suit==spades   ? "♠️  ":
                 suit==hearts   ? "♥️  ":
                 suit==diamonds ? "♦️  ":
                 suit==clubs    ? "♣️  ": "???");
}

static void dump_num(cardnum_t num) 
{
    if (1 < num && num < 11) {
        printf("%-2d", num);
    } else {
        printf("%-2s", num==1 ? "A":
                       num==11 ? "J":
                       num==12 ? "Q":
                       num==13 ? "K": "???");

    }
}

// Representation invariant
/*static bool invrep(pokerdeck deck) 
{
    // Chequeo que sea un mazo valido
    node_t nodo;
    bool valid=false;
    if (deck == NULL)
    {
        valid = false;
    }
    else
    {
        nodo = deck->first;
        unsigned int i;
        i = 0;
        while (nodo != NULL)
        {
            i = i + 1;
            nodo = nodo->nextcard;
        }
    }
    // Me fijo si la cantidad de cartas son iguales a la que indica el mazo

    
    if (i != deck->cantidad)
    {
        valid = false;
    }
    
    return valid;
}
*/
static bool invrep(pokerdeck deck) {
    bool valid=false;
    if (deck == NULL) valid = false;

    if (deck->first == NULL) {
        valid = deck->cantidad == 0;
    }

    // verif si la cantidad de nodos coincide con deck->size
    unsigned int count = 0;
    node_t current = deck->first;

    while (current != NULL) {
        count++;
        current = current->nextcard;
    }
    valid = (count == deck->cantidad);


    return valid;
}

// Creates a single node in dynamic memory with a given card
static node_t create_node(cardnum_t num, cardsuit_t suit) 
{
    node_t newnode;

    newnode = malloc(sizeof(struct s_node));
    newnode->nextcard = NULL;
    newnode->num = num;
    newnode->suit = suit;

    assert(newnode!=NULL);
    return newnode;
}

// Destroy a single node
static node_t destroy_node(node_t node) 
{

    free(node);
    node = NULL;

    assert(node==NULL);
    return node;
}

// Public functions of the Abstract Data Type
pokerdeck pokerdeck_empty(void) 
{
    pokerdeck mazo_vacio;
    mazo_vacio = malloc(sizeof(struct s_pokerdck));
    mazo_vacio->first = NULL;
    mazo_vacio->cantidad = 0;
    //assert(invrep(mazo_vacio));
    return mazo_vacio;
}

bool pokerdeck_is_empty(pokerdeck deck) 
{

    return ((deck->first == NULL) && (deck->cantidad == 0));
}

pokerdeck pokerdeck_add(pokerdeck deck, cardnum_t num, cardsuit_t suit) 
{
    node_t newcart;
    newcart = create_node(num, suit);

    if (deck->first == NULL)
    {
        deck->first = newcart;
    }
    else
    {
        node_t nodo;
        nodo = deck->first;
        while (nodo->nextcard != NULL)
        {
            nodo = nodo->nextcard;
        }
        nodo->nextcard = newcart;
    }
    
    deck->cantidad = deck->cantidad + 1;
   // assert(invrep(deck));

    return deck;
}

pokerdeck pokerdeck_push(pokerdeck deck,  cardnum_t num, cardsuit_t suit) 
{
    assert(invrep(deck));
    node_t newnode;
    newnode = create_node(num, suit);
    newnode->nextcard = deck->first;
    deck->first = newnode;
    deck->cantidad = deck->cantidad + 1;

    assert(invrep(deck) && !pokerdeck_is_empty(deck));
    return deck;
}

pokerdeck pokerdeck_pop(pokerdeck deck, cardnum_t *popped_num, cardnum_t *popped_suit) 
{
    node_t newnode;
    newnode = deck->first;
    deck->first = newnode->nextcard;
    newnode->nextcard = NULL;

    if (popped_num!=NULL) *popped_num = newnode->num;
    if (popped_suit!=NULL) *popped_suit= newnode->suit;

    destroy_node(newnode);
    deck->cantidad = deck->cantidad - 1;

    return deck;
}

unsigned int pokerdeck_length(pokerdeck deck) 
{
    assert(invrep(deck));
    return deck->cantidad;
}

pokerdeck pokerdeck_remove(pokerdeck deck, cardnum_t num, cardsuit_t suit) 
{
    node_t actual;
    node_t anterior;
    actual = deck->first;
    anterior = NULL;

    bool eliminada = false;

    while ((actual != NULL) && (eliminada == false))
    {
        if ((actual->num == num) && (actual->suit == suit))
        {
            if (anterior == NULL)
            {
                deck->first = actual->nextcard;
            }
            else
            {
                anterior->nextcard = actual->nextcard;
            }
            actual->nextcard = NULL;
            actual = destroy_node(actual);

            eliminada = true;
            deck->cantidad = deck->cantidad - 1;
        }
        else
        {   
            anterior = actual;
            actual = actual->nextcard;
        }
        
    }
    
    return deck;
}

unsigned int pokerdeck_count(pokerdeck deck, cardsuit_t suit)
{
    node_t node;
    unsigned int count;
    node = deck->first;
    count = 0;

    while (node->nextcard != NULL)
    {
        if (node->suit == suit)
        {
            count = count + 1;
        }
        node = node->nextcard;
    }
        
    return count;
}

struct card * pokerdeck_to_array(pokerdeck deck) 
{
    struct card *array = NULL;

    if (deck->cantidad > 0)
    {
        array = malloc(deck->cantidad * sizeof(struct card));
        node_t node;
        unsigned int i = 0;
        node = deck->first;

        while (node->nextcard != NULL)
        {
            array[i].num = node->num;
            array[i].suit = node->suit;
            node = node->nextcard;
            i = i + 1;
        }    
    }
    
    return array;
}

void card_dump(cardnum_t num, cardsuit_t suit) 
{
    printf("|");
    dump_num(num);
    printf(" ");
    dump_suit(suit);
    printf("|");
}

void pokerdeck_dump(pokerdeck deck) 
{
    assert(invrep(deck));
    node_t node = deck->first;
    while (node != NULL) {
        card_dump(node->num, node->suit);
        printf(" ");
        node = node->nextcard;
    }
    printf("\n");
}

pokerdeck pokerdeck_destroy(pokerdeck deck) 
{
    node_t actual;

    actual = deck->first;

    while (actual != NULL)
    {
        node_t siguiente;
        siguiente = actual->nextcard;
        destroy_node(actual);
        actual = siguiente; 
    }
    
    free(deck);
    return NULL;
}

