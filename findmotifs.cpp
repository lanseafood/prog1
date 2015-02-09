// Implement your solutions in this file
#include "findmotifs.h"
#include "hamming.h"
#include <iostream>
#include <algorithm>

std::vector<bits_t> get_all_variations(unsigned int n, unsigned int d, unsigned int l, uint64_t s1, std::vector<bits_t> enumerations, int start, int num_inversions);

// implements the sequential findmotifs function
std::vector<bits_t> findmotifs(unsigned int n, unsigned int l,
                               unsigned int d, const bits_t* input)
{
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


    //now compare "s1" to all other ones in input
    uint64_t possible;
    uint64_t elt;
    int valid = 1;

    std::vector<bits_t> enumerations;

    enumerations = get_all_variations(n, d, l, input[0], enumerations, 0, 0);

    std::sort(enumerations.begin(), enumerations.end());
    enumerations.erase(std::unique(enumerations.begin(), enumerations.end()), enumerations.end());

    for (unsigned j=0; j< enumerations.size(); j++) {
        possible = enumerations[j];

        //std::cerr << "possible" << possible << " ";

        for (int i=1; i<n; i++) {
            elt = input[i];
            if (hamming(elt, possible) > d) {
                //std::cerr << "im invalid"<< std::endl;
                valid = 0; //i am invalid
            }
        }
        if (valid==1) { //if valid against all si's
            result.push_back(possible);
        }

    }



    return result;
}

std::vector<bits_t> get_all_variations(unsigned int n, unsigned int d, unsigned int l, uint64_t s1, std::vector<bits_t> enumerations, int start, int num_inversions) {

    //int num_inversions = 0;

    for (int i=start; i<l; i++) {
        if (num_inversions < d) {

//bool isPresent = (std::find(vec.begin(), vec.end(), target) != vec.end());

 
            enumerations.push_back(s1); //add the non inverted version
//            std::cerr << "hi" <<s1 << std::endl;

            enumerations = get_all_variations(n, d, l, s1, enumerations, i+1, num_inversions);
        
            s1 ^= 1 << (l-i-1); //flip ith bit
    
            enumerations.push_back(s1);
 //           std::cerr << s1 << std::endl;
            num_inversions++;
            enumerations = get_all_variations(n, d, l, s1, enumerations, i+1, num_inversions);
        }
        else { //we've reached d inversions, do nothing...?
            //enumerations.push_back(s1);
        }

    }
    return enumerations;
}
