#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 100000

int * merge(int *v1, int n1, int *v2, int n2)
{
	int i,j,k;
	int * result;

	result = (int *)malloc((n1+n2)*sizeof(int));

	/*
		i : pointer of v1
		j : pointer of v2
		k : pointer of k
	*/
	i=0; j=0; k=0;
	while(i<n1 && j<n2)
		if(v1[i]<v2[j])
		{
			result[k] = v1[i];
			i++; k++;
		}
		else
		{
			result[k] = v2[j];
			j++; k++;
		}
	if(i==n1)
		while(j<n2)
		{
			result[k] = v2[j];
			j++; k++;
		}
	else
		while(i<n1)
		{
			result[k] = v1[i];
			i++; k++;
		}
	return result;
}

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
	int i;
	int* A;
	int* chunk;
	int* other;
	clock_t start, end;
	double elapsed_time, t1, t2;

	int step;
	int m;
	MPI_Status status;

	int id, p;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&id);
	MPI_Comm_size(MPI_COMM_WORLD,&p);

	int s;

	if(id == 0) {
		int r;
		s = N/p;
		r = N%p;
		t1 = MPI_Wtime();
		A = (int *)malloc(sizeof(int)*N);
		if (A == NULL) {
			printf("Fail to malloc\n");
			exit(0);
		}
		for (i=N-1; i>=0; i--)
			A[N-1-i] = i;
		if(r != 0) {
			for(i = N; i < N+p-r; i++)
				A[i] = 0;
			s=s+1;
		}

		MPI_Bcast(&s,1,MPI_INT,0,MPI_COMM_WORLD);
		chunk = (int *)malloc(s*sizeof(int));
		MPI_Scatter(A,s,MPI_INT,chunk,s,MPI_INT,0,MPI_COMM_WORLD);

		if (isSorted(A, N))
		  printf("Array is sorted\n");
		else
		  printf("Array is NOT sorted\n");

		bubbleSort(chunk,s);
		printArray(&A[N-10], 10);
	}
	else {
		MPI_Bcast(&s,1,MPI_INT,0,MPI_COMM_WORLD);
		chunk = (int *)malloc(s*sizeof(int));
		MPI_Scatter(A,s,MPI_INT,chunk,s,MPI_INT,0,MPI_COMM_WORLD);

		bubbleSort(chunk,s);
	}

	step = 1;
	while(step<p)
	{
		if(id%(2*step)==0)
		{
			if(id+step<p)
			{
				MPI_Recv(&m,1,MPI_INT,id+step,0,MPI_COMM_WORLD,&status);
				other = (int *)malloc(m*sizeof(int));
				MPI_Recv(other,m,MPI_INT,id+step,0,MPI_COMM_WORLD,&status);
				chunk = merge(chunk,s,other,m);
				s = s+m;
			}
		}
		else
		{
			int near = id-step;
			MPI_Send(&s,1,MPI_INT,near,0,MPI_COMM_WORLD);
			MPI_Send(chunk,s,MPI_INT,near,0,MPI_COMM_WORLD);
			break;
		}
		step = step*2;
	}
	if(id==0)
	{
		if (isSorted(chunk, N))
		  printf("Array is sorted\n");
		else
		  printf("Array is NOT sorted\n");

		t2 = MPI_Wtime();
		printf( "Elapsed time MPI_Wtime is %f\n", t2 - t1 );
	}
	MPI_Finalize();
	return 0;
}
