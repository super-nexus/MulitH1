#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>


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

bool usersMatchInNMoviesAndMovieId(int userId1, int userId2, int movieId, int n, std::unordered_map<int, std::vector<Rating> > &ratings) {
    std::vector<Rating>::iterator u1Iterator = ratings[userId1].begin();
    std::vector<Rating>::iterator u2Iterator = ratings[userId2].begin();

    int count = 0;
    bool foundMovieId = false;

    while (u1Iterator != ratings[userId1].end() && u2Iterator != ratings[userId2].end()) {
        if(u2Iterator->movieId == movieId) {
            foundMovieId = true;
        }
        
        if (u1Iterator->movieId == u2Iterator->movieId) {
            count++;
            u1Iterator++;
            u2Iterator++;
        } else if (u1Iterator->movieId < u2Iterator->movieId) {
            u1Iterator++;
        } else {
            u2Iterator++;
        }
    }

    while (u2Iterator != ratings[userId2].end() && !foundMovieId) {
        if(u2Iterator->movieId == movieId) {
            foundMovieId = true;
        }
        u2Iterator++;
    }

    return count >= n && foundMovieId;
}

int findTheClosestUserThatWatchedTheMovie(int movieId, int userId, std::unordered_map<int, std::vector<Rating> > &ratings) {
    int closestUserId = -1;
    float closestDistance = 1000000;

    for (auto &pair : ratings) {
        int currentUserId = pair.first;
        std::vector<Rating> &currentRatings = pair.second;

        if (currentUserId == userId) {
            continue;
        }

        if (usersMatchInNMoviesAndMovieId(userId, currentUserId, movieId, 5, ratings)) {
            float distance = averageMovieRatingDistance(userId, currentUserId, ratings);
            if (distance < closestDistance) {
                closestDistance = distance;
                closestUserId = currentUserId;
            }
        }
    }

    return closestUserId;
}

int main(int argc, char *argv[]) {

    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " <userId> <user2Id>" << std::endl;
        return 1;
    }

    int userId = std::stoi(argv[1]);
    int movieId = std::stoi(argv[2]);

    std::unordered_map<int, std::vector<Rating> > ratings;
    std::string line;

    std::ifstream ratingsFile("ratings.dat");
    while (getline(ratingsFile, line)) {
        auto tokens = split(line, "::");
        Rating rating = {std::stoi(tokens[0]), std::stoi(tokens[1]), std::stof(tokens[2]), std::stoi(tokens[3])};
        ratings[rating.userId].push_back(rating);
    }

    int closestUserId = findTheClosestUserThatWatchedTheMovie(movieId, userId, ratings);
    int ratingDistance = averageMovieRatingDistance(userId, closestUserId, ratings);
    int userMovieRating = std::find_if(ratings[closestUserId].begin(), ratings[closestUserId].end(), [movieId](Rating &rating) { return rating.movieId == movieId; })->rating;

    std::cout << "The predicted rating for UserID: " << userId << " and MovieID: " << movieId << " is " << userMovieRating << " based on the most similar user: " << closestUserId << " with a rating distance: " << ratingDistance << std::endl; 

    return 0;
}
