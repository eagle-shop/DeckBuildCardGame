#include "General.hpp"

// 文字列に指定した文字が存在していれば、それを返す
void getString(const string **dst, const string *allString, vector<const string*> *partString){
    for(auto i = 0; i < partString->size(); i++){
        if(STR_SEARCH(allString, *(*partString)[i])){
            *dst = (*partString)[i];
            return;
        }
    }
}
