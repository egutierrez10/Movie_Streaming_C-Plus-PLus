#include <iostream>
#include <fstream> 
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <cctype>
#include <regex>

#include "hash.h"
#include "hashmap.h"
using namespace std;

struct Movie{
    int ID, Year, Runtime;
    string Title,Age, Rotten;
    string Director, Genre, Country, Language;
    float IMDb;
    bool Netflix, Hulu, Prime, Disney;
    Movie(){
        ID = 0, Year = 0, Runtime = 0;
        Title = "", Age = "", Rotten = "";
        Director = "", Genre = "", Country = "", Language = "";
        IMDb = 0.0;
        Netflix = false;
        Hulu = false;
        Prime = false;
        Disney = false;
    }
};

void displayCommands(){
    cout << "Available commands: " << endl;
    cout << "Enter 'help' to display all commands" << endl;
    cout << "Enter year to display all shows released (e.g. year 2019)" << endl;
    cout << "Enter to search for specific age group (e.g. 18)" << endl;
    cout << "Enter age to search age group greater than or equal to (e.g. >19)" << endl;
    cout << "Enter IMDB rating for really high rated shows (e.g. rating 8.0)" << endl;
    cout << "Enter Rotten Tomatoes rating to find really good shows (e.g. rotten 90)" << endl;
}

bool isNumeric(string s){
    regex pattern("[[:d:]]+", regex::ECMAScript);

	smatch matchResults;  // provides more details on the match

	if (regex_match(s, matchResults, pattern))
		return true;
	else
		return false;
}

bool readFile(string fileName, hashmap<int, Movie>& movies){
    ifstream infile(fileName);
    if(!infile.good()){
        cout << "**Error: Unable to open " << fileName << "..." << endl;
        return false;
    }
    string line;
    int movieCount = 0;
    string id, year, runtime, title, age, rotten, director, genre;
    string country, language, imdb, netflix, hulu, prime, disney;
    getline(infile, line);
    while(getline(infile, line)){
        stringstream s(line);
        getline(s, id, ',');
        getline(s, title, ',');
        getline(s, year, ',');
        getline(s, age, ',');
        getline(s, imdb, ',');
        getline(s, rotten, ',');
        getline(s, netflix, ',');
        getline(s, hulu, ',');
        getline(s, prime, ',');
        getline(s, disney, ',');
        getline(s, director, ',');
        getline(s, genre, ',');
        getline(s, country, ',');
        getline(s, language, ',');
        getline(s, runtime, ',');

        Movie movie;
        movie.ID = stoi(id);
        movie.Title = title;
        movie.Year = stoi(year);
        movie.Age = age;
        movie.IMDb = stof(imdb);
        movie.Rotten = rotten;
        movie.Netflix = (netflix == "1")?true:false;
        movie.Hulu = (hulu == "1")?true:false;
        movie.Prime = (prime == "1")?true:false;
        movie.Disney = (disney == "1")?true:false;
        movie.Director = director;
        movie.Genre = genre;
        movie.Country = country;
        movie.Language = language;
        movie.Runtime = stoi(runtime);
        
        movies.insert(movie.ID, movie, HashId);
        movieCount++;
    } 
    cout << "# of Movies: " << movieCount << endl;
    return true;
}

void searchYear(string command, hashmap<int, Movie>& movies){
    stringstream ss(command);
    string com, year;
    getline(ss, com, ' ');
    getline(ss, year, ' ');
    int yr;
    if(year.length() <= 3 || year.length() > 4){
        cout << "Invalid year entered" << endl;
        return;
    }
    try{
        yr = stoi(year);
    }catch(exception e){
        cout << "Invalid year entered" << endl;
        return;
    }
    
    vector<Movie> yearMov;
    unordered_set<int> keys;
    Movie mov;
    keys = movies.getKeys();
    for(auto itr = keys.begin(); itr != keys.end(); itr++){
        bool found = movies.search(*itr, mov,HashId);
        if(!found)
            continue;
        if(mov.Year == yr)
            yearMov.push_back(mov);
    }  

    cout << "Movies at your year request of: " << yr << endl;
    for(auto i = yearMov.begin(); i != yearMov.end();++i){
        cout << "Movie Name: " << (*i).Title << endl;
        string streaming = (*i).Netflix == 1 ? "yes":"no";
        cout << "   Netflix: " << streaming << endl;
        streaming = (*i).Hulu == 1 ? "yes":"no";
        cout << "   Hulu: " << streaming << endl;
        streaming = (*i).Prime == 1 ? "yes":"no";
        cout << "   Amazon Prime: " << streaming << endl;
        streaming = (*i).Disney == 1 ? "yes":"no";
        cout << "   Disney+: " << streaming << endl;
        cout << endl;
    }
}

