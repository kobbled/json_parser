load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "main",
    srcs = ["main.c"],
    deps = ["//lib:jsonParser"],
    copts = ["-I lib"]
)