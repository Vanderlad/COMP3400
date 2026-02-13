#include <cstddef>
#include <iostream>
// Assignment 4, Vlad Mihaescu, 110014634 //
#include <cstddef>
#include <iostream>
#include <map>
#include <print>
#include <set>
#include <string>

auto invert(std::map<std::string,std::size_t> const& histogram)
{
    std::map<std::size_t, std::set<std::string>> retval;

    for(auto const& entry: histogram){
        //entry.first -> string(original key);
        // entry.second -> size_t(original value);
        auto pos = retval.find(entry.second);
        if (pos == retval.end()) { // If frequency does not exist (pointer to one past end!!)
            retval[entry.second].insert(entry.first); // Create a pair with frequency as the key and insert string into the set
        }
        else {
            pos->second.insert(entry.first); 
            // This is redundant, I can just do retval[entry.second].insert(entry.first); 
            // since it will create if doesn't exist or insert in one line, no if cases needed.
            // But I will keep the if, else for the sake of the assignment instructions.
        }
    }
    return retval;
}

int main()
{
    using namespace std;

    string start;
    string stop;
    set<std::size_t> frequency;
    map<string, size_t> hist;

    string word;
    int start_stop = 0;
    // input file is passed in using input redirection ./a04 < input.dat
    while (cin >> word) 
    {
        // Setting first word
        if (start_stop == 0) {
                start = word;
                start_stop = 1;
                continue; // Skip histogram for the 'start' word
            } 
            else if (start_stop == 1) {
                stop = word;
                start_stop = 2;
                continue; // Skip histogram for the 'stop' word
            }                                                                          
        hist[word]++; //add word to map and increment frequency || increment frequency if key "word" exists
    }
        // Check if we have at least 2 words as input and histogram is not empty, otherwise Invalid input
        if (start_stop < 2 && hist.empty()) {
            cerr << "Invalid input. Aborting...\n";
            return 1; 
        }    

    // Original Histogram:
    print("orig: {{");
    for (auto entry = hist.begin(); entry != hist.end(); ++entry) {

        auto const& [line, frequency] = *entry; // For clarity outputting "line": frequency 
        print("\"{}\": {}", line, frequency); 

        if (std::next(entry) != hist.end()) // Logic for commas except for last element, std::next for clarity
            print(", ");
    }
    println("}}");
    
    //Inverted Histogram:
    auto inverted_hist = invert(hist);
    print("inverted: {{");

    for(auto entry = inverted_hist.begin(); entry != inverted_hist.end(); ++entry){
        print("{}: ", entry->first);
        print("{{");
        for (auto setEntry = entry->second.begin(); setEntry != entry->second.end();){ // Tried different method to get next element
            print("\"{}\"", *setEntry);

            if (++setEntry != entry->second.end()) // Increment setEntry here and check if its the last element
                print(", ");
        }
        if (std::next(entry)!= inverted_hist.end()){
            print("}}, ");
        }
        else {
            print("}}");
        }
    }
    println("}}");

    auto first = hist.lower_bound(start);
    auto last = hist.lower_bound(stop); 
    // NOTE:
    // I know the assignment said use upper_bound, but that would be a closed interval, not a half open interval
    // If I use upper_bound on stop it'll get an element greater than or equal to
    // which im assuming in [bay, elephant) that is not what you want
    for(auto entry = first; entry != last; ++entry){
        frequency.insert(entry->second);
    }
    if (frequency.empty()){
        println("In [{},{}) there are no data points.", start, stop);
    }
    else {
        println("In [{},{}) frequencies vary between {} and {} inclusively.", start, stop, *frequency.begin(), *prev(frequency.end())); // Can't I just use *frequency.rbegin() here?
    }
    return 0;
}

