


enum type {empty = -1, num = 0, var = 1, oper = 2, important_oper = 3};

enum op {add = '+', sub = '-', mul = '*', divi = '/', pow = '^'};

struct Node {

    enum type TYPE = empty;

    union {
        int numvalue;
        char *varvalue;
        enum op value;
    } data;

    Node *left = NULL;
    Node *right = NULL;
    Node *parent = NULL;
};


Node* difroot = NULL;

bool notfirst = false;

int n = 0;

Node* NewNum (int newdata);

Node* NewOp  (op Opname, Node* L, Node* R);

Node* NewVar (char *name);

Node* copy (Node* old, Node* newparent);

void print_inorder (Node *node, FILE *output);

void print_text    (Node *node);

int simplify_tree (Node **node);

void become_left (Node **node, int *changes);

void become_right (Node **node, int *changes);

#include <Windows.h>

void Graph_def (Node *node);

void Graph_tree (Node *node, FILE *output);




void Graph_dif (Node *node) {

    notfirst = false;

    FILE *output = fopen ("outputdif.txt", "wb");

    fprintf (output, "digraph G\n");
    fprintf (output, "{\n");
    fprintf (output, "graph [dpi = 150];\n");

    Graph_tree (node, output);

    fprintf (output, "}\n");

    fclose (output);

    system("dot outputdif.txt -T png -o resdif.png");

    system("resdif.png");

    Sleep(1000);

    //system("taskkill /fi \"windowtitle eq resdef.png*\""); //taskkill /fi "windowtitle eq resdef.png*"
}




void Graph_tree (Node *node, FILE *output) {

    if (node -> TYPE == num) fprintf (output, "%d[shape = record, style=\"filled\", fillcolor = \"lightcyan2\", label = \"%d | %p\"];\n", node, node -> data.numvalue, (void*) node);

    else if (node -> TYPE == oper) fprintf (output, "%d[shape = record, style=\"filled\", fillcolor = \"pink2\", label = \"%c | %p\"];\n", node, (char) (node -> data.value), (void*) node);

    else if (node -> TYPE == important_oper) fprintf (output, "%d[shape = record, style=\"filled\", fillcolor = \"palegreen1\", label = \"%c | %p\"];\n", node, (char) (node -> data.value), (void*) node);

    else if (node -> TYPE == var) fprintf (output, "%d[shape = record, style=\"filled\", fillcolor = \"burlywood1\", label = \"%s | %p\"];\n", node, node -> data.varvalue, (void*) node);

    if (node -> parent != NULL && notfirst) fprintf (output, "%d->%d;\n", (node -> parent), node);

    notfirst = true;

    if (node -> left != NULL)  Graph_tree (node -> left,  output);

    if (node -> right != NULL) Graph_tree (node -> right, output);

}






Node* NewNum (int newdata) {

    Node *node = (Node *) calloc (1, sizeof (Node));

    node -> TYPE = num;

    node -> data.numvalue = newdata;

    return node;

}



Node* NewOp (op Opname, Node* L, Node* R) {

    Node *node = (Node *) calloc (1, sizeof (Node));

    if (Opname == add || Opname == sub) node -> TYPE = oper;

    else if (Opname == mul || Opname == divi) node -> TYPE = important_oper;

    node -> data.value = (op) Opname;

    node -> left = L;
    L -> parent = node;

    node -> right = R;
    R -> parent = node;

    return node;

}



Node* NewVar (char *name) {

    Node *node = (Node *) calloc (1, sizeof (Node));

    node -> TYPE = var;

    node -> data.varvalue = name;

    return node;
}



Node* copy (Node* old, Node* newparent) {

    if (old == NULL) return NULL;

    Node *newnode = (Node *) calloc (1, sizeof (Node));

    newnode -> TYPE = old -> TYPE;

    newnode -> data = old -> data;

    newnode -> parent = newparent;

    newnode -> left = copy (old -> left, newnode);

    newnode -> right = copy (old -> right, newnode);

    return newnode;
}


void print_text (Node *node) {

    FILE *output = fopen ("outputtext.txt", "wb");

    print_inorder (node, output);

    fclose (output);
}


