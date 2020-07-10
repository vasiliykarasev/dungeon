package(default_visibility = ["//visibility:public"])

cc_library(
    name = "llvm",
    includes = ["-Iexternal/llvm"],
    strip_include_prefix = "include/",
    hdrs = glob(["include/**/**/*"]),
    srcs = ["lib/libLLVM.so", "lib/libclang.so", "lib/libLTO.so", "lib/libRemarks.so"],
    linkopts = ["-L/usr/lib/llvm-10/lib/"], # "-lLLVM", "-lclang"],
    linkstatic = False,
    deps = [":static"],
)

cc_library(
    name = "static",
    includes = ["-Iexternal/llvm"],
    strip_include_prefix = "include/",
    hdrs = glob(["include/**/**/*"]),
    srcs = glob(["lib/*.a"]),
    #linkopts = ["-L/usr/lib/llvm-10/lib/"],
    linkstatic = True,
)
