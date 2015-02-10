// Implement your solutions in this file
#include "findmotifs.h"
#include "hamming.h"
#include <mpi.h>
#include <algorithm>

#define WORK 1
#define DIE 2

std::vector<bits_t> get_all(unsigned int n, unsigned int d, unsigned int l, uint64_t s1, std::vector<bits_t> enumerations, int start, unsigned num_inversions, unsigned int k);

std::vector<bits_t> findmotifs_worker(const unsigned int n,
                       const unsigned int l,
                       const unsigned int d,
                       const bits_t* input,
                       const unsigned int startbitpos,
                       bits_t start_value)
{
    std::vector<bits_t> results;
    bits_t s1, possible, elt;
    unsigned int num_inversions;
    unsigned int k;
    int valid = 1;
    std::vector<bits_t> enumerations;

    //compare s1 to start_value to find num inversions
    s1 = input[0];
    num_inversions = hamming(s1, start_value);

    k=startbitpos;

    //solve from k to l and produce results
    enumerations = get_all(n, d, l, s1, enumerations, k, 0, l); //CHECK THIS LOGIC

    std::sort(enumerations.begin(), enumerations.end());
    enumerations.erase(std::unique(enumerations.begin(), enumerations.end()), enumerations.end());

    for (unsigned j=0; j< enumerations.size(); j++) {
        possible = enumerations[j];

        //std::cerr << "possible" << possible << " ";

        for (unsigned i=1; i<n; i++) {
            elt = input[i];
            if (hamming(elt, possible) > d) {
                //std::cerr << "im invalid"<< std::endl;
                valid = 0; //i am invalid
            }
        }
        if (valid==1) { //if valid against all si's
            results.push_back(possible);
        }

    }


    return results;
}

