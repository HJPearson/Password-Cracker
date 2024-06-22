# Given a txt file containing passwords, this script will load it into a SQLite database where each password is also accompanied by
# an integer, which will represent the number of past occurrences of that password

import sqlite3
import os

# Function to create the database and table
def create_database(db_file):
    conn = sqlite3.connect(db_file)
    cursor = conn.cursor()
    cursor.execute('''
        CREATE TABLE IF NOT EXISTS passwords (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            password TEXT UNIQUE NOT NULL,
            frequency INTEGER DEFAULT 0
        );
    ''')
    conn.commit()
    conn.close()

# Function to import passwords from a text file into the SQLite database
def import_passwords(txt_file, db_file):
    conn = sqlite3.connect(db_file)
    cursor = conn.cursor()
    
    with open(txt_file, 'r') as file:
        for line in file:
            line = line.rstrip('\n')  # Remove the newline character
            if len(line) < 2:
                print(f"Skipping invalid line (too short): {line}")
                continue

            # Split the line at the second-to-last character
            password = line[:-2]
            frequency = line[-1]
            
            if not frequency.isdigit():
                print(f"Skipping invalid line (non-numeric frequency): {line}")
                continue

            try:
                cursor.execute('''
                    INSERT INTO passwords (password, frequency)
                    VALUES (?, ?)
                    ON CONFLICT(password) DO UPDATE SET frequency = excluded.frequency;
                ''', (password, int(frequency)))
            except sqlite3.Error as e:
                print(f"Error inserting {password}: {e}")

    conn.commit()
    conn.close()

# Main function to run the script
def main():
    txt_file = ''  # Path to your text file
    db_file = ''  # Path to your SQLite database file
    
    if not os.path.isfile(txt_file):
        print(f"Text file '{txt_file}' not found.")
        return
    
    create_database(db_file)
    import_passwords(txt_file, db_file)
    print(f"Imported passwords from '{txt_file}' into '{db_file}'.")

if __name__ == '__main__':
    main()