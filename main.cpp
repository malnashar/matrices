#include <iostream>
#include <bits/stdc++.h>
#include <pthread.h>
#include <time.h>

using namespace std;

int ** matA;
int ** matB;
int ** matC;
int rowA;
int rowB;
int colA;
int colB;
clock_t t;
struct data
{
    int row;
    int col;
};

void read_data();
void multiply_element();
void* calc1(void* arg);
void multiply_row();
void* calc2(void* arg);
void initialize_output();

int main()
{
    read_data();
    //start to evaluate time of calculations
    if(colA == rowB){
    t = clock();
    multiply_element();
    cout << endl;
    t = clock();
    multiply_row();
    }
    else
        cout << "wrong matrices" << endl;
    return 0;
}

//read 2 matrices from the file and number of rows and
//columns of each one.
void read_data()
{
    //open the file to read from it.
    freopen("matrix-readme.txt","r",stdin);

    cin >> rowA >> colA;
    //create the first matrix
    matA = new int*[rowA];
    for(int i = 0 ; i<rowA ; i++)
    {
        matA[i] = new int[colA];
    }
    //read data of first matrix.
    for(int i = 0 ; i < rowA ; i++)
    {
        for(int j = 0 ; j < colA ; j++)
        {
            cin >> matA[i][j];
        }
    }

    cin >> rowB >> colB;
    //create the second matrix
    matB = new int*[rowB];
    for(int i = 0 ; i<rowB ; i++)
    {
        matB[i] = new int[colB];
    }
    //read data of second matrix
    for(int i = 0 ; i < rowB ; i++)
    {
        for(int j = 0 ; j < colB ; j++)
        {
            cin >> matB[i][j];
        }
    }
    //create matrix for the answer
    matC = new int*[rowA];
    for(int i = 0; i < rowA ; i++)
    {
        matC[i] = new int[colB];
    }

}

//multiply 2 matrices with one thread to calculate
//each element
void multiply_element()
{
    initialize_output();
    //create 2D array of threads to use one thread in specific
    //position to calculate the element in this position.
    //to be sure that each 1 thread work only to calculate
    //specific 1 element.
    pthread_t threads[rowA][colB];

    for(int i = 0; i < rowA; i++)
    {
        for(int j = 0; j<colB; j++)
        {
            // save the position of each element in struct
            // and create a thread to calculate this element
            // using this parameters
            data* d=new data();
            d->row=i;
            d->col=j;
            pthread_create(&threads[i][j], NULL, calc1, (void*)(d));

        }
    }

    // loop for each thread we used to wait untill it finish its work
    for(int i = 0; i < rowA; i++)
    {
        for(int j = 0; j<colB; j++)
        {
            pthread_join(threads[i][j],NULL);
        }
    }

    // end the time counter at the end of calculations
    t = clock() - t;
    double taken = ((double)t)/CLOCKS_PER_SEC;

    cout << "Multiplication of A and B usin thread for each element" <<endl;
    for (int i = 0; i < rowA; i++)
    {
        for (int j = 0; j < colB; j++)
            cout << matC[i][j] << " ";
        cout << endl;
    }
    cout << "output is calculated in ";
    cout << taken << endl;

}


// calculate the value of each element
// and put the answer in the answer matrix
void* calc1(void* arg)
{
    struct data* d;
    d=(data*)arg;

    for (int k = 0; k < rowB; k++)
    {
        matC[d->row][d->col] += matA[d->row][k] * matB[k][d->col];
    }

}

//multiply 2 matrices with one thread to calculate
//each row
void multiply_row()
{
    initialize_output();
    // create array of threads of size number of rows of
    //first matrix to use each one the evaluate the first row
    //of the answer
    pthread_t threads[rowA];

    for(int i = 0; i < rowA; i++)
    {
        // save the position of each row in struct
        // and create a thread to calculate this row
        // using this parameter
        data* d=new data();
        d->row=i;
        pthread_create(&threads[i], NULL, calc2, (void*)(d));

    }

    // loop for all threads we used to wait
    // untill it finish each work
    for(int i = 0; i < rowA; i++)
    {
        pthread_join(threads[i],NULL);
    }

    // end time counter at the end of calculations
    t = clock() - t;
    double taken = ((double)t)/CLOCKS_PER_SEC;
    cout << "Multiplication of A and B usin thread for each row" <<endl;
    for (int i = 0; i < rowA; i++)
    {
        for (int j = 0; j < colB; j++)
            cout << matC[i][j] << " ";
        cout << endl;
    }
    cout << "output is calculated in ";
    cout << taken << endl;
}

// calculate the value of each row
// and put the answer in the answer matrix
void* calc2(void* arg)
{
    struct data* d;
    d=(data*)arg;
    for(int j = 0 ; j < colB ; j++)
    {
        for (int k = 0; k < colA; k++)
        {
            matC[d->row][j] += matA[d->row][k] * matB[k][j];
        }
    }

}
// initialize answer matrix with 0 in all positions
void initialize_output()
{
    for(int i = 0 ; i <rowA ; i++)
        for(int j = 0 ; j < colB ; j++)
            matC[i][j] = 0;
}





