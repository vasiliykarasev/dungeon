load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# Standard useful libraries.
http_archive(
    name = "gtest",
    build_file = "gtest.BUILD",
    sha256 = "b58cb7547a28b2c718d1e38aee18a3659c9e3ff52440297e965f5edffe34b6d0",
    strip_prefix = "googletest-release-1.7.0",
    url = "https://github.com/google/googletest/archive/release-1.7.0.zip",
)

http_archive(
    name = "glog",
    strip_prefix = "glog-4cc89c9e2b452db579397887c37f302fb28f6ca1",
    urls = [
        "https://github.com/google/glog/archive/4cc89c9e2b452db579397887c37f302fb28f6ca1.tar.gz",
    ],
)

# Eigen
http_archive(
    name = "eigen",
    build_file = "eigen.BUILD",
    sha256 = "3a66f9bfce85aff39bc255d5a341f87336ec6f5911e8d816dd4a3fdc500f8acf",
    strip_prefix = "eigen-eigen-c5e90d9e764e",
    url = "https://bitbucket.org/eigen/eigen/get/c5e90d9.tar.gz",
)

git_repository(
    name = "com_github_gflags_gflags",
    commit = "28f50e0fed19872e0fd50dd23ce2ee8cd759338e",
    remote = "https://github.com/gflags/gflags.git",
)

bind(
    name = "gflags",
    actual = "@com_github_gflags_gflags//:gflags",
)

git_repository(
    name = "com_github_benchmark_benchmark",
    commit = "7d97a057e16597e8020d0aca110480fe82c9ca67",
    remote = "https://github.com/google/benchmark.git",
)

bind(
    name = "benchmark",
    actual = "@com_github_benchmark_benchmark//:benchmark",
)

git_repository(
    name = "absl",
    commit = "66f9becbb98ecc083f4db349b4b1e0ca9de93b15",
    remote = "https://github.com/abseil/abseil-cpp.git",
)

# Protobuf.
http_archive(
    name = "rules_proto",
    sha256 = "602e7161d9195e50246177e7c55b2f39950a9cf7366f74ed5f22fd45750cd208",
    strip_prefix = "rules_proto-97d8af4dc474595af3900dd85cb3a29ad28cc313",
    urls = [
        "https://mirror.bazel.build/github.com/bazelbuild/rules_proto/archive/97d8af4dc474595af3900dd85cb3a29ad28cc313.tar.gz",
        "https://github.com/bazelbuild/rules_proto/archive/97d8af4dc474595af3900dd85cb3a29ad28cc313.tar.gz",
    ],
)

http_archive(
    name = "rules_cc",
    sha256 = "35f2fb4ea0b3e61ad64a369de284e4fbbdcdba71836a5555abb5e194cf119509",
    strip_prefix = "rules_cc-624b5d59dfb45672d4239422fa1e3de1822ee110",
    urls = [
        "https://mirror.bazel.build/github.com/bazelbuild/rules_cc/archive/624b5d59dfb45672d4239422fa1e3de1822ee110.tar.gz",
        "https://github.com/bazelbuild/rules_cc/archive/624b5d59dfb45672d4239422fa1e3de1822ee110.tar.gz",
    ],
)

http_archive(
    name = "rules_java",
    sha256 = "ccf00372878d141f7d5568cedc4c42ad4811ba367ea3e26bc7c43445bbc52895",
    strip_prefix = "rules_java-d7bf804c8731edd232cb061cb2a9fe003a85d8ee",
    urls = [
        "https://mirror.bazel.build/github.com/bazelbuild/rules_java/archive/d7bf804c8731edd232cb061cb2a9fe003a85d8ee.tar.gz",
        "https://github.com/bazelbuild/rules_java/archive/d7bf804c8731edd232cb061cb2a9fe003a85d8ee.tar.gz",
    ],
)

load("@rules_cc//cc:repositories.bzl", "rules_cc_dependencies")

rules_cc_dependencies()

load("@rules_proto//proto:repositories.bzl", "rules_proto_dependencies", "rules_proto_toolchains")

rules_proto_dependencies()

rules_proto_toolchains()

# proto_library, cc_proto_library, and java_proto_library rules implicitly
# depend on @com_google_protobuf for protoc and proto runtimes.
# This statement defines the @com_google_protobuf repo.
http_archive(
    name = "com_google_protobuf",
    sha256 = "c90d9e13564c0af85fd2912545ee47b57deded6e5a97de80395b6d2d9be64854",
    strip_prefix = "protobuf-3.9.1",
    urls = ["https://github.com/google/protobuf/archive/v3.9.1.zip"],
)

bind(
    name = "protobuf",
    actual = "@com_google_protobuf//:protobuf",
)

new_local_repository(
    name = "llvm",
    build_file = "external/llvm.BUILD",
    path = "/usr/lib/llvm-10/",
)

# OpenCV is a local repository; not packaged here.
# On ubuntu, it can be installed as:
#   sudo apt-get install -y libopencv-dev
new_local_repository(
    name = "opencv",
    build_file = "external/opencv.BUILD",
    path = "/usr/",
)

# QT5 is a local repository; not packaged here.
# On ubuntu, it can be installed as:
#   TODO(vasiliy)
new_local_repository(
    name = "qt5",
    build_file = "external/qt5.BUILD",
    path = "/usr/",
)

# OpenGL is a local repository; not packaged here.
new_local_repository(
    name = "opengl",
    build_file = "external/opengl.BUILD",
    path = "/usr/",
)

# QGLViewer is a local repository; not packaged here.
# On ubuntu, it can be installed as:
#   sudo apt-get install -y libqglviewer-dev-qt5
new_local_repository(
    name = "qglviewer",
    build_file = "external/qglviewer.BUILD",
    path = "/usr/",
)
