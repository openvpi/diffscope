#ifndef DSPXMODELGLOBAL_P_H
#define DSPXMODELGLOBAL_P_H

#include <substate/node.h>
#include <qsubstate/private/entity_p.h>
#include <dspxmodel/dspxmodelglobal.h>

namespace QDspx {

    template <class T, class... Args>
    inline T *tagged_node(const std::string &key, Args &&...arguments) {
        static_assert(std::is_base_of<Substate::Node, T>::value,
                      "T should inherit from Substate::Node");
        auto p = new T(arguments...);
        p->setDynamicData(Substate::entity_dyn_key, key);
        return p;
    }

}

#define DSPXMODEL_DECLARE_SHEET_CLASS_IMPL(Container, Key)                                         \
    Container::Container(QObject *parent)                                                          \
        : Container(QDspx::tagged_node<Substate::SheetNode>(Key), parent) {                        \
    }

#define DSPXMODEL_DECLARE_VECTOR_CLASS_IMPL(Container, Key)                                        \
    Container::Container(QObject *parent)                                                          \
        : Container(QDspx::tagged_node<Substate::VectorNode>(Key), parent) {                       \
    }

#endif // DSPXMODELGLOBAL_P_H
