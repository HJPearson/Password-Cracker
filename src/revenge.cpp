#include "../include/revenge.h"


/*
* Function that outputs help text when the --help option is used
*/
void help() {
    std::cout << "Welcome to the Occam's Revenge password cracker. Below you can find proper usage of this program as well as additional options and features.\n" << std::endl;
    std::cout << "Usage: ./revenge [path/to/shadowfile] [path/to/dictionaryFile] [options]\n" << std::endl;
    options();
}

/*
* Function that lists available options
*/
void options() {
    std::cout << "Available options:" << std::endl;
    std::cout << std::setw(10) << "" << "--help: Gives a comprehensive overview of the usage and features of Occam's Revenge." << std::endl;
    std::cout << std::setw(10) << "" << "--options: Lists all available options that can be used when running Occam's Revenge." << std::endl;
}

void crackShadowFile(std::string shadowFile, std::string dictionaryFile) {
    // Throw an error and exit if the shadow file does not exist
    if (!std::filesystem::exists(shadowFile)) {
        std::cerr << "Error: File not found: " << shadowFile << "\n";
        exit(EXIT_FAILURE);
    }
    // Open the shadow file
    std::ifstream file(shadowFile);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file: " << shadowFile << "\n";
        exit(EXIT_FAILURE);
    }
    // Get each line of the shadowFile
    int count_total = 0;
    int count_found = 0;
    std::string line;
    // Open the SQLite database
    sqlite3 *db;
    int rc;
    rc = sqlite3_open(dictionaryFile.c_str(), &db);
    if (rc) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
    }
    auto start = std::chrono::high_resolution_clock::now();
    while (std::getline(file, line)) {
        // Call parseLine function
        std::vector<std::string> fields = parseShadowLine(line);
        // Break up the shadowFile line into the desired variables
        // std::string hashAlgorithm = fields[1];   // Currently not using this, as we are only using sha-256 atm
        std::string salt = fields[2];
        std::string hashedPassword = fields[3];
        std::cout << "Successfully parsed shadow file" << std::endl;

        bool found = findPasswordInDatabase(db, hashedPassword, salt);
        if (found) 
            count_found++;
        count_total++;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    sqlite3_close(db);
    float crackedRatio = (count_found / count_total) * 100;
    std::cout << "Percentage of shadow file cracked: " << crackedRatio << "%" << std::endl;
    std::cout << "Time to crack shadow file: " << elapsed.count() << std::endl;
    // Write the elapsed time to a file for logging
    std::ofstream timeFile("crackTimes.txt", std::ios::app);
    // Open the file in append mode
    // file.open("crackTimes.txt", std::ios::app);
    if (timeFile.is_open()) {
        timeFile << "Elapsed time: " << elapsed.count() << " seconds" << std::endl;
        timeFile.close();
    } else {
        std::cerr << "Unable to open file: " << "crackTimes.txt" << std::endl;
    }
}

bool findPasswordInDatabase(sqlite3 *db, const std::string& hashedPassword, const std::string& salt) {
    sqlite3_stmt *stmt;
    int rc;

    // Open the SQLite database
    // rc = sqlite3_open(dbPath.c_str(), &db);

    // Prepare the SQL statement
    const char *sql = "SELECT password FROM passwords ORDER BY frequency DESC, id";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to execute query: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    std::cout << "Successfully opened and queried database" << std::endl;
    std::cout << "Scanning database for matching password" << std::endl;

    // Iterate through the results
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        // Get the password from the current row
        const unsigned char *plaintext = sqlite3_column_text(stmt, 0);
        std::string plaintextPassword = reinterpret_cast<const char*>(plaintext);

        // Hash the plaintext password
        std::string hashedFromDB = genSha256(plaintextPassword, salt);

        // Compare with the hashed password from the shadow file
        if (hashedFromDB == hashedPassword) {
            std::cout << "Match found. Cracked password is: " << plaintextPassword << std::endl;
            updatePasswordFrequency(db, plaintextPassword);
            // sqlite3_close(db);
            return true;
            break;
        }
    }
    std::cout << "Password not found in database" << std::endl;
    return false;
}

