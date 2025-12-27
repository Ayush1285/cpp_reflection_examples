CXX = /opt/gcc_reflection/bin/g++
CXXFLAGS = -std=c++26 -freflection -I$(INCLUDE_DIR)

INCLUDE_DIR = include
SRC_DIR = src
BIN_DIR = bin

EXAMPLES = $(BIN_DIR)/example_enum_to_string \
           $(BIN_DIR)/example_optimize_type_layout \
           $(BIN_DIR)/example_aggregate_formatter \
           $(BIN_DIR)/example_structure_of_arrays \
           $(BIN_DIR)/example_serialize_aggregate

all: $(BIN_DIR) $(EXAMPLES)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BIN_DIR)/example_enum_to_string: $(SRC_DIR)/example_enum_to_string.cpp $(INCLUDE_DIR)/enum_to_string.hpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(BIN_DIR)/example_optimize_type_layout: $(SRC_DIR)/example_optimize_type_layout.cpp $(INCLUDE_DIR)/optimize_type_layout.hpp $(INCLUDE_DIR)/ctx_definition.hpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(BIN_DIR)/example_aggregate_formatter: $(SRC_DIR)/example_aggregate_formatter.cpp $(INCLUDE_DIR)/aggregate_formatter.hpp $(INCLUDE_DIR)/ctx_definition.hpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(BIN_DIR)/example_structure_of_arrays: $(SRC_DIR)/example_structure_of_arrays.cpp $(INCLUDE_DIR)/structure_of_arrays.hpp $(INCLUDE_DIR)/ctx_definition.hpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(BIN_DIR)/example_serialize_aggregate: $(SRC_DIR)/example_serialize_aggregate.cpp $(INCLUDE_DIR)/serialize_aggregate.hpp $(INCLUDE_DIR)/ctx_definition.hpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -rf $(BIN_DIR)

.PHONY: all clean
