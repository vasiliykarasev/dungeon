cc_library(
    name = "bloom_filter",
    srcs = ["bloom_filter.cpp"],
    hdrs = ["bloom_filter.h"],
    copts = ["-std=c++17"],
    deps = ["//external:glog"],
)

cc_library(
    name = "trie",
    hdrs = ["trie.h"],
    copts = ["-std=c++17"],
    deps = ["//external:glog"],
)

cc_test(
    name = "bloom_filter_test",
    srcs = ["bloom_filter_test.cpp"],
    copts = ["-Iexternal/gtest/include"] + ["-std=c++17"],
    deps = [
        ":bloom_filter",
        "@gtest//:main",
    ],
)

cc_test(
    name = "trie_test",
    srcs = ["trie_test.cpp"],
    copts = ["-Iexternal/gtest/include"] + ["-std=c++17"],
    deps = [
        ":trie",
        "@gtest//:main",
    ],
)
