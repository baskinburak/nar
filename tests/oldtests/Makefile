all: compressor

compressor: compressor.cpp ../nar/narnode/FileKeeper/FileKeeper.cpp ../nar/narnode/FileKeeper/FileCompressor.cpp
	g++ -o a compressor.cpp ../nar/narnode/FileKeeper/FileKeeper.cpp ../nar/narnode/FileKeeper/FileCompressor.cpp -lz

clean:
	rm -rf a
