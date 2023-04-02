# JSON Parser

This is a JSON Parser written for bare metal C. 

> [!Note]
> This repository is for personal development, and not for external use.

## Usage

Compile project with

```
 bazel run src:jsonParser
```

Build Library

```
 bazel build lib:ParserLib
```

Just compile and run tests with

```
bazel test --test_output=all tests/test_parser
```

## Dependencies

- [Stumpless](https://github.com/goatshriek/stumpless) : Install on your machine (Manually)