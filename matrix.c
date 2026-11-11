#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX 20
#define NUM_THREADS 10

int matA[MAX][MAX]; 
int matB[MAX][MAX]; 

int matSumResult[MAX][MAX];
int matDiffResult[MAX][MAX]; 
int matProductResult[MAX][MAX]; 

void fillMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i<MAX; i++) {
        for(int j = 0; j<MAX; j++) {
            matrix[i][j] = rand()%10+1;
        }
    }
}

void printMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i<MAX; i++) {
        for(int j = 0; j<MAX; j++) {
            printf("%5d", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}


void* computeSum(void* args) { 
    int thread_id = *(int*)args;
    int rows_per_thread = MAX / NUM_THREADS;
    int start_row = thread_id * rows_per_thread;
    int end_row = start_row + rows_per_thread;

    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < MAX; j++) {
            matSumResult[i][j] = matA[i][j] + matB[i][j];
        }
    }
    free(args); // Free the memory allocated in main
    return NULL;
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matDiffResult at the coordinates to the difference of the
// values at the coordinates of matA and matB.
void* computeDiff(void* args) { 
    int thread_id = *(int*)args;
    int rows_per_thread = MAX / NUM_THREADS;
    int start_row = thread_id * rows_per_thread;
    int end_row = start_row + rows_per_thread;

    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < MAX; j++) {
            matDiffResult[i][j] = matA[i][j] - matB[i][j];
        }
    }
    free(args);
    return NULL;
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matProductResult at the coordinates to the inner product
// of matA and matB.
void* computeProduct(void* args) { 
    int thread_id = *(int*)args;
    int rows_per_thread = MAX / NUM_THREADS;
    int start_row = thread_id * rows_per_thread;
    int end_row = start_row + rows_per_thread;

    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < MAX; j++) {
            matProductResult[i][j] = 0;
            for (int k = 0; k < MAX; k++) {
                matProductResult[i][j] += matA[i][k] * matB[k][j];
            }
        }
    }
    free(args);
    return NULL;
}

int main() {
    srand(time(0));  // Do Not Remove. Just ignore and continue below.
    
    // 1. Fill the matrices (matA and matB) with random values.
    fillMatrix(matA);
    fillMatrix(matB);
    
    // 2. Print the initial matrices.
    printf("Matrix A:\n");
    printMatrix(matA);
    printf("Matrix B:\n");
    printMatrix(matB);
    
    // 3. Create pthread_t objects for our threads.
    pthread_t sum_threads[NUM_THREADS];
    pthread_t diff_threads[NUM_THREADS];
    pthread_t prod_threads[NUM_THREADS];
    
    // 4. Create a thread for each chunk of the matrix operations.
    // Using malloc to pass the thread ID to each thread safely.
    for (int i = 0; i < NUM_THREADS; i++) {
        int* arg_sum = malloc(sizeof(int));
        *arg_sum = i;
        pthread_create(&sum_threads[i], NULL, computeSum, arg_sum);

        int* arg_diff = malloc(sizeof(int));
        *arg_diff = i;
        pthread_create(&diff_threads[i], NULL, computeDiff, arg_diff);

        int* arg_prod = malloc(sizeof(int));
        *arg_prod = i;
        pthread_create(&prod_threads[i], NULL, computeProduct, arg_prod);
    }
    
    // 5. Wait for all threads to finish.
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(sum_threads[i], NULL);
        pthread_join(diff_threads[i], NULL);
        pthread_join(prod_threads[i], NULL);
    }
    
    // 6. Print the results.
    printf("Results:\n");
    printf("Sum:\n");
    printMatrix(matSumResult);
    printf("Difference:\n");
    printMatrix(matDiffResult);
    printf("Product:\n");
    printMatrix(matProductResult);
    
    return 0;
}