void worker_main()
{

  //so has the master solved up unTIL master_depth, or through master-depth?

  uint64_t partial;
  MPI_Status status;
  std::vector<bits_t> worker_results;
  unsigned int n, d, l;
  bits_t* input;
  unsigned int master_depth;
  
  //do these input receiving lines belong here or in loop?
  MPI_Recv(&n, 1, MPI_UNSIGNED, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
  MPI_Recv(&l, 1, MPI_UNSIGNED, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
  MPI_Recv(&d, 1, MPI_UNSIGNED, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
  MPI_Recv(&input, 1, MPI_UNSIGNED, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
  MPI_Recv(&master_depth, 1, MPI_UNSIGNED, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

  //need to receive input from master... above??? 

  //receive work from master
  while(1) {
    MPI_Recv(&partial, 1, MPI_UNSIGNED, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

    if (status.MPI_TAG == DIE) {
      return;
    }

    //else do work
    worker_results = findmotifs_worker(n, l, d, input, master_depth, partial);\

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

//should give all variations up til first k bits???
std::vector<bits_t> get_all(unsigned int n, unsigned int d, unsigned int l, uint64_t s1, std::vector<bits_t> enumerations, int start, unsigned num_inversions, unsigned int k) {

    //int num_inversions = 0;

    for (unsigned i=start; i<k; i++) {
        if (num_inversions < d) {

//bool isPresent = (std::find(vec.begin(), vec.end(), target) != vec.end());

 
            enumerations.push_back(s1); //add the non inverted version
//            std::cerr << "hi" <<s1 << std::endl;

            enumerations = get_all(n, d, l, s1, enumerations, i+1, num_inversions, k);
        
            s1 ^= 1 << (l-i-1); //flip ith bit
    
            enumerations.push_back(s1);
 //           std::cerr << s1 << std::endl;
            num_inversions++;
            enumerations = get_all(n, d, l, s1, enumerations, i+1, num_inversions, k);
        }
        else { //we've reached d inversions, do nothing...?
            //enumerations.push_back(s1);
        }

    }
    return enumerations;
}


std::vector<bits_t> findmotifs_master(const unsigned int n,
                                      const unsigned int l,
                                      const unsigned int d,
                                      const bits_t* input,
                                      const unsigned int till_depth)
{
    std::vector<bits_t> results;

    uint64_t possible;
    uint64_t elt;
    int valid = 1;

    std::vector<bits_t> enumerations;

    enumerations = get_all(n, d, l, input[0], enumerations, 0, 0, till_depth);

    std::sort(enumerations.begin(), enumerations.end());
    enumerations.erase(std::unique(enumerations.begin(), enumerations.end()), enumerations.end());

    for (unsigned j=0; j< enumerations.size(); j++) {
        possible = enumerations[j];

        //std::cerr << "possible" << possible << " ";

        for (unsigned i=1; i<n; i++) {
            elt = input[i];
            if (hamming(elt, possible) > d) {
                //std::cerr << "im invalid"<< std::endl;
                valid = 0; //i am invalid
            }
        }
        if (valid==1) { //if valid against all si's
            results.push_back(possible);
        }

    }


    return results;
}

std::vector<bits_t> master_main(unsigned int n, unsigned int l, unsigned int d,
                                const bits_t* input, unsigned int master_depth)
{

  std::vector<bits_t> results;

  int num_tasks, rank;
  MPI_Status status;
  std::vector<bits_t> master_partial;
  uint64_t send_to_worker;
  std::vector<bits_t> worker_results;
  MPI_Comm_size(MPI_COMM_WORLD, &num_tasks);
  unsigned i;

  master_partial = findmotifs_master(n, l, d, input, master_depth);

  //send INPUT to workers...???

  for (rank=1; rank<num_tasks; rank++) {
    MPI_Send(&n, 1, MPI_UNSIGNED, rank, WORK, MPI_COMM_WORLD);
    MPI_Send(&l, 1, MPI_UNSIGNED, rank, WORK, MPI_COMM_WORLD);
    MPI_Send(&d, 1, MPI_UNSIGNED, rank, WORK, MPI_COMM_WORLD);
    MPI_Send(&input, 1, MPI_UNSIGNED, rank, WORK, MPI_COMM_WORLD);
    MPI_Send(&master_depth, 1, MPI_UNSIGNED, rank, WORK, MPI_COMM_WORLD);


    send_to_worker = master_partial[rank-1]; //figure out what to do if doesn't exist
    MPI_Send(&send_to_worker, 1, MPI_UNSIGNED, rank, WORK, MPI_COMM_WORLD);
  }

  //loop over getting work til no more work remains

  i = num_tasks;
  while (i!=master_partial.size()) { //is my criteria for stopping correct?
    //receive results from slave -- is this mpi-unsigned??
    MPI_Recv(&worker_results, 1, MPI_UNSIGNED, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

    //add worker results to global results
    for (unsigned j=0; j<worker_results.size(); j++) {
      results.push_back(worker_results[j]);
    }

    send_to_worker = master_partial[i]; //figure out what to do if doesn't exist
    MPI_Send(&send_to_worker, 1, MPI_UNSIGNED, status.MPI_SOURCE, WORK, MPI_COMM_WORLD);
    i++;
  }

  //receive all outstanding solns?
  for (rank=1; rank<num_tasks; rank++) {
    MPI_Recv(&worker_results, 1, MPI_UNSIGNED, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    //add worker results to global results
    for (unsigned j=0; j<worker_results.size(); j++) {
      results.push_back(worker_results[j]);
    }
  }

  //all slaves die
  for (rank=1; rank<num_tasks; rank++) {
    MPI_Send(0, 0, MPI_INT, rank, DIE, MPI_COMM_WORLD);
  }

 /*   // TODO
    // 1.) send input to all workers (including n, l, d, input, depth) <-- seeding the slaves?

    // 2.) solve problem till depth `master_depth` and then send subproblems
    //     to the workers and receive solutions in each communication
    //     Use your implementation of `findmotifs_master(...)` here.
    std::vector<bits_t> results;


    // 3.) receive last round of solutions
    // 4.) terminate (and let the workers know)
*/
    return results;
}

