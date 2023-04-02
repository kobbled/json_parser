load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "test_parser",
    srcs = ["tests.cc"],
    copts = ["-Ilib"],
    deps = [
        "//lib:jsonParser",
        "@com_google_googletest//:gtest_main",
    ],
)