# Given a dictionary/password database file, this script will turn every password into a shadow file entry with a random salt
# Can be used to create a database of shadow file entries for testing purposes
# Currently uses sha-256, but can be changed to use any hash function

import hashlib
import random
import string

def generate_salt(length=8):
    """Generates a random salt of given length."""
    characters = string.ascii_letters + string.digits
    return ''.join(random.choice(characters) for _ in range(length))

def hash_password_sha256(password, salt):
    """Hashes a password with SHA-256 including a salt."""
    hash_object = hashlib.sha256((password + salt).encode())
    return hash_object.hexdigest()

def create_shadow_file(input_file, output_file):
    with open(input_file, 'r') as infile:
        lines = infile.readlines()

    with open(output_file, 'w') as outfile:
        user_count = 1
        for line in lines:
            password = line.split(':')[0]
            salt = generate_salt()
            hashed_password = hash_password_sha256(password, salt)
            # If you change from sha-256 to a different hash, you might want to change the $5$ to the proper tag for consistency
            shadow_line = f"user{user_count}:$5${salt}${hashed_password}:18930:0:99999:7:::\n"
            outfile.write(shadow_line)
            user_count += 1

# Specify the input and output files
input_file = ''
output_file = ''

# Call the function to create the shadow file
create_shadow_file(input_file, output_file)
