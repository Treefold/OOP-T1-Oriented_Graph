/**
    Tema de laborator nr.1 - Cerinte tema 17:

    clasa graf orientat sa contina metode pentru:
    - citirea grafului, care sa supraincarce operatorul >>
    - afisarea grafului, care sa supraincarce operatorul <<
    - citirea si afisarea sa foloseasca acelasi mod de reprezentare a grafului, care sa
        arate intreaga structura a grafului, ceea ce parcurgerile urmatoare nu pot face
    - parcurgerea in latime + adancime
    - determinarea matricii (existentei) drumurilor
    - determinarea componentelor tare conexe, nu ca grafuri, ci ca liste de varfuri
    - o metoda care sa determine daca graful este tare conex, care poate folosi oricare dintre metodele anterioare
    - o metoda care sa realizeze sortarea topologica si sa arunce o exceptie (afiseaza pe ecran) in caz ca graful are cicluri
    - o metoda de supraincarcare a operatorului +, care sa determine, din doua grafuri orientate avand aceeasi multime
        de varfuri, graful orientat cu aceeasi multime de varfuri ca si acele doua grafuri, si cu multimea arcelor
        egala cu,reuniunea multimilor arcelor acelor doua grafuri (bineinteles, tinand cont de orientarea arcelor);

    * ATENTIE: functiile pe care le-am numit mai sus metode (fie ca sunt supraincarcari de
    operatori, fie altfel de functii), pot fi implementate ca functii prieten in loc de metode
    ale claselor respective, daca se considera ca aceasta alegere este mai naturala;
*/

#include <iostream>
#include <fstream>
#include <cstdarg>
using namespace std;

class Graph;
class NodeList;

class Node
{
    int info;
    Node* next;
public:
    friend class NodeList;
    friend class Graph;
    Node (int x = 0, Node *nxt = NULL) {info = x; next = nxt;}
    Node (Node* other); /// =
    ~Node () {if (this != NULL) {delete next;}}
    friend istream & operator >> (istream & is, Graph* & g);
    friend ostream & operator << (ostream & os, Graph* & g);
    friend Graph* sum (Graph* a, Graph* b);
    bool operator< (Node* & other) {return this->info < other->info;}
    void sort ();
    friend void extendToMD (NodeList *matDr);
    friend void printMD (NodeList *matDr);
};

class NodeList
{
    int key;
    Node* info;
    NodeList* next;
public:
    friend class Graph;
    NodeList (int k = 0, Node* nod = NULL, NodeList* nxt = NULL) {key = k; info = nod; next = nxt;}
    NodeList (NodeList* other); /// =
    ~NodeList () {if (this != NULL) {if (this != NULL) {delete info; delete next;}}}
    friend istream & operator >> (istream & is, Graph* & g);
    friend ostream & operator << (ostream & os, Graph* & g);
    friend Graph* sum (Graph* a, Graph* b);
    bool operator< (NodeList* & other) {return this->key < other->key;}
    void sort();
    void BFS (int n, int x);
    void DFS (int *v, int x);
    bool doYouKnowTheWay (int x, int y);
    void dfsSortTop(int *v, int x, NodeList *p);
    friend void extendToMD (NodeList *matDr);
    friend void printMD (NodeList *matDr);
};

class Graph
{
    int n;
    NodeList* mat;
public:
    Graph (int nr = 0, NodeList* info = NULL) {n = nr; mat = info;}
    Graph (Graph* g);
    ~Graph () {if (this != NULL) {delete mat;}}
    friend void deleteGraph (int n, ...); /// ~NodeList
    friend istream & operator >> (istream & is, Graph* & g);
    friend ostream & operator << (ostream & os, Graph* & g);
    Graph* operator+= (Graph* other);
    Graph* operator+ (Graph* other);
    friend Graph* sum (Graph* a, Graph* b);
    void sort() {if (this != NULL) {this->mat->sort();}}
    void BFS (int x) {cout << "BFS: ";this->mat->BFS (n, x);}
    void DFS (int x) {cout << "DFS: ";int *v = new int[n+1]; for (int i = 0; i <= n; ++i) {v[i] = 0;} this->mat->DFS (v, x); delete[] v; cout << "\n";}
    bool doYouKnowTheWay (int x, int y) {return this->mat->doYouKnowTheWay (x, y);}
    int nrCompTareConex (bool pr);
    bool tareConex (bool pr) {return (this->nrCompTareConex(pr) == 1);}
    void sortareTopologica ();
    NodeList* getMat() {return mat;}
};

