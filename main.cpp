#include <iostream>
#include "configuration.cpp"
#include "string"

int rotorStep(int rotor, int input, int direction, int position);//Проецирование символа через ротор
int projectLetter(int input, int rots[], int * poss); //Проецирование символа через все роторы
void newPositions(int * positions); //Расчет новых положений роторов
std::string encryptText(std::string input, int rots[], int * poss); //Шифрование строки
bool updatePlugboardSetup(char A, char B); //Изменение настройки коммутационной панели
void resetPlugboardSetup(); //Сбросить настройки коммутационной панели
void showCurrentSetup(int rots[], int poss[]);
std::string generatePassword(int rots[], int poss[]);
void applyPassword(std::string password, int * rots, int * poss);

//Общие настройки
bool saveSpaces = false;
bool saveUnencryptableSymbols = false;
//Общие настройки


int main() {
    int activeRotors[] = {0, 2, 1, 5}; //Настройка активных роторов и рефлектора
    int activeRotors_poss[] = {11, 23, 12}; //Настройка начального положения

//    std::string text = "bebrabebra";
//    std::string encrypted = encryptText(text, activeRotors, activeRotors_poss);
//    std::cout << encrypted;


    int state = 0;
    bool showInput = true;
    while (true){
        switch (state) {
            case -1:{
                break;
            }
            case 0:{
                std::cout << "Active rotors:  \t";
                for (int activeRotor : activeRotors) std::cout << "[" << rotorNames[activeRotor] << "] \t";
                std::cout << std::endl;

                std::cout << "Rotors positions:\t";
                for (int activeRotors_pos : activeRotors_poss) std::cout << "[" << activeRotors_pos << "] \t";
                std::cout << std::endl;

                std::cout << "Type \"help\" to get info about app.\n";
                break;
            }
            case 1:{
                std::cout << "\"updpos [index (1-3)] [pos (1-26)]\" - update positions\n";
                std::cout << "\"selrot [index (1-4)] [rot (1 - 5 or B or C)]\" - select rotors\n";
                std::cout << "\"editpb [A] [B]\" - edit plugboard.\n";
                std::cout << "\"respb\" - reset plugboard.\n";
                std::cout << "\"savespace [on/off]\"\n";
                std::cout << "\"saveunencryptables [on/off]\"\n";
                std::cout << "\"menu\" - show menu\n";
                break;
            }
            case 2:{
                std::cout << "\"*\" - hide original text\n";
                std::cout << "\"&\" - menu\n";
                std::cout << "\"?\" - show current setup\n";
                std::cout << "\"#\" - export password\n";
                std::cout << "\"@\" - apply password\n";
                std::cout << "\"[word]\" - encrypt\n";
                std::cout << "Dont forget to remember the settings before encrypting!\n";
                break;
            }
        }

        std::cout << "> ";
        std::string request;
        getline(std::cin, request);

        if(request == "help" && state != 2){
            std::cout << "\"setup\" - edit configuration.\n";
            std::cout << "\"setup\" -> \"updpos [index (1-3)] [pos (1-26)]\" - update positions.\n";
            std::cout << "\"setup\" -> \"selrot [index (1-4)] [rot (1 - 5 or B or C)]\" - select rotors.\n";
            std::cout << "\"setup\" -> \"editpb [A] [B]\" - edit plugboard.\n";
            std::cout << "\"setup\" -> \"respb\" - reset plugboard.\n";
            std::cout << "\"setup\" -> \"savespace [on/off]\"\n";
            std::cout << "\"setup\" -> \"saveunencryptables [on/off]\"\n";
            std::cout << "\"encrypt\" - start encrypting.\n";
            std::cout << "\"encrypt\" -> \"*\" - hide original text.\n";
            std::cout << "\"encrypt\" -> \"&\" - menu\n";
            std::cout << "\"encrypt\" -> \"?\" - show current setup\n";
            std::cout << "\"encrypt\" -> \"#\" - export password\n";
            std::cout << "\"encrypt\" -> \"@\" - apply password\n";
            std::cout << "\"menu\" - show menu.\n";
            std::cout << "\"exit\" - close app.\n";
        }
        else{
            switch (state) {
                case 0:{
                    if (request == "setup"){
                        state = 1;
                    }
                    else if(request == "encrypt"){
                        state = 2;
                    }
                    else if(request == "menu"){
                        state = 0;
                    }
                    else if(request == "exit") return 0;
                    else {
                        std::cout << "Undefined command.\n";
                    }
                    break;
                }
                case 1:{
                    if (request == "respb") {
                        resetPlugboardSetup();
                        std::cout << "Plugboard is now reset\n";
                    }
                    else if(request.find("editpb") != request.npos) {
                        if(updatePlugboardSetup(toupper(request[7]), toupper(request[9]))){
                            std::cout << "Plugboard updated: [" << char(toupper(request[7])) << "] <-> [" << char(toupper(request[9])) << "]\n";
                        }
                        else
                            std::cout << "Undefined symbol found.\n";
                    }
                    else if(request.find("updpos") != request.npos) {
                        int temp = 0;
                        int i = 9;
                        while (request[i] >= 48 && request[i] <= 57) {
                            temp = temp * 10 + int(request[i]) - 48;
                            i++;
                        }
                        activeRotors_poss[int(request[7]) - 49] = temp % 26;
                        std::cout << "Positions updated: [rotor " << int(request[7]) - 49 << "] -> " << temp % 26 << "\n";
                    }
                    else if(request.find("selrot") != request.npos){
                        int selectedIndex = int(request[7]) - 49;
                        int selectedRotor = int(toupper(request[9]));
                        if (selectedIndex >= 0 && selectedIndex <= 2 && selectedRotor >= 49 && selectedRotor <= 53){
                            activeRotors[selectedIndex] = selectedRotor - 49;
                            std::cout << "Rotor selected: [" << selectedIndex + 1 << "] -> " << rotorNames[selectedRotor - 49] << "\n";
                        }
                        else if(selectedIndex == 3 && (selectedRotor == 66 || selectedRotor == 67)){
                            activeRotors[3] = selectedRotor - 61;
                            std::cout << "Reflector selected: " << char(selectedRotor) << "\n";
                        }
                        else std::cout << "Undefined index or rotor\n";
                    }
                    else if(request.find("savespace") != request.npos){
                        if(request.find("on") != request.npos) {
                            saveSpaces = true;
                            std::cout << "Spaces will be saved in encrypted messages\n";
                        }
                        else if(request.find("off") != request.npos) {
                            saveSpaces = false;
                            std::cout << "Spaces will NOT be saved in encrypted messages\n";
                        }
                        else std::cout << "Undefined command\n";
                    }
                    else if(request.find("saveunencryptables") != request.npos){
                        if(request.find("on") != request.npos) {
                            saveUnencryptableSymbols = true;
                            std::cout << "Unencryptable symbols will be saved in encrypted messages\n";
                        }
                        else if(request.find("off") != request.npos) {
                            saveUnencryptableSymbols = false;
                            std::cout << "Unencryptable symbols will NOT be saved in encrypted messages\n";
                        }
                        else std::cout << "Undefined command\n";
                    }
                    else if(request == "menu") state = 0;
                    break;
                }
                case 2:{
                    if(request == "*") showInput = !showInput;
                    else if(request == "&") state = 0;
                    else if(request == "?") {
                        showCurrentSetup(activeRotors, activeRotors_poss);
                    }
                    else if(request == "#") std::cout << "Password: " << generatePassword(activeRotors,
                                                                                          activeRotors_poss) << "\n";
                    else if(request == "@"){
                        std::cout << "Type password: ";
                        std::string pass;
                        getline(std::cin, pass);
                        applyPassword(pass, activeRotors, activeRotors_poss);
                        std::cout << "Password applied!\n";
                        showCurrentSetup(activeRotors, activeRotors_poss);
                    }
                    else{
                        for(auto & c : request) c = toupper(c);
                        std::cout << "Input: " << (showInput ? request : (std::string(request.length(), '*'))) << "\n";
                        std::cout << "Output: " << encryptText(request, activeRotors, activeRotors_poss) << "\n";
                    }
                }
            }
        }
    }
}

