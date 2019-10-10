#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void mpi_iittp_barrier()
{
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	int k;											//no. of children for each process.
	if(world_size<2)
		k=1;
	else if(world_size<7)							// if no. of children decreases, work load is  
		k=2;										// distributed more accuratly. But becomes more serialized  
	else if(world_size<40)							//and height of tree increases. 
		k=3;										// So I assumed if number of children is equal to 
	else if(world_size<341)							// the height of the tree, then it will be optimal.
		k=4;										// so if no. of processes is 3906, no. of children
	else if(world_size<3906)						// should be 5 so that height of the tree will be 5.
		k=5;
	else if(world_size<55987)
		k=6;
	else if(world_size<960800)
		k=7;
	else if(world_size<19173961)
		k=8;
	else if(world_size<435848050)
		k=9;
	else
		k=10;
	int level=0,sum=0,p=1;
	while(sum<=world_rank)
	{
		sum+=p;
		level++;
		p=p*k;
	}
	p=p/k;
	sum-=p;
	int child=sum+p+(world_rank-sum)*k;				//index of first child
	int parent =sum-p/k+(world_rank-sum)/k;			// index of parent


	//printf("%d :%d %d\n",world_rank,child,parent);
	for(int i=child;i<child+k && i<world_size;i++)
	{
		MPI_Recv(&k, 1, MPI_INT, i, 0, MPI_COMM_WORLD,			
               MPI_STATUS_IGNORE);							//receiving message from children
	}
	if(world_rank!=0)
	MPI_Send(&k, 1, MPI_INT, parent, 0, MPI_COMM_WORLD);	//sending message to parent

	if(world_rank!=0)
	MPI_Recv(&k, 1, MPI_INT, parent, 0, MPI_COMM_WORLD,
               MPI_STATUS_IGNORE);							//receiving message from parent
	for(int i=child;i<child+k && i<world_size;i++)
	{
		MPI_Send(&k, 1, MPI_INT, i, 0, MPI_COMM_WORLD);		//sending message to chlidren
	}


}

int main(int argc, char const *argv[])
{
	MPI_Init(NULL, NULL);
	mpi_iittp_barrier();
	MPI_Finalize();
	return 0;
}