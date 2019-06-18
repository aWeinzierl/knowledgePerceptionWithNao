#include "PrologClient.h"

PrologClient::PrologClient() {
        class_to_current_id_associations_ = {
                {"Carrot",  0},
                {"Donut",   0},
                {"HotWing", 0},
        };
    }

uint PrologClient::CreateTypeInstance(const std::string &associatedClass) {
    auto supposedId = class_to_current_id_associations_.find(associatedClass)->second + 1;

    json_prolog::PrologQueryProxy bdgs = _pl.query(
            "rdf_costom_instance_from_class('http://knowrob.org/kb/knowrob.owl#" + associatedClass + "',_," +
            std::to_string(supposedId) + ",ObjInst)");

    class_to_current_id_associations_[associatedClass] = supposedId;

    for (auto const & bdg : bdgs){
        std::cout << bdg["ObjInst"] << "\n";
    }

    return supposedId;
}
