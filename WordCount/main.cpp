#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <unordered_map>
#include <string>
#include <assert.h>

class WordCounterContainer {
public:
    using WordType = std::string;
    
    void AddWord(const WordType& word) {
        auto it = wordCounters.find(word);
        if (it != wordCounters.end()) {
            // remove from index
            auto& words = sortedWordCounters[it->second];
            words.erase(word);
            if (words.empty()) {
                sortedWordCounters.erase(it->second);
            }
            // increment counter
            ++it->second;
            // insert to index
            sortedWordCounters[it->second].insert(word);
        } else {
            // insert to container and update index
            wordCounters[word] = 1;
            sortedWordCounters[1].insert(word);
        }
    }
    
private:
    friend std::ostream& operator << (std::ostream& ostream, const WordCounterContainer& container);
    
    using CounterType = long;
    using WordCounters = std::unordered_map<WordType, CounterType>;
    // container
    WordCounters wordCounters;
    using SortedWordCounters = std::map<CounterType, std::set<WordType>, std::greater<CounterType>>;
    // index
    SortedWordCounters sortedWordCounters;
};

std::ostream& operator << (std::ostream& ostream, const WordCounterContainer& container) {
    for (const auto& counter : container.sortedWordCounters) {
        for (const auto& word : counter.second) {
            ostream << counter.first << " ";
            ostream << word << std::endl;
        }
    }
    return ostream;
}

static WordCounterContainer calcWordCounters(std::istream& infile) {
    WordCounterContainer wordCounterContainer;
    std::string line;
    while (std::getline(infile, line)) {
        auto ScanSpaces = [&line](auto start, auto end) {
            for (auto pos = start; pos < end; ++pos) {
                if (iswalpha(line[pos]))
                    return pos;
            }
            return std::string::npos;
        };
        auto ScanWord = [&line](auto start, auto end, auto& word) {
            word.clear();
            for (auto pos = start; pos < end; ++pos) {
                if (iswalpha(line[pos])) {
                    word += towlower(line[pos]);
                } else {
                    return pos;
                }
            }
            return std::string::npos;
        };
        
        std::string::size_type startWordPos = 0;
        auto size = line.size();
        std::string word;
        while (true) {
            auto endWordPos = ScanWord(startWordPos, size, word);
            if (endWordPos == std::string::npos) {
                break;
            }
            if (!word.empty()) {
                wordCounterContainer.AddWord(word);
                word.clear();
            }
            startWordPos = ScanSpaces(endWordPos, size);
            if (startWordPos == std::string::npos) {
                break;
            }
        }
        if (!word.empty()) {
            wordCounterContainer.AddWord(word);
        }
    }
    return wordCounterContainer;
}

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

void test() {
    for (std::size_t i = 0; i < sizeof(texts) / sizeof(texts[0]); ++i) {
        auto text = texts[i];
        std::stringstream inStream(text);
        auto wordCounters = calcWordCounters(inStream);
        std::stringstream outStream;
        outStream << wordCounters;
        auto output = outStream.str();
        assert(output == outputs[i]);
    }
}

int main(int argc, char* argv[])
{
    //test();
    static const char* usage = R"(Word counting utility.
Splits infile by spaces and any symbol except [a-zA-Z]). Counts words, outputs the counters to outfile in frequency descending order.
Usage: ./WordCount <infile> <outfile>.
Example: ./WordCount huge.txt out.txt)";

    if (argc == 1) {
        std::cerr << usage << std::endl;
        return 1;
    }
    if (argc == 2) {
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

    auto wordCounters = calcWordCounters(infile);
    outfile << wordCounters;
}
