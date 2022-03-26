# Common include for building the interpreter

OBJ = obj/$(ARCH)/machine/interpreter.o obj/$(ARCH)/host/standard_test_host_mem.o obj/$(ARCH)/host/standard_test_host_io.o obj/$(ARCH)/host/standard_test_host_vector_math.o obj/$(ARCH)/host/standard_test_host_display.o obj/$(ARCH)/host/standard_test_host_display_context_$(USE_DISP_CTX).o obj/$(ARCH)/host/standard_test_host_audio.o obj/$(ARCH)/host/standard_test_host_audio_output_$(USE_AUDIO_OUT).o obj/$(ARCH)/main.o obj/$(ARCH)/host/definition.o obj/$(ARCH)/host/standard_test_host_def.o obj/$(ARCH)/host/runtime.o obj/$(ARCH)/loader/symbol.o obj/$(ARCH)/loader/binary.o obj/$(ARCH)/loader/executable.o obj/$(ARCH)/misc/version.o

$(BIN): $(OBJ) Makefile.$(MEXT)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(BIN) $(LIBS)

obj/$(ARCH)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	$(RM) $(OBJ) $(BIN)

prepare:
	mkdir -p obj/$(ARCH)/host obj/$(ARCH)/loader obj/$(ARCH)/machine obj/$(ARCH)/misc
