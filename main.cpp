#include <iostream>
#include "configuration.cpp"
#include "string"

int rotorStep(int rotor, int input, int direction, int position);//Проецирование символа через ротор
int projectLetter(int input, int rots[], int * poss); //Проецирование символа через все роторы
void newPositions(int * positions); //Расчет новых положений роторов
std::string encryptText(std::string input, int rots[], int * poss); //Шифрование строки
void updatePlugboardSetup(char A, char B); //Изменение настройки коммутационной панели
void resetPlugboardSetup(); //Сбросить настройки коммутационной панели

//Общие настройки
bool saveSpaces = false;
bool saveUnencryptableSymbols = false;
//Общие настройки


int main() {
    int activeRotors[] = {0, 1, 2, 5}; //Настройка активных роторов и рефлектора
    int activeRotors_poss[] = {11, 23, 12}; //Настройка начального положения

    std::string text = "bebrabebra";
    std::string encrypted = encryptText(text, activeRotors, activeRotors_poss);
    std::cout << encrypted;
    return 0;
}

std::string encryptText(std::string input, int rots[], int * poss){
    bool unencryptedSymbols = false;
    int length = int(input.length());
    std::string temp;
    for(int i = 0; i < length; i++){
        int currentSymbol = int(toupper(input[i])) - 65;
        if(currentSymbol >= 0 && currentSymbol <= 25){
            temp += char(projectLetter(currentSymbol, rots, poss) + 65);
        }
        else if (currentSymbol == -33){
            if(saveSpaces) temp += ' ';
        }
        else{
            unencryptedSymbols = true;
            if(saveUnencryptableSymbols) temp += input[i];
        }
    }
    if(unencryptedSymbols) std::cout << "String contains unencryptable symbols\n";
    return temp;
}

int rotorStep(int rotor, int input, int direction, int position){
    return  (rotors[rotor][direction]
            [(input + (direction ? 0 : position)) % 26]
            - (direction ? position : 0)) % 26;
}

int projectLetter(int input, int rots[], int * poss){
    int temp = plugboardSetup[input];
    for (int i = 3; i > -4 ; i--){
        temp = rotorStep(rots[3 - abs(i)], temp, (i >= 0 ? 0 : 1), (i ? poss[3 - abs(i)] : 0));
        while (temp < 0) temp += 26;
    }
    newPositions(poss);
    for (int i = 0; i < 3; i++){
        std::cout << "[" << i << "]: " << poss[i] << "\t";
    }
    std::cout << std::endl;
    return plugboardSetup[temp];
}

void newPositions(int * positions){
    positions[0]++;
    for (int i = 0; i < 3; i++){
        if(positions[i] > 25){
            positions[i] = 0;
            if(i < 2) positions[i + 1]++;
        }
    }
}

void updatePlugboardSetup(char A, char B){
    plugboardSetup[int(A) - 65] = int(B) - 65;
    plugboardSetup[int(B) - 65] = int(A) - 65;
}

void resetPlugboardSetup() {
    for (int i = 0; i < 26; i++){
        plugboardSetup[i] = i;
    }
}