#include <stdio.h>
#include <stdlib.h>

typedef int Element;

typedef struct Node {
    Element val;
    struct Node* g;   // left
    struct Node* d;   // right
} Node;

typedef Node* Tree;

/* empty-tree() = NULL */
static Tree empty_tree(void) { return NULL; }

/* <o, B1, B2>  (cons) */
static Tree cons(Element x, Tree left, Tree right) {
    Tree n = (Tree)malloc(sizeof(Node));
    if (!n) { perror("malloc"); exit(1); }
    n->val = x;
    n->g = left;
    n->d = right;
    return n;
}

/* ===== Traversals (Parcours) =====
   Prefix  : P-G-D
   Infix   : G-P-D
   Postfix : G-D-P
   (من الكورس: Parcours عام ثم تخصيصات) :contentReference[oaicite:16]{index=16}
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

/* ===== Measurements ===== */

/* Size */
static int size_tree(Tree A) {
    if (A == NULL) return 0;
    return 1 + size_tree(A->g) + size_tree(A->d);
}

/* Height (Depth of tree) : h(B)=max depth */
static int height_tree(Tree A) {
    if (A == NULL) return -1;          // حتى تكون ورقة height=0
    int hg = height_tree(A->g);
    int hd = height_tree(A->d);
    return 1 + (hg > hd ? hg : hd);
}

/* Leaf? */
static int is_leaf(Tree A) {
    return A != NULL && A->g == NULL && A->d == NULL;
}

/* LCE: sum of depths of leaves */
static int lce(Tree A, int depth) {
    if (A == NULL) return 0;
    if (is_leaf(A)) return depth;
    return lce(A->g, depth + 1) + lce(A->d, depth + 1);
}

/* LCI: sum of depths of internal nodes */
static int lci(Tree A, int depth) {
    if (A == NULL) return 0;
    if (is_leaf(A)) return 0;
    return depth + lci(A->g, depth + 1) + lci(A->d, depth + 1);
}

/* Count leaves (for PE) */
static int count_leaves(Tree A) {
    if (A == NULL) return 0;
    if (is_leaf(A)) return 1;
    return count_leaves(A->g) + count_leaves(A->d);
}

/* Free */
static void free_tree(Tree A) {
    if (A == NULL) return;
    free_tree(A->g);
    free_tree(A->d);
    free(A);
}

/* ===== Demo ===== */
int main(void) {
    /* مثال شجرة بسيطة:
            1
           / \
          2   3
             /
            4
    */
    Tree A = cons(1,
                  cons(2, empty_tree(), empty_tree()),
                  cons(3, cons(4, empty_tree(), empty_tree()), empty_tree()));

    printf("Prefix  (P-G-D): ");
    print_prefix(A);  printf("\n");

    printf("Infix   (G-P-D): ");
    print_infix(A);   printf("\n");

    printf("Postfix (G-D-P): ");
    print_postfix(A); printf("\n");

    int S  = size_tree(A);
    int H  = height_tree(A);
    int LCE = lce(A, 0);
    int LCI = lci(A, 0);
    int LC  = LCE + LCI;
    int F   = count_leaves(A);
    double PE = (F == 0) ? 0.0 : ((double)LCE / (double)F);

    printf("\nSize=%d\nHeight=%d\nLCE=%d\nLCI=%d\nLC=%d\nLeaves=%d\nPE=%.2f\n",
           S, H, LCE, LCI, LC, F, PE);

    free_tree(A);
    return 0;
}