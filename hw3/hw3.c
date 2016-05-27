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

int *merge(int * v1, int n1, int * v2, int n2)
{
  int * result = (int *)malloc((n1 + n2) * sizeof(int));
  int i = 0;
  int j = 0;
  int k;
  for (k = 0; k < n1 + n2; k++) {
      if (i >= n1) {
	        result[k] = v2[j];
	        j++;
	      }
      else if (j >= n2) {
	        result[k] = v1[i];
	        i++;
	      }
      else if (v1[i] < v2[j]) { // indices in bounds as i < n1 && j < n2
	        result[k] = v1[i];
	        i++;
	      }
      else { // v2[j] <= v1[i]
	        result[k] = v2[j];
	        j++;
	      }
    }
  return result;
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
	int* A = NULL;
	clock_t start, end;
	double elapsed_time, t1, t2;

	int rank, size;
	int c;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if(rank == 0) {
		// compute chunk size
		c = N/size;
		if (N%size) c++;
		// read data from file
		A = (int *)malloc(size*c * sizeof(int));
		if (A == NULL) {
			printf("Fail to malloc\n");
			exit(0);
		}
		for (i=N-1; i>=0; i--)
			A[N-1-i] = i;
		// pad data with 0 -- doesn't matter
		//for (i = n; i < p*c; i++)
		 // data[i] = 0;
	}

	t1 = MPI_Wtime();
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
