#ifndef CRACKER_H
#define CRACKER_H

#include <iostream>
#include <string>
#include <iomanip>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstring>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <sqlite3.h>
#include <chrono>


void help();
void options();
void parseInputForOptions(int argc, char* argv[]);
void crackShadowFile(std::string shadowFile, std::string dictionaryFile);
std::vector<std::string> parseShadowLine(std::string& line);
std::vector<std::string> split(const std::string& s, char delimiter);
std::string genSha256(const std::string& password, const std::string& salt);
bool findPasswordInDatabase(sqlite3 *db, const std::string &hashedPassword, const std::string& salt);
void updatePasswordFrequency(sqlite3 *db, const std::string &password);


#endif