void showCurrentSetup(int rots[], int poss[]){
    std::cout << "Active rotors:  \t";
    for (int i = 0; i < 4; i++) std::cout << "[" << rotorNames[rots[i]] << "] \t";
    std::cout << std::endl;

    std::cout << "Rotors positions:\t";
    for (int i = 0; i < 3; i++) std::cout << "[" << poss[i] << "] \t";
    std::cout << std::endl << "Plugboard:\n";
    bool def = true;
    std::string added;
    for(int i = 0; i < 26; i++){
        if (plugboardSetup[i] != i){
            if(added.find(char(i + 65)) != added.npos) continue;
            std::cout << "[" << char(plugboardSetup[i] + 65) << "] <-> [" << char(i + 65) << "]\n";
            added += char(i + 65);
            added += char(plugboardSetup[i] + 65);
            def = false;
        }
    }
    if (def) std::cout << "default\n";
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

bool updatePlugboardSetup(char A, char B){
    if(A >= 65 && A <= 90 && B >= 65 && B <=90){

        int charA = int(A) - 65;
        int charB = int(B) - 65;

        if(plugboardSetup[charA] == charA && plugboardSetup[charB] == charB){
            plugboardSetup[charA] = charB;
            plugboardSetup[charB] = charA;
        }
        else
        {
            plugboardSetup[plugboardSetup[charA]] = plugboardSetup[charA];
            plugboardSetup[charA] = charB;
            plugboardSetup[plugboardSetup[charB]] = plugboardSetup[charB];
            plugboardSetup[charB] = charA;
        }
        return true;
    }
    else return false;

}

void resetPlugboardSetup() {
    for (int i = 0; i < 26; i++){
        plugboardSetup[i] = i;
    }
}

std::string generatePassword(int rots[], int poss[]){
    std::string temp;
    for (int i = 0; i < 4; i++) temp += char(rots[i] + 48);
    for (int i = 0; i < 3; i++) {
        std::string number;
        int tempInt = poss[i];
        while(tempInt != 0){
            number = char((tempInt % 10) + 48) + number;
            tempInt = tempInt / 10;
        }
        temp += "-";
        temp += number;
    }
    std::string added;
    for(int i = 0; i < 26; i++){
        if(i != plugboardSetup[i]){
            if(added.find(char(i + 65)) != added.npos) continue;
            added += char(i + 65);
            added += char(plugboardSetup[i] + 65);
        }
    }
    temp += added;
    return temp;
}

void applyPassword(std::string password, int * rots, int * poss){
    int size = password.length();
    for (int i = 0; i < 4; i++) {
        rots[i] = int(password[i] - 48);
    }
    int index = 5;
    for (int i = 0; i < 3; i++) {
        int temp = 0;
        while (password[index] >= 48 && password[index] <= 57) {
            temp = temp * 10 + int(password[index]) - 48;
            index++;
        }
        poss[i] = temp % 26;
        index++;
    }
    resetPlugboardSetup();
    while (index < size){
        updatePlugboardSetup(password[index - 1], password[index]);
        index += 2;
    }
}