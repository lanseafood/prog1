// Implement your solutions in this file
#include "findmotifs.h"
#include "hamming.h"
#include <iostream>
#include <algorithm>

 struct Node {

         uint64_t val; 
         int num_inv;

         Node *left;   
         Node *right;  

         /*Node(uint64_t item, int num) {
            val = item;
            num_inv = num;
            left = NULL;
            right = NULL;
         }*/
}; 


std::vector<bits_t> get_all_variations(unsigned int n, unsigned int d, unsigned int l, uint64_t s1, std::vector<bits_t> enumerations, int start, int num_inversions);
void insert_into_tree(unsigned int n, unsigned int d, unsigned int l, uint64_t s1, Node *tree, int start, int num_inversions);



bool find(Node *root, uint64_t val) {

    if (root==NULL) {
        return false;
    } else if (val==root->val) {
        return true;
    } else if (val < root->val) {
        return find(root->left, val);
    } else {
        return find(root->right, val);
    }
}

void insert(Node **root, uint64_t val, int num_inv) {
    Node *temp = NULL;
    uint64_t rootval;

    if (!(*root)) {
        temp = (Node *)malloc(sizeof(Node));
        temp->left = NULL;
        temp->right = NULL;
        temp->val = val;
        temp->num_inv = num_inv;
        *root = temp;
        return;
    }

    if (val < (*root)->val) {
        //rootval= (*root)->val;
        //std::cerr<<"inserting left "<<val<<", root val: " <<rootval<<std::endl;
        insert(&(*root)->left, val, num_inv);
    } else {
        //std::cerr<<"inserting right "<<val<<std::endl;
        insert(&(*root)->right, val, num_inv);
    }
}


 void postorderPrint( Node *root ) {
           // Print all the items in the tree to which root points.
           // The items in the left subtree are printed first, followed 
           // by the items in the right subtree and then the item in the
           // root node.
        if ( root != NULL ) {  // (Otherwise, there's nothing to print.)
           postorderPrint( root->left );    // Print items in left subtree.
           postorderPrint( root->right );   // Print items in right subtree.
           std::cerr << root->val << " ";       // Print the root item.
        }
} // end postorderPrint()



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
 /*   std::vector<bits_t> result;


    //now compare "s1" to all other ones in input
    uint64_t possible;
    uint64_t elt;
    int valid = 1;

    uint64_t s1=input[0];

    std::vector<bits_t> enumerations;

    //Node *root = new Node(s1,0);
    Node *root = (Node *)malloc(sizeof(Node));
    root->val = s1;
    root->num_inv = 0;
    root->left = NULL;
    root->right = NULL;

    insert_into_tree(n,d,l,s1,root,0,0);
    //insert(&root, input[1], 0);
    //insert(root, input, 0);
    //std::cerr<<"root: " <<root->val << ", root left: " << root->left->val << std::endl;

    postorderPrint(root);


 /*   enumerations = get_all_variations(n, d, l, input[0], enumerations, 0, 0);

    std::cerr<<"DONE"<<std::endl;
    return enumerations;

    std::sort(enumerations.begin(), enumerations.end());
    enumerations.erase(std::unique(enumerations.begin(), enumerations.end()), enumerations.end());

    for (unsigned i = 0; i<enumerations.size(); i++) {
        std::cerr<<"enum: " << enumerations[i]<<std::endl;
    }

    for (unsigned j=0; j< enumerations.size(); j++) {
        possible = enumerations[j];

        //std::cerr << "possible" << possible << " ";
        valid = 1;
        for (int i=1; i<n; i++) {
            elt = input[i];
            if (hamming(elt, possible) > d) {
            //    std::cerr << "im invalid: "<< possible << std::endl;
                valid = 0; //i am invalid
            }
        }
        if (valid==1) { //if valid against all si's
            result.push_back(possible);
        }

    }

*/



    //DYNAMIC PROGRAMMING
    std::vector<std::vector<bits_t> > ans;
    std::vector<bits_t> temp;
    std::vector<std::vector<bits_t> > prev;
    unsigned i_bit, i_inv;
    uint64_t s1 = input[0];

    for (unsigned i=0; i<=l; i++) {
        temp.clear();
        //std::cerr<<"size: " <<temp.size()<<std::endl;
        temp.push_back(s1%(1<<i));
        ans.push_back(temp);
    }
    //ans[l].push_back(s1);

    for (unsigned int i=1; i<=d; i++) {
        prev = ans;
        for (unsigned int j=0; j<prev.size(); j++) {
            ans[j].clear();
            //std::cerr<<"size: " <<ans[j].size()<<std::endl;
        }

        for (unsigned int j=i; j<=l; j++) {
            i_bit = s1&(1<<j-1);
            i_inv = i_bit ^ (1<<j-1);

            std::cerr<<"j: " <<j<<std::endl;
            for (unsigned int z=0; z<prev[j-1].size(); z++) {
                ans[j].push_back(i_inv + prev[j-1][z]);
                ans[j].push_back(prev[j-1][z]);
                //std::cerr<<"hey: " <<prev[j-1][z]<<std::endl;
            }
            
            for (unsigned int z=0; z<ans[j-1].size(); z++) {
                ans[j].push_back(i_bit + ans[j-1][z]);
            }

            //for (unsigned int z=0; z<prev[j-1].size(); z++) {
            //    ans[j].push_back(prev[j-1][z]);
            //}

        }
        std::cerr<<"i: "<<i<<std::endl;
    }

    ans[l].push_back(s1);

    std::sort(ans[l].begin(), ans[l].end());
    ans[l].erase(std::unique(ans[l].begin(), ans[l].end()), ans[l].end());

    int valid = 0;
    std::vector<bits_t> result;
    uint64_t possible, elt;

    for (unsigned j=0; j< ans[l].size(); j++) {
        possible = ans[l][j];

        //std::cerr << "possible" << possible << " ";
        valid = 1;
        for (int i=1; i<n; i++) {
            elt = input[i];
            if (hamming(elt, possible) > d) {
            //    std::cerr << "im invalid: "<< possible << std::endl;
                valid = 0; //i am invalid
            }
        }
        if (valid==1) { //if valid against all si's
            result.push_back(possible);
        }

        std::cerr<<"j: "<<j<<std::endl;
    }

    uint64_t h = 6903923;

    std::cerr<<"HI: " << hamming(h, s1)<<std::endl;

    return result;
}

