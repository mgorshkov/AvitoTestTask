#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <assert.h>
#include <memory>

#include "TimeLogger.hpp"
#include "WordCollector.hpp"
#include "LineProcessor.hpp"
#include "ThreadedLineProcessor.hpp"

// test
static const char* texts[] = {R"(The time has come, the Walrus said, to talk of many things...)",
    R"(The THE the time has come, the the Walrus said, to talk of many things...)"
};

static const char* outputs[] = {R"(2 the
1 come
1 has
1 many
1 of
1 said
1 talk
1 things
1 time
1 to
1 walrus
)",
R"(5 the
1 come
1 has
1 many
1 of
1 said
1 talk
1 things
1 time
1 to
1 walrus
)"};

static void run(std::istream& infile, std::ostream& outfile, bool threaded) {
    TimeLogger timeLogger;
    
    WordCollector wordCounters;
    {
        std::unique_ptr<ILineProcessor> lineProcessor;
        if (threaded) {
            lineProcessor = std::make_unique<ThreadedLineProcessor>(&wordCounters);
        } else {
            lineProcessor = std::make_unique<LineProcessor>(&wordCounters);
        }
        static const std::size_t bufferSize = 1 << 20;
        char buffer[bufferSize * 2];

        while (infile) {
            infile.read(buffer, bufferSize);
            auto sizeRead = infile.gcount();
            if (infile) { // read more until a line separator or eof
                infile.getline(buffer + bufferSize, bufferSize);
                if (infile.fail()) { // line separator is not found
                    throw std::runtime_error("Line is too long");
                }
                sizeRead += infile.gcount();
            }
            lineProcessor->processBuf(buffer, sizeRead);
        }
    }
    
    outfile << wordCounters;
}

static void test() {
    for (std::size_t i = 0; i < sizeof(texts) / sizeof(texts[0]); ++i) {
        auto text = texts[i];
        std::stringstream inStream(text);
        std::stringstream outStream;
        run(inStream, outStream, false);
        auto output = outStream.str();
        assert(output == outputs[i]);
    }
}

static void test_threaded() {
    for (std::size_t i = 0; i < sizeof(texts) / sizeof(texts[0]); ++i) {
        auto text = texts[i];
        std::stringstream inStream(text);
        std::stringstream outStream;
        run(inStream, outStream, true);
        auto output = outStream.str();
        assert(output == outputs[i]);
    }
}

int main(int argc, char* argv[])
{
    //test();
    //test_threaded();
    static const char* usage = R"(Word counting utility.
Splits infile by spaces and any symbol except [a-zA-Z]). Counts words, outputs the counters to outfile in frequency descending order.
Usage: ./WordCount <infile> <outfile> <threaded> (yes|no).
Example: ./WordCount huge.txt out.txt yes)";

    if (argc == 1) {
        std::cerr << usage << std::endl;
        return 1;
    }
    if (argc < 4) {
        std::cerr << "Incorrect number of arguments" << std::endl;
        std::cerr << usage << std::endl;
        return 1;
    }
    
    char* infilename = argv[1];
    char* outfilename = argv[2];
    
    std::ifstream infile(infilename);
    if (!infile.is_open()){
        std::cerr << "Can't open file for reading: " << infilename << std::endl;
        return 1;
    }

    std::ofstream outfile(outfilename);
    if (!outfile.is_open()){
        std::cerr << "Can't open file for writing: " << outfilename << std::endl;
        return 1;
    }
    std::string threaded = argv[3];
    
    run(infile, outfile, threaded == "yes");
    return 0;
}