int main()
{
    ifstream in ("date.in");
    if (in == NULL) {cout << "fisierul de citire \"date.in\" nu a fost gasit"; return 0;}

    Graph *g = new Graph, *gg = new Graph, *ggg = NULL;
    in >> g >> gg;
    g->sort();
    gg->sort();
    ggg = ((*g) + gg);
    ggg->sort();
    cout << g << gg << ggg;

    ggg->BFS (1);
    ggg->DFS (1);

    NodeList *matDr = new NodeList (ggg->getMat());
    extendToMD (matDr);
    printMD (matDr);

    cout << "Graful " << (ggg->tareConex(1) ? "" : "NU ") << "este Tare Conex\n";
    ggg->sortareTopologica();

    deleteGraph (3, g, gg, ggg);
    delete matDr;
    in.close();
    return 0;
}

Node:: Node (Node* other)
{
    if (other == NULL) {info = 0; next = NULL; return;}
    this->info = other->info;
    this->next = (other->next == NULL) ? NULL : new Node (other->next);
}

NodeList:: NodeList (NodeList* other)
{
    if (other == NULL) {key = 0; info = NULL; next = NULL; return;}
    this->key = other->key;
    this->info = (other->info == NULL) ? NULL : new Node (other->info);
    this->next = (other->next == NULL) ? NULL : new NodeList (other->next);
}

Graph:: Graph (Graph* other)
{
    if (other == NULL) {n = 0; mat = NULL; return;}
    this->n = other->n;
    this->mat = (other->mat == NULL) ? NULL : new NodeList(other->mat);
}

void deleteGraph (int n, ...)
{
    va_list v;
    va_start (v, n);
    for (int i = 0; i < n; ++i) {
        delete va_arg (v, Graph*);
    }
    va_end (v);
}

istream & operator >> (istream & is, Graph* & g)
{
    if (g == NULL) {g = new Graph;}
    else {delete g->mat;}
    int m, x, y;
    NodeList *i;
    Node *j;
    is >> g->n >> m;
    for (int k = 0; k < m; ++k) {
        is >> x >> y;
        if (x > g->n) {g->n = x;}
        if (y > g->n) {g->n = y;}
        i = g->mat;
        while (i != NULL && i->key != x) {i = i->next;}
        if (i == NULL) {g->mat = new NodeList(x, new Node (y), g->mat);}
        else {
            j = i->info;
            while (j != NULL && j->info != y) {j = j->next;}
            if (j == NULL) {i->info = new Node (y, i->info);}
        }
    }
    return is;
}

ostream & operator << (ostream & os, Graph* & g)
{
    if (g == NULL) {return os << "NULL\n";}
    NodeList *i = g->mat;
    Node *j;
    int nr = 0;
    while (i) {
        j = i->info;
        while (j) {
            ++nr;
            j = j->next;
        }
        i = i->next;
    }
    os << "\tNr. Nodes: " << g->n << "\n\tNr. Arcs: " << nr << "\nArcs: (x y) = arc x -> y\n";
    i = g->mat;
    while (i) {
        j = i->info;
        while (j) {
            os << i->key << " " << j->info << endl;
            j = j->next;
        }
        i = i->next;
    }
    return os << "\n";
}

Graph* Graph:: operator+= (Graph* other)
{
    if (other == NULL || this->mat == other->mat || other->mat == NULL) {return ((this == NULL) ? new Graph : this);}
    this->n = max (this->n, other->n);
    if (this->mat == NULL) {this->mat = new NodeList (other->mat); return this;}
    NodeList *i = other->mat, *j;
    while (i != NULL) {
        j = this->mat;
        while (j->next != NULL && j->key != i->key) {j = j->next;}
        if (j->key != i->key) {j->next = new NodeList (i->key, new Node (i->info));}
        ///else {i->info += j->info;}
        else {
            Node *ii = i->info, *jj;
            while (ii) {
                jj = j->info;
                while (jj->next != NULL && jj->info != ii->info) {jj = jj->next;}
                if (jj->info != ii->info) {jj->next = new Node (ii->info);}
                ii = ii->next;
            }
        }
        i = i->next;
    }
    return this;
}

