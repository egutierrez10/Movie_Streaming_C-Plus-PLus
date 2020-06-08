/*hashmap.h*/

#pragma once

#include <iostream>
#include <functional>
#include <unordered_set>

using namespace std;

template<typename KeyT, typename ValueT>
class hashmap
{
private:
  struct HashEntry
  {
    bool   ContainsData;  // false => empty, true => contains data
    KeyT   Key;           // (key, value)
    ValueT Value;

    HashEntry()
    {
      ContainsData = false;
    }
  };

  HashEntry* HT;  // hash table array of structs
  int        N;   // capacity of hash table (# of locations or array size)

public:
  //
  // constructor: N is the size of the underlying hash table
  //
  hashmap(int N)
  {
    this->N = N;
    this->HT = new HashEntry[N];
  }
  
  //
  // destructor
  //
  virtual ~hashmap()
  {
    delete[] this->HT;
  }

	//
	// copy constructor:
	//
	hashmap(const hashmap& other){
	  this->N = other.N;
	  this->HT = new HashEntry[N];
	  for(int i = 0; i < N; ++i){ // Perform deep copy of other hash table 
		this->HT[i].Key = other.HT[i].Key;
		this->HT[i].Value = other.HT[i].Value;
		this->HT[i].ContainsData = other.HT[i].ContainsData;
	  }
	}

	
	//
	//operator= 
	//
	hashmap& operator=(const hashmap& other){
 	   delete[] this->HT; 
	   this->N = other.N;
	   this->HT = new HashEntry[N];
	   for(int i = 0; i < this->N; ++i){ // Perform deep copy of other hash table 
		this->HT[i].Key = other.HT[i].Key;
		this->HT[i].Value = other.HT[i].Value;
		this->HT[i].ContainsData = other.HT[i].ContainsData;
	  }
 	  return *this;
	}


  //
  // getN
  //
  // Returns the capacity of the hash table, i.e. the value N that
  // was passed to the constructor.
  //
  int getN() const
  {
    return this->N;
  }

  //
  // insert
  //
  // Inserts the given (key, value) pair into the hash table, overwriting
  // the previous value if already inserted.  If the insert was successful
  // then true is returned, otherwise false is returned (meaning the key
  // was not hashed successfully, e.g. due to improper formatting).
  //
  // NOTE: the caller must provide a function Hash(key, N) that returns
  // an integer in the the range 0..N-1, inclusive, if the key is valid.
  // If the key is invalid, the hash function must return a negative
  // integer. The hash function is provided by simply calling insert
  // with the name of your hash function.  Example:
  //
  //   int myHash(string key, int N)
  //   { ... }
  //
  //   int N = 10000;
  //   hashmap<string, double>  hmap(N);  
  //
  //   if (hmap.insert("deep-dish pizza", 19.99, myHash))
  //     cout << "inserted" << endl;
  //   else
  //     cout << "not inserted?!" << endl;
  //
  bool insert(KeyT key, ValueT value, function<int(KeyT,int)> Hash)
  {
    //
    // Call the provided hash function with the key, and N, the size
    // of our hash table:
    //
	  int index = Hash(key, N); //determine what bucket to start in by calling passed Hash function
	  int bucketsProbed = 0;
	  if(index < 0 || index >= N) // return false as index falls out of bounds
		  return false;
	  
	  // Perform probing searching further down the hash table for an empty index
	  while(bucketsProbed < N){
		  if(HT[index].ContainsData && HT[index].Key == key){ // key exists already, update value and return true
			  HT[index].Value = value;
			  return true;
		  }
		  if(!HT[index].ContainsData){ // check to see we have found an empty index in the hash table
			// insert (key, value) pair into hash table
			HT[index].Key = key;
			HT[index].Value = value;
			HT[index].ContainsData = true;
			return true;
		  }
		  index = (index + 1) % N;  // allows us to loop back to the start of the hash table
		  ++bucketsProbed;
	  }
	  return false; // (Key, Value) pair could not be isnerted
  }

  //
  // search
  //
  // Searches the hash table for the matching key, and if found then
  // (1) the value is returned via the reference parameter and (2) true
  // is returned.  If the key could not be found, or if the hash 
  // function returned an invalid value (e.g. due to an improperly 
  // formatted key), then false is returned.
  //
  // NOTE: the caller must provide a function Hash(key, N) that returns
  // an integer in the the range 0..N-1, inclusive, if the key is valid.
  // If the key is invalid, the hash function must return a negative
  // integer. The hash function is provided by simply calling search
  // with the name of your hash function.  Example:
  //
  //   int myHash(string key, int N)
  //   { ... }
  //
  //   int N = 10000;
  //   hashmap<string, double>  hmap(N);
  //   ...
  //   double price;
  //   if (hmap.search("deep-dish pizza", price, myHash))
  //     cout << "deep-dish piazza costs " << price << endl;
  //   else
  //     cout << "not found?!" << endl;
  //
  bool search(KeyT key, ValueT& value, function<int(KeyT,int)> Hash) const
  {
    //
    // Call the provided hash function with the key, and N, the size
    // of our hash table:
    //
	int index = Hash(key, N); //determine what bucket to start in by calling passed Hash function
	int bucketsProbed = 0;
	if(index < 0 || index >= N) // return false as index falls out of bounds
		return false;
    // Perform probing searching further down the hash table for the passed key value
	while(HT[index].ContainsData && bucketsProbed < N){
		if(HT[index].ContainsData && HT[index].Key == key){ // check to see if key has been found
			value = HT[index].Value; // return value at this index
			return true;
		}
		index = (index + 1) % N; // allows us to loop back to the start of the hash table
		++bucketsProbed;
	}
	return false; // key was not found
  }
	
  //getKeys
  //
  // Function will return an unordered_set of keys from the Hash table.
  // We loop through the entire Hash table and inserting each key into 
  // the unordered_set. 
  // NOTE: function is used later on in part 2 of Project 06
  unordered_set <KeyT> getKeys(){
	  unordered_set<KeyT> keys;
	  for(int i = 0; i < N; ++i) //loop through Hash table
		keys.insert(this->HT[i].Key);
	  return keys;
  }
  
};
