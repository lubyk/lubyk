/****************************************************************************
** Meta object code from reading C++ file 'CheckBox.h'
**
** Created: Tue Feb 21 11:56:45 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/mimas/CheckBox.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CheckBox.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_mimas__CheckBox[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       2,   19, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      25,   17,   16,   16, 0x08,

 // properties: name, type, flags
      51,   43, 0x0a095001,
      63,   57, 0x87095103,

       0        // eod
};

static const char qt_meta_stringdata_mimas__CheckBox[] = {
    "mimas::CheckBox\0\0checked\0clickedSlot(bool)\0"
    "QString\0class\0float\0hue\0"
};

void mimas::CheckBox::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CheckBox *_t = static_cast<CheckBox *>(_o);
        switch (_id) {
        case 0: _t->clickedSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData mimas::CheckBox::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject mimas::CheckBox::staticMetaObject = {
    { &QCheckBox::staticMetaObject, qt_meta_stringdata_mimas__CheckBox,
      qt_meta_data_mimas__CheckBox, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &mimas::CheckBox::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *mimas::CheckBox::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *mimas::CheckBox::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_mimas__CheckBox))
        return static_cast<void*>(const_cast< CheckBox*>(this));
    if (!strcmp(_clname, "ThreadedLuaObject"))
        return static_cast< ThreadedLuaObject*>(const_cast< CheckBox*>(this));
    return QCheckBox::qt_metacast(_clname);
}

int mimas::CheckBox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QCheckBox::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = cssClass(); break;
        case 1: *reinterpret_cast< float*>(_v) = hue(); break;
        }
        _id -= 2;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 1: setHue(*reinterpret_cast< float*>(_v)); break;
        }
        _id -= 2;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 2;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
