#include <iostream>
#include <cstring>
#include <gmp.h>

void gcdCalc(mpz_t& dest, mpz_t& a, mpz_t& b);
void divCalc(mpz_t& a, mpz_t& b, mpz_t& quot, mpz_t& rem);

int main(){
    mpz_t num1, num2, gcdResult, quotient, remainder, valid;
    std::string stNum1, stNum2;
    char* c_strNum1 = nullptr; 
    char* c_strNum2 = nullptr;

    //Get input from user:
    std::cout << "Please provide two integers such that:\n1 <= x <= 2^50\n1 <= y < x\n" << std::endl;
    std::cout << "Input value for x: ";
    std::getline(std::cin, stNum1);
    std::cout << "Input value for y: ";
    std::getline(std::cin, stNum2);

    //Convert to c_strings and initialize numbers:
    c_strNum1 = new char[stNum1.length() + 1];
    c_strNum2 = new char[stNum2.length() + 1];
    strcpy(c_strNum1, stNum1.c_str());
    strcpy(c_strNum2, stNum2.c_str());
    mpz_init_set_str(num1, c_strNum1, 10);
    mpz_init_set_str(num2, c_strNum2, 10);

    //Calculate GCD and display:
    mpz_init(gcdResult);
    gcdCalc(gcdResult, num1, num2);
    std::cout << "Greatest common denominator is: ";
    mpz_out_str(stdout, 10, gcdResult); 
    std::cout << std::endl << std::endl;

    //Calculate quotient and remainder and display:
    mpz_init(quotient);
    mpz_init(remainder);
    mpz_set_str(num1, c_strNum1, 10);
    mpz_set_str(num2, c_strNum2, 10);
    divCalc(num1, num2, quotient, remainder);
    std::cout << "The values for q and r such that x = yq + r are:\nq = ";
    mpz_out_str(stdout, 10, quotient);
    std::cout << "\nr = ";
    mpz_out_str(stdout, 10, remainder);
    std::cout << std::endl << "Validation that yq + r = x: ";
    mpz_init(valid);
    mpz_mul(valid, quotient, num2);
    mpz_add(valid, valid, remainder);
    mpz_out_str(stdout, 10, valid);
    std::cout << std::endl;

    //Cleanup:
    mpz_clear(num1);
    mpz_clear(num2);
    mpz_clear(gcdResult);
    mpz_clear(quotient);
    mpz_clear(remainder);
    mpz_clear(valid);
    delete [] c_strNum1;
    delete [] c_strNum2;
    
    return 0;
}

//Calculate GCD of two mpz_t numbers using Euclidean algorithm:
void gcdCalc(mpz_t& dest, mpz_t& a, mpz_t& b){
    mpz_t rem;
    mpz_init(rem);

    while(mpz_cmp_ui(b, 0)){
        mpz_mod(rem, a, b);
        mpz_set(a, b);
        mpz_set(b, rem);
    }
    mpz_set(dest, a);
    mpz_clear(rem);
}

//Calculate quotient and remainder using division algorithm:
void divCalc(mpz_t& a, mpz_t& b, mpz_t& quot, mpz_t& rem){
    mpz_set_ui(quot, 0);
    mpz_set(rem, a);
    while(mpz_cmp(rem, b) >= 0){
        mpz_sub(rem, rem, b);
        mpz_add_ui(quot, quot, 1);
    }
}
