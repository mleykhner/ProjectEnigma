#include <iostream>
#include "configuration.cpp"
#include "string"

int rotorStep(int rotor, int input, int direction, int position);
int projectLetter(int input, int rots[], int * poss);
void newPositions(int * positions);
std::string encryptText(std::string input, int rots[], int * poss);

int main() {
    int activeRotors[] = {0, 1, 2, 5}; //Настройка активных роторов и рефлектора
    int activeRotors_poss[] = {11, 23, 12, 0}; //Настройка начального положения //Изменение позиции рефлектора ломает все

    std::string text = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
    std::string encrypted = encryptText(text, activeRotors, activeRotors_poss);
    std::cout << encrypted;
    return 0;
}

std::string encryptText(std::string input, int rots[], int * poss){
    bool unencryptedSymbols = false;
    int length = input.length();
    std::string temp;
    for(int i = 0; i < length; i++){
        int currentSymbol = int(toupper(input[i])) - 65;
        if(currentSymbol >= 0 && currentSymbol <= 25){
            temp += char(projectLetter(currentSymbol, rots, poss) + 65);
        }
        else if (currentSymbol == -33){
            temp += ' ';
        }
        else{
            if (currentSymbol != -65) unencryptedSymbols = true;
            temp += input[i];
        }
    }
    if(unencryptedSymbols) std::cout << "String contains unencryptable symbols\n";
    return temp;
}

int rotorStep(int rotor, int input, int direction, int position){
    return (rotors[rotor][direction][(input + (direction ? 0 : position)) % 26] - (direction ? position : 0)) % 26;
}

int projectLetter(int input, int rots[], int * poss){
    int temp = input;
    for (int i = 3; i > -4 ; i--){
        temp = rotorStep(rots[3 - abs(i)], temp, (i >= 0 ? 0 : 1), poss[3 - abs(i)]);
        while (temp < 0) temp += 26;
    }
    newPositions(poss);
    return temp;
}

void newPositions(int * positions){
    (positions[0])++;
    for (int i = 0; i < 4; i++){
        if(positions[i] > 25){
            positions[i] = 0;
            if(i < 3) (positions[i + 1])++;
        }
    }
}