#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <unordered_map>
#include <algorithm>

using CounterType = int;
using WordCounters = std::unordered_map<std::string, CounterType>;
using WordCountersSorted = std::map<CounterType, std::set<std::string>, std::greater<CounterType>>;

static WordCounters calcWordCounters(std::istream& infile) {
    WordCounters wordCounters;
    std::string line;
    while (std::getline(infile, line)) {
        auto FindNextWordOrSep = [&line](auto start, bool isWord) {
            auto end = line.length();
            for (auto pos = start; pos < end; ++pos) {
                if (isWord == isalpha(line[pos]))
                    return pos;
            }
            return std::string::npos;
        };
        auto FindNextSep = [FindNextWordOrSep](auto start) {
            return FindNextWordOrSep(start, false);
        };
        auto FindNextWord = [FindNextWordOrSep](auto start) {
            return FindNextWordOrSep(start, true);
        };
        auto AddWord = [&wordCounters, &line](auto start, auto size) {
            auto word = line.substr(start, size);
            word[0] = tolower(word[0]);
            ++wordCounters[word];
        };
        
        std::string::size_type zero = 0;
        auto start = FindNextWord(zero);
        if (start == std::string::npos)
            continue;
        auto end = FindNextSep(start + 1);
        while (end != std::string::npos) {
            AddWord(start, end - start);

            start = FindNextWord(end + 1);
            if (start == std::string::npos)
                break;
            end = FindNextSep(start + 1);
        }
        if (start != std::string::npos)
            AddWord(start, std::string::npos);
    }
    return wordCounters;
}

static WordCountersSorted sortWordCounters(const WordCounters& wordCounters) {
    WordCountersSorted wordCountersSorted;
    for (auto counter : wordCounters)
        wordCountersSorted[counter.second].insert(counter.first);
    
    return wordCountersSorted;
}

static void outputWordCounters(const WordCountersSorted& wordCounters, std::ostream& outfile) {
    for (auto counter : wordCounters) {
        for (const auto& word : counter.second) {
            outfile << counter.first << " ";
            outfile << word << std::endl;
        }
    }
}

int main(int argc, char* argv[])
{
    static const char* usage = R"(Word counting utility.
Splits infile by spaces (any symbol but [a-zA-Z]). Counts words, outputs the counters to outfile.
Usage: freq <infile> <outfile>.)";

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
    auto sortedWordCounters = sortWordCounters(wordCounters);
    outputWordCounters(sortedWordCounters, outfile);
}
