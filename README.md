# GCC C++26 Compile-Time Reflection experiments(based on P2996)

This repository contains small, focused examples exploring the experimental C++26 compile-time reflection support in GCC based on proposal P2996(using `<meta>`, `^^T`, `template for`..).

The goal is to demonstrate how reflection can replace common boilerplate and enable new patterns, such as:

- `enum_to_string`: map enum values to identifiers at compile time.
- `aggregate_formatter`: iterate over aggregate members to format/print structs without manually writing field-by-field code.
- `optimize_type_layout`: generate a new aggregate type with members reordered (e.g., for padding/alignment improvements).
- `structure_of_arrays`: transform an AoS struct into an SoA layout type by turning each member into an array.
- `serialize_aggregate`: reflection-driven binary serialization/deserialization for aggregates composed of arithmetic/enum/array/aggregate members.

### Prerequisites

- GCC experimental reflection branch built from source. Link: https://forge.sourceware.org/marek/gcc.git

### Build and run

This repo’s `Makefile` assumes the reflection-enabled compiler is available at:

`/opt/gcc_reflection/bin/g++`

To build everything:

```bash
make -j
```

To run specific examples:

```bash
./bin/example_enum_to_string
./bin/example_aggregate_formatter
./bin/example_optimize_type_layout
./bin/example_structure_of_arrays
./bin/example_serialize_aggregate
```


### Notes

- These examples target an experimental compiler implementation and may break as the reflection design and GCC implementation evolve.

### Thanks

Thanks to the GCC implementors and contributors working on the compile-time reflection effort.

### Possible Future Work

- Reflection-driven aggregate to JSON string conversion.
- Compile-time SIMD dispatch based on reflected type/layout information.
- Reflection-driven algorithmic differentiation for aggregates.
