cmake_minimum_required(VERSION 3.5)

project(BookShop VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Sql LinguistTools Charts REQUIRED)


set(TS_FILES BookShop_ru_RU.ts)

set(PROJECT_SOURCES
    main.cpp
    SETTINGS/SETTINGS.h
    SETTINGS/SETTINGS.cpp
    MainWindow/mainwindow.cpp
    MainWindow/mainwindow.h
    MainWindow/mainwindow.ui
    UserWindow/userwindow.h
    UserWindow/userwindow.cpp
    UserWindow/userwindow.ui
    EmployeeWindow/employeewindow.h
    EmployeeWindow/employeewindow.cpp
    EmployeeWindow/employeewindow.ui
    Widgets/AvailableBooksWidget/availablebookswidget.h
    Widgets/AvailableBooksWidget/availablebookswidget.cpp
    Widgets/AvailableBooksWidget/availablebookswidget.ui
    Widgets/UsersOrdersWidget/usersorderswidget.h
    Widgets/UsersOrdersWidget/usersorderswidget.cpp
    Widgets/UsersOrdersWidget/usersorderswidget.ui
    Widgets/Reports/reports.h
    Widgets/Reports/reports.cpp
    Widgets/Reports/reports.ui
    Widgets/AllOrders/allorders.h
    Widgets/AllOrders/allorders.cpp
    Widgets/AllOrders/allorders.ui
    Widgets/PublisherBooks/publisherbooks.h
    Widgets/PublisherBooks/publisherbooks.cpp
    Widgets/PublisherBooks/publisherbooks.ui
    Widgets/OrderByDate/orderbydate.h
    Widgets/OrderByDate/orderbydate.cpp
    Widgets/OrderByDate/orderbydate.ui
    Widgets/GistogrammBookPrice/gistogrammbookprice.h
    Widgets/GistogrammBookPrice/gistogrammbookprice.cpp
    Widgets/GistogrammBookPrice/gistogrammbookprice.ui
    Widgets/DiagrammCarSupplier/diagrammcarsupplier.h
    Widgets/DiagrammCarSupplier/diagrammcarsupplier.cpp
    Widgets/DiagrammCarSupplier/diagrammcarsupplier.ui
    Widgets/PublisherForm/publisherform.h
    Widgets/PublisherForm/publisherform.cpp
    Widgets/PublisherForm/publisherform.ui
    Widgets/SupplierForm/supplierform.h
    Widgets/SupplierForm/supplierform.cpp
    Widgets/SupplierForm/supplierform.ui
    Widgets/ClientForm/clientform.h
    Widgets/ClientForm/clientform.cpp
    Widgets/ClientForm/clientform.ui
    Widgets/SellerForm/sellerform.h
    Widgets/SellerForm/sellerform.cpp
    Widgets/SellerForm/sellerform.ui
    Widgets/ReferenceInformation/referenceinformation.h
    Widgets/ReferenceInformation/referenceinformation.cpp
    Widgets/ReferenceInformation/referenceinformation.ui
    Widgets/BookForm/bookform.h
    Widgets/BookForm/bookform.cpp
    Widgets/BookForm/bookform.ui
    Widgets/OrderForm/orderform.h
    Widgets/OrderForm/orderform.cpp
    Widgets/OrderForm/orderform.ui
    Widgets/OrderFormForUser/orderformforuser.h
    Widgets/OrderFormForUser/orderformforuser.cpp
    Widgets/OrderFormForUser/orderformforuser.ui
    Widgets/UsersOrdersWidgetForUser/usersorderswidgetforuser.h
    Widgets/UsersOrdersWidgetForUser/usersorderswidgetforuser.cpp
    Widgets/UsersOrdersWidgetForUser/usersorderswidgetforuser.ui
)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(BookShop
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(BookShop SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(BookShop
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(BookShop PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql Qt${QT_VERSION_MAJOR}::Charts)

set_target_properties(BookShop PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS BookShop
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(BookShop)
endif()
