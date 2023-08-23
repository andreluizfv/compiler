#include <iostream>
#include <string>
#include <map>

#include "t_token.h"
#include "t_const.h"
#include "Utils.h"
using namespace std;

// classe para usar a searchName
// Antes de chamar o metodo search name, fazer um if retorno da searchKeyName == 48
// Esse retorno representa o UNKNOWN de registros, ou seja, nao tem token primario 
class Tokens{
    private:
        map<string, int> mapObject;
        int count = 0; // representa o tamanho da hash table

    public:
        int searchName(const std::string& word){
            if (mapObject.find(word) == mapObject.end()){
                mapObject[word] = count;
                count++;
            }
            return mapObject[word];         
        }
};


int main(int argc, char* argv[]){
    cout << Utils::searchKeyword("ELSE") << endl;
    Tokens tk;
    cout << tk.searchName("var1") << endl;
    cout << tk.searchName("var2") << endl;
}