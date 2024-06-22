# Simple script for testing hash functions in Python
# Can also be changed a bit and used to generate hashes for shadow files

import hashlib

def hash_password_sha256(password: str, salt: str) -> str:
    # Combine the password and salt
    combined = password + salt
    
    # Create a SHA-256 hash object
    sha256 = hashlib.sha256()
    
    # Update the hash object with the combined password and salt
    sha256.update(combined.encode('utf-8'))
    
    # Get the hexadecimal digest of the hash
    hashed_password = sha256.hexdigest()
    
    return hashed_password


if __name__ == "__main__":
    password = "password"
    salt = "salt"
    hashed = hash_password_sha256(password, salt)
    print(hashed)
