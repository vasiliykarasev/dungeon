package(default_visibility = ["//visibility:public"])

cc_library(
    name = "opencv",
    includes = ["-Iexternal/opencv"],
    strip_include_prefix = "include/opencv4/",
    hdrs = glob(["include/opencv4/opencv2/**/**/*"]),
    srcs = glob(["lib/x86_64-linux-gnu/*libopencv_*.so"]),
    linkopts = ["-L/usr/lib/x86_64-linux-gnu/"],
    linkstatic = False,
)
