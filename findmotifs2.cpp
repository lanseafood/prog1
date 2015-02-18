// Implement your solutions in this file
#include "findmotifs.h"
#include "hamming.h"

// implements the sequential findmotifs function



//iterated through each permutation for the first S1. No repeats allowed.

void buildSet(unsigned int l, unsigned int d,
              int *counterP, bits_t bits, std::vector<bits_t> *allPossibleVals){
    
    printf("%llu\n",bits);
    bits ^= 1 << 1;
    printf("%llu\n", bits);
    
    for (int i = 0; i< n ; i++) {
        <#statements#>
    }
    for (int i = 0; i < l; i++){ //for the whole length of the bit (for each bit position)
        if (*counterP <= d){
            for (int j = i; j >= 0; j--){ //for each bit position given the length
                bits ^= 1 << j;
                *counterP++;
                if (std::find((*allPossibleVals).begin(), (*allPossibleVals).end(), bits) != (*allPossibleVals).end()){
                    (*allPossibleVals).push_back(bits);
                }
                buildSet(l,d, counterP, bits, allPossibleVals);
            }
        }
    }
}

std::vector<bits_t> get_all_variations(unsigned int n, unsigned int d, unsigned int l, uint64_t s1, std::vector<bits_t> enumerations, int start, int num_inversions) {

    //int num_inversions = 0;

    for (int i=start; i<l; i++) {
        if (num_inversions < d) {

            if (enumerations.size() == 0) {
                enumerations.push_back(s1); //add the non inverted version
                std::cerr << "s1 no flip" <<s1 << std::endl;
                std::cerr<< "numinv no flip: " <<num_inversions <<std::endl;
            } else if (enumerations.size()>0 && enumerations[enumerations.size()-1] != s1) {
                enumerations.push_back(s1); //add the non inverted version
                std::cerr << "s1 no flip" <<s1 << std::endl;
                std::cerr<< "numinv no flip: " <<num_inversions <<std::endl;
            }

            enumerations = get_all_variations(n, d, l, s1, enumerations, i+1, num_inversions);

            s1 ^= 1 << (l-i-1); //flip ith bit

            enumerations.push_back(s1);
            std::cerr<<"s1 flip: " << s1 << std::endl;
            num_inversions++;
            std::cerr<< "numinv flip: " <<num_inversions <<std::endl;
            enumerations = get_all_variations(n, d, l, s1, enumerations, i+1, num_inversions);
        }
        else { //we've reached d inversions, do nothing...?
            break
        }

    }
    return enumerations;
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
    std::vector<bits_t> *resultP = &result;
    // TODO: implement your solution here
    
    //
    //    printf("%d\n", sizeof(input));
    //    for (int i = 0; i < n; i++){
    //        printf("%llu\n", input[i]);
    //    }
    bits_t possibleMatch; //what we have in our set
    bits_t nextS; //our S1,S2...etc
    bool satisfiedAll = true;
    int hamDist;
    
    int counter = 0;
    int *counterP = &counter;
    
    buildSet(l,d, counterP, input, resultP); //added everything to the set results
    
    printf("finished building set");
    for (int i = 0; i < sizeof(result); i++){
        printf("%llu",result[i]);
    }
    //    for (int i = 0; i < sizeof(result); i++) { //going through all the sets
    //        possibleMatch = result[i];
    //        for (int j = 0; j < n; j++){ //checking for each s1,s2...
    //            nextS = input[j];
    //            hamDist = hamming(nextS, possibleMatch);
    //            if ((hamDist > d) or (hamDist == 0)) { //if it's not d and itself.
    //                result.erase(result.begin()+i); //might need to be i-1
    //                break;
    //            }
    //        }
    //    }
    return result;
}

//OLDER FIRST VERSION
//// Implement your solutions in this file
//#include "findmotifs.h"
//#include "hamming.h"
//
//// implements the sequential findmotifs function
//
//
//
////iterated through each permutation for the first S1. No repeats allowed.
//
//void buildSet(unsigned int l, unsigned int d,
//              int *counterP, bits_t bits, std::vector<bits_t> *allPossibleVals){
//    
//    printf("%llu\n",bits);
//    bits ^= 1 << 1;
//    printf("%llu\n", bits);
//    
//    for (int i = 0; i < l; i++){ //for the whole length of the bit (for each bit position)
//        if (*counterP <= d){
//            for (int j = i; j >= 0; j--){ //for each bit position given the length
//                bits ^= 1 << j;
//                *counterP++;
//                if (std::find((*allPossibleVals).begin(), (*allPossibleVals).end(), bits) != (*allPossibleVals).end()){
//                    (*allPossibleVals).push_back(bits);
//                }
//                buildSet(l,d, counterP, bits, allPossibleVals);
//            }
//        }
//    }
//}
//
//
//std::vector<bits_t> findmotifs(unsigned int n, unsigned int l,
//                               unsigned int d, const bits_t* input)
//{
//    //n is size
//    // l is the size of each number
//    //d is the limit of the hamming distance
//    // input is
//    
//    // If you are not familiar with C++ (using std::vector):
//    // For the output (return value) `result`:
//    //                  The function asks you to return all values which are
//    //                  of a hamming distance `d` from all input values. You
//    //                  should return all these values in the return value
//    //                  `result`, which is a std::vector.
//    //                  For each valid value that you find (i.e., each output
//    //                  value) you add it to the output by doing:
//    //                      result.push_back(value);
//    //                  Note: No other functionality of std::vector is needed.
//    // You can get the size of a vector (number of elements) using:
//    //                      result.size()
//    
//    // create an empty vector
//    std::vector<bits_t> result;
//    std::vector<bits_t> *resultP = &result;
//    // TODO: implement your solution here
//    
//    //
//    //    printf("%d\n", sizeof(input));
//    //    for (int i = 0; i < n; i++){
//    //        printf("%llu\n", input[i]);
//    //    }
//    bits_t possibleMatch; //what we have in our set
//    bits_t nextS; //our S1,S2...etc
//    bool satisfiedAll = true;
//    int hamDist;
//    
//    int counter = 0;
//    int *counterP = &counter;
//    
//    buildSet(l,d, counterP, input[0], resultP); //added everything to the set results
//    printf("finished building set");
//    for (int i = 0; i < sizeof(result); i++){
//        printf("%llu",result[i]);
//    }
//    //    for (int i = 0; i < sizeof(result); i++) { //going through all the sets
//    //        possibleMatch = result[i];
//    //        for (int j = 0; j < n; j++){ //checking for each s1,s2...
//    //            nextS = input[j];
//    //            hamDist = hamming(nextS, possibleMatch);
//    //            if ((hamDist > d) or (hamDist == 0)) { //if it's not d and itself.
//    //                result.erase(result.begin()+i); //might need to be i-1
//    //                break;
//    //            }
//    //        }
//    //    }
//    return result;
//}
//
