/****************************************************************************
** Meta object code from reading C++ file 'SocketNotifier.h'
**
** Created: Mon Sep 19 20:50:04 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/mimas/SocketNotifier.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SocketNotifier.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_mimas__SocketNotifier[] = {

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
      30,   23,   22,   22, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_mimas__SocketNotifier[] = {
    "mimas::SocketNotifier\0\0socket\0"
    "activatedSlot(int)\0"
};

const QMetaObject mimas::SocketNotifier::staticMetaObject = {
    { &QSocketNotifier::staticMetaObject, qt_meta_stringdata_mimas__SocketNotifier,
      qt_meta_data_mimas__SocketNotifier, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &mimas::SocketNotifier::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *mimas::SocketNotifier::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *mimas::SocketNotifier::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_mimas__SocketNotifier))
        return static_cast<void*>(const_cast< SocketNotifier*>(this));
    if (!strcmp(_clname, "LuaObject"))
        return static_cast< LuaObject*>(const_cast< SocketNotifier*>(this));
    return QSocketNotifier::qt_metacast(_clname);
}

int mimas::SocketNotifier::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QSocketNotifier::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: activatedSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
