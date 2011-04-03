/****************************************************************************
** Meta object code from reading C++ file 'TableView.h'
**
** Created: Sun Apr 3 16:53:16 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/mimas/TableView.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TableView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_mimas__TableView[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       2,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
      25,   17, 0x0a095001,
      37,   31, 0x87095103,

       0        // eod
};

static const char qt_meta_stringdata_mimas__TableView[] = {
    "mimas::TableView\0QString\0class\0float\0"
    "hue\0"
};

const QMetaObject mimas::TableView::staticMetaObject = {
    { &QTableView::staticMetaObject, qt_meta_stringdata_mimas__TableView,
      qt_meta_data_mimas__TableView, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &mimas::TableView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *mimas::TableView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *mimas::TableView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_mimas__TableView))
        return static_cast<void*>(const_cast< TableView*>(this));
    if (!strcmp(_clname, "DeletableOutOfLua"))
        return static_cast< DeletableOutOfLua*>(const_cast< TableView*>(this));
    return QTableView::qt_metacast(_clname);
}

int mimas::TableView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTableView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
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
