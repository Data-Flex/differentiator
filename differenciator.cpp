

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <assert.h>
#include "getdata.h"
#include "tree.h"
#include "stack.h"


Node* GetG (const char* str);

Node* GetE ();

Node* GetT ();

Node* GetP ();

Node* GetN ();

Node* Dif (Node *root);

void finish (FILE **input);


const char* S = NULL;
const char* Start = NULL;


int main() {

    FILE *input = NULL;

    int size = 0;
    char *buf = NULL;

    getdata (&input, &size, &buf);

    S = buf; Start = buf;

    Node* root = GetG (S);

    difroot = Dif (root);

    system("taskkill /fi \"windowtitle eq resdif.png*\" ");

    Graph_dif (difroot); n++;

    while (simplify_tree (&difroot))

    for (int i = 0; i < n; i++) system("taskkill /fi \"windowtitle eq resdif.png*\" ");

    print_text (difroot);

    finish (&input);

    return 0;
}



Node* Dif (Node *root) {

    Node *node = NULL;

    switch (root->TYPE) {

    case num:
        node = NewNum (0);
        break;

    case var:
        node = NewNum (1);
        break;

    case oper:
        node = NewOp (root -> data.value, Dif (root -> left), Dif (root -> right));
        Graph_dif (root);
        Graph_dif (node);
        system("taskkill /fi \"windowtitle eq resdif.png*\" ");
        system("taskkill /fi \"windowtitle eq resdif.png*\" ");
        break;

    case important_oper:
        if(root -> data.value == mul) {

            node = NewOp ((op) add, NewOp ((op) mul, Dif  (root -> left),       copy (root -> right, NULL)),
                                    NewOp ((op) mul, copy (root -> left, NULL), Dif  (root -> right)));
        }

        else if(root -> data.value == divi) {

            node = NewOp ((op) divi, NewOp ((op) sub, NewOp ((op) mul, Dif (root -> left), copy (root -> right, NULL)), NewOp ((op) mul, copy (root -> left, NULL), Dif (root -> right))),
                                     NewOp ((op) mul, copy (root -> right, NULL), copy (root -> right, NULL)));
        }
        Graph_dif (root);
        Graph_dif (node);
        system("taskkill /fi \"windowtitle eq resdif.png*\" ");
        system("taskkill /fi \"windowtitle eq resdif.png*\" ");
        break;
    }

    return node;
}



Node* GetG (const char* str) {

    S = str;

    Node* val = GetE();

    if (*S != '\0') printf ("Syntax Error. Operator %c is unknown", *S);

    for (int i = 0; i < n; i++) system("taskkill /fi \"windowtitle eq resdif.png*\" ");

    n = 0;

    Graph_dif (val);

    return val;
}



Node* GetE () {

    Node* L = GetT();

    Node* R = NULL;

    while (*S == '+' || *S == '-') {

        char Opname = *S;

        S++;

        R = GetT();

        L = NewOp ((op) Opname, L, R);

        Graph_dif (L); n++;

    }

    return L;
}




Node* GetT () {

    Node* L = GetP();

    Node* R = NULL;

    while (*S == '*' || *S == '/') {

        char Opname = *S;

        S++;

        R = GetP();

        L = NewOp ((op) Opname, L, R);

        Graph_dif (L); n++;
    }



    return L;
}




Node* GetP () {

    Node* val = NULL;

    if (*S == '(') {

        S++;

        val = GetE();

        if (*S != ')') printf ("Syntax Error. Operator ) is missed. Position: %d", S - Start + 1);

        S++;
    }

    else val = GetN();

    return val;
}




Node* GetN () {

    int val = 0;

    const char* SOLD = S;

    while ('0' <= *S && *S <= '9') {

        val = val*10 + *S - '0';

        S++;
    }

    if (S > SOLD) return NewNum (val);

    else if (S == SOLD) {

        stack name;

        StackCtor_ (&name, 1);

        while (('a' <= *S && *S <= 'z') || ('A' <= *S && *S <= 'Z')) {

            Push (&name, *S);

            S++;
        }

        if (S > SOLD) return NewVar (name.data);
    }

    if (S <= SOLD) printf ("Syntax Error. Position: %d", S - Start + 1);

    return NewNum (val);

}


void finish (FILE **input) {

    fclose (*input);

    system("outputtext.txt");
}
