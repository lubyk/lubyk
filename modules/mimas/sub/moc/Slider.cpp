/****************************************************************************
** Meta object code from reading C++ file 'Slider.h'
**
** Created: Sat Jul 30 16:57:10 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/mimas/Slider.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Slider.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_mimas__Slider[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       3,   29, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      49,   36,   14,   14, 0x0a,
      72,   66,   14,   14, 0x0a,

 // properties: name, type, flags
      97,   89, 0x0a095001,
     109,  103, 0x87095103,
     117,  113, 0x02095003,

       0        // eod
};

static const char qt_meta_stringdata_mimas__Slider[] = {
    "mimas::Slider\0\0valueChanged(double)\0"
    "remote_value\0setValue(double)\0value\0"
    "callback(double)\0QString\0class\0float\0"
    "hue\0int\0border\0"
};

const QMetaObject mimas::Slider::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_mimas__Slider,
      qt_meta_data_mimas__Slider, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &mimas::Slider::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *mimas::Slider::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *mimas::Slider::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_mimas__Slider))
        return static_cast<void*>(const_cast< Slider*>(this));
    if (!strcmp(_clname, "LuaCallback"))
        return static_cast< LuaCallback*>(const_cast< Slider*>(this));
    if (!strcmp(_clname, "DeletableOutOfLua"))
        return static_cast< DeletableOutOfLua*>(const_cast< Slider*>(this));
    return QWidget::qt_metacast(_clname);
}

int mimas::Slider::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: setValue((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: callback((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 3;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = cssClass(); break;
        case 1: *reinterpret_cast< float*>(_v) = hue(); break;
        case 2: *reinterpret_cast< int*>(_v) = borderWidth(); break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 1: setHue(*reinterpret_cast< float*>(_v)); break;
        case 2: setBorderWidth(*reinterpret_cast< int*>(_v)); break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 3;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void mimas::Slider::valueChanged(double _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
