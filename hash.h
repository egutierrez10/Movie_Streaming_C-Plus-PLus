/*hash.h*/

#include <string>

using namespace std;

// Hash Function
int HashId(int ID, int N){
    return ID % N;
}