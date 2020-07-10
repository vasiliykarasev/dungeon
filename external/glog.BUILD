cc_library(
    name = "glog",
    srcs = [
        "upstream/src/demangle.cc",
        "upstream/src/logging.cc",
        "upstream/src/raw_logging.cc",
        "upstream/src/signalhandler.cc",
        "upstream/src/symbolize.cc",
        "upstream/src/utilities.cc",
        "upstream/src/vlog_is_on.cc",
        # Private headers
        "glog_build/k8/src/config.h",
        "upstream/src/base/commandlineflags.h",
        "upstream/src/base/googleinit.h",
        "upstream/src/base/mutex.h",
        "upstream/src/glog/log_severity.h",
        "upstream/src/stacktrace.h",
        "upstream/src/stacktrace_generic-inl.h",
        "upstream/src/stacktrace_libunwind-inl.h",
        "upstream/src/stacktrace_powerpc-inl.h",
        "upstream/src/stacktrace_x86-inl.h",
        "upstream/src/stacktrace_x86_64-inl.h",
    ],
    hdrs = [
        "glog_build/k8/include/glog/logging.h",
        "glog_build/k8/include/glog/raw_logging.h",
        "glog_build/k8/include/glog/stl_logging.h",
        "glog_build/k8/include/glog/vlog_is_on.h",
        "upstream/src/demangle.h",
        "upstream/src/mock-log.h",
        "upstream/src/symbolize.h",
        "upstream/src/utilities.h",
    ],
    copts = [
        "-DNO_FRAME_POINTER",
        "-Ithird_party/glog/glog_build/k8/src",
    ],
    includes = [
        "glog_build/k8/include",
        "upstream/src",
    ],
    linkopts = [
        "-pthread",
    ],
    visibility = ["//visibility:public"],
    #deps = [
    #    "//third_party/gflags",
    #    "//third_party/libunwind:libunwind-k8",
    #],
)