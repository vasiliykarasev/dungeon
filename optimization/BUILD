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
        "//base:enumerate",
        "@eigen",
        "@glog",
    ],
)

cc_test(
    name = "deferred_acceptance_test",
    srcs = ["deferred_acceptance_test.cpp"],
    deps = [
        ":deferred_acceptance",
        "@gtest//:gtest_main",
    ],
)
