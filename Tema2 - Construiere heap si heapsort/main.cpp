/*
 *  Simina Dan-Marius
 *  Grupa: 2
 *
 *  Am implementat build heap bottom-up folosind functia de heapify, heapsort si build heap top-down folosind push heap.
 *
 *  Heapify functioneaza in felul urmator:
 *      pentru un element dat verifica daca acesta e mai mare decat fiul stang sau fiul drept.
 *      apoi daca unul e mai mare ii interschimba si face din nou heapify pentru elemntul respectiv pana cand este asezat pe pozitia corecta astfel incat sa se respecte proprietatea de heap.
 *      (daca ambii fii sunt mai mari se ia cel mai mare dintre ei)
 *      complexitate O(log n)
 *  Build heap bottom up functioneaza in felul urmator:
 *      incep de jos in sus(de la primele noduri care nu sunt frunze) si aplic heapify
 *      complexitate O(n)
 *  Construirea heap-ului prin metoda buttom up are complexitate O(n)
 *
 *  Push heap functioneaza in felul urmator:
 *      insereaza elementul dat pe ultima pozitie in heap, iar apoi il urca pana cand ajunge pe o pozitie unde respecta regula de heap
 *      complexitate O(log n)
 *  Build heap varianta top down apeleaza functia de push heap pentru fiecare element pe care dorim sa il adaugam in heap.
 *      complexitate O(n * log n)
 *
 *  Heapsort functioneaza in felul urmator:
 *      interschimba elementul maxim(cel de pe prima pozitie) cu elementul de pe ultima pozitie si reface heap-ul fara a mai lua in considerare elementul de pe ultima pozitie
 *      repeta acest lucru pana cand heap-ul ajunge sa fie compus dintr-un singur element
 *      complexitate O(n * log n)
 *
 *  Cazul mediu:
 *      atat din punct de vedere al comparatiilor cat si al atribuirilor metoda bottom-up face mai putine.
 *
 *  Cazul defavorabil:
 *      atat din punct de vedere al comparatiilor cat si al atribuirilor metoda bottom-up face mult mai putine, decat top-down.
 */

#include <iostream>
#include "Profiler.h"

using namespace std;

Profiler p("Heap");

#define maxSize 10000
#define stepSize 100
#define nrTests 5


struct heap {
    int length;
    int heapSize;
    int A[maxSize];
};

int parent(int i) {
    return (i - 1) / 2;
}

int left(int i) {
    return 2 * i + 1;
}

int right(int i) {
    return 2 * i + 2;
}

void heapify(heap &H, int i) {

    Operation opCompBuildHeap = p.createOperation("Comparatii build heap", H.length);
    Operation opAttrBuildHeap = p.createOperation("Atribuiri build heap", H.length);

    int l = left(i);
    int r = right(i);
    int largest;

    if (l < H.heapSize) {

        opCompBuildHeap.count();
        if(H.A[l] > H.A[i]){
            largest = l;
        }else largest = i;
    } else largest = i;

    if (r < H.heapSize) {

        opCompBuildHeap.count();
        if(H.A[r] > H.A[largest]){
            largest = r;
        }
    }

    if (largest != i) {
        opAttrBuildHeap.count(3);
        swap(H.A[i], H.A[largest]);
        heapify(H, largest);
    }
}

void buildHeapBottomUp(heap &H) {

    Operation opAttrBuildHeap = p.createOperation("Atribuiri build heap", H.length);

    opAttrBuildHeap.count();
    H.heapSize = H.length;

    for (int i = H.heapSize / 2 - 1; i >= 0; i--) {
        heapify(H, i);
    }
}

void heapSort(heap &H) {
    buildHeapBottomUp(H);

    for (int i = H.length - 1; i >= 1; i--) {
        swap(H.A[i], H.A[0]);
        H.heapSize--;
        heapify(H, 0);
    }
}

void pushHeap(heap &H, int key) {

    Operation opCompPushHeap = p.createOperation("Comparatii push heap", H.length);
    Operation opAttrPushHeap = p.createOperation("Atribuiri push heap", H.length);

//    opCompPushHeap.count();
    if(H.heapSize == H.length){
        cout << "Nu se mai pot adauga elemente in heap";
        return;
    }

    opAttrPushHeap.count(2);
    H.heapSize++;
    H.A[H.heapSize - 1] = key;

    int i = H.heapSize - 1;

    while (i > 0) {
        opCompPushHeap.count();
        if (H.A[parent(i)] < H.A[i]) {

            opAttrPushHeap.count(3);
            swap(H.A[parent(i)], H.A[i]);

            i = parent(i);
        } else break;
    }
}

