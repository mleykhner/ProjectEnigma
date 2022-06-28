#include <iostream>
#include "configuration.cpp"
#include "string"

int rotorStep(int rotor, int input, int direction, int position);//Проецирование символа через ротор
int projectLetter(int input, int rots[], int * poss); //Проецирование символа через все роторы
void newPositions(int * positions); //Расчет новых положений роторов
std::string encryptText(std::string input, int rots[], int * poss); //Шифрование строки
bool updatePlugboardSetup(char A, char B); //Изменение настройки коммутационной панели
void resetPlugboardSetup(); //Сбросить настройки коммутационной панели
void showCurrentSetup(int rots[], int poss[]); //Отобразить текущие настройки
std::string generatePassword(int rots[], int poss[]); //Генерация пароля
void applyPassword(std::string password, int * rots, int * poss); //Ввод пароля

//Общие настройки
bool saveSpaces = false; // Сохранять пробелы в шифруемом тексте?
bool saveUnencryptableSymbols = false; //Сохранять символы, которые не могут быть зашифрованы?
bool showInput = true; //Показывать входные строки на экране?
//Общие настройки


int main() {
    int activeRotors[] = {0, 2, 1, 5}; //Настройка активных роторов и рефлектора
    int activeRotors_poss[] = {11, 23, 12}; //Настройка начального положения


    int state = 0; //Текущее состояние программы
    while (true){
        switch (state) { //Вывод информации на экран в зависимости от состояния программы
            case -1:{ //Пустой вывод
                break;
            }
            case 0:{ //Начальное состояние программы
                std::cout << "Active rotors:  \t"; //Вывод активных роторов и рефлектора
                for (int activeRotor : activeRotors) std::cout << "[" << rotorNames[activeRotor] << "] \t";
                std::cout << std::endl;

                std::cout << "Rotors positions:\t"; //Вывод позиции роторов
                for (int activeRotors_pos : activeRotors_poss) std::cout << "[" << activeRotors_pos << "] \t";
                std::cout << std::endl;

                std::cout << "Type \"help\" to get info about app.\n";
                break;
            }
            case 1:{ //Вывод информации о возможных настройках
                std::cout << "\"updpos [index (1-3)] [pos (1-26)]\" - update positions\n"; //Изменить активный ротор
                std::cout << "\"selrot [index (1-4)] [rot (1 - 5 or B or C)]\" - select rotors\n"; //Изменить положение ротора
                std::cout << "\"editpb [A] [B]\" - edit plugboard.\n"; //Изменить настройки коммутационной панели
                std::cout << "\"respb\" - reset plugboard.\n"; //Сбросить настройки коммутационной панели
                std::cout << "\"savespaces [on/off]\"\n"; //Сохранять пробелы в шифруемом тексте?
                std::cout << "\"saveunencryptables [on/off]\"\n"; //Сохранять символы, которые невозможно зашифровать?
                std::cout << "\"menu\" - show menu\n"; //Вернуться в меню
                break;
            }
            case 2:{ //Меню в состоянии шифрования
                std::cout << "\"*\" - hide original text\n"; //Скрывать входной текст?
                std::cout << "\"&\" - menu\n"; //Вернуться в меню
                std::cout << "\"?\" - show current setup\n"; //Показать текущие настройки
                std::cout << "\"#\" - export password\n"; //Сгенериговать пароль с текущими настройками
                std::cout << "\"@\" - apply password\n"; //Применить пароль
                std::cout << "\"[word]\" - encrypt\n"; //Зашифровать слово
                std::cout << "Dont forget to remember the settings before encrypting!\n";
                break;
            }
            default:
                break;
        }

        std::cout << "> "; //Вывод приглашения к вводу
        std::string request;
        getline(std::cin, request); //Сохранение ввода в request

        if(request == "help" && state != 2){ //Вывод всех доступных команд
            std::cout << "\"setup\" - edit configuration.\n";
            std::cout << "\"setup\" -> \"updpos [index (1-3)] [pos (1-26)]\" - update positions.\n";
            std::cout << "\"setup\" -> \"selrot [index (1-4)] [rot (1 - 5 or B or C)]\" - select rotors.\n";
            std::cout << "\"setup\" -> \"editpb [A] [B]\" - edit plugboard.\n";
            std::cout << "\"setup\" -> \"respb\" - reset plugboard.\n";
            std::cout << "\"setup\" -> \"savespaces [on/off]\"\n";
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
                case 0:{ //Переключение состояния из меню
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
                    if (request == "respb") { //Сброс настроек коммутационнной панели
                        resetPlugboardSetup();
                        std::cout << "Plugboard is now reset\n";
                    }
                    else if(request.find("editpb") != std::string::npos) { //Обновление настроек коммутационной панели
                        if(updatePlugboardSetup(toupper(request[7]), toupper(request[9]))){
                            std::cout << "Plugboard updated: [" << char(toupper(request[7])) << "] <-> [" << char(toupper(request[9])) << "]\n";
                        }
                        else
                            std::cout << "Undefined symbol found.\n";
                    }
                    else if(request.find("updpos") != request.npos) { //Изменение положения ротора
                        int temp = 0;
                        int i = 9;
                        while (request[i] >= 48 && request[i] <= 57) {
                            temp = temp * 10 + int(request[i]) - 48;
                            i++;
                        }
                        activeRotors_poss[int(request[7]) - 49] = temp % 26;
                        std::cout << "Positions updated: [rotor " << int(request[7]) - 48 << "] -> " << temp % 26 << "\n";
                    }
                    else if(request.find("selrot") != request.npos){ //Выбор активных роторв и рефлектора
                        int selectedIndex = int(request[7]) - 49; //Позиция выбранного ротора
                        int selectedRotor = int(toupper(request[9])); //Выбранный ротор
                        if (selectedIndex >= 0 && selectedIndex <= 2 && selectedRotor >= 49 && selectedRotor <= 53){
                            activeRotors[selectedIndex] = selectedRotor - 49; //На первых трех позициях может быть один из пяти роторв
                            std::cout << "Rotor selected: [" << selectedIndex + 1 << "] -> " << rotorNames[selectedRotor - 49] << "\n";
                        }
                        else if(selectedIndex == 3 && (selectedRotor == 66 || selectedRotor == 67)){
                            activeRotors[3] = selectedRotor - 61; //На четвертой позиции может находиться только рефлектор B или С
                            std::cout << "Reflector selected: " << char(selectedRotor) << "\n";
                        }
                        else std::cout << "Undefined index or rotor\n";
                    }
                    else if(request.find("savespaces") != request.npos){
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
                    if(request == "*") showInput = !showInput; //Переключение showInput
                    else if(request == "&") state = 0; //Возврат в меню
                    else if(request == "?") { //Показать текущие настройки
                        showCurrentSetup(activeRotors, activeRotors_poss);
                    }
                    else if(request == "#") std::cout << "Password: " << generatePassword(activeRotors,
                                                                                          activeRotors_poss) << "\n"; //Генерация пароля
                    else if(request == "@"){ //Применение пароля
                        std::cout << "Type password: ";
                        std::string pass;
                        getline(std::cin, pass);
                        applyPassword(pass, activeRotors, activeRotors_poss); //Применить пароль
                        std::cout << "Password applied!\n";
                        showCurrentSetup(activeRotors, activeRotors_poss); //Показать текущие настройки
                    }
                    else{
                        for(auto & c : request) c = toupper(c); //Приведение входного текста к верхнему регистру
                        std::cout << "Input: " << (showInput ? request : (std::string(request.length(), '*'))) << "\n";
                        std::cout << "Output: " << encryptText(request, activeRotors, activeRotors_poss) << "\n";
                    }
                }
                default:
                    break;
            }
        }
    }
}

