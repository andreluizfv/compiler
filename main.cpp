#include <iostream>
#include <string.h>
#include <map>
#include "t_token.h"
#include "t_const.h"
#include "searchKeyWord.h"
using namespace std;

// classe para usar a searchName
// Antes de chamar o metodo search name, fazer um if retorno da searchKeyName == 48
// Esse retorno representa o UNKNOWN de registros, ou seja, nao tem token primario 
class Tokens{
    private:
        map<string, int> mapObject;
        int count = 0; // representa o tamanho da hash table

    public:
        int searchName(char* word){
            // checando se existe, caso nao exista cria um novo elemento
            if (mapObject.find(word) == mapObject.end()){
                mapObject[word] = count;
                count = count + 1;
            }
            return mapObject[word];         
        }
};


int main(int argc, char* argv[]){
    cout << searchKeyWord("ELSE") << endl;
    Tokens tk;
    cout << tk.searchName("var1") << endl;
    cout << tk.searchName("var2") << endl;
}