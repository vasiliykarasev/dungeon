package(default_visibility = ["//visibility:public"])

cc_library(
    name = "opengl",
    strip_include_prefix = "include/",
    hdrs = glob(["include/GL/*"]),
    srcs = ["lib/x86_64-linux-gnu/libOpenGL.so"],
    linkopts = ["-L/usr/lib/x86_64-linux-gnu/"],
    linkstatic = False,
)
