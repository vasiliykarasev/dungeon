package(default_visibility = ["//visibility:public"])

cc_library(
    name = "qt5",
    deps = [
        ":qt5_core",
        ":qt5_headers",
        ":qt5_gui",
        ":qt5_xml",
        ":qt5_opengl",
        ":qt5_widgets",
    ],
)

cc_library(
    name = "qt5_opengl",
    strip_include_prefix = "include/x86_64-linux-gnu/qt5/QtOpenGL/",
    hdrs = glob(["include/x86_64-linux-gnu/qt5/QtOpenGL/*"]),
    srcs = ["lib/x86_64-linux-gnu/libQt5OpenGL.so"],
    linkopts = ["-L/usr/lib/x86_64-linux-gnu/"],
    linkstatic = False,
    deps = ["@opengl"],
)

cc_library(
    name = "qt5_widgets",
    strip_include_prefix = "include/x86_64-linux-gnu/qt5/QtWidgets/",
    hdrs = glob(["include/x86_64-linux-gnu/qt5/QtWidgets/*"]),
    srcs = ["lib/x86_64-linux-gnu/libQt5Widgets.so"],
    linkopts = ["-L/usr/lib/x86_64-linux-gnu/"],
    linkstatic = False,
)

cc_library(
    name = "qt5_core",
    strip_include_prefix = "include/x86_64-linux-gnu/qt5/QtCore/",
    hdrs = glob(["include/x86_64-linux-gnu/qt5/QtCore/*"]),
    srcs = ["lib/x86_64-linux-gnu/libQt5Core.so"],
    linkopts = ["-L/usr/lib/x86_64-linux-gnu/"],
    linkstatic = False,
)


# Headers are made available as e.g. #include <QXmlReader>
cc_library(
    name = "qt5_xml",
    strip_include_prefix = "include/x86_64-linux-gnu/qt5/QtXml/",
    hdrs = glob(["include/x86_64-linux-gnu/qt5/QtXml/*"]),
    srcs = ["lib/x86_64-linux-gnu/libQt5Xml.so"],
    linkopts = ["-L/usr/lib/x86_64-linux-gnu/"],
    linkstatic = False,
)

# Headers are made available as e.g. #include <QWindow>
cc_library(
    name = "qt5_gui",
    strip_include_prefix = "include/x86_64-linux-gnu/qt5/QtGui",
    hdrs = glob(["include/x86_64-linux-gnu/qt5/QtGui/*"]),
    srcs = ["lib/x86_64-linux-gnu/libQt5Gui.so"],
    linkopts = ["-L/usr/lib/x86_64-linux-gnu/"],
    linkstatic = False,
)

# Just the headers, made available as e.g. #include <QtOpenGL/QGLWidget>
cc_library(
    name = "qt5_headers",
    strip_include_prefix = "include/x86_64-linux-gnu/qt5/",
    hdrs = glob(["include/x86_64-linux-gnu/qt5/**/*"]),
)
