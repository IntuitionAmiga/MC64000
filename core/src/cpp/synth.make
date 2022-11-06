# Common include for building the synth engine (isolated)

OBJ = obj/$(ARCH)/synth/note.o obj/$(ARCH)/synth/packet.o obj/$(ARCH)/synth/waveform.o obj/$(ARCH)/synth/oscillator.o obj/$(ARCH)/synth/stream_operator.o obj/$(ARCH)/host/memory.o obj/$(ARCH)/host/standard_test_host_audio_output_$(USE_AUDIO_OUT).o obj/$(ARCH)/synthtest.o

$(BIN): $(OBJ) Makefile.synth.$(MEXT)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(BIN) $(LIBS)

obj/$(ARCH)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	$(RM) $(OBJ) $(BIN)

prepare:
	mkdir -p obj/$(ARCH)/synth