Graph* Graph:: operator+ (Graph* other)
{
    if (this == other || other == NULL) {return new Graph (this);}
    if (this == NULL) {return new Graph (other);}
    Graph *s = new Graph (this);
    s->n = max (this->n, other->n);
    if (this->mat == NULL) {s->mat = new NodeList (other->mat); return s;}
    return (*s += other);
}

Graph* sum (Graph* a, Graph* b)
{
    if (a == b || b == NULL) {return new Graph (a);}
    if (a == NULL) {return new Graph (b);}
    Graph *c = new Graph (a);
    c->n = max (a->n, b->n);
    if (a->mat == NULL) {c->mat = new NodeList (b->mat); return c;}
    return (*c += b);
}

void Node:: sort()
{
    if (this == NULL) {return;}
    Node *i = this, *j, *x;
    while (i->next != NULL) {
        j = i->next;
        x = i;
        while (j != NULL) {
            if (*j < x) {x = j;}
            j = j->next;
        }
        swap (i->info, x->info);
        i = i->next;
    }
}

void NodeList:: sort()
{
    ///if (this == NULL) {return;}
    NodeList *i = this, *j, *x;
    while (i != NULL) {
        j = i->next;
        x = i;
        while (j != NULL) {
            if (*j < x) {x = j;}
            j = j->next;
        }
        swap (i->key, x->key);
        swap (i->info, x->info);
        i->info->sort();
        i = i->next;
    }
}

void NodeList:: BFS (int n, int root)
{
    int x, *v = new int[n+1], *q = new int[n+1], st = 0, dr = 1;
    for (int i = 0; i <= n; ++i) {v[i] = 0;}
    v[root] = 1;
    q[0] = root;
    NodeList *i;
    Node *j;
    while (st < dr) {
        cout << (x = q[st++]) << " ";
        i = this;
        while (i != NULL && i->key != x) {i = i->next;}
        if (i == NULL) {continue;}
        j = i->info;
        while (j != NULL) {
            if (!v[j->info]) {
                v[j->info] = 1;
                q[dr++] = j->info;
            }
            j = j->next;
        }
    }
    delete[] v;
    delete[] q;
    cout << "\n";
}

void NodeList:: DFS (int *v, int x)
{
    cout << x << " ";
    v[x] = 1; /// visited
    NodeList *i = this;
    while (i != NULL && i->key != x) {i = i->next;}
    if (i == NULL) {return;}
    Node *j = i->info;
    while (j != NULL) {
        if (!v[j->info]) {i->DFS (v, j->info);}
        j = j->next;
    }
}

void extendToMD (NodeList *matDr)
{
    matDr->sort();
    bool hasChanged = 1;
    NodeList *i, *j;
    Node *k, *p, *q;
    while (hasChanged) {
        hasChanged = 0;
        i = matDr;
        while (i != NULL) { /// parcurc varfurile
            k = i->info;
            while (k != NULL) { /// parcurc vecinii varfului curent
                if (k->info == i->key) {k = k->next; continue;}
                j = matDr;
                while (j != NULL && j->key < k->info) {j = j->next;} /// caut vecinul in lista de varfuri
                if (j == NULL || j->key != k->info) {k = k->next; continue;}
                if (i->info == NULL) {i->info = new Node(j->info);k = k->next; continue;}
                p = i->info;
                q = j->info;
                while (q) { /// daca pot ajunge in i->info => pot ajunge int toti vecinii lui i->info
                    while (q && q->info < p->info) {
                        hasChanged = 1;
                        p->next = new Node (p->info, p->next);
                        p->info = q->info;
                        p = p->next;
                        q = q->next;
                    }
                    if (q && p->next == NULL) {
                        if (q->info == p->info) {q = q->next;}
                        if (q != NULL) {
                            hasChanged = 1;
                            p->next = new Node (q);
                        }
                        break;
                    }
                    if (q && p->info == q->info) {q = q->next;}
                    p = p->next;
                }
                k = k->next;
            }
            i = i->next;
        }
        hasChanged = 0;
    }
}

