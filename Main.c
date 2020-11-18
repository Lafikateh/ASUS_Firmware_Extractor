// C Standard Library Headers
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Program Headers
#include "Main.h"
#include "Files.h"
#include "Strings.h"

#define ASUS_AMI_CAP_Header_Size 2048

// Program Entry Point
int main(int argument_count, char* argument_list[])
{
	// Print program header
	printf("ASUS UEFI Firmware Extractor v1.0.1\n");

	if (argument_count == 1)
	{
		// Print usage
		printf("Usage: input_file [output_file]\n");

		// Report success
		return 0;
	}
	else if (argument_count == 2 || argument_count == 3)
	{
		// Determine the source and target file path
		char* source_path = argument_list[1];
		char* target_path = NULL;
		if (argument_count == 3)
		{
			target_path = argument_list[2];
		}
		else
		{
			target_path = argument_list[1];
			target_path = c_replace_string_c(target_path, ".CAP", ".rom");
			target_path = c_replace_string_c(target_path, ".cap", ".rom");
		}

		// Check if the source file exists
		if (check_if_exists(source_path) == true)
		{
			// Attempt to read the source file
			data_buffer* source_buffer = read_file(source_path);
			if (source_buffer != NULL)
			{
				// Allocate and zero-fill the target data buffer
				unsigned long target_data_buffer_size = source_buffer->size - ASUS_AMI_CAP_Header_Size;
				unsigned char* target_data_buffer = calloc(target_data_buffer_size, sizeof(unsigned char));

				// Copy the UEFI Image to the target file buffer
				memcpy(target_data_buffer, source_buffer->data + ASUS_AMI_CAP_Header_Size, target_data_buffer_size);

				// Create the target file buffer
				data_buffer* target_buffer = create_buffer(target_data_buffer, target_data_buffer_size);

				// Attempt to write the modified file
				bool write_result = write_file(target_path, target_buffer);

				// Free the file buffers
				free_buffer(target_buffer);
				free_buffer(source_buffer);

				if (write_result == true)
				{
					return 0;
				}
				else
				{
					// Print error
					printf("Failed to write file \"%s\"", target_path);

					// Return failure
					return 1;
				}
			}
			else
			{
				// Print error
				printf("Failed to read file \"%s\"", source_path);

				// Return failure
				return 1;
			}
		}
		else
		{
			// Print error
			printf("Input file does not exist!\n");

			// Return failure
			return 1;
		}
	}
	else if (argument_count > 3)
	{
		// Print error
		printf("Too many arguments provided!\n");

		// Return failure
		return 1;
	}
}
