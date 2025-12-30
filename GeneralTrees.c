/*  General Trees (GenTree + Forest) in C
    Based strictly on ADT operations/signature + traversal algorithm from Chap3/DT3.

    ADT signature: cons, root, Tree-list, empty-forest, ith, Tree-nb, insert
    (Chap3 - TREES) :contentReference[oaicite:4]{index=4}

    Traversal of a general tree (parcrs) :contentReference[oaicite:5]{index=5}
    Exercise 04 printing mapping for TPREF/TSUF/TERM/T(i) :contentReference[oaicite:6]{index=6}
*/

#include <stdio.h>
#include <stdlib.h>

/* ---------- Data Types (concrete representation) ---------- */
/* GenTree node: root value + Forest of children (sons ordered left->right) :contentReference[oaicite:7]{index=7} */

typedef int Node;   /* in course: Uses Node, int :contentReference[oaicite:8]{index=8} */

struct GenTree;     /* forward */

typedef struct Forest {
    struct GenTree **trees; /* array of pointers to GenTree */
    int nb;                 /* Tree-nb(F) */
} Forest;

typedef struct GenTree {
    Node x;     /* root content */
    Forest F;   /* Tree-list(A) */
} GenTree;

/* ---------- ADT Operations (as in the course signature) ---------- */

Forest empty_forest(void) { /* empty-forest : -> Forest :contentReference[oaicite:9]{index=9} */
    Forest f;
    f.trees = NULL;
    f.nb = 0;
    return f;
}

int Tree_nb(Forest f) { /* Tree-nb : Forest x -> int :contentReference[oaicite:10]{index=10} */
    return f.nb;
}

GenTree* ith(Forest f, int i) { /* ith : Forest x int -> GenTree :contentReference[oaicite:11]{index=11} */
    if (i < 1 || i > f.nb) return NULL; /* i is 1-based in the course algorithm */
    return f.trees[i - 1];
}

Node root(GenTree *A) { /* root : GenTree -> Node :contentReference[oaicite:12]{index=12} */
    return A->x;
}

Forest Tree_list(GenTree *A) { /* Tree-list : GenTree -> Forest :contentReference[oaicite:13]{index=13} */
    return A->F;
}

GenTree* cons(Node x, Forest f) { /* cons : Node x Forest -> GenTree :contentReference[oaicite:14]{index=14} */
    GenTree *A = (GenTree*)malloc(sizeof(GenTree));
    if (!A) { perror("malloc"); exit(1); }
    A->x = x;
    A->F = f;
    return A;
}

/* insert : Forest x int x GenTree -> Forest :contentReference[oaicite:15]{index=15}
   Behavior illustrated in the course example:
   - ith(F’, k=1/2)=ith(F,k)
   - ith(F’, k=3)=A
   - ith(F’, k=4)=ith(F,k-1) :contentReference[oaicite:16]{index=16}
*/
Forest insert(Forest f, int i, GenTree *A) {
    Forest fp;
    int new_nb = f.nb + 1;
    if (i < 1) i = 1;
    if (i > new_nb) i = new_nb;

    fp.nb = new_nb;
    fp.trees = (GenTree**)malloc(sizeof(GenTree*) * new_nb);
    if (!fp.trees) { perror("malloc"); exit(1); }

    /* copy elements before i */
    for (int k = 1; k < i; k++) {
        fp.trees[k - 1] = ith(f, k);
    }

    /* place A at position i */
    fp.trees[i - 1] = A;

    /* shift remaining */
    for (int k = i + 1; k <= new_nb; k++) {
        fp.trees[k - 1] = ith(f, k - 1);
    }

    return fp;
}

/* NOTE about delete:
   The course mentions "Possibility of extension ... delete a tree in a forest" but gives no algo :contentReference[oaicite:17]{index=17}
   So we do NOT implement it here under your strict rule.
*/

/* ---------- Traversal (parcrs) as in the course ---------- */

/* leaf(A) = (Tree-list(A) == empty-forest) :contentReference[oaicite:18]{index=18} */
int leaf(GenTree *A) {
    return (Tree_nb(Tree_list(A)) == 0);
}

/* Treatments (TPREF, TSUF, TERM, T(i)) are callbacks */
typedef struct Treatments {
    void (*TPREF)(GenTree *A);
    void (*TSUF)(GenTree *A);
    void (*TERM)(GenTree *A);
    void (*Ti)(GenTree *A, int i); /* T(i) */
} Treatments;

/* Void parcrs(GenTree A) { ... } :contentReference[oaicite:19]{index=19} */
void parcrs(GenTree *A, Treatments t) {
    int nb = Tree_nb(Tree_list(A));

    if (leaf(A)) {
        if (t.TERM) t.TERM(A);
    } else {
        if (t.TPREF) t.TPREF(A); /* treatment before seeing his sons */

        for (int i = 1; i < nb; i++) {
            parcrs(ith(Tree_list(A), i), t);
            if (t.Ti) t.Ti(A, i); /* T(i) */
        }

        parcrs(ith(Tree_list(A), nb), t);
        if (t.TSUF) t.TSUF(A);   /* treatment after seeing his sons */
    }
}

/* ---------- Exercise 04: printing expression mapping (DT3 Solution) ---------- */
/* TPREF -> print '(' + root
   TSUF  -> print ')'
   TERM  -> print '(' + root + ')'
   T(i)  -> nothing
   :contentReference[oaicite:20]{index=20}
*/

void TPREF_print(GenTree *A) {
    printf("(%d", root(A));
}
void TSUF_print(GenTree *A) {
    (void)A;
    printf(")");
}
void TERM_print(GenTree *A) {
    printf("(%d)", root(A));
}
void Ti_nothing(GenTree *A, int i) {
    (void)A; (void)i;
    /* nothing */
}

/* ---------- Demo: build a small General Tree and print ---------- */
int main(void) {
    /* Build leaves */
    GenTree *n5 = cons(5, empty_forest());
    GenTree *n6 = cons(6, empty_forest());
    GenTree *n7 = cons(7, empty_forest());

    /* Forest for node 1: <n5, n6, n7> */
    Forest F1 = empty_forest();
    F1 = insert(F1, 1, n5);
    F1 = insert(F1, 2, n6);
    F1 = insert(F1, 3, n7);
    GenTree *n1 = cons(1, F1);

    /* Node 8 leaf; node 2 has child 8 */
    GenTree *n8 = cons(8, empty_forest());
    Forest F2 = empty_forest();
    F2 = insert(F2, 1, n8);
    GenTree *n2 = cons(2, F2);

    /* Node 3 leaf */
    GenTree *n3 = cons(3, empty_forest());

    /* Root n0 has children: n1, n2, n3 (example subset of DT) */
    Forest F0 = empty_forest();
    F0 = insert(F0, 1, n1);
    F0 = insert(F0, 2, n2);
    F0 = insert(F0, 3, n3);
    GenTree *n0 = cons(0, F0);

    Treatments t;
    t.TPREF = TPREF_print;
    t.TSUF  = TSUF_print;
    t.TERM  = TERM_print;
    t.Ti    = Ti_nothing;

    parcrs(n0, t);
    printf("\n");

    return 0;
}