void ageSearch(string command, hashmap<int, Movie>& movies){
    int age = stoi(command);
    int curAge;
    vector<Movie> ageMovies;
    unordered_set<int> keys;
    Movie mov;
    keys = movies.getKeys();
    for(auto itr = keys.begin(); itr != keys.end(); itr++){
        bool found = movies.search(*itr, mov,HashId);
        if(!found)
            continue;
        if(mov.Age == "-"){
            continue;
        }else if(mov.Age.length() == 3 && mov.Age != "all"){
            curAge = stoi(mov.Age.substr(0,2));
            if(age == curAge)
                ageMovies.push_back(mov);
        }else if(mov.Age.length() == 2){
             curAge = stoi(mov.Age.substr(0,1));
            if(age == curAge)
                ageMovies.push_back(mov);
        }else{ //all age group
            ageMovies.push_back(mov);
        }
    }   

    cout << "Movies at your age request of: " << age << endl;
    for(auto i = ageMovies.begin(); i != ageMovies.end();++i){
        cout << "Movie Name: " << (*i).Title << endl;
        string streaming = (*i).Netflix == 1 ? "yes":"no";
        cout << "   Netflix: " << streaming << endl;
        streaming = (*i).Hulu == 1 ? "yes":"no";
        cout << "   Hulu: " << streaming << endl;
        streaming = (*i).Prime == 1 ? "yes":"no";
        cout << "   Amazon Prime: " << streaming << endl;
        streaming = (*i).Disney == 1 ? "yes":"no";
        cout << "   Disney+: " << streaming << endl;
        cout << endl;
    }
}

void greaterAgeSearch(string command, hashmap<int, Movie>& movies){
    command = command.erase(0,1);
    int age = stoi(command);
    int curAge;
    vector<Movie> ageMov;
    unordered_set<int> keys;
    Movie mov;
    keys = movies.getKeys();
    for(auto itr = keys.begin(); itr != keys.end(); itr++){
        bool found = movies.search(*itr, mov,HashId);
        if(!found)
            continue;
        if(mov.Age == "-"){
            continue;
        }else if(mov.Age.length() == 3 && mov.Age != "all"){
            curAge = stoi(mov.Age.substr(0,2));
            if(age <= curAge)
                ageMov.push_back(mov);
        }else if(mov.Age.length() == 2){
             curAge = stoi(mov.Age.substr(0,1));
            if(age <= curAge)
                ageMov.push_back(mov);
        }else{ //all age group
            ageMov.push_back(mov);
        }
    }   

    cout << "Movies at your age request of: " << age << endl;
    for(auto i = ageMov.begin(); i != ageMov.end();++i){
        cout << "Movie Name: " << (*i).Title << endl;
        string streaming = (*i).Netflix == 1 ? "yes":"no";
        cout << "   Netflix: " << streaming << endl;
        streaming = (*i).Hulu == 1 ? "yes":"no";
        cout << "   Hulu: " << streaming << endl;
        streaming = (*i).Prime == 1 ? "yes":"no";
        cout << "   Amazon Prime: " << streaming << endl;
        streaming = (*i).Disney == 1 ? "yes":"no";
        cout << "   Disney+: " << streaming << endl;
        cout << endl;
    }
}

void searchRating(string command, hashmap<int, Movie>& movies){
    stringstream ss(command);
    string com, rating;
    getline(ss, com, ' ');
    getline(ss, rating, ' ');
    float r;
    try{
        r = stof(rating);
    }catch(exception e){
        cout << "Invalid rating entered" << endl;
        return;
    }
    vector<Movie> ratingMovies;
    unordered_set<int> keys;
    Movie mov;
    keys = movies.getKeys();
    for(auto itr = keys.begin(); itr != keys.end(); itr++){
        bool found = movies.search(*itr, mov,HashId);
        if(!found)
            continue;
        float curR = mov.IMDb;
        if(curR == r)
            ratingMovies.push_back(mov);
    }

    cout << "Movies at your rating request of: " << r << endl;
    for(auto i = ratingMovies.begin(); i != ratingMovies.end();++i){
        cout << "Movie Name: " << (*i).Title << endl;
        string streaming = (*i).Netflix == 1 ? "yes":"no";
        cout << "   Netflix: " << streaming << endl;
        streaming = (*i).Hulu == 1 ? "yes":"no";
        cout << "   Hulu: " << streaming << endl;
        streaming = (*i).Prime == 1 ? "yes":"no";
        cout << "   Amazon Prime: " << streaming << endl;
        streaming = (*i).Disney == 1 ? "yes":"no";
        cout << "   Disney+: " << streaming << endl;
        cout << endl;
    }
}

