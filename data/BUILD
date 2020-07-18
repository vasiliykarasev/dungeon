cc_library(
    name = "bloom_filter",
    srcs = ["bloom_filter.cpp"],
    hdrs = ["bloom_filter.h"],
    deps = ["@glog"],
)

cc_library(
    name = "trie",
    hdrs = ["trie.h"],
    deps = ["@glog"],
)

cc_library(
    name = "interval",
    srcs = ["interval.h"],
    deps = [],
)

cc_library(
    name = "interval_tree",
    srcs = ["interval_tree.cpp"],
    hdrs = ["interval_tree.h"],
    deps = [
        ":interval",
        "@glog",
    ],
)

cc_library(
    name = "count_min_sketch",
    srcs = ["count_min_sketch.cpp"],
    hdrs = ["count_min_sketch.h"],
    deps = ["@glog"],
)

cc_library(
    name = "lru_cache",
    hdrs = ["lru_cache.h"],
)

cc_test(
    name = "bloom_filter_test",
    srcs = ["bloom_filter_test.cpp"],
    deps = [
        ":bloom_filter",
        "@gtest//:gtest_main",
    ],
)

cc_test(
    name = "trie_test",
    srcs = ["trie_test.cpp"],
    deps = [
        ":trie",
        "@gtest//:gtest_main",
    ],
)

cc_test(
    name = "count_min_sketch_test",
    srcs = ["count_min_sketch_test.cpp"],
    deps = [
        ":count_min_sketch",
        "@gtest//:gtest_main",
    ],
)

cc_test(
    name = "lru_cache_test",
    srcs = ["lru_cache_test.cpp"],
    deps = [
        ":lru_cache",
        "@gtest//:gtest_main",
    ],
)

cc_binary(
    name = "sketching_main",
    srcs = ["sketching_main.cpp"],
    deps = [
        ":bloom_filter",
        ":count_min_sketch",
        "@absl//absl/strings:str_format",
        "@glog",
    ],
)

cc_test(
    name = "interval_tree_test",
    srcs = ["interval_tree_test.cpp"],
    deps = [
        ":interval_tree",
        "@gtest//:gtest_main",
    ],
)
