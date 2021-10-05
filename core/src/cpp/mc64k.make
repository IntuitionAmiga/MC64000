# Common include for building the interpreter

OBJ = obj/$(ARCH)/main.o obj/$(ARCH)/host/definition.o obj/$(ARCH)/host/standard_test_host_mem.o obj/$(ARCH)/host/standard_test_host_io.o obj/$(ARCH)/host/standard_test_host_def.o obj/$(ARCH)/host/runtime.o obj/$(ARCH)/loader/symbol.o obj/$(ARCH)/loader/binary.o obj/$(ARCH)/loader/executable.o obj/$(ARCH)/machine/interpreter.o obj/$(ARCH)/machine/interpreter_ea.o obj/$(ARCH)/machine/interpreter_run.o obj/$(ARCH)/misc/version.o

$(BIN): $(OBJ) Makefile.$(MEXT)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(BIN)

obj/$(ARCH)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	$(RM) $(OBJ) $(BIN)

prepare:
	mkdir -p obj/$(ARCH)/host obj/$(ARCH)/loader obj/$(ARCH)/machine obj/$(ARCH)/misc
