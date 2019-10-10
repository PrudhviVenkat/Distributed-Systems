#include <mpi.h>
#include <stdio.h>
#include<time.h> 
#include <stdlib.h>
int main(int argc, char** argv) {
	MPI_Init(NULL, NULL);
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int name_len;
	MPI_Get_processor_name(processor_name, &name_len);
	int size[3]={0,0,0};
	if(world_rank==0)			//root process is dividing the number of elements evenly to the processes
	{
		int n;
		printf("enter the number of elements to be sorted:");
		scanf("%d",&n);
		int worldsize1=world_size;				// divides such that each process gets more than 
		int k[3];								// two elements or gets zero elements.
		k[0]=n/2;
		k[2]=n;
		if(world_size>n/2){
			world_size=n/2;
			k[0]=n/2;
		//printf("{%d}\n",world_size);
		size[0]=n/2;
		}
		k[1]=n/world_size;
		int k1=n%world_size;
		size[1]=k[1];
		size[2]=n;
		size[0]=k[0];
		int i=0;
		
		for (i=1;i<world_size-k1;i++)
		{

			MPI_Send(&k, 3, MPI_INT, i, 0, MPI_COMM_WORLD);
			
		}
		k[1]++;
		for (i=world_size-k1;i<worldsize1;i++)
		{

			MPI_Send(&k, 3, MPI_INT, i, 0, MPI_COMM_WORLD);
			
		}

	}
	//printf("%d\n",world_size );
	if(world_rank!=0)
		MPI_Recv(&size, 3, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	//printf("%d:%d:%d\n",world_rank,world_size,size[0] );
	if(world_size>size[0]){
		world_size=size[0];
	}
	
	//MPI_Barrier(MPI_COMM_WORLD);
	if(world_rank<world_size){
	
		//printf("%d:%d:%d\n",world_rank,world_size,size[1] );
	int A[size[1]];
	time_t t;
	srand(world_rank+ (unsigned) time(&t));
	for(int i=0;i<size[1];i++)
	{
		A[i] = rand() % 100 + 1;		//each process is generating random numbers as a 
	}

										//part of the whole array of elements
	if(world_rank!=0)
	MPI_Send(&A, size[1], MPI_INT, 0, 0, MPI_COMM_WORLD);
	else 										//printing elements before sorting
	{
		for(int i=0;i<size[1];i++)
		{
			printf("%d ",A[i] );
		}
		int B[size[1]+1];
		int p1=size[2]%world_size;
		//printf("{%d}\n",world_size-p1 );
		for (int i = 1; i < world_size-p1; i++)
		{
			MPI_Recv(&B, size[1], MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			for (int j = 0; j < size[1]; j++)
			{
				printf("%d ",B[j]);
			}
		}
		for (int i = world_size-p1; i < world_size; i++)
		{
			MPI_Recv(&B, size[1]+1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			for (int j = 0; j < size[1]+1; j++)
			{
				printf("%d ",B[j]);
			}
		}
		printf("\n");

	}
	fflush(stdout);
	
	
	for(int i=0;i<size[2];i++)			//sorting
	{ int p;
		for(int j=0;j<size[1]-1;j+=2)
		{
			if(A[j]>A[j+1])
			{
				int t=A[j+1];
				A[j+1]=A[j];
				A[j]=t;
			}
		}
		for(int j=1;j<size[1]-1;j+=2)
		{
			if(A[j]>A[j+1])
			{
				int t=A[j+1];
				A[j+1]=A[j];
				A[j]=t;
			}
		}
		if(world_rank!=0)
		{
			MPI_Send(&A[0], 1, MPI_INT, world_rank-1, 0, MPI_COMM_WORLD);
		}
		if(world_rank!=world_size-1)
		{
			MPI_Recv(&p, 1, MPI_INT, world_rank+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		if(A[size[1]-1]<p)
		{
				int temp1=p;
				p=A[size[1]-1];
				A[size[1]-1]=temp1;
		}

		}

		if(world_rank!=world_size-1){
			MPI_Send(&A[size[1]-1], 1, MPI_INT, world_rank+1, 0, MPI_COMM_WORLD);
			A[size[1]-1]=p;
		}
		if(world_rank!=0)
			MPI_Recv(&A[0], 1, MPI_INT, world_rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}

	if (world_rank==0)
	{
		printf("\nsorted elements are:\n");
	}
	if(world_rank!=0)						//printing elements after sorting
	MPI_Send(&A, size[1], MPI_INT, 0, 0, MPI_COMM_WORLD);
	else
	{
		for(int i=0;i<size[1];i++)
		{
			printf("%d ",A[i] );
		}
		int B[size[1]+1];
		int p1=size[2]%world_size;
		for (int i = 1; i < world_size-p1; i++)
		{
			MPI_Recv(&B, size[1], MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			for (int j = 0; j < size[1]; j++)
			{
				printf("%d ",B[j]);
			}
		}
		for (int i = world_size-p1; i < world_size; i++)
		{
			MPI_Recv(&B, size[1]+1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			for (int j = 0; j < size[1]+1; j++)
			{
				printf("%d ",B[j]);
			}
		}
		printf("\n");

	}}
	MPI_Finalize();
}
