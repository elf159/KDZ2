#include <chrono>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <vector>

const std::vector<std::string> DNA = {"A", "C", "T", "G"};
const std::vector<std::string> binary = {"0", "1"};

int getRandom(std::size_t mod) {
    int random = rand() % mod;
    return random;
}

std::vector<std::string> texts(){
    std::string binaryS10_000;
    std::string binaryS100_000;

    std::string DNAS10_000;
    std::string DNAS100_000;
    int x;

    for (int i = 0; i < 10000; i++) {
        x = getRandom(2);
        binaryS10_000 += binary[x];
    }

    for (int i = 0; i < 100000; i++) {
        x = getRandom(2);
        binaryS100_000 += binary[x];
    }

    for (int i = 0; i < 10000; i++) {
        x = getRandom(4);
        DNAS10_000 += DNA[x];
    }

    for (int i = 0; i < 100000; i++) {
        x = getRandom(4);
        DNAS100_000 += DNA[x];
    }

    std::vector<std::string> vecTexts = {binaryS10_000, binaryS100_000, DNAS10_000, DNAS100_000};
    return vecTexts;
}

std::vector<std::string> samples(const std::string& s)
{
    std::vector<std::string> substrings;
    int rd;
    for (int i = 100; i <= 3000; i += 100) {
        rd = rand() % 6000;
        substrings.push_back(s.substr(rd, i));
    }
    return substrings;
}

void naiveSearch(int alphabetSize, int n, std::string text, std::vector<std::string> patterns) {
    for (auto pattern : patterns) {
        std::cout << alphabetSize << ";" << n << ";naiveSearch;" << pattern.size() << ";";
        int m = pattern.length();
        auto start = std::chrono::steady_clock::now();
        int count = 0;
        for (int i = 0; i <= n - m; i++) {
            bool found = true;
            for (int j = 0; j < m; j++) {
                if (text[i + j] != pattern[j]) {
                    found = false;
                    break;
                }
            }
            if (found) {
                count++;
            }
        }
        auto end = std::chrono::steady_clock::now();
        auto diff = end - start;
        std::cout << static_cast <int>(std::chrono::duration <double, std::micro> (diff).count()) << ";" << std::endl;
    }
    std::cout << std::endl;
}

void kmpSearch(int alphabetSize, int n, std::string text, std::vector<std::string> patterns) {
    for (auto pattern : patterns) {
        std::cout << alphabetSize << ";" << n << ";kmpSearch;" << pattern.size() << ";";
        auto start = std::chrono::steady_clock::now();
        std::string str = text + "%" + pattern;
        int n = static_cast<int>(str.size());
        std::vector<int> br(n);
        for (int i = 1; i < n; ++i) {
            int j = br[i - 1];
            while (j > 0 && str[i] != str[j]) {
                j = br[j - 1];
            }
            if (str[i] == str[j]) {
                j++;
            }
            br[i] = j;
        }
        std::vector<int> brs(n);
        for (int i = 1; i < n; ++i) {
            brs[i] = (str[br[i] + 1] != str[i]) ? br[i] : brs[br[i]];
        }
        size_t g = 0, l = 0;
        std::vector<size_t> result;
        while (g < pattern.size()) {
            if (pattern[g] == text[l]) {
                g++;
                l++;
                if (l == text.size()) {
                    result.push_back(g - l);
                }
            } else {
                if (l == 0) {
                    g++;
                } else {
                    l = brs[l - 1];
                }
            }
        }
        auto end = std::chrono::steady_clock::now();
        auto time = end - start;
        std::cout << static_cast <int>(std::chrono::duration <double, std::micro> (time).count()) << ";" << std::endl;
    }
}

void kmpBetterSearch(int alphabetSize, int n, std::string text, std::vector<std::string> patterns) {
    for (auto pattern : patterns) {
        std::cout << alphabetSize << ";" << n << ";kmpBetterSearch;" << pattern.size() << ";";
        auto start = std::chrono::steady_clock::now();
        std::string concatenated_str = text + "%" + pattern;
        int concatenated_str_size = static_cast<int>(concatenated_str.size());
        std::vector<int> border_array(concatenated_str_size);
        for (int i = 1; i < concatenated_str_size; ++i) {
            int border = border_array[i - 1];
            while (border > 0 && concatenated_str[i] != concatenated_str[border]) {
                border = border_array[border - 1];
            }
            if (concatenated_str[i] == concatenated_str[border]) {
                border++;
            }
            border_array[i] = border;
        }
        std::vector<int> border_suffix_array(concatenated_str_size);
        for (int i = 1; i < concatenated_str_size; ++i) {
            if (concatenated_str[border_array[i] + 1] != concatenated_str[i]) {
                border_suffix_array[i] = border_array[i];
            } else {
                border_suffix_array[i] = border_suffix_array[border_array[i]];
            }
        }
        size_t g = 0;
        size_t l = 0;
        std::vector<size_t> matches;
        while (g < pattern.size()) {
            if (pattern[g] == text[l]) {
                g++;
                l++;
                if (l == text.size()) {
                    matches.emplace_back(g - l);
                }
            } else {
                if (l == 0) {
                    g++;
                } else {
                    l = border_suffix_array[l - 1];
                }
            }
        }
        auto end = std::chrono::steady_clock::now();
        auto time = end - start;
        std::cout << static_cast <int>(std::chrono::duration <double, std::micro> (time).count()) << ";" << std::endl;
    }
}

int main() {
    std::vector<std::string> vecTexts = texts();

    std::vector<std::string> vecSamplesS10 = samples(vecTexts[0]);
    std::vector<std::string> vecSamplesS100 = samples(vecTexts[1]);
    std::vector<std::string> vecSamplesDNA10 = samples(vecTexts[2]);
    std::vector<std::string> vecSamplesDNA100 = samples(vecTexts[3]);

    naiveSearch(2, 10000, vecTexts[0], vecSamplesS10);
    std::cout << "----- -----" << std::endl;
    kmpSearch(2, 10000, vecTexts[0], vecSamplesS10);
    std::cout << "----- -----" << std::endl;
    kmpBetterSearch(2,10000, vecTexts[0], vecSamplesS10);
    std::cout << "----- -----" << std::endl;

    naiveSearch(2,100000, vecTexts[1], vecSamplesS100);
    std::cout << "----- -----" << std::endl;
    kmpSearch(2,100000, vecTexts[1], vecSamplesS100);
    std::cout << "----- -----" << std::endl;
    kmpBetterSearch(2,100000, vecTexts[1], vecSamplesS100);
    std::cout << "----- -----" << std::endl;

    naiveSearch(4,10000, vecTexts[2], vecSamplesDNA10);
    std::cout << "----- -----" << std::endl;
    kmpSearch(4,10000, vecTexts[2], vecSamplesDNA10);
    std::cout << "----- -----" << std::endl;
    kmpBetterSearch(4,10000, vecTexts[2], vecSamplesDNA10);
    std::cout << "----- -----" << std::endl;

    naiveSearch(4,100000, vecTexts[3], vecSamplesDNA100);
    std::cout << "----- -----" << std::endl;
    kmpSearch(4,100000, vecTexts[3], vecSamplesDNA100);
    std::cout << "----- -----" << std::endl;
    kmpBetterSearch(4,100000, vecTexts[3], vecSamplesDNA100);
    std::cout << "----- -----" << std::endl;

    return 0;
}