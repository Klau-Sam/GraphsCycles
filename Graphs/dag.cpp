#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

using namespace std;


struct slistEl {
    slistEl * next;
    int v;
};


//globalne
int sptr, * S;
slistEl ** graf;
char * visited;

const char WHITE = 0; //kolory wierzchołków
const char GRAY  = 1;
const char GREEN = 2;


char ** adjacency_matrix(int n, int saturation) {
    ifstream fi; //wczytywanie macierzy z pliku
    fi.open("test");
    if (!fi.good()) cout << "problem z plikiem" << endl;

    char ** A = new char * [n];
    int v1, v2;

    for (int i = 0; i < n; i++) A[i] = new char [n];

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) A[i][j] = 0;

    // cout << ((n*(n-1))/2)*saturation;
    while(!fi.eof()) {
        fi >> v1;
        fi >> v2;
        A[v1][v2] = -1; //oznaczanie
        A[v2][v1] = 1;
    }

    fi.close();
    return A;
}

int search_am(int i, int j, char ** B) {
    return B[i][j];
}


slistEl ** adjacency_list (int n, int m, int saturation) {
    ifstream fi;
    fi.open("test2");
    if (!fi.good()) cout << "problem z plikiem" << endl;

    slistEl ** A;
    slistEl * p, * r;
    int v1, v2;

    A = new slistEl * [n];

    for( int i = 1; i <= n; i++ ) A[i] = NULL;

    // saturation
    for ( int i = 1; i <= m; i++ )
    {
        fi >> v1;
        fi >> v2;
        p = new slistEl;
        p->v = v2;
        p->next = A[v1];
        A[v1] = p;
    }

    fi.close();
    return A;
}


bool DFStsort (int v) { //rekurencyjny sort topologiczny
    slistEl * p;

    if (visited[v] == GRAY)
    {
        return false;
    }
    if (visited[v] == WHITE)
    {
        visited[v] = GRAY;
        for (p = graf[v];p;p = p->next)
            if(!DFStsort(p->v)) return false;
        visited[v] = GREEN;
        S[sptr++] = v;
    }
    return true;
}


int main() {

    int n = 13, v1, v2;
    // cin >> n;
    // int m = (n*(n-1))/2;
    int m = 13;
    int saturation = m * 0.3;
    slistEl * p, * r;
    S = new int [n]; sptr = 0;
    visited = new char [n];
    double seconds;

    // char ** B = adjacency_matrix(n, saturation);
    graf = adjacency_list(n, m, saturation);

    for (int i = 1; i <= n; i++) { //pokazujemy
        cout << "B[" << i << "] =";
        p = graf[i];
        while(p)
        {
            cout << setw(5) << p->v;
            p = p->next;
        }
        cout << endl;
    }

    for (int i = 1; i <= n; i++) //przygotowujemy wierzcholki
        visited[i] = WHITE;

    clock_t Begin = clock();
    for (int i = 1; i <= n; i++)
        if(visited[i] == WHITE)
        {
            if(!DFStsort(i)) break;
        }
    clock_t End = clock();
    clock_t Find = End - Begin;
    seconds = (double) Find / CLOCKS_PER_SEC;

    if (sptr == n) //pokazujemy
        for (int i = n-1; i >= 0; i--) cout << S[i] << " ";

    cout << endl << seconds << endl; //ile czasu to zajelo

    //czyszczenie
    for ( int i = 1; i <= n; i++ ) delete [] graf[i];
    delete [] graf;

    for ( int i = 0; i < n; i++)
    {
        p = graf[i];
        while(p)
        {
            r = p;
            p = p->next;
            delete r;
        }
    }

    delete [] visited;
    delete [] S;
    return 0;
}