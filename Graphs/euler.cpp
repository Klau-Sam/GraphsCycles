#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stack>
#include <cmath>
#include <ctime>

using namespace std;


struct slistEl
{
    slistEl * next;
    int v;
    int counter;
};

//globalne
int n, m;
float saturation;
slistEl **A;
int **M;
stack <int> SS;


void adjacency_list_2() {
    srand(time(NULL));
    int e = m * saturation;
    cout << e << " ";
    slistEl * p, * r;
    for ( int i = 0; i < n; i++ )
        A[i] = new slistEl;

    int c = 0;
    for ( int i = 0; i < n; i++ ) {
        A[i]->v = i;
        A[i]->next = NULL;
        A[i]->counter = 0;
        if (i >= 1) {
            int x = rand() % i;
            p = A[x];
            r = A[i];

            p->counter++;
            r->counter++;

            while (p->next != NULL)
                p = p->next;

            p->next = new slistEl;
            p->next->v = i;
            p->next->next = NULL;

            while (r->next != NULL)
                r = r->next;

            r->next = new slistEl;
            r->next->v = x;
            r->next->next = NULL;
        }
        c++;
    }
    c--;
    if ( c >= e )
    {
        return;
    }
    else {
        while ( c != e ) {
            int x, y;
            bool check = false;
            while ( !check ) {
                check = true;
                x = rand() % n;
                y = rand() % n;
                while ( x == y ) {
                    x = rand() % n;
                    y = rand() % n;
                }
                p = A[x];
                while ( p->next != NULL ) {
                    p = p->next;
                    if ( p->v == y )
                        check = false;
                }
            }
            p = A[x];
            p->counter++;

            while (p->next != NULL)
                p = p->next;

            p->next = new slistEl;
            p->next->v = y;
            p->next->next = NULL;

            p = A[y];
            p->counter++;

            while (p->next != NULL)
                p = p->next;

            p->next = new slistEl;
            p->next->v = x;
            p->next->next = NULL;

            c++;
        }
    }
    return;
}


void create_euler() {
    stack <int> S;

    for ( int i = 0; i < n; i++ )
        if (A[i]->counter % 2 == 1) S.push(i);

    slistEl * p, * r;
    while (!S.empty())
    {
        int x, y;
        x = S.top();
        S.pop();
        y = S.top();
        S.pop();

        p = A[x];
        r = A[y];

        bool check = false;
        while ( p->next != NULL )
        {
            p = p->next;
            if (p->v == y) { check = true; break; }
        }
        if ( !check )
        {
            A[x]->counter++;
            p->next = new slistEl;
            p->next->next = NULL;
            p->next->v = y;

            while ( r->next != NULL )
                r = r->next;
            A[y]->counter++;
            r->next = new slistEl;
            r->next->next = NULL;
            r->next->v = x;
        }
        else
        {
            slistEl * pp = NULL, * rr = NULL;
            pp = A[x];
            A[x]->counter--;
            while ( pp->next->v != y )
                pp = pp->next;

            if ( p->next != NULL )
                rr = p->next;

            delete p;
            if ( rr != NULL )
                pp->next = rr;
            else
                pp->next = NULL;

            pp = A[y];
            A[y]->counter--;
            while ( pp->next->v != x )
                pp = pp->next;
            r = pp->next;
            if ( r->next != NULL )
                rr = r->next;

            delete r;
            if ( rr != NULL )
                pp->next = rr;
            else
                pp->next = NULL;
        }

    }
    return;
}


bool find_euler_cycle() {
    for ( int i = 0; i < n; i++ )
    {
        slistEl * p = A[i];
        if ( p->counter % 2 != 0 )
            return false;
    }
    return true;
}


void coherent() {
    bool * visited = new bool [n];
    stack <int> S;
    slistEl * p;
    int c = 0, x, y;

    for ( int i = 0; i < n; i++) {
        visited[i] = false;
    }

    S.push(0);
    visited[0] = true;  //odwiedzony

    while(!S.empty())  //przejście DFS
    {
        x = S.top();
        S.pop();
        c++;
        for ( p = A[x]; p; p = p->next )
        {
            y = p->v;
            if(!visited[y])
            {
                visited[y] = true;
                S.push(y);
            }
        }
    }

    //wynik
    if( c == n ) cout << "connected graph"; else cout << "disconnected graph";
    cout << endl;
    return;
}


void adjacency_matrix_2() {
    int e = m * saturation;

    for ( int i = 0; i < n; i++ )
        M[i] = new int [n];

    for ( int i = 0; i < n; i++ )
        for ( int j = 0; j < n; j++ )
            M[i][j] = 0;

    for (int i = 0; i < n; i++)
    {
        slistEl * p = A[i];
        while ( p->next != NULL )
        {
            p = p->next;
            M[i][p->v] = 1;
        }
    }
    return;
}


void print_am() {
    cout << "   ";
    for ( int i = 0; i < n; i++) cout << setw(3) << i;
    cout << endl << endl;
    for( int i = 0; i < n; i++)
    {
        cout << setw(3) << i;
        for ( int j = 0; j < n; j++ ) cout << setw(3) << M[i][j];
        cout << endl;
    }
    return;
}


stack <int> STACK;

void DFSEuler(int v) {
    for ( int i = 0; i < n; i++ )
        while ( M[v][i] )
        {
            M[v][i] = 0;
            M[i][v] = 0;
            DFSEuler(i);
        }
    STACK.push(v);
    return;
}


bool find_euler () {
    int v = 0;
    DFSEuler(v);
    return true;
}


stack <int> CE;

void new_euler(int v) {
    int u;
    SS.push(v);
    while(!SS.empty()) {
        v = SS.top();
        for (int i = 0; i < n; i++ ){
            if ( M[v][i] != 0 )
            {
                u = i;
                SS.push(u);
                M[v][u] = 0;
                M[u][v] = 0;
                break;
            }
            else if ( i == n - 1 ){ SS.pop(); CE.push(v); }
        }
    }
    return;
}


int main()
{
    ofstream plik;
    plik.open("euler", ios_base::app);
    for (n = 1200; n <= 1600; n += 100) {
        m = (n * (n - 1)) / 2;
        saturation = 0.95;
        A = new slistEl *[n];
        M = new int *[n];
        double seconds;


        cout << "Dla " << n << " wierzchołków" << endl;

        adjacency_list_2();
        create_euler();
        coherent();
        adjacency_matrix_2();
        // print_am();

        clock_t Start = clock();
        // cout << find_euler() << endl;
        new_euler(0);
        clock_t End = clock();

        clock_t Find = End - Start; //ile czasu to zajelo
        seconds = (double) Find / CLOCKS_PER_SEC;
        cout << endl << seconds << endl;
        plik << seconds << endl;

        /* while (!CE.empty()) {
            int x = CE.top();
            CE.pop();
            cout << x << " ";
        } */

        //czyszczenie
        slistEl *temp1, *temp2;
        if (A != NULL) {
            for ( int i = 0; i < n; i++ ) {
                while (A[i]->counter != 0) {
                    temp1 = A[i];
                    for (int j = 0; j < A[i]->counter; j++)
                        temp1 = temp1->next;
                    delete temp1;
                    A[i]->counter--;
                }
                temp1 = A[i];
                delete temp1;
            }
            delete[] A;
        }

        if (M != NULL) {
            for ( int i = 0; i < n; i++ )
                delete[] M[i];
            delete[] M;
            M = NULL;
        }
    }

    return 0;
}