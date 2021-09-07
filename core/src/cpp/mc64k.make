# Common include for building the interpreter

OBJ = obj/$(ARCH)/main.o obj/$(ARCH)/standard_test_host.o obj/$(ARCH)/host.o obj/$(ARCH)/interpreter.o obj/$(ARCH)/interpreter_ea.o obj/$(ARCH)/interpreter_run.o obj/$(ARCH)/loader.o obj/$(ARCH)/misc.o

$(BIN): $(OBJ) Makefile.$(MEXT)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(BIN)

obj/$(ARCH)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<
