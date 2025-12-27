# GCC C++26 Compile-Time Reflection experiments(based on P2996)

This repository contains small, focused examples exploring the experimental C++26 compile-time reflection support in GCC based on proposal P2996(using `<meta>`, `^^T`, `template for`..).

The goal is to demonstrate how reflection can replace common boilerplate and enable new patterns, such as:

- `enum_to_string`: map enum values to identifiers at compile time.
- `aggregate_formatter`: iterate over aggregate members to format/print structs without manually writing field-by-field code.
- `optimize_type_layout`: generate a new aggregate type with members reordered (e.g., for padding/alignment improvements).
- `structure_of_arrays`: transform an AoS struct into an SoA layout type by turning each member into an array.
- `serialize_aggregate`: reflection-driven binary serialization/deserialization for aggregates composed of arithmetic/enum/array/aggregate members.

### Prerequisites

- GCC experimental reflection branch build from source. Link: https://forge.sourceware.org/marek/gcc.git

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

### Sample output

`./bin/example_enum_to_string`

```text
Color::Red -> Red
Color::Blue -> Blue

Planners::RRT -> RRT
Planners::HYBRID_ASTAR -> HYBRID_ASTAR

RobotState::Idle -> Idle
RobotState::Moving -> Moving
RobotState::EmergencyStop -> EmergencyStop
```

`./bin/example_aggregate_formatter`

```text
Printing Point struct:
  Point{ x=1.5, y=2.5, z=3.5 }

Printing Quaternion struct:
  Quaternion{ w=0.707, x=0, y=0.707, z=0 }

Printing Pose struct:
  Pose{ position = Point{ x = 1, y = 2, z = 3 }, orientation = Quaternion{ w = 1, x = 0, y = 0, z = 0 } }

Printing RobotState struct:
  RobotState{ id = 42, pose = Pose{ position = Point{ x = 5, y = 6, z = 7 }, orientation = Quaternion{ w = 0.5, x = 0.5, y = 0.5, z = 0.5 } }, velocity = 1.5, active = 1 }
```

`./bin/example_optimize_type_layout`

```text
BadLayout original:
  sizeof: 48 bytes
  alignof: 8

OptimizedBadLayout using optimize_type_layout:
  sizeof: 24 bytes
  alignof: 8

Space saved: 24 bytes
```

`./bin/example_structure_of_arrays`

```text
Accessing SoA data:
  All X values: 0 1 2 3 4 5 6 7 8 9
  All Y values: 0 2 4 6 8 10 12 14 16 18
  All Z values: 0 3 6 9 12 15 18 21 24 27
```

`./bin/example_serialize_aggregate`

```text
  serialized_size: 161 bytes, written: 161 bytes
  read: 161 bytes
  roundtrip equal: true

  serialized_size: 36 bytes, written: 36 bytes
  read: 36 bytes
  roundtrip equal: true
```


### Notes

- These examples target an experimental compiler implementation and may break as the reflection design and GCC implementation evolve.
- The `serialize_aggregate` example is meant as a reflection-driven technique demo; it is not intended to define a stable, production code.

### Thanks

Thanks to the GCC implementors and contributors working on the compile-time reflection effort.

### Possible Future Work

- Reflection-driven aggregate to JSON string conversion.
- Compile-time SIMD dispatch based on reflected type/layout information.
- Reflection-driven algorithmic differentiation for aggregates.
