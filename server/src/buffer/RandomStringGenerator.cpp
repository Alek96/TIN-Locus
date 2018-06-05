#include "RandomStringGenerator.h"
#include <string>
#include <random>

std::vector<unsigned char> randomStringGenerator(int max_length) {
    std::string possible_characters = "0123456789"
                                      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                      "abcdefghijklmnopqrstuvwxyz";
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_int_distribution<> dist(0, possible_characters.size() - 1);
    std::string ret = "";
    for (int i = 0; i < max_length; i++) {
        int random_index = dist(engine); //get index between 0 and possible_characters.size()-1
        ret += possible_characters[random_index];
    }
    return std::vector<unsigned char>(ret.begin(), ret.end());
}
