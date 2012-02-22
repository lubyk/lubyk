/****************************************************************************
** Meta object code from reading C++ file 'Callback.h'
**
** Created: Wed Feb 22 08:55:27 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/mimas/Callback.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Callback.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_mimas__Callback[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x0a,
      34,   28,   16,   16, 0x0a,
      51,   28,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_mimas__Callback[] = {
    "mimas::Callback\0\0callback()\0value\0"
    "callback(double)\0callback(int)\0"
};

void mimas::Callback::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Callback *_t = static_cast<Callback *>(_o);
        switch (_id) {
        case 0: _t->callback(); break;
        case 1: _t->callback((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _t->callback((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData mimas::Callback::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject mimas::Callback::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_mimas__Callback,
      qt_meta_data_mimas__Callback, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &mimas::Callback::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *mimas::Callback::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *mimas::Callback::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_mimas__Callback))
        return static_cast<void*>(const_cast< Callback*>(this));
    if (!strcmp(_clname, "lubyk::ThreadedLuaObject"))
        return static_cast< lubyk::ThreadedLuaObject*>(const_cast< Callback*>(this));
    return QObject::qt_metacast(_clname);
}

int mimas::Callback::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
