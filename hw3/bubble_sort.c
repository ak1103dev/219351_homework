#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 100000

void swap(int *xp, int *yp) {
  int temp = *xp;
  *xp = *yp;
  *yp = temp;
}

// A function to implement bubble sort
void bubbleSort(int arr[], int n) {
  int i, j;
  for (i = 0; i < n-1; i++)      
    for (j = 0; j < n-i-1; j++)
      if (arr[j] > arr[j+1])
	swap(&arr[j], &arr[j+1]);
}

int isSorted(int *a, int size) {
  int i;
  for (i = 0; i < size - 1; i++) {
    if (a[i] > a[i + 1]) {
      return 0;
    }
  }
  return 1;
}

// Function to print an array
void printArray(int arr[], int size)
{
  int i;
  for (i=0; i < size; i++)
    printf("%d ", arr[i]);
  printf("\n");
}


int main(int argc, char** argv) {
	int i, n;
	int* A;
	clock_t start, end;
	double elapsed_time, t1, t2;

	MPI_Init(NULL, NULL);

	t1 = MPI_Wtime();
	A = (int *)malloc(sizeof(int)*N);
	if (A == NULL) {
		printf("Fail to malloc\n");
		exit(0);
	}
	for (i=N-1; i>=0; i--)
		A[N-1-i] = i;
	
	if (isSorted(A, N))
	  printf("Array is sorted\n");
	else
	  printf("Array is NOT sorted\n");
	  
	bubbleSort(A, N);
	printArray(&A[N-10], 10);
	
	if (isSorted(A, N))
	  printf("Array is sorted\n");
	else
	  printf("Array is NOT sorted\n");
	 
	t2 = MPI_Wtime();
	printf( "Elapsed time MPI_Wtime is %f\n", t2 - t1 ); 
	MPI_Finalize();
	return 0;
}
