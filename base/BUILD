package(default_visibility = ["//visibility:public"])

cc_library(
    name = "lazy",
    hdrs = ["lazy.h"],
    deps = ["@glog"],
)

cc_library(
    name = "enumerate",
    srcs = ["enumerate.h"],
)

cc_library(
    name = "thread_pool",
    srcs = ["thread_pool.cpp"],
    hdrs = ["thread_pool.h"],
    deps = [
        "@glog",
    ],
)

cc_library(
    name = "task_queue",
    srcs = ["task_queue.cpp"],
    hdrs = ["task_queue.h"],
    deps = ["@glog"],
)

cc_library(
    name = "periodic_task",
    srcs = ["periodic_task.cpp"],
    hdrs = ["periodic_task.h"],
    deps = ["@glog"],
)

cc_test(
    name = "enumerate_test",
    srcs = ["enumerate_test.cpp"],
    copts = ["-Iexternal/gtest/include"],
    deps = [
        ":enumerate",
        "@gtest//:main",
    ],
)

cc_test(
    name = "task_queue_test",
    srcs = ["task_queue_test.cpp"],
    copts = ["-Iexternal/gtest/include"],
    deps = [
        ":task_queue",
        "@gtest//:main",
    ],
)

cc_test(
    name = "periodic_task_test",
    srcs = ["periodic_task_test.cpp"],
    copts = ["-Iexternal/gtest/include"],
    deps = [
        ":periodic_task",
        "@glog",
        "@gtest//:main",
    ],
)

cc_test(
    name = "thread_pool_test",
    srcs = ["thread_pool_test.cpp"],
    copts = ["-Iexternal/gtest/include"],
    deps = [
        ":thread_pool",
        "@gtest//:main",
    ],
)

cc_test(
    name = "lazy_test",
    srcs = ["lazy_test.cpp"],
    copts = ["-Iexternal/gtest/include"],
    deps = [
        ":lazy",
        "@glog",
        "@gtest//:main",
    ],
)