void showCurrentSetup(int rots[], int poss[]){ //Показать текущие настройки
    std::cout << "Active rotors:  \t"; //Активные роторы и рефлектор
    for (int i = 0; i < 4; i++) std::cout << "[" << rotorNames[rots[i]] << "] \t";
    std::cout << std::endl;

    std::cout << "Rotors positions:\t"; //Позиуии роторов
    for (int i = 0; i < 3; i++) std::cout << "[" << poss[i] << "] \t";
    std::cout << std::endl << "Plugboard:\n"; //Коммутационная панель
    bool def = true; //Переменная хранит статус коммутационной панели. Изменены ли её настройки, или нет.
    std::string added;
    for(int i = 0; i < 26; i++){
        if (plugboardSetup[i] != i){ //Если выход коммутационной панели для символа i не равен входу...
            if(added.find(char(i + 65)) != added.npos) continue; //...проверяем, не выводилась ли уже эта конфигурация?
            std::cout << "[" << char(plugboardSetup[i] + 65) << "] <-> [" << char(i + 65) << "]\n"; //Если нет, то выводим такую конфигурацию
            added += char(i + 65);
            added += char(plugboardSetup[i] + 65); //И добавляем её в список выведенных
            def = false;
        }
    }
    if (def) std::cout << "default\n"; //Если конфиругация по-умолчанию, сообщаем об этом пользователю
}

std::string encryptText(std::string input, int rots[], int * poss){ //Шифрование текста
    bool unencryptedSymbols = false; //Есть ли в строке символы, которые нельзя зашифровать?
    int length = int(input.length()); //Сохраняем длину строки
    std::string temp; //Переменная для хранения зашифрованной строки
    for(int i = 0; i < length; i++){ //Шифруем посимвольно
        int currentSymbol = int(toupper(input[i])) - 65; //Определяем номер символа в латинском алфавите
        if(currentSymbol >= 0 && currentSymbol <= 25){ //Если символ входит в алфовит, то шифруем его
            temp += char(projectLetter(currentSymbol, rots, poss) + 65); //Возвращаем в символьный вид char
        }
        else if (currentSymbol == -33){ //Если символ - пробел, то в зависимости от saveSpaces сохраняем или не сохраняем его
            if(saveSpaces) temp += ' ';
        }
        else{ //При любом другом символе - сохраняем его только если включена настройка saveUnencryplableSymbols
            unencryptedSymbols = true; //Запомниаем, что в тексте встречались символы, которые невозможно зашифровать
            if(saveUnencryptableSymbols) temp += input[i];
        }
    }
    if(unencryptedSymbols) std::cout << "String contains unencryptable symbols\n";
    //Если встретились нешифруемые символы
    // сообщаем об этом пользователю
    return temp;
}

