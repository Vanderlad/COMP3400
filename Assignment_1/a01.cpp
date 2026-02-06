#include <iostream>
#include <filesystem>
#include <generator>
#include <queue>

bool is_direct_child_path_of(std::filesystem::path const &possible_child_path, std::filesystem::path const &known_path) {

    namespace fs = std::filesystem;
    auto cpath{ fs::canonical(possible_child_path) }; 
    // cpath is constructed by fs::canonical which converts a path into its absolute, normalized, symlink resolved form,
    //      so path comparisons reflect the real filesystem

    // consider a path that does not have a parent path to be under known_path... 
    if (not cpath.has_parent_path()) // basically if cpath doesnt have a parent, then it is the root
        return true;
    auto parent_cpath{ cpath.parent_path() };
    // otherwise ensure possible_child_path's parent path is known_path
    // and use canonical() to determine this...
    auto known_canonical_path{ fs::canonical(known_path) };
    return parent_cpath == known_canonical_path;
}

std::generator<std::filesystem::path> bfs_scan(std::filesystem::path root_path) { // take root path as input from argv[i] (initial path we look at)
    std::queue<std::filesystem::path> path_chain; // declaring queue of respective type
    path_chain.push(root_path);                   // push root/current path onto queue

    while (!path_chain.empty()) {        // while paths still exist
        auto cur = path_chain.front();   // get first element of queue
        path_chain.pop();                // pop the oldest item aka top of queue

        for (auto const& directory_entry : std::filesystem::directory_iterator{cur})  {

            // If current entry is a symbolic link and still points to a child directory under our current parent directory --> cur
            
            if(directory_entry.is_symlink()){
                // Try Catch since canonical was giving filesystem error for symlink "link3" which was a file not a directory
                try {
                    if (is_direct_child_path_of(directory_entry.path(), cur)){
                        co_yield directory_entry.path(); // Then co_yield entry path() so we return path then resume
                    }
                }
                catch (const std::exception& e) {
                    // ignore this file and move on
                }
            }
             else if (!directory_entry.is_symlink() && directory_entry.status().type() == std::filesystem::file_type::directory){ // if true directory
                path_chain.push(directory_entry);
                    
            } else if (directory_entry.status().type() == std::filesystem::file_type::regular){
                co_yield directory_entry.path();
            }
        }
        
    }
    co_return; // end of generator
} 


int main (int argc, char* argv[]){
    using namespace std;
    if (argc <= 1) {
        cerr << "\nNo detected arguments passed to program, please pass args.\n";
        cerr << "Usage: " << argv[0] << " PATH [PATH]... \n\n";
        return 1;
    }
    else {
        for (int i = 1; i < argc; ++i){
            try {
                //Call bfs_scan() passing the current argv[i] value (i.e., the current path from the command line) to it.
                //Output "Processing path " followed by the path (i.e., argv[i]) being processed followed by a newline to std::cerr.
                auto generator = bfs_scan(argv[i]);
                cerr << "Processing path " << argv[i] << "\n";
                for (auto const& file: generator) { // iterate over elements(files or symlinks) saved in coroutine and output to cout
                    cout << file << "\n";
                }
            }
            catch(std::filesystem::filesystem_error const& e){
                cerr << "EXCEPTION: path: " << argv[i] << ", reason: " << e.what() << '\n';
            }
            catch(const exception& e){
                cerr << "EXCEPTION: Unknown exception.";
            }
        }
    }

    return 0;
}