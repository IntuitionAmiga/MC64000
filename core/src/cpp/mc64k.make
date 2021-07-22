# Common include for building the interpreter

OBJ = obj/$(ARCH)/main.o obj/$(ARCH)/interpreter.o obj/$(ARCH)/interpreter_ea.o obj/$(ARCH)/interpreter_run.o obj/$(ARCH)/loader.o

$(BIN): $(OBJ) Makefile.$(MEXT)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(BIN)

obj/$(ARCH)/main.o: main.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

obj/$(ARCH)/interpreter.o: interpreter.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

obj/$(ARCH)/interpreter_ea.o: interpreter_ea.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

obj/$(ARCH)/interpreter_run.o: interpreter_run.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

obj/$(ARCH)/loader.o: loader.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