int rotorStep(int rotor, int input, int direction, int position){ //Переход по одному ротору
    return  (rotors[rotor][direction]                       //[rotor] - индекс ротора или рефлектора
            [(input + (direction ? 0 : position)) % 26]     //[direction] - направление, в котором сейчас проходит сигнал.
            - (direction ? position : 0)) % 26;             //От направления зависит с какой стороны ротора необходимо отложить
                                                            //текущее положение ротора.
}

int projectLetter(int input, int rots[], int * poss){ //Шифрование одного символа
    int temp = plugboardSetup[input]; //Сохраняем символ, пропустив его через настройки коммутационной панели
    for (int i = 3; i > -4 ; i--){ //По очереди проходим по всем роторам в двух направлениях
        temp = rotorStep(rots[3 - abs(i)], temp, (i >= 0 ? 0 : 1),
                         (i ? poss[3 - abs(i)] : 0));   //Рефлектор не изменяет своего положения,
                                                                //поэтому его положение всегда 0
        while (temp < 0) temp += 26; //Если из-за смещения получилось отрицательное число, находим первое положительное
    }
    newPositions(poss); //Обновляем позиции роторов
    return plugboardSetup[temp]; //Возвращаем букву, пропустив ее через коммутационную панель
}

void newPositions(int * positions){ //Расчет новых позиций роторов
    positions[0]++;
    for (int i = 0; i < 3; i++){
        if(positions[i] > 25){
            positions[i] = 0;
            if(i < 2) positions[i + 1]++;
        }
    }
}

bool updatePlugboardSetup(char A, char B){ //Изменение настроек коммутационной панели
    if(A >= 65 && A <= 90 && B >= 65 && B <=90){ //Если оба символа - буквы

        int charA = int(A) - 65; //Считаем их номер в алфавите
        int charB = int(B) - 65;

        if(plugboardSetup[charA] != charA || plugboardSetup[charB] != charB){   //Если эти буквы участвуют в какой либо настройке,
            plugboardSetup[plugboardSetup[charA]] = plugboardSetup[charA];      //то обнуляем ее
            plugboardSetup[plugboardSetup[charB]] = plugboardSetup[charB];
        }

        plugboardSetup[charA] = charB; //Применяем новые настройки
        plugboardSetup[charB] = charA;
        return true;
    }
    else return false; //Иначе возвращаем ошибку

}

void resetPlugboardSetup() { //Сброс настроек коммутационной панели
    for (int i = 0; i < 26; i++){
        plugboardSetup[i] = i;
    }
}

std::string generatePassword(int rots[], int poss[]){ //Генерация пароля
    std::string temp;
    for (int i = 0; i < 4; i++) temp += char(rots[i] + 48); //Записываем активные роторы и рефлектор
    for (int i = 0; i < 3; i++) { //Записываем положения роторов
        std::string number;
        int tempInt = poss[i];
        if(tempInt == 0)  number = '0';
        else
        while(tempInt != 0){
            number = char((tempInt % 10) + 48) + number;
            tempInt = tempInt / 10;
        }
        temp += "-";
        temp += number;
    }
    std::string added; //Сохраняем настройки коммутационной панели, которые уже внесли в пароль
    for(int i = 0; i < 26; i++){
        if(i != plugboardSetup[i]){
            if(added.find(char(i + 65)) != added.npos) continue;
            added += char(i + 65);
            added += char(plugboardSetup[i] + 65);
        }
    }
    temp += added;
    return temp; //Возвращаем пароль
}

void applyPassword(std::string password, int * rots, int * poss){ //Применение пароля
    int size = password.length(); //Сохраняем длину пароля
    for (int i = 0; i < 4; i++) { //Первые четыре символа - активные роторы и релектор
        rots[i] = int(password[i] - 48);
    }
    int index = 5;
    for (int i = 0; i < 3; i++) { //Далее идут три числа, обозначающие положения каждого ротора
        int temp = 0;
        while (password[index] >= 48 && password[index] <= 57) { //Пока символ - цифра...
            temp = temp * 10 + int(password[index]) - 48; //Добавляем ее в позицию ротора
            index++;
        }
        poss[i] = temp % 26;
        index++;
    }
    resetPlugboardSetup(); //Сбрасываем настройки коммутационной панели
    while (index < size){ //Если в пароле еще остались символы, применяем их в качестве настоек для коммутационной панели
        updatePlugboardSetup(password[index - 1], password[index]);
        index += 2;
    }
}