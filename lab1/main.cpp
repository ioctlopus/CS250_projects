/*This program was written to conform with the output presented in
  the example lab. This means it is case-sensitive and includes
  punctuation and digits*/

#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <cctype>
#include <algorithm>

int main(){
    int ret = EXIT_SUCCESS;
    std::ifstream infile;
    std::string filename, curr;
    std::set<std::string> wordSet;
    std::set<std::string>::iterator index;
    
    //Get filename, open file, and exit if file fails to open:
    std::cout << "Enter input file name: ";
    std::cin >> filename;
    infile.open(filename);
    std::cout << std::endl;
    
    if(!infile.is_open()){
        ret = EXIT_FAILURE;
    }
    else{
        //Build set:
        while(!infile.eof()){
            infile >> curr;
            wordSet.insert(curr);
        }
        
        //Output:
        std::cout << "Words in the file:" <<std::endl;
        
        for(index = wordSet.begin(); index != wordSet.end(); ++index)
            std::cout << *index << std::endl;
        infile.close();
    }
    return ret;
}
