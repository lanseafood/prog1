// Implement your solutions in this file
#include "findmotifs.h"
#include "hamming.h"

std::vector<bits_t> get_all_variations(unsigned int n, unsigned int d, unsigned int l, uint64_t s1, std::vector<bits_t> enumerations);

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

    enumerations = get_all_variations(n, d, l, input[0], enumerations);

    for (unsigned j=0; j< enumerations.size(); j++) {
        possible = enumerations[j];


        for (int i=1; i<n; i++) {
            elt = input[i];
            if (hamming(elt, possible) > d) {
                valid = 0;
            }
        }
        if (valid==1) {
            result.push_back(possible);
        }

    }

    // TODO: implement your solution here
    //Psuedocode:
//    for each bit:
//        if up to the d'th bit to flip:
//            stop
//        else:
//        flip or don't flip (try both)
//          add to solution space
//        

    return result;
}

std::vector<bits_t> get_all_variations(unsigned int n, unsigned int d, unsigned int l, uint64_t s1, std::vector<bits_t> enumerations) {

    int num_inversions = 0;

    for (int i=0; i<l; i++) {
        if (num_inversions < d) {
            enumerations.push_back(s1); //add the non inverted version

            s1 ^= 1 << i; //flip ith bit
            enumerations.push_back(s1);
            enumerations = get_all_variations(n, d, l, s1, enumerations);
            
            num_inversions++;
        }
        else { //we've reached d inversions, do nothing...?
            //enumerations.push_back(s1);
        }

    }
    return enumerations;
}

/*int main() {

    unsigned int n=2, l=4, d=2; 
    std::vector<bits_t> results;
    const bits_t input[2] = {9, 8};

    results = findmotifs(n, l, d, input);

    return 0;


}
*/