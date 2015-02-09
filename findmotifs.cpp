// Implement your solutions in this file
#include "findmotifs.h"
#include "hamming.h"

// implements the sequential findmotifs function



//iterated through each permutation for the first S1. No repeats allowed.

std::vector<bits_t> buildSet(unsigned int l, unsigned int d,
                       int counter, bits_t bits, std::vector<bits_t> allPossibleVals){
    
    //std::vector<bits_t> allPossibleVals;
    //int count = 0;
    
    for (int i = 0; i < l; i++){ //for the whole length
        if (counter <= d){
            for (int j = i; j >= 0; j--){ //for each bit position given the length
                bits ^= 1 << j;
                counter++;
                if (std::find(allPossibleVals.begin(), allPossibleVals.end(), bits) != allPossibleVals.end()){
                    allPossibleVals.push_back(bits);
                }
                allPossibleVals = buildSet(l,d, counter, bits, allPossibleVals);
            }
        }
    }
    return allPossibleVals;
}


std::vector<bits_t> findmotifs(unsigned int n, unsigned int l,
                               unsigned int d, const bits_t* input)
{
    //n is size
    // l is the size of each number
    //d is the limit of the hamming distance
    // input is
    
    // If you are not familiar with C++ (using std::vector):
    // For the output (return value) `result`:
    //                  The function asks you to return all values which are
    //                  of a hamming distance `d` from all input values. You
    //                  should return all these values in the return value
    //                  `result`, which is a std::vector.
    //                  For each valid value that you find (i.e., each output
    //                  value) you add it to the output by doing:
    //                      result.push_back(value);
    //                  Note: No other functionality of std::vector is needed.
    // You can get the size of a vector (number of elements) using:
    //                      result.size()

    // create an empty vector
    std::vector<bits_t> result;

    // TODO: implement your solution here

//
    unsigned int currentBits = 0; //what we have in our set
    unsigned int originalVal = 0; //our S1,S2...etc
    bool satisfiedAll = true;
    int hamDist;

    result = buildSet(l,d,0, input[0], result); //added everything to the set results
    
    for (int i = 0; i < sizeof(result); i++) { //going through all the sets
        currentBits = result[i];
        for (int j = 0; j < n; j++){ //checking for each s1,s2...
            originalVal = input[j];
            hamDist = hamming(originalVal, currentBits);
            if ((hamDist > d) or (hamDist == 0)) { //if it's not d and itself.
                result.erase(result.begin()+i);
                break;
            }
        }
    }
    return result;
}

