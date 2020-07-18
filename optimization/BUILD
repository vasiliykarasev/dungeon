cc_library(
    name = "ransac",
    srcs = [
        "ransac.cpp",
        "ransac_impl.cpp",
    ],
    hdrs = [
        "ransac.h",
        "ransac_impl.h",
    ],
    deps = [
        "//math:sampling",
        "@eigen",
        "@glog",
    ],
)

cc_test(
    name = "ransac_test",
    srcs = ["ransac_test.cpp"],
    deps = [
        ":ransac",
        "@com_github_gflags_gflags//:gflags",
        "@gtest//:gtest_main",
    ],
)

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
