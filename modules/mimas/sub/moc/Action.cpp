/****************************************************************************
** Meta object code from reading C++ file 'Action.h'
**
** Created: Thu Dec 8 11:56:40 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/mimas/Action.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Action.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_mimas__Action[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_mimas__Action[] = {
    "mimas::Action\0\0triggeredSlot()\0"
};

const QMetaObject mimas::Action::staticMetaObject = {
    { &QAction::staticMetaObject, qt_meta_stringdata_mimas__Action,
      qt_meta_data_mimas__Action, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &mimas::Action::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *mimas::Action::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *mimas::Action::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_mimas__Action))
        return static_cast<void*>(const_cast< Action*>(this));
    if (!strcmp(_clname, "ThreadedLuaObject"))
        return static_cast< ThreadedLuaObject*>(const_cast< Action*>(this));
    return QAction::qt_metacast(_clname);
}

int mimas::Action::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAction::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: triggeredSlot(); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