void printMD (NodeList *matDr)
{
    cout << "\nMatricea (Existentei) Drumurilor este:";
    NodeList *i = matDr;
    Node *j;
    while (i) {
        cout << "\n" << i->key << ": ";
        j = i->info;
        while (j) {
            cout << j->info << ", ";
            j = j->next;
        }
        i = i->next;
    }
    cout << "\n";
}

bool NodeList:: doYouKnowTheWay (int x, int y)
{
    NodeList *i = this;
    while (i != NULL) {
        if (i->key == x) {
            Node *j = i->info;
            while (j != NULL) {
                if (j->info == y) {return 1;}
                j = j->next;
            }
            return 0;
        }
        i = i->next;
    }
    return 0;
}

int Graph:: nrCompTareConex (bool pr)
{
    if (pr) {cout << "\nComponentele Tare Conexe sunt:\n";}
    int *v = new int[this->n+1], *q = new int[this->n+1], nr = this->n, st, dr, x, x_y, y_x, nraf = 0;
    for (int i = 0; i <= this->n; ++i) {v[i] = 0;}
    NodeList *matDr = new NodeList (this->mat);
    extendToMD (matDr);

    NodeList *k = matDr, *i;
    Node *j;

    while (k != NULL) {
        if (!v[k->key]) {
            if (pr) {cout << ++nraf << ": ";}
            v[k->key] = 1;
            q[st = 0] = k->key;
            dr = 1;
            while (st < dr) {
                x = q[st++];
                if (pr) {cout << x << " ";}
                i = matDr;
                while (i != NULL && i->key != x) {i = i->next;}
                if (i == NULL) {continue;}
                j = i->info;
                while (j != NULL) {
                    x_y = matDr->doYouKnowTheWay (i->key, j->info);
                    y_x = matDr->doYouKnowTheWay (j->info, i->key);
                    if (!v[j->info] && x_y && y_x) {
                        --nr;
                        v[j->info] = 1;
                        q[dr++] = j->info;
                    }
                    j = j->next;
                }
            }
            if (pr) {cout <<"\n";}
        }
        k = k->next;
    }

    if (pr) {
        for (int i = 1; i <= this->n; ++i) {
            if (!v[i]) {cout << ++nraf << ": " << i << "\n";}
        }
        cout << "\n";
    }

    delete[] v;
    delete[] q;
    delete matDr;
    return nr;
}

void NodeList:: dfsSortTop(int *v, int x, NodeList* p)
{
    v[x] = 1; /// visited
    NodeList *i = this;
    while (i != NULL && i->key != x) {i = i->next;}
    if (i == NULL) {p->info = new Node (x, p->info); return;}
    Node *j = i->info;
    while (j != NULL) {
        if (!v[j->info]) {i->dfsSortTop (v, j->info, p);}
        j = j->next;
    }
    p->info = new Node (x, p->info);
}

void Graph:: sortareTopologica ()
{
    /// exista o componenta tare conexa cu mai mult de un nod <=> exista drum x->y ^ y->x => exista ciclul x ->...-> y ->...-> x, x!=y
    if (this->nrCompTareConex(0) < this->n) {cout << "\nGraful are Cicluri => Sortatea Topologica nu se poate efectua\n"; return;}

    cout << "\nUna dintre posibilele sortari topologice este:";
    NodeList *p = new NodeList;
    Node *q;
    int *v = new int [this->n+1];
    for (int i = 1; i <= this->n; ++i) {v[i] = 0;}

    for (int i = 1; i <= this->n; ++i) {
        if (!v[i]) {this->mat->dfsSortTop(v, i, p);}
    }

    while (p->info) {
        cout << " " << p->info->info;
        q = p->info;
        p->info = p->info->next;
        delete q;
    }
    delete p;
    delete[] v;
    cout <<"\n";
}
