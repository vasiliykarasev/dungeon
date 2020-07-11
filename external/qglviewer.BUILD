package(default_visibility = ["//visibility:public"])

cc_library(
    name = "qglviewer",
    includes = ["-Iexternal/qglviewer"],
    strip_include_prefix = "include/",
    hdrs = glob(["include/QGLViewer/*"]),
    srcs = glob(["lib/x86_64-linux-gnu/libQGLViewer-qt5.so*"]),
    linkopts = ["-L/usr/lib/x86_64-linux-gnu/"],
    linkstatic = False,
    deps = ["@opengl", "@qt5"],
)
