# Script to reset all password frequencies in the SQLite database to 0

import sqlite3


def reset_password_frequencies(db_path):
    try:
        # Connect to the SQLite database
        conn = sqlite3.connect(db_path)
        cursor = conn.cursor()
        
        # SQL statement to reset the frequency to 0
        sql = "UPDATE passwords SET frequency = 0"
        
        # Execute the SQL statement
        cursor.execute(sql)
        
        # Commit the changes
        conn.commit()
        
        print("Frequencies reset to 0 for all passwords.")
        
    except sqlite3.Error as e:
        print(f"Error occurred: {e}")
        
    finally:
        # Close the database connection
        if conn:
            conn.close()

if __name__ == "__main__":    
    # Reset password frequencies
    database_path = ""
    reset_password_frequencies(database_path)