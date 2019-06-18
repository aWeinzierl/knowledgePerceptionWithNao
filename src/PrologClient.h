#pragma once

#include <string>
#include <iostream>
#include <unordered_map>

#include <json_prolog/prolog.h>

class PrologClient {
    std::unordered_map<std::string, unsigned int> class_to_current_id_associations_;
    json_prolog::Prolog _pl;

public:

    PrologClient();

    uint CreateTypeInstance(const std::string &associatedClass);

};