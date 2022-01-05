#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <iomanip>

const char dAnd = '^';
const char dOr = 'v';

struct Var{
    std::string var;
    bool isNot;
};

struct Clause{
    Var v1;
    Var v2;
    Var v3;

    Clause(Var a, Var b, Var c): v1(a), v2(b), v3(c) {}
    void display(){ std::cout << "Integer 1: " << v1.var << "\nInteger 2: " << v2.var << "\nInteger 3: " << v3.var << std::endl << std::endl; }
};

std::string removeWS(const std::string& aExpr);
std::vector<std::string>& splitExpr(const std::string& aExpr, char delimiter, std::vector<std::string>& parts);
std::vector<std::string> splitExpr(const std::string& aExpr, char delimiter);
void getClauses(std::vector<Clause>& cList, const std::string& aExpr);
void generateVarSet(const std::vector<Clause>& cList, std::set<std::string>& aSet);
std::vector<std::vector<int>> buildInputs(size_t aSize);
bool evalClauses(const std::vector<Clause>& aClauses, const std::vector<int>& vList);
char boolToChar(bool in);

int main(){
    std::vector<Clause> clauses;
    std::string expr;
    std::set<std::string> varSet;
    std::vector<std::vector<int>> inputs;
    bool currEval;
   
    //Get expression from user, build containers, and print the clauses for the user:
    std::cout << "Please input a 3CNF formula. Use '^' for and and 'v' (lowercase) for or. Do NOT use parentheses. " 
              << "Label your variables as successive integers (1, 2, 3): " << std::endl;
    std::getline(std::cin, expr); 

    getClauses(clauses, expr);
    generateVarSet(clauses, varSet);
    inputs = buildInputs(varSet.size());
    
    std::cout << "\nHere is the formula you entered:\n" << std::endl;
    for(size_t i = 0; i < clauses.size(); i++){
        std::cout << "Clause " << i << std::endl;
        clauses[i].display();
    }

    //Formatting display of table based on size of expression, number of variables, etc
    //Not Super pretty but it accomplishes variable-length input sizes without sacrificing aesthetics:
    std::cout << "\nTruth Table\n======";
    for(std::set<std::string>::iterator it = varSet.begin(); it != varSet.end(); it++)
        std::cout << "======";

    std::cout << "=====";
    for(size_t i = 0; i < expr.size(); i++)
        std::cout << "=";

    std::cout << std::endl << "      ";
    for(std::set<std::string>::iterator it = varSet.begin(); it != varSet.end(); it++)
        std::cout << *it << "      ";
    std::cout << expr << std::endl;

    std::cout <<  "======";
    for(std::set<std::string>::iterator it = varSet.begin(); it != varSet.end(); it++)
        std::cout << "======";
    
    std::cout << "=====";
    for(size_t i = 0; i < expr.size(); i++)
        std::cout << "=";
    std::cout << std::endl;

    //Print truth table:
    for(std::vector<std::vector<int>>::iterator itO = inputs.begin(); itO != inputs.end(); itO++){
        for(std::vector<int>::iterator itI = itO->begin(); itI != itO->end(); itI++){
            std::cout << std::setw(7) << boolToChar(static_cast<bool>(*itI));
        }
        
        currEval = evalClauses(clauses, *itO);
        std::cout << std::setw(10) << boolToChar(currEval);
        if(!currEval)
            std::cout << "   <--Not Satisfied.";
        std::cout << std::endl;
    }
  
    return 0;
}

//Just a little helper for bool output formatting:
char boolToChar(bool in){
    return in ? 'T' : 'F';
}

//Generate matrix of input values for the 3CNF:
std::vector<std::vector<int>> buildInputs(size_t aSize){
    std::vector<int> row;
    std::vector<std::vector<int>> inputList;
    int rows = static_cast<int>(pow(2, aSize));
    int val = 0;

    for(int i = 0; i < rows; i++){
        for(int j = aSize - 1; j >= 0; j--){
            val = (i / static_cast<int>(pow(2, j))) % 2;
            row.push_back(val);
        }
        inputList.push_back(row);
        row.clear();
    }

    return inputList;
}

