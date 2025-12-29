#include <stdio.h>
#include <stdlib.h>

/* ===== BST Node / Tree ===== */
typedef int Element;

typedef struct Node {
    Element val;
    struct Node* g;   /* left  */
    struct Node* d;   /* right */
} Node;

typedef Node* Tree;

/* empty-tree */
static Tree empty_tree(void) { return NULL; }

/* cons <x, left, right> */
static Tree cons(Element x, Tree left, Tree right) {
    Tree n = (Tree)malloc(sizeof(Node));
    if (!n) {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }
    n->val = x;
    n->g = left;
    n->d = right;
    return n;
}

/* ===== Traversals (Parcours) =====
   Prefix : P-G-D
   Infix  : G-P-D  (gives ascending order for BST)
   Postfix: G-D-P
*/
static void print_prefix(Tree A) {
    if (A == NULL) return;
    printf("%d ", A->val);
    print_prefix(A->g);
    print_prefix(A->d);
}

static void print_infix(Tree A) {
    if (A == NULL) return;
    print_infix(A->g);
    printf("%d ", A->val);
    print_infix(A->d);
}

static void print_postfix(Tree A) {
    if (A == NULL) return;
    print_postfix(A->g);
    print_postfix(A->d);
    printf("%d ", A->val);
}

/* ===== BST Insertion (addition to leaves) =====
   Rule from course:
   left subtree: <= v
   right subtree: > v
*/
static Tree bst_insert(Tree A, Element x) {
    if (A == NULL) return cons(x, NULL, NULL);

    if (x <= A->val) A->g = bst_insert(A->g, x);
    else            A->d = bst_insert(A->d, x);

    return A;
}

/* ===== BST Search ===== */
static int bst_contains(Tree A, Element x) {
    while (A != NULL) {
        if (x == A->val) return 1;
        if (x <= A->val) A = A->g;
        else             A = A->d;
    }
    return 0;
}

/* ===== Helper: find minimum node in a BST ===== */
static Tree bst_min_node(Tree A) {
    if (A == NULL) return NULL;
    while (A->g != NULL) A = A->g;
    return A;
}

/* ===== BST Deletion =====
   Cases:
   1) leaf -> remove
   2) one child -> replace by child
   3) two children -> replace by successor (min of right subtree)
*/
static Tree bst_delete(Tree A, Element x) {
    if (A == NULL) return NULL;

    if (x < A->val) {
        A->g = bst_delete(A->g, x);
    } else if (x > A->val) {
        A->d = bst_delete(A->d, x);
    } else {
        /* Found node to delete */
        if (A->g == NULL && A->d == NULL) {
            free(A);
            return NULL;
        }
        if (A->g == NULL) {
            Tree tmp = A->d;
            free(A);
            return tmp;
        }
        if (A->d == NULL) {
            Tree tmp = A->g;
            free(A);
            return tmp;
        }

        /* two children: successor = min(right subtree) */
        Tree succ = bst_min_node(A->d);
        A->val = succ->val;
        /* delete successor value from right subtree */
        A->d = bst_delete(A->d, succ->val);
    }
    return A;
}

/* ===== Free whole tree ===== */
static void bst_free(Tree A) {
    if (A == NULL) return;
    bst_free(A->g);
    bst_free(A->d);
    free(A);
}

/* ===== Demo main =====
   You can replace values with the set from DT3 exercise.
*/
int main(void) {
    Tree A = empty_tree();

    /* Example: insert values */
    int values[] = {25, 60, 35, 10, 5, 20, 65, 45, 30, 15};
    int n = (int)(sizeof(values) / sizeof(values[0]));

    for (int i = 0; i < n; i++) {
        A = bst_insert(A, values[i]);
    }

    printf("Infix  (sorted): ");
    print_infix(A);
    printf("\n");

    printf("Prefix: ");
    print_prefix(A);
    printf("\n");

    printf("Postfix: ");
    print_postfix(A);
    printf("\n");

    /* Deletion + insertion example (like DT3 style) */
    A = bst_delete(A, 60);
    A = bst_insert(A, 22);

    printf("\nAfter delete 60 and insert 22:\n");
    printf("Infix  (sorted): ");
    print_infix(A);
    printf("\n");

    bst_free(A);
    return 0;
}