void updatePasswordFrequency(sqlite3 *db, const std::string &password) {
    // sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;

    // Open the SQLite database
    // rc = sqlite3_open(dbPath.c_str(), &db);
    // if (rc) {
    //     std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
    //     return;
    // }

    // Prepare the SQL statement to update frequency
    const char *sql = "UPDATE passwords SET frequency = frequency + 1 WHERE password = ?";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare update statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    // Bind the password to the SQL statement
    sqlite3_bind_text(stmt, 1, password.c_str(), -1, SQLITE_STATIC);

    // Execute the statement
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Failed to execute update: " << sqlite3_errmsg(db) << std::endl;
    }

    // Clean up
    sqlite3_finalize(stmt);
    // sqlite3_close(db);
}


/*
* Function to parse a line of the shadowFile and extract the info we want, like salt and hashed password.
* Username can be extracted as well if we want to use it somehow, but it is commented out and not used anywhere for now.
*/
std::vector<std::string> parseShadowLine(std::string& line) {
    // Call the split function, which is implemented a few functions down in this file
    auto fields = split(line, ':');
    if (fields.size() < 2) {
        std::cerr << "Invalid shadow line format: " << line << "\n";
        exit(EXIT_FAILURE);
    }
    // std::string username = fields[0];
    std::string hashSaltPassword = fields[1];
    auto passwordField = split(hashSaltPassword, '$');
    return passwordField; 
}

/*
* Function to hash password with salt appended using SHA-256
*/
std::string genSha256(const std::string& password, const std::string& salt) {
    // Create a vector to hold the hash
    std::vector<unsigned char> hash(EVP_MAX_MD_SIZE);
    unsigned int hashLength = 0;

    // Create and initialize the context
    EVP_MD_CTX* context = EVP_MD_CTX_new();
    if (!context) {
        throw std::runtime_error("Failed to create OpenSSL context");
    }
    // Initialize the hash function (SHA-256)
    if (EVP_DigestInit_ex(context, EVP_sha256(), nullptr) != 1) {
        EVP_MD_CTX_free(context);
        throw std::runtime_error("Failed to initialize digest");
    }
    // Update the context with the password
    if (EVP_DigestUpdate(context, password.data(), password.size()) != 1) {
        EVP_MD_CTX_free(context);
        throw std::runtime_error("Failed to update digest with password");
    }
    // Update the context with the salt
    if (EVP_DigestUpdate(context, salt.data(), salt.size()) != 1) {
        EVP_MD_CTX_free(context);
        throw std::runtime_error("Failed to update digest with salt");
    }
    // Finalize the digest
    if (EVP_DigestFinal_ex(context, hash.data(), &hashLength) != 1) {
        EVP_MD_CTX_free(context);
        throw std::runtime_error("Failed to finalize digest");
    }
    // Resize hash to actual length
    hash.resize(hashLength);
    std::ostringstream oss;
    for (const auto& byte : hash) {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    }
    std::string hashString = oss.str();
    // Clean up
    EVP_MD_CTX_free(context);

    return hashString;
}

/*
* Splits the shadowFile line by the specified delimiter, which will be either : or $
*/
std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while(std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

/*
* Parses the user input for options and calls corresponding function(s)
*/
void parseInputForOptions(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            help();
        }
        else if (strcmp(argv[i], "--options") == 0) {
            options();
        }
    }
}

/*
* When a password is cracked, increment its frequency in the dictionary
*/
void incrementFrequency(const std::string& password) {
    std::ofstream outFile;
    outFile.open("./pwddatabase/dictionary.txt");
    if (!outFile.is_open()) {
            std::cerr << "Error: Could not open capture file.\n";
            exit(EXIT_FAILURE);
    }
    // Finish here
}