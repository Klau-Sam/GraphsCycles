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

// globalne zmienne
int n, m, sptr = 0;
float saturation;
slistEl **A;
int **M;
stack <int> SS;
// bool *visited;


void adjacency_list_2() {
    srand(time(NULL));
    int e = m * saturation;
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
            r = A[y];

            p->counter++;
            r->counter++;

            while (p->next != NULL)
                p = p->next;

            p->next = new slistEl;
            p->next->v = y;
            p->next->next = NULL;

            while (r->next != NULL)
                r = r->next;

            r->next = new slistEl;
            r->next->v = x;
            r->next->next = NULL;

            c++;
        }
    }
    return;
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
    visited[0] = true; //odwiedzony

    while(!S.empty()) //przejście DFS
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


void adjacency_matrix_2()
{
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


void print_am() { //sprawdzenie macierzy
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


void DFSHamilton(int n, int v, int **M, int *S, int *visited, int &nr, int &CH, int &found, double *tt, clock_t &Start)
{
    if (found) return;
    clock_t End = clock();
    if ( (End - Start)/1000000 > 20)
    {
        cout << "BREAK" << endl;
        *tt = 20 * 1000000;
        found = 1;
    }
    S[nr++] = v;

    if (nr != n)
    {
        visited[v] = 1;
        for (int i = 0; i < n; i++)
        {
            if ( M[v][i] && !visited[i] )
                DFSHamilton(n, i, M, S, visited, nr, CH, found, tt, Start);
        }
        visited[v] = 0;
    }
    else if ( M[v][0])
    {
        if (!found)
        {
            clock_t Duration = clock();
            *tt = Duration - Start;
            /* cout << "CH: ";
            for ( int i = 0; i < n; i++ ) { cout << S[i] << ","; }
            cout << endl; */
            found = 1;
            CH++;
        }
    }
    nr--;
    return;
}


void find_hamilton_cycle(int n, double *tt, int **M)
{
    int nr = 0, CH = 0;
    int found = 0;
    int *S = new int[n];
    int *visited = new int[n];
    for (int i = 0; i < n; i++)
        visited[i] = 0;
    int v = rand() % n;
    clock_t Start = clock();
    DFSHamilton(n, v, M, S, visited, nr, CH, found, tt, Start);

    delete[] S;
    delete[] visited;
    return;
}


int main() {
    double time = 0, time_temp = 0;
    double *true_time = &time_temp;
    ofstream plik;
    plik.open("hamilton", ios_base::app);

    for ( int s = 5; s < 9; s++ ) { //wybor nasycenia
        if (s < 5) saturation = 0.1 * s;
        else if (s == 5) saturation = 0.25;
        else if (s == 6) saturation = 0.8;
        else if (s == 7) saturation = 0.95;
        else if (s == 8) saturation = 0.98;

        for (n = 300; n <= 350; n += 5) {
            for (int k = 0; k < 10; k++) {
                m = (n * (n - 1)) / 2;
                A = new slistEl *[n];
                M = new int *[n];

                time_temp = 0;
                adjacency_list_2();
                coherent();
                adjacency_matrix_2();

                find_hamilton_cycle(n, true_time, M);
                cout << endl << time_temp / CLOCKS_PER_SEC << endl; //czas danego poszukiwania
                time += (time_temp/CLOCKS_PER_SEC);
                cout << time << endl; //lacznie 10 prob bedzie

                //czyszczenie
                slistEl *temp1, *temp2;
                if (A != NULL) {
                    for (int i = 0; i < n; i++) {
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
                    A = NULL;
                }

                if (M != NULL) {
                    for (int i = 0; i < n; i++)
                        delete[] M[i];
                    delete[] M;
                    M = NULL;
                }

                // delete [] visited;
                // delete [] S;
            }
            cout << "Dla " << n << " wierzcholkow: " << (time/10);
            plik << n << " = " << (time/10);
            time = 0;

        }
    }

    return 0;
}