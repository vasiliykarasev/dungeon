cc_library(
    name = "deferred_acceptance",
    srcs = [
        "deferred_acceptance.cpp",
        "deferred_acceptance_impl.cpp",
    ],
    hdrs = [
        "deferred_acceptance.h",
        "deferred_acceptance_impl.h",
    ],
    copts = ["-Iexternal/eigen"],
    deps = [
        "@eigen",
        "@glog",
    ],
)

cc_test(
    name = "deferred_acceptance_test",
    srcs = ["deferred_acceptance_test.cpp"],
    copts = ["-Iexternal/gtest/include"],
    deps = [
        ":deferred_acceptance",
        "@gtest//:main",
    ],
)
