#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>


struct Rating {
    int userId;
    int movieId;
    float rating;
    int timestamp;
};

std::vector<std::string> split(const std::string &s, const std::string &delimiter) {
    std::vector<std::string> tokens;
    size_t pos = 0, lastPos = 0;
    while ((pos = s.find(delimiter, lastPos)) != std::string::npos) {
        tokens.push_back(s.substr(lastPos, pos - lastPos));
        lastPos = pos + delimiter.length();
    }
    tokens.push_back(s.substr(lastPos));
    return tokens;
}

float averageMovieRatingDistance(int user1Id, int user2Id, std::unordered_map<int, std::vector<Rating> > &ratings) {
    std::vector<Rating>::iterator u1Iterator = ratings[user1Id].begin();
    std::vector<Rating>::iterator u2Iterator = ratings[user2Id].begin();

    float sum = 0;
    int count = 0;

    while (u1Iterator != ratings[user1Id].end() && u2Iterator != ratings[user2Id].end()) {
        if (u1Iterator->movieId == u2Iterator->movieId) {
            sum += std::abs(u1Iterator->rating - u2Iterator->rating);
            count++;
            u1Iterator++;
            u2Iterator++;
        } else if (u1Iterator->movieId < u2Iterator->movieId) {
            u1Iterator++;
        } else {
            u2Iterator++;
        }
    }

    return sum / count;
}

int main(int argc, char *argv[]) {

    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " <user1Id> <user2Id>" << std::endl;
        return 1;
    }

    int user1Id = std::stoi(argv[1]);
    int user2Id = std::stoi(argv[2]);

    std::unordered_map<int, std::vector<Rating> > ratings;
    std::string line;

    std::ifstream ratingsFile("ratings.dat");
    while (getline(ratingsFile, line)) {
        auto tokens = split(line, "::");
        Rating rating = {std::stoi(tokens[0]), std::stoi(tokens[1]), std::stof(tokens[2]), std::stoi(tokens[3])};
        ratings[rating.userId].push_back(rating);
    }

    std::cout << averageMovieRatingDistance(user1Id, user2Id, ratings) << std::endl;

    return 0;
}
