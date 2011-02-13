/****************************************************************************
** Meta object code from reading C++ file 'GLWindow.h'
**
** Created: Sun Feb 13 22:54:36 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/mimas/GLWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GLWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_mimas__GLWindow[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       1,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
      24,   16, 0x0a095001,

       0        // eod
};

static const char qt_meta_stringdata_mimas__GLWindow[] = {
    "mimas::GLWindow\0QString\0class\0"
};

const QMetaObject mimas::GLWindow::staticMetaObject = {
    { &QGLWidget::staticMetaObject, qt_meta_stringdata_mimas__GLWindow,
      qt_meta_data_mimas__GLWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &mimas::GLWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *mimas::GLWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *mimas::GLWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_mimas__GLWindow))
        return static_cast<void*>(const_cast< GLWindow*>(this));
    if (!strcmp(_clname, "DeletableOutOfLua"))
        return static_cast< DeletableOutOfLua*>(const_cast< GLWindow*>(this));
    return QGLWidget::qt_metacast(_clname);
}

int mimas::GLWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = cssClass(); break;
        }
        _id -= 1;
    } else if (_c == QMetaObject::WriteProperty) {
        _id -= 1;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 1;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
