#include <iostream>
#include "configuration.cpp"

int rotorStep(int rotor, int input, int direction, int position);
void calculatePositions(int * rotA, int * rotB, int * rotC, int * ref);
int projectChar(int input, int rots[], int poss[]);

int main() {
    int activeRotors[] = {0, 1, 2, 5};
    int activeRotors_poss[] = {0, 0, 0, 0};
    std::cout << char(projectChar(int('N') - 65, activeRotors, activeRotors_poss) + 65) << std::endl;
    calculatePositions(&activeRotors_poss[0], &activeRotors_poss[1], &activeRotors_poss[2], &activeRotors_poss[3]);
    std::cout << char(projectChar(int('E') - 65, activeRotors, activeRotors_poss) + 65) << std::endl;
    calculatePositions(&activeRotors_poss[0], &activeRotors_poss[1], &activeRotors_poss[2], &activeRotors_poss[3]);
    return 0;
}

int rotorStep(int rotor, int input, int direction, int position){
        return rotors[rotor][direction][(input + position) % 26];
}

int projectChar(int input, int rots[], int poss[]){
    int temp = input;
    std::cout << char(temp) << " -> ";
    for (int i = 3; i > -4 ; i--){
        temp = rotorStep(rots[3 - abs(i)], temp, (i >= 0 ? 0 : 1), poss[3 - abs(i)]);
        std::cout << char(temp) << " -> ";
    }
    return temp;
}

void calculatePositions(int * rotA, int * rotB, int * rotC, int * ref){
    (*rotA)++;
    if(*rotA > 25){
        *rotA = 0;
        (*rotB)++;
    }
    if(*rotB > 25){
        *rotB = 0;
        (*rotC)++;
    }
    if(*rotC > 25){
        *rotC = 0;
        (*ref)++;
    }
    if(*ref > 25){
        *ref = 0;
    }
}