void insert_into_tree(unsigned int n, unsigned int d, unsigned int l, uint64_t s1, Node *tree, int start, int num_inversions) {

    uint64_t val = s1;
    bool check = false;

    for (int i=start; i<l; i++) {
        if (num_inversions < d) {
            check = find(tree, val);

            if (!check) { //non flipped bit
                std::cerr<<"find: " <<check << ", val: " <<val<<std::endl;
                insert(&tree, val, num_inversions);
            }

            insert_into_tree(n,d,l,val, tree,i+1,num_inversions); //no flip

            //flipped bit
            val ^= 1<<(l-i-1); //flip ith bit
            check = find(tree, val);
            num_inversions++;
            if (!check) {
                std::cerr<<"flip find: " <<check << ", val: " <<val<<", i: "<<i<<std::endl;
                insert(&tree, val, num_inversions);
            }

            
            insert_into_tree(n,d,l,val,tree,i+1,num_inversions); //flip
        
        } else {
            if (!find(tree, val)) { //if doesn't exist
                insert(&tree, val, num_inversions);
            }
        }
    }

}

std::vector<bits_t> get_all_variations(unsigned int n, unsigned int d, unsigned int l, uint64_t s1, std::vector<bits_t> enumerations, int start, int num_inversions) {

    //int num_inversions = 0;

    if (num_inversions >= d || start>=l) {
        std::cerr<<"REACHED D: "<<d<<std::endl;
        return enumerations;
    }

    for (int i=start; i<l; i++) {
        if (num_inversions < d) {

//bool isPresent = (std::find(vec.begin(), vec.end(), target) != vec.end());

 
            //TO_DO EISHA: its inserting the same one multiple times
            //std::cerr<<"size: " <<enumerations.size()<<std::cerr;
            if (enumerations.size() == 0) {
                enumerations.push_back(s1); //add the non inverted version
                //std::cerr << "s1 no flip" <<s1 << std::endl;
                std::cerr<< "numinv no flip: " <<num_inversions <<std::endl;
            } else if (enumerations.size()>0 && enumerations[enumerations.size()-1] != s1) {
                enumerations.push_back(s1); //add the non inverted version
                std::cerr << "s1 no flip" <<s1 << std::endl;
                std::cerr<< "numinv no flip: " <<num_inversions <<std::endl;
            }
            
            std::cerr<<"l="<<l<<", i="<<i<<", sending i+1="<<i+1<<std::endl;
            enumerations = get_all_variations(n, d, l, s1, enumerations, i+1, num_inversions);
        
            s1 ^= 1 << (l-i-1); //flip ith bit
    
            enumerations.push_back(s1);
            //std::cerr<<"s1 flip: " << s1 << std::endl;
 //           std::cerr << s1 << std::endl;
            num_inversions++;
            std::cerr<< "numinv flip: " <<num_inversions <<std::endl;
            enumerations = get_all_variations(n, d, l, s1, enumerations, i+1, num_inversions);
        }
        else { //we've reached d inversions, do nothing...?
            //enumerations.push_back(s1);
        }

    }
    return enumerations;
}




























