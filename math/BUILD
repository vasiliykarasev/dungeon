package(default_visibility = ["//visibility:public"])

cc_library(
    name = "sampling",
    hdrs = ["sampling.h"],
    deps = ["@glog"],
)

cc_test(
    name = "sampling_test",
    srcs = ["sampling_test.cpp"],
    deps = [
        ":sampling",
        "@gtest//:gtest_main",
    ],
)
