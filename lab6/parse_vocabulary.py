# Input and output file paths
input_file_path = "train.txt"
output_file_path = "vocab.txt"

# Open the input file for reading and output file for writing
with open(input_file_path, 'r') as input_file, open(output_file_path, 'w') as output_file:
    # Iterate through each line in the input file
    for line in input_file:
        # Remove the first space from each line
        modified_line = line.partition(' ')[2]
        # Write the modified line to the output file
        output_file.write(modified_line)

print("File processing completed. Output saved to", output_file_path)
