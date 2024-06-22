import os

def compile_passwords(input_dir, output_file):
    passwords = set()  # To store unique passwords

    def read_file_and_filter(file_path):
        lines = []
        try:
            with open(file_path, 'r', encoding='utf-8') as file:
                for line in file:
                    try:
                        # Decode and clean the line: remove whitespace around and within
                        cleaned_line = ''.join(line.split())
                        if cleaned_line:  # Avoid empty lines
                            lines.append(cleaned_line)
                    except UnicodeDecodeError:
                        # Skip lines that can't be decoded
                        print(f"Skipped a problematic line in {file_path}.")
                        continue
        except Exception as e:
            print(f"Error reading {file_path}: {e}")
        return lines

    # Iterate through all files in the directory
    for filename in os.listdir(input_dir):
        file_path = os.path.join(input_dir, filename)
        
        # Check if it is a file
        if os.path.isfile(file_path) and filename.endswith('.txt'):
            print(f"Processing file: {file_path}")
            lines = read_file_and_filter(file_path)
            passwords.update(lines)

    # Write the unique passwords to the output file
    with open(output_file, 'w') as out_file:
        for password in sorted(passwords):
            out_file.write(password + '\n')

    print(f"Compiled {len(passwords)} unique passwords into {output_file}.")

# Directory containing the password files
input_directory = 'pwddatabase/raw'

# Output file where the compiled passwords will be saved
output_filename = 'pwddatabase/passwords.txt'

# Compile passwords
compile_passwords(input_directory, output_filename)









# import os

# def compile_passwords(input_dir, output_file):
#     passwords = set()  # To store unique passwords

#     # Iterate through all files in the directory
#     for filename in os.listdir(input_dir):
#         file_path = os.path.join(input_dir, filename)
        
#         # Check if it is a file
#         if os.path.isfile(file_path):
#             with open(file_path, 'r', encoding='utf-8', errors='ignore') as file:
#                 for line in file:
#                     # Clean the line: remove whitespace around and within
#                     cleaned_line = ''.join(line.split())
#                     if cleaned_line:  # Avoid empty lines
#                         passwords.add(cleaned_line)

#     # Write the unique passwords to the output file
#     with open(output_file, 'w', encoding='utf-8', errors='ignore') as out_file:
#         for password in sorted(passwords):
#             out_file.write(password + '\n')

#     print(f"Compiled {len(passwords)} unique passwords into {output_file}.")

# # Directory containing the password files
# input_directory = 'pwddatabase/raw'  # Replace with your directory path

# # Output file where the compiled passwords will be saved
# output_filename = 'pwddatabase/passwords.txt'  # Replace with your desired output file path

# # Compile passwords
# compile_passwords(input_directory, output_filename)