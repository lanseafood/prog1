// Implement your solutions in this file
#include "findmotifs.h"
#include "hamming.h"
#include <mpi.h>

#define WORK 1
#define DIE 2

std::vector<bits_t> findmotifs_worker(const unsigned int n,
                       const unsigned int l,
                       const unsigned int d,
                       const bits_t* input,
                       const unsigned int startbitpos,
                       bits_t start_value)
{
    std::vector<bits_t> results;

    // TODO: implement your solution here

    return results;
}

void worker_main()
{

  std::vector<bits_t> partial_results;
  MPI_Status status;
  std::vector<bits_t> worker_results;

  //how to receive input from master...?

  while(1) {
    //receive msg from master
    MPI_Recv(&partial_results, 1, MPI_UNSIGNED, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    
    if (status.MPI_TAG == DIE) {
      return;
    }

    //else do work
    worker_results = findmotifs_worker(); //need to first figure out how to send input?

    //send the result back
    MPI_Send(&worker_results, 1, MPI_UNSIGNED, 0, 0, MPI_COMM_WORLD);



  }

    // TODO:
    // 1.) receive input from master (including n, l, d, input, master-depth)

    // 2.) while the master is sending work:
    //      a) receive subproblems
    //      b) locally solve (using findmotifs_worker(...))
    //      c) send results to master

    // 3.) you have to figure out a communication protocoll:
    //     - how does the master tell the workers that no more work will come?
    //       (i.e., when to break loop 2)
    // 4.) clean up: e.g. free allocated space
}



std::vector<bits_t> findmotifs_master(const unsigned int n,
                                      const unsigned int l,
                                      const unsigned int d,
                                      const bits_t* input,
                                      const unsigned int till_depth)
{
    std::vector<bits_t> results;

    // TODO: implement your solution here

    return results;
}

std::vector<bits_t> master_main(unsigned int n, unsigned int l, unsigned int d,
                                const bits_t* input, unsigned int master_depth)
{

 
  int num_tasks, rank;
  MPI_Status status;
  std::vector<bits_t> partial_results;
  std::vector<bits_t> worker_results; //???

  unsigned int k;

  MPI_Comm_size(MPI_COMM_WORLD, &num_tasks);

  for (rank=1; rank<num_tasks; ++rank) {

    partial_results = findmotifs_master(n, l, d, input, k);

    MPI_Send(&partial_results, 1, MPI_UNSIGNED, rank, WORK, MPI_COMM_WORLD);

  }

  //loop over getting work til no more work remains
  partial_results = findmotifs_master(n, l, d, input, k);

  while (partial_results != NULL) {
    //receive results from slave
    MPI_Recv(&worker_results, 1, MPI_UNSIGNED, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, & status);

    //send slave a new work unit?
    MPI_Send(&partial_results, 1, MPI_UNSIGNED, status.MPI_SOURCE, WORK, MPI_COMM_WORLD);

    //get next unit of work to be done??
    partial_results = findmotifs_master(n, l, d, input, k);
  }

  //receive all outstanding 
  for (rank=1; rank<num_tasks; ++rank) {
    MPI_Recv(&worker_results, 1, MPI_UNSIGNED, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
  }

  //all slaves die
  for (rank=1; rank<num_tasks; ++rank) {
    MPI_Send(0, 0, MPI_INIT, rank, DIE, MPI_COMM_WORLD);
  }

    // TODO
    // 1.) send input to all workers (including n, l, d, input, depth) <-- seeding the slaves?

    // 2.) solve problem till depth `master_depth` and then send subproblems
    //     to the workers and receive solutions in each communication
    //     Use your implementation of `findmotifs_master(...)` here.
    std::vector<bits_t> results;


    // 3.) receive last round of solutions
    // 4.) terminate (and let the workers know)

    return results;
}

