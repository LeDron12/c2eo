#ifndef __ALIASES__
#define __ALIASES__
#include "map"
#include "set"
#include "string"

std::set<std::string> known_types = {
    "memory",
    "seq",

};

std::map<std::string,std::string> known_aliases =
    {
        {"ram", "+alias c2eo.system.ram"},
        {"address", "+alias c2eo.system.address"},
        {"add-int64", "+alias c2eo.coperators.add-int64"},
        {"write", "+alias c2eo.coperators.write"},
        {"read-as-int64", "+alias c2eo.coperators.read-as-int64"},
        {"printf", "+alias c2eo.stdio.printf"},
        {"while", "+alias c2eo.coperators.while"},
        {"if", "+alias c2eo.coperators.if"},
        {"add", "+alias c2eo.coperators.add"},
        {"plug", "+alias c2eo.coperators.plug"},
        {"greater", "+alias c2eo.coperators.greater"},
        {"add", "+alias c2eo.coperators.add"},

    };


#endif
