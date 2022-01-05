#include <iostream>
#include <vector>
#include <iomanip>

#define ROWS 8
#define COLS 3

enum ops{OR, AND, IFTHEN, XOR, IFF};
bool operate(bool p, bool q, ops op);
bool getPremise(bool p, bool q, bool r);
bool getConclusion(bool p, bool q);

int main(){
    //Set up booleans and input matrix:
    bool P, Q, R, resultP, resultC;
    bool validity = true;
    std::vector<int> invalidRows;
    bool pqrTable[ROWS][COLS] = { 
                 {1, 1, 1},
                 {1, 1, 0},
                 {1, 0, 1},
                 {1, 0, 0},
                 {0, 0, 1},
                 {0, 1, 0},
                 {0, 1, 1},
                 {0, 0, 0}
               };

    std::cout << "\nLogic Module Application Assignment\n\n"
              << "Premise: ((P v Q) ^ (Q -> R) XOR (P ^ R)) <-> (R ^ Q)\n"
              << "Conclusion: (P v R)\n" << std::endl
              << "+---+---+---+----------------------------------------------+---------+\n"
              << "| P | Q | R | ((P v Q) ^ (Q -> R) XOR (P ^ R)) <-> (R ^ Q) | (P v R) |\n" 
              << "+---+---+---+----------------------------------------------+---------+" << std::endl;
    
    //Iterate through table and generate premise and conclusion for each set of inputs:
    for(int i = 0; i < ROWS; i++){
        P = pqrTable[i][0];
        Q = pqrTable[i][1];
        R = pqrTable[i][2];
        std::cout << "|" << std::setw(2) << P << " |" << std::setw(2) << Q
                  << " |" << std::setw(2) << R << " |";
        
        resultP = getPremise(P, Q, R);
        resultC = getConclusion(P, R);
        //If premise is true but conclusion is not, invalidate and add index + 1 to 
        //list of counterexamples:
        if(resultP && !resultC){
            validity = false;
            invalidRows.push_back(i + 1);
            std::cout << std::setw(23) << "--(" << resultP << ")--" << std::setw(20) << "|"
                      << std::setw(4) << "--(" << resultC << ")--" << " |";
        }
        //Otherwise, just print the results:
        else{
            std::cout << std::setw(24) << resultP << std::setw(23) << " |" << std::setw(5) << resultC
                      << "    |";
        }

        std::cout << std::endl;
    }
    std::cout << "+---+---+---+----------------------------------------------+---------+\n" << std::endl;
    //State conclusion and print invalid rows if applicable:
    std::cout << "The argument is ";
    if(validity){
        std::cout << "valid!" <<std::endl;
    }
    else{
        std::cout << "invalid!\nRows ( ";
        for(size_t i = 0; i < invalidRows.size(); i++){
            std::cout << invalidRows[i];
            if(i < invalidRows.size() - 1)
                std::cout << ", ";
        }
        std::cout << " ) have a true premise, and a false conclusion.\n" << std::endl;

    }
    return 0;
}

//Evaluate full premise:
bool getPremise(bool p, bool q, bool r){
    bool ret = false;
    bool pvqAndqifr = operate(operate(p, q, OR), operate(q, r, IFTHEN), AND);
    bool leftXorpar = operate(pvqAndqifr, operate(p, r, AND), XOR);
    ret = operate(leftXorpar, operate(r, q, AND), IFF);
    return ret;
}

//Evaluate full conclusion:
bool getConclusion(bool p, bool q){
    return operate(p, q, OR);
}

//Retrieve boolean result of basic logical binary operators.
//Note that this can be used as a generic for both getPremise() and getConclusion():
bool operate(bool p, bool q, ops op){
    bool ret = false;
    switch(op){
        case OR:
            ret = p || q;
            break;
        case AND:
            ret = p && q;
            break;
        case IFTHEN:
            ret = !p || q;
            break;
        case XOR:
            ret = p != q;
            break;
        case IFF:
            ret = (!p || q) && (p || !q);
            break;
        default:
            ret = false;
            break;
    }
    return ret;
}
