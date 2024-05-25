#ifndef DSPXMODELGLOBAL_H
#define DSPXMODELGLOBAL_H

#include <QtCore/QtGlobal>

#ifdef DSPXMODEL_STATIC
#  define DSPXMODEL_EXPORT
#else
#  ifdef DSPXMODEL_LIBRARY
#    define DSPXMODEL_EXPORT Q_DECL_EXPORT
#  else
#    define DSPXMODEL_EXPORT Q_DECL_IMPORT
#  endif
#endif

#define DSPXMODEL_DECLARE_VECTOR_CLASS(Container, T)                                               \
    class DSPXMODEL_EXPORT Container : public Substate::VectorEntityBase,                          \
                                       public Substate::VectorEntityHelper<Container, T> {         \
        Q_OBJECT                                                                                   \
        Q_SUBSTATE_DECLARE_VECTOR(Container, T)                                                    \
    public:                                                                                        \
        explicit Container(QObject *parent = nullptr);                                             \
                                                                                                   \
    protected:                                                                                     \
        inline explicit Container(Substate::Node *node, QObject *parent = nullptr)                 \
            : Substate::VectorEntityBase(node, parent) {                                           \
        }                                                                                          \
        friend class Private::Initializer;                                                              \
    };

#define DSPXMODEL_DECLARE_SHEET_CLASS(Container, T)                                                \
    class DSPXMODEL_EXPORT Container : public Substate::SheetEntityBase,                           \
                                       public Substate::SheetEntityHelper<Container, T> {          \
        Q_OBJECT                                                                                   \
        Q_SUBSTATE_DECLARE_SHEET(Container, T)                                                     \
    public:                                                                                        \
        explicit Container(QObject *parent = nullptr);                                             \
                                                                                                   \
    protected:                                                                                     \
        inline explicit Container(Substate::Node *node, QObject *parent = nullptr)                 \
            : Substate::SheetEntityBase(node, parent) {                                            \
        }                                                                                          \
        friend class Private::Initializer;                                                              \
    };

#endif // DSPXMODELGLOBAL_H
