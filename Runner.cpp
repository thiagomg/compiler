//
// Created by Thiago on 10/1/15.
//

#include "Runner.h"
#include "utils.h"

#include <iostream>

using namespace std;
using namespace generator;
using namespace Utils;

void Runner::run(generator::FuncExprPtr func)
{
    //built-in

    std::string level = "@";

    cout << level << " " << func->name << " " << f->body.size() << endl;
    for( auto e : func->body ) {
        std::string inner = "@" + level;

        cout << level << " " << e->name << endl;

        if( is_equal(e->name, "escreva") ) {
            for (auto t : func->params) {
                cout << t;
            }
            cout << endl;
        }
    }


}
