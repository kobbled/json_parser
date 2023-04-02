load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

git_repository(
    name = "com_google_googletest",
    #// Dec 27, 2021
    commit = "b796f7d44681514f58a683a3a71ff17c94edb0c1",
    remote = "https://github.com/google/googletest",
    # tag = "release-1.13.0",
)

new_local_repository(
    name = "stumpless",
    path = "/usr/local/lib",
    build_file = "deps/stumpless.BUILD",
)