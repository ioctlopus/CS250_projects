#include <iostream>
#include <climits>
#include <cmath>

#define T1 1
#define T2 2
#define T3 3

struct Stack{
    int cap;
    int top;
    int* arr;
};

void rHanoi(unsigned int numDisk, unsigned int from, unsigned int aux, unsigned int to);
Stack* buildStack(int aCap);
bool isFull(Stack* aStack);
bool isEmpty(Stack* aStack);
void push(Stack* aStack, int what);
int pop(Stack* aStack);
void moveDisk(int from, int to, int which);
void moveBetween(Stack* source, Stack* dest, int s, int d);
void iHanoi(int numDisks, Stack* src, Stack* aux, Stack* dst);
void freeStack(Stack* aStack);

int main(){
    unsigned int disks;
    Stack *sc, *dt, *ax;

    std::cout << "Enter how many disks: ";
    std::cin >> disks;

    //Show recursive solution:
    std::cout << "\nRecursive output:\n" << std::endl;
    rHanoi(disks, T1, T2, T3);

    //Show iterative solution:
    std::cout << "\nIterative output:\n" << std::endl;
    sc = buildStack(disks);
    dt = buildStack(disks);
    ax = buildStack(disks);
    iHanoi(disks, sc, ax, dt);

    //Cleanup:
    freeStack(sc);
    freeStack(dt);
    freeStack(ax);

    return 0;
}

//Recursive solution:
void rHanoi(unsigned int numDisk, unsigned int from, unsigned int aux, unsigned int to){
    if(numDisk == 1){
        std::cout << "moved disk " << numDisk << " from " << from << " to " << to << std::endl;
        return;
    }
    rHanoi(numDisk - 1, from, to, aux);
    std::cout << "moved disk " << numDisk << " from " << from << " to " << to << std::endl;
    rHanoi(numDisk - 1, aux, from, to);
}

//Construct stack and prepare for use:
Stack* buildStack(int aCap){
    Stack* ret = new Stack;
    ret->cap = aCap;
    ret->top = -1;
    ret->arr = new int[ret->cap];
    return ret;
}

//Free stack:
void freeStack(Stack* aStack){
    delete [] aStack->arr;
    delete aStack;
}

//Move disks between two towers:
void moveBetween(Stack* source, Stack* dest, int s, int d){
    int t1Top = pop(source);
    int t2Top = pop(dest);

    //T1 empty:
    if(t1Top == INT_MIN){
        push(source, t2Top);
        moveDisk(d, s, t2Top);
    }
    //T2 empty:
    else if(t2Top == INT_MIN){
        push(dest, t1Top);
        moveDisk(s, d, t1Top);
    }
    //t1Top > t2Top:
    else if(t1Top > t2Top){
        push(source, t1Top);
        push(source, t2Top);
        moveDisk(d, s, t2Top);
    }
    //t1Top < t2Top:
    else{
        push(dest, t2Top);
        push(dest, t1Top);
        moveDisk(s, d, t1Top);
    }
}

//Glue for iterative Hanoi:
void iHanoi(int numDisks, Stack* src, Stack* aux, Stack* dst){
    int numMoves;
    int s = T1, d = T3, a = T2;

    //If numDisks is even, interchange dest and aux:
    if(numDisks % 2 == 0){
        int tmp = d;
        d = a;
        a = tmp;
    }
    numMoves = pow(2, numDisks) - 1;

    //Push larger disks first:
    for(int i = numDisks; i >= 1; i--)
        push(src, i);
    for(int i = 1; i <= numMoves; i++){
        if(i % 3 == 1)
            moveBetween(src, dst, s, d);
        else if(i % 3 == 2)
            moveBetween(src, aux, s, a);
        else if(i % 3 == 0)
            moveBetween(aux, dst, a, d);
    }
}

//Print disk moves:
void moveDisk(int from, int to, int which){
    std::cout << "moved disk " << which << " from " << from << " to " << to << std::endl;
}

//Push onto stack:
void push(Stack* aStack, int what){
    if(isFull(aStack))
        return;
    aStack->arr[++aStack->top] = what;
}

//Pop from stack:
int pop(Stack* aStack){
    int ret;
    if(isEmpty(aStack))
        ret = INT_MIN;
    else
        ret = aStack->arr[aStack->top--];
    return ret;
}

//Helpers for determining stack size:
bool isFull(Stack* aStack){
    return aStack->top == aStack->cap - 1;
}

bool isEmpty(Stack* aStack){
    return aStack->top == -1;
}
