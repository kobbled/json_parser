load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "jsonParser",
    srcs = ["jsonparser.c"],
    hdrs = glob([
        "*.h",
        "jsonparser/*.h"
    ]),
    copts = ["-Ilib/jsonparser", "-Ilib"],
    visibility = ["//visibility:public"],
)