void searchRotten(string command, hashmap<int, Movie>& movies){
    stringstream ss(command);
    string com, rating;
    getline(ss, com, ' ');
    getline(ss, rating, ' ');
    float r;
    try{
        r = stof(rating);
    }catch(exception e){
        cout << "Invalid rating entered" << endl;
        return;
    }
    vector<Movie> rotMovies;
    unordered_set<int> keys;
    Movie mov;
    keys = movies.getKeys();
    for(auto itr = keys.begin(); itr != keys.end(); itr++){
        bool found = movies.search(*itr, mov,HashId);
        if(!found)
            continue;
        string rotten = mov.Rotten;
        if(rotten == "-"){
            continue;
        }
        float curR = stof(rotten.substr(0,2));
        if(curR == r)
            rotMovies.push_back(mov);
    }

    cout << "Movies at your rotten rating request of: " << r << endl;
    for(auto i = rotMovies.begin(); i != rotMovies.end();++i){
        cout << "Movie Name: " << (*i).Title << endl;
        string streaming = (*i).Netflix == 1 ? "yes":"no";
        cout << "   Netflix: " << streaming << endl;
        streaming = (*i).Hulu == 1 ? "yes":"no";
        cout << "   Hulu: " << streaming << endl;
        streaming = (*i).Prime == 1 ? "yes":"no";
        cout << "   Amazon Prime: " << streaming << endl;
        streaming = (*i).Disney == 1 ? "yes":"no";
        cout << "   Disney+: " << streaming << endl;
        cout << endl;
    }
}

void searchTitle(string command, hashmap<int, Movie>& movies){
    vector<Movie> titleMovies;
    unordered_set<int> keys;
    Movie mov;
    keys = movies.getKeys();
    for(auto itr = keys.begin(); itr != keys.end(); itr++){
        bool found = movies.search(*itr, mov,HashId);
        if(!found)
            continue;
        if(mov.Title.find(command) != string::npos)
            titleMovies.push_back(mov);
    }

    cout << "Movies at your title request of: " << command << endl;
    for(auto i = titleMovies.begin(); i != titleMovies.end();++i){
        cout << "Movie Name: " << (*i).Title << endl;
        string streaming = (*i).Netflix == 1 ? "yes":"no";
        cout << "   Netflix: " << streaming << endl;
        streaming = (*i).Hulu == 1 ? "yes":"no";
        cout << "   Hulu: " << streaming << endl;
        streaming = (*i).Prime == 1 ? "yes":"no";
        cout << "   Amazon Prime: " << streaming << endl;
        streaming = (*i).Disney == 1 ? "yes":"no";
        cout << "   Disney+: " << streaming << endl;
        cout << endl;
    }
}

int main(){
    const int N = 16800;
    hashmap<int, Movie> movies(N);
    string fileName = "Movies_Streaming.csv";
    cout << "**Movie Streaming Finder**" << endl;
    cout << "Reading file " << fileName << endl;
    bool success = readFile(fileName, movies);
    if(!success){
        cout << "File error occurred. Exiting program..." << endl;
        return 0;
    }
    string command;
    cout << "What we in the mood for today?" << endl;
    cout << "Please enter a command, help, or #>";
    getline(cin, command);

    while(command != "#"){
        if(command == "help"){
            displayCommands();
        }else if(command.substr(0,4) == "year"){
            searchYear(command, movies);
        }else if(isNumeric(command)){
            ageSearch(command, movies);
        }else if(command[0] == '>'){
            greaterAgeSearch(command, movies);
        }else if(command.substr(0,6) == "rating"){
            searchRating(command, movies);
        }else if(command.substr(0,6) == "rotten"){
            searchRotten(command, movies);
        }else{
            searchTitle(command, movies);
        }
        cout << endl;
        cout << "What we in the mood for today?" << endl;
        cout << "Please enter a command, help, or #>";
        getline(cin, command);
    }
    return 0;
}