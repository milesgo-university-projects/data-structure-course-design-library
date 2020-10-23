#-------------------------------------------------
#
# Project created by QtCreator 2020-10-22T23:06:53
#
#-------------------------------------------------

QT       += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = library
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    addauthoritybox.cpp \
    addbookborroworreservewidget.cpp \
    addbooksvariety.cpp \
    addfeeacount.cpp \
    addnewbook.cpp \
    addpublisherbox.cpp \
    addreaderbox.cpp \
    administratormanagewidget.cpp \
    authority.cpp \
    authoritywidget.cpp \
    book.cpp \
    bookborrowedbyreader.cpp \
    bookborroworreservemanagewidget.cpp \
    bookborrowreservemodel.cpp \
    bookmodel.cpp \
    bookviewwidget.cpp \
    borroworreserve.cpp \
    changepasswordwidget.cpp \
    changesamebookdistinctionitem.cpp \
    comboboxwithdropdown.cpp \
    ExportExcelObject.cpp \
    friendtablewidget.cpp \
    functions.cpp \
    institutewidget.cpp \
    login.cpp \
    logs.cpp \
    main.cpp \
    maintabwidget.cpp \
    myapplication.cpp \
    mytime.cpp \
    publishertablemodel.cpp \
    publishertableviewwidget.cpp \
    qcustomplot.cpp \
    qpasswordlineedit.cpp \
    reader.cpp \
    readerinformationwidget.cpp \
    readermodel.cpp \
    readerviewwidget.cpp \
    searchlineedit.cpp \
    selectdatewidget.cpp \
    statisticdata.cpp \
    statisticsgraph.cpp \
    statusbar.cpp \
    timesimulation.cpp \
    titlebar.cpp \
    twosizebutton.cpp \
    waitingqueuetablewidget.cpp \
    widget.cpp

HEADERS += \
    addauthoritybox.h \
    addbookborroworreservewidget.h \
    addbooksvariety.h \
    addfeeacount.h \
    addnewbook.h \
    addpublisherbox.h \
    addreaderbox.h \
    administratormanagewidget.h \
    authority.h \
    authoritywidget.h \
    book.h \
    bookborrowedbyreadertablewidget.h \
    bookborroworreservemanagewidget.h \
    bookborrowreservemodel.h \
    bookmodel.h \
    bookviewwidget.h \
    borroworreserve.h \
    changepasswordwidget.h \
    changesamebookdistinctionitem.h \
    comboboxwithdropdown.h \
    ExportExcelObject.h \
    friendtablewidget.h \
    functions.h \
    institutewidget.h \
    login.h \
    logs.h \
    maintabwidget.h \
    myapplication.h \
    mytime.h \
    publisher.h \
    publishertablemodel.h \
    publishertableviewwidget.h \
    qcustomplot.h \
    qpasswordlineedit.h \
    Reader.h \
    readerinformationwidget.h \
    readermodel.h \
    readerviewwidget.h \
    searchlineedit.h \
    selectdatewidget.h \
    statisticdata.h \
    statisticsgraph.h \
    statusbar.h \
    timesimulation.h \
    titlebar.h \
    twosizebutton.h \
    waitingqueuetablewidget.h \
    widget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