void print_inorder (Node *node, FILE *output) {

    if      (node -> TYPE == num) fprintf (output, "%d", node -> data.numvalue);

    else if (node -> TYPE == var) fprintf (output, "%s", node -> data.varvalue);

    else if (node -> TYPE == oper || node -> TYPE == important_oper) {

        bool brackets = false; bool subbrackets = false;

        if ((node -> parent != NULL) && ((node -> parent) -> TYPE == important_oper) && ((node -> TYPE == oper)|| (node -> parent) -> data.value == (op) divi)) brackets = true;

        if (brackets) fprintf (output, "(");

        assert (node -> left != NULL);

        print_inorder (node -> left, output);

        fprintf (output, " %c ", (char) (node -> data.value));

        assert (node -> right != NULL);

        if (node -> data.value == (op) sub && (node -> right) -> TYPE != num && (node -> right) -> TYPE != var) subbrackets = true;

        if (subbrackets) fprintf (output, "(");

        print_inorder (node -> right, output);

        if (subbrackets) fprintf (output, ")");

        if (brackets) fprintf (output, ")");
    }
}


void become_left (Node **node, int *changes) {

    Node *tmp = *node;
    *node = ((*node) -> left);
    ((*node) -> parent) = tmp -> parent;
    free ((void*) (tmp -> right));
    free ((void*) (tmp));
    (*changes)++;
}


void become_right (Node **node, int *changes) {

    Node *tmp = *node;
    *node = ((*node) -> right);
    ((*node) -> parent) = tmp -> parent;
    free ((void*) (tmp -> left));
    free ((void*) (tmp));
    (*changes)++;
}


int simplify_tree (Node **node) {

    int changes = 0;

    if ((*node) -> TYPE == oper || (*node) -> TYPE == important_oper) {

        int changesL = simplify_tree (&((*node) -> left));

        int changesR = simplify_tree (&((*node) -> right));

        if (((*node) -> left) -> TYPE == num && ((*node) -> right) -> TYPE == num) {

            changes++;

            int L = ((*node) -> left) -> data.numvalue;

            int R = ((*node) -> right) -> data.numvalue;

            (*node) -> TYPE = num;

            free ((void*) ((*node) -> left)); (*node) -> left = NULL;

            free ((void*) ((*node) -> right)); (*node) -> right = NULL;

            switch ((*node) -> data.value) {

            case add:
                (*node) -> data.numvalue = L + R;
                break;

            case sub:
                (*node) -> data.numvalue = L - R;
                break;

            case mul:
                (*node) -> data.numvalue = L * R;
                break;

            case divi:
                (*node) -> data.numvalue = L / R;
                break;
            }
        }

        else if ((((*node) -> left) -> TYPE == num)) {

            if (((*node) -> left) -> data.numvalue == 0) {

                if ((*node) -> data.value == add) {

                    become_right (node, &changes);
                }

                else if ((*node) -> data.value == mul || (*node) -> data.value == pow || (*node) -> data.value == divi) {

                    become_left (node, &changes);
                }
            }

            else if (((*node) -> left) -> data.numvalue == 1) {

                if ((*node) -> data.value == mul) {

                    become_right (node, &changes);
                }

                else if ((*node) -> data.value == pow) {

                    become_left (node, &changes);
                }
            }
        }

        else if ((((*node) -> right) -> TYPE == num)) {

            if (((*node) -> right) -> data.numvalue == 0) {

                if ((*node) -> data.value == add || (*node) -> data.value == sub) {

                    become_left (node, &changes);
                }

                else if ((*node) -> data.value == mul) {

                    become_right (node, &changes);
                }

                else if ((*node) -> data.value == pow) {

                    changes++;

                    (*node) -> TYPE = num;

                    (*node) -> data.numvalue = 1;

                    free ((void*) ((*node) -> left)); (*node) -> left = NULL;

                    free ((void*) ((*node) -> right)); (*node) -> right = NULL;
                }
            }

            else if ((((*node) -> right) -> data.numvalue == 1) && (   (*node) -> data.value == mul
                                                                    || (*node) -> data.value == divi
                                                                    || (*node) -> data.value == pow )){

                become_left (node, &changes);
            }
        }

        if (changes) {

            Graph_dif (difroot);
            n++;
        }

    changes += changesL + changesR;
    }

    return changes;
}