void buildHeapTopDown(heap &H, int a[]){
    for (int i = 0; i < H.length; i++)
        pushHeap(H, a[i]);
};

void copy(int dest[], int sursa[], int n) {
    for (int i = 0; i < n; i++)
        dest[i] = sursa[i];
}

void afiseaza(heap H) {
    for (int i = 0; i < H.length; i++)
        cout << H.A[i] << " ";
    cout << "\n";
}

void verif(heap H) {
    for (int i = 0; i < H.length / 2; i++) {
        int l = left(i);
        int r = right(i);

        if ((l < H.length and H.A[i] < H.A[l]) or ((r < H.length and H.A[i] < H.A[r]))) {
            cout << "Greseala";
            exit(0);
        }
    }
}

void demo() {
    int a[] = {1, 5, 2, 1, 2, 7, 2, 4, 3};
    int n = sizeof(a) / sizeof(a[0]);

    heap H1 = {0};
    H1.length = n;
    copy(H1.A, a, H1.length);

    buildHeapBottomUp(H1);
    verif(H1);
    cout << ("Heap-ul construit cu build heap: ");
    afiseaza(H1);

    heapSort(H1);
    cout << "Heap-sort: ";
    afiseaza(H1);


    heap H2 = {0};
    H2.length = n;

    buildHeapTopDown(H2, a);
    verif(H2);
    cout << ("Heap-ul construit cu push heap: ");
    afiseaza(H2);
}

void perf() {
    int n;
    int a[maxSize];

//    caz mediu

    for (n = stepSize; n <= maxSize; n += stepSize) {
        for (int test = 0; test < nrTests; test++) {
            FillRandomArray(a, n);

            heap H1 = {0};
            H1.length = n;
            copy(H1.A, a, n);
            buildHeapBottomUp(H1);

            heap H2 = {0};
            H2.length = n;
            buildHeapTopDown(H2, a);

            verif(H1);
            verif(H2);
        }
    }

    p.divideValues("Comparatii build heap", nrTests);
    p.divideValues("Atribuiri build heap", nrTests);
    p.divideValues("Comparatii push heap", nrTests);
    p.divideValues("Atribuiri push heap", nrTests);

    p.addSeries("Total build heap", "Comparatii build heap", "Atribuiri build heap");
    p.addSeries("Total push heap", "Comparatii push heap", "Atribuiri push heap");

    p.createGroup("Mediu comparatii", "Comparatii build heap", "Comparatii push heap");
    p.createGroup("Mediu atribuiri", "Atribuiri build heap", "Atribuiri push heap");
    p.createGroup("Mediu total", "Total build heap", "Total push heap");

//    caz defavorabil

    p.reset();

    for (n = stepSize; n <= maxSize; n += stepSize) {
        FillRandomArray(a, n, 10, 50000, false, ASCENDING);

        heap H1 = {0};
        H1.length = n;
        copy(H1.A, a, n);
        buildHeapBottomUp(H1);

        heap H2 = {0};
        H2.length = n;
        buildHeapTopDown(H2, a);

        verif(H1);
        verif(H2);
    }

    p.addSeries("Total build heap", "Comparatii build heap", "Atribuiri build heap");
    p.addSeries("Total push heap", "Comparatii push heap", "Atribuiri push heap");

    p.createGroup("Defavorabil comparatii", "Comparatii build heap", "Comparatii push heap");
    p.createGroup("Defavorabil atribuiri", "Atribuiri build heap", "Atribuiri push heap");
    p.createGroup("Defavorabil total", "Total build heap", "Total push heap");

    p.showReport();
}

void verifHeapSort() {
    int n;
    int a[maxSize];

    for (n = stepSize; n <= maxSize; n += stepSize) {
        FillRandomArray(a, n);

        heap H;
        H.length = n;
        copy(H.A, a, n);
        heapSort(H);
        sort(a, a + n);

        for(int i = 0; i < n; i++){
            if(H.A[i] != a[i]){
                cout << "Greseala";
                return;
            }
        }
    }
    cout << "ok";
}

int main() {
    demo();
//    perf();
//    verifHeapSort();
    return 0;
}
