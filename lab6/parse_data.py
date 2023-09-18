# Input and output file paths
input_file_path = "test.txt"
output_file_path = "parsed_test.txt"

# Open the input file for reading and output file for writing
with open(input_file_path, 'r') as input_file, open(output_file_path, 'w') as output_file:
    # Iterate through each line in the input file
    for line in input_file:
        # Remove the first space from each line
        classification, _, sentence = line.partition(' ')
        classification = classification.partition(':')[0]
        modified_line = classification + " " + sentence
        # Write the modified line to the output file
        output_file.write(modified_line)

print("File processing completed. Output saved to", output_file_path)
