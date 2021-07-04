// C Standard Library Headers
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ASUS_AMI_CAP_Header_Size 2048

bool read_file(FILE* handle, void* buffer, unsigned int* size)
{
	if(handle != NULL && size != NULL)
	{
		if(buffer != NULL)
		{
			fseek(handle, 0, SEEK_END);
			*size = ftell(handle);
			fseek(handle, 0, SEEK_SET);
			fread(buffer, 1, *size, handle);
		}
		else
		{
			fseek(handle, 0, SEEK_END);
			*size = ftell(handle);
			fseek(handle, 0, SEEK_SET);
		}
		
		return true;
	}
	else
	{
		return false;
	}
}

// Program Entry Point
int main(int argument_count, char* argument_list[])
{
	// Print program header
	printf("ASUS UEFI Firmware Extractor v1.0.2\n");

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
			target_path = strdup(argument_list[1]);
			int length = strlen(target_path);
			
			if(strcmp(target_path + (length - 3), "CAP") == 0)
			{
				strcpy(target_path + (length - 3), "rom");
			}
			else if(strcmp(target_path + (length - 3), "cap") == 0)
			{
				strcpy(target_path + (length - 3), "rom");
			}
		}

		// Attempt to open the source file
		FILE* source_file = fopen(source_path, "rb");
		if(source_file != NULL)
		{
			// Get the source file size
			unsigned int source_file_size = 0;
			read_file(source_file, NULL, &source_file_size);
			
			// Read the source file
			unsigned char* source_file_buffer = malloc(source_file_size);
			bool read_result = read_file(source_file, source_file_buffer, &source_file_size);
			if (read_result == true)
			{
				// Allocate and zero-fill the target data buffer
				unsigned long target_file_size = source_file_size - ASUS_AMI_CAP_Header_Size;
				unsigned char* target_file_buffer = malloc(target_file_size);

				// Copy the UEFI Image to the target file buffer
				memcpy(target_file_buffer, source_file_buffer + ASUS_AMI_CAP_Header_Size, target_file_size);

				// Write the modified file
				FILE* destination_file = fopen(target_path, "wb");
				fwrite(target_file_buffer, 1, target_file_size, destination_file);

				// Free the file buffers
				free(target_file_buffer);
				free(source_file_buffer);
				
				// Close the file handles
				fclose(destination_file);
				fclose(source_file);

				return 0;
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
