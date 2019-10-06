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
    copts = ["-Iexternal/gtest/include"],
    deps = [
        ":bloom_filter",
        "@gtest//:main",
    ],
)

cc_test(
    name = "trie_test",
    srcs = ["trie_test.cpp"],
    copts = ["-Iexternal/gtest/include"],
    deps = [
        ":trie",
        "@gtest//:main",
    ],
)

cc_test(
    name = "count_min_sketch_test",
    srcs = ["count_min_sketch_test.cpp"],
    copts = ["-Iexternal/gtest/include"],
    deps = [
        ":count_min_sketch",
        "@gtest//:main",
    ],
)

cc_test(
    name = "lru_cache_test",
    srcs = ["lru_cache_test.cpp"],
    copts = [
        "-Iexternal/gtest/include",
    ],
    deps = [
        ":lru_cache",
        "@gtest//:main",
    ],
)

cc_binary(
    name = "sketching_main",
    srcs = ["sketching_main.cpp"],
    deps = [
        ":bloom_filter",
        ":count_min_sketch",
        "@com_google_absl//absl/strings:str_format",
        "@glog",
    ],
)
