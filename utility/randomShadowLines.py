# Script to randomly select a certain number of shadow file entries from a larger database of shadow file entries and write them to a file
# This can be used to randomly select shadow file entries for testing purposes

import random

def choose_random_lines(input_file, output_file, num_lines, total_lines):
    # Open the input file in read mode
    with open(input_file, 'r') as infile:
        # Read all lines from the input file
        lines = infile.readlines()

    # Ensure num_lines does not exceed total_lines
    num_lines = min(num_lines, total_lines)

    # Choose random lines without replacement
    random_lines = random.sample(lines, num_lines)

    # Write the chosen lines to the output file
    with open(output_file, 'w') as outfile:
        outfile.writelines(random_lines)

# Specify the input and output files
input_file = ''
output_file = ''
num_lines =  0 # Number of random lines you want to select, which is about 20% of lines in file (0.19999999242% to be exact)
total_lines = 0  # Total number of lines in the file

# Call the function to choose random lines
choose_random_lines(input_file, output_file, num_lines, total_lines)