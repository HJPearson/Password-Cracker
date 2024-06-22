# Password-Cracker
A password cracker made to improve upon standard dictionary attacks by tracking occurrences of cracked passwords in a SQLite database in order to attempt by order of past occurrence.

This password cracker performs dictionary attacks that take advantage of the fact that some passwords will be used more frequently than others, as it will attempt passwords in order of which it has seen most frequently in the past. It assumes access to a shadow file and currently is only set up to work on sha-256 hashed passwords (with salts). Functionality of this could be extended further. Feel free to take it and do so if you wish.

Several password files from the internet have been provided in the pawddatabase/raw folder

There are several utility scripts included in this repo for doing things such as:
- compiling the raw password files into one large txt file
- making a shadow file from a given password txt file
- choosing a random subset of shadow file entries from a larger shadow file/database
- creating a sqlite from your final password list that tracks their occurrences
- resetting the frequencies of passwords in the sqlite database back to 0

To compile this program, you will need openssl, sqlite, and to include them in the compilation. Here is an example compilation:

g++ -std=c++17 src/main.cpp src/cracker.cpp -o cracker -I/opt/homebrew/opt/openssl@3/include -L/opt/homebrew/opt/openssl@3/lib -lssl -lcrypto -lsqlite3

Note that you will likely need to change your include path for openssl.

This was built on MacOS.