//Evaluate clauses based on inputs:
bool evalClauses(const std::vector<Clause>& aClauses, const std::vector<int>& vList){
    int i1, i2, i3;
    bool p, q, r, ret;
    std::vector<bool> clauseEvals;
    
    for(size_t i = 0; i < aClauses.size(); i++){
        //Assign appropriate input values to appropriate clause variables:
        for(size_t j = 0; j < vList.size(); j++){
            if(static_cast<unsigned int>(j + 1) == static_cast<unsigned int>(abs(stoi(aClauses[i].v1.var))))
                i1 = vList[j];
            if(static_cast<unsigned int>(j + 1) == static_cast<unsigned int>(abs(stoi(aClauses[i].v2.var))))
                i2 = vList[j];
            if(static_cast<unsigned int>(j + 1) == static_cast<unsigned int>(abs(stoi(aClauses[i].v3.var))))
                i3 = vList[j];
        }
        //Handle negations and evaluate the clause:
        p = aClauses[i].v1.isNot ? !static_cast<bool>(i1) : static_cast<bool>(i1);
        q = aClauses[i].v2.isNot ? !static_cast<bool>(i2) : static_cast<bool>(i2);
        r = aClauses[i].v3.isNot ? !static_cast<bool>(i3) : static_cast<bool>(i3);

        clauseEvals.push_back(p || q || r);
    }
    
    //Finally, AND together each of the prior clause evaluations:
    ret = clauseEvals[0];
    for(size_t i = 1; i < clauseEvals.size(); i++)
        ret = ret && clauseEvals[i];

    return ret;
}

//Generate set of unique variable names. The idea here is to strip negations and discard duplicates:
void generateVarSet(const std::vector<Clause>& cList, std::set<std::string>& aSet){
    std::string stripped;
    for(size_t i = 0; i < cList.size(); i++){
       stripped = cList[i].v1.var;
       stripped.erase(std::remove(stripped.begin(), stripped.end(), '-'), stripped.end());
       aSet.insert(stripped);
       stripped = cList[i].v2.var;       
       stripped.erase(std::remove(stripped.begin(), stripped.end(), '-'), stripped.end());
       aSet.insert(stripped);
       stripped = cList[i].v3.var;
       stripped.erase(std::remove(stripped.begin(), stripped.end(), '-'), stripped.end());
       aSet.insert(stripped);
    }
}

//Generate clauses from the expression:
void getClauses(std::vector<Clause>& cList,  const std::string& aExpr){
    std::vector<std::string> innerExprs;
    std::vector<std::string> clauseExprs;
    Var tv1, tv2, tv3;

    //Tokenize the expression:
    std::string noWSExpr = removeWS(aExpr);
    splitExpr(noWSExpr, dAnd, clauseExprs);
    for(size_t i = 0; i < clauseExprs.size(); i++){
        splitExpr(clauseExprs[i], dOr, innerExprs);
    }
    for(size_t i = 0; i < innerExprs.size(); i++){
        innerExprs[i] = removeWS(innerExprs[i]);
    }
    
    if(innerExprs.size() % 3 != 0 || innerExprs.size() == 0){
        std::cout << "NOT A VALID 3CNF!" << std::endl;
        exit(EXIT_FAILURE);
    }

    //Construct list of clauses from tokens:
    for(size_t i = 0; i < innerExprs.size(); i += 3){
       tv1.var = innerExprs[i] ;
       tv2.var = innerExprs[i + 1];
       tv3.var = innerExprs[i + 2];

       tv1.isNot = tv1.var.size() == 2 ? true : false;
       tv2.isNot = tv2.var.size() == 2 ? true : false;
       tv3.isNot = tv3.var.size() == 2 ? true : false;

       cList.push_back(Clause(tv1, tv2, tv3));
    }
}

//Remove leading and trailing whitespace:
std::string removeWS(const std::string& aExpr){
    std::string::const_iterator curr = aExpr.begin();
    while(curr != aExpr.end() && isspace(*curr))
        curr++;
    
    std::string::const_reverse_iterator bcurr = aExpr.rbegin();
    while(bcurr.base() != curr && isspace(*bcurr))
        bcurr++;

    return std::string(curr, bcurr.base());
}

//The next two functions split the epression into parts based on a delimiter:
std::vector<std::string>& splitExpr(const std::string& aExpr, char delimiter, std::vector<std::string>& parts){
    std::stringstream exprStream(aExpr);
    std::string part;
    while(std::getline(exprStream, part, delimiter))
        parts.push_back(part);
    return parts;
}

std::vector<std::string> splitExpr(const std::string& aExpr, char delimiter){
    std::vector<std::string> parts;
    splitExpr(aExpr, delimiter, parts);
    return parts;
}
