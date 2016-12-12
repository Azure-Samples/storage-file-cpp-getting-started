//----------------------------------------------------------------------------------
// Microsoft Developer & Platform Evangelism
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
// THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, 
// EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES 
// OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
//----------------------------------------------------------------------------------
// The example companies, organizations, products, domain names,
// e-mail addresses, logos, people, places, and events depicted
// herein are fictitious.  No association with any real company,
// organization, product, domain name, email address, logo, person,
// places, or events is intended or should be inferred.
//----------------------------------------------------------------------------------
#include <cstdio>
#include <iostream>
#include <string>
#include "was/common.h"
#include "was/storage_account.h"
#include "was/file.h"
#include "was/blob.h"
#include "cpprest/filestream.h"
#include "getting_started.h"

using namespace azure::storage;

getting_started::getting_started()
{
}


getting_started::~getting_started()
{
}

void getting_started::run_file_operations(utility::string_t connection_string) {

        // These are used in the finally block to clean up the objects created during the demo.
        cloud_file cloud_file_sample = cloud_file();
        cloud_file_directory file_directory = cloud_file_directory();
        utility::string_t dest_file;

        try {
            //*******
            /*SETUP*/
            //*******

            cloud_storage_account storage_account = cloud_storage_account::parse(connection_string);

            // Create a file client for interacting with the file service.
            std::cout << "Instantiating file client.\n";
            cloud_file_client cloud_file_client = storage_account.create_cloud_file_client();

            // Create the share name
            utility::string_t share_name = U("demotest");

            // Name of folder to put the files in 
            utility::string_t  cloud_folder = U("testfolder");

            // Name of file to upload and download 
            utility::string_t  test_file = U("HelloWorld.png");

            // Folder where the HelloWorld.png file resides 
            utility::string_t  local_folder = U("downloadfolder/");

            // ******************************
            // **** Create a file share *****//
            // ******************************

            std::cout << "\n ** Creating a share\n";

            cloud_file_share share = cloud_file_client.get_share_reference(share_name);
            try
            {
                if(share.create_if_not_exists()){
                std::cout << "Share created successfully.\n";
                }
            }
            catch (const storage_exception& ex_storage)
            {
                std::cout << ex_storage.what();
                std::cout << "Exception thrown creating the share.\n";
                std::getchar();
                throw;
            }
            catch (const std::exception& ex)
            {
                std::cout << "Exception thrown creating the share.\n";
                std::cout << ex.what();
                throw;
            }

            // ***********************************************
            // **** Create a directory on the file share *****//
            // ***********************************************

            std::cout << "\n ** Creating a directory.\n";

            // First, get a reference to the root directory, because that's where you're going to put the new directory.
            cloud_file_directory root_dir = share.get_root_directory_reference();

            // Set a reference to the file directory.
            // There was a folder specified, so return a reference to that folder and create.
            file_directory = root_dir.get_subdirectory_reference(cloud_folder);
            file_directory.create_if_not_exists();
            std::cout << "Directory created successfully.\n";

            //***** Upload a file to the file share *****//
            // Set a reference to the file.
            cloud_file_sample = file_directory.get_file_reference(test_file);

            // Upload a file to the share.
            std::cout << "Uploading file to share.\n";

			FILE *fptr;

			#ifdef _WIN32
                
                errno_t err = fopen_s(&fptr, utility::conversions::to_utf8string(test_file).c_str(), "r");

				if (err == 0)
				{
					// Upload from the local file to the file share in azure.
					cloud_file_sample.upload_from_file(test_file);
					std::cout << "Successfully uploaded file to share.\n";
		        }
				else
				{
					std::cout << "File not found, so not uploaded.\n";
					std::cout << err;
				}

            #else
				
			    fptr = fopen(utility::conversions::to_utf8string(test_file).c_str(), "r");
			
				if (fptr != NULL)
				{
					// Upload from the local file to the file share in azure.
					cloud_file_sample.upload_from_file(test_file);
					std::cout << "Successfully uploaded file to share.\n";
				}
				else
				{
					std::cout << "File not found, so not uploaded.\n";
				}

            #endif


            // *************************************************************
            // **** Get list of all files/directories on the file share*****//
            // *************************************************************

            // List all files/directories under the root directory.

			std::cout << "\n ** Getting list of all files/directories under the root directory of the share.\n";

            list_file_and_diretory_result_iterator file_list = root_dir.list_files_and_directories();

            // Print all files/directories listed above.
            for(list_file_and_directory_item item : file_list)
            {
                // listItem type will be cloud_file or cloud_file_directory.
                if (item.is_directory())
                {
                    ucout << "Directory: " << item.as_directory().uri().primary_uri().to_string() << "\n";
                }
                else if (item.is_file())
                {
                    ucout << "File: " << item.as_file().uri().primary_uri().to_string() << "\n";
                }
            }

            // ***********************************************
            // **** Download a file from the file share *****//
            // ***********************************************

            // Download the file to the downloadFolder in the temp directory.
            // Check and if the directory doesn't exist (which it shouldn't), create it.
            ucout << "\n ** Downloading file from share to local temp folder " << local_folder << "\n";

            // Download the file.
            cloud_file_sample.download_to_file((local_folder+test_file));
            ucout << "File downloaded\n";

            // *******************************************************************************
            // **** Copy a file from the file share to blob storage, then abort the copy *****//
            // *******************************************************************************

            // Copies can sometimes complete before there's a chance to abort. 
            // If that happens with the file you're testing with, try copying the file 
            //   to a storage account in a different region. If it still finishes too fast,
            //   try using a bigger file and copying it to a different region. That will almost always 
            //   take long enough to give you time to abort the copy. 
            // If you want to change the file you're testing the Copy with without changing the value for the 
            //   rest of the sample code, upload the file to the share, then assign the name of the file 
            //   to the testFile variable right here before calling GetFileReference. 
            //   Then it will use the new file for the copy and abort but the rest of the code
            //   will still use the original file.

            // Copy the file to blob storage.
            ucout << "Copying file to blob storage. Container name = " << share_name << "\n";

            // First get a reference to the blob. 
            cloud_blob_client blob_client = storage_account.create_cloud_blob_client();

            // Get a reference to the blob container and create it if it doesn't already exist.
            cloud_blob_container container = blob_client.get_container_reference(share_name);
            container.create_if_not_exists();

            // Get a blob reference to the target blob.
            cloud_block_blob target_blob = container.get_blob_reference(test_file);

            // Create a SAS for the file that's valid for 24 hours.
            // Note that when you are copying a file to a blob, or a blob to a file, you must use a SAS
            // to authenticate access to the source object, even if you are copying within the same
            // storage account.
            utility::datetime expiry = utility::datetime::utc_now() + utility::datetime::from_minutes(900);
            utility::string_t sas_token = cloud_file_sample.get_shared_access_signature(file_shared_access_policy(expiry, file_shared_access_policy::permissions::read));

            // Construct the URI to the source file, including the SAS token.
            cloud_file file_with_sas(storage_credentials(sas_token).transform_uri(cloud_file_sample.uri().primary_uri()));

            // Start the copy of the file to the blob.
            auto copy_id = target_blob.start_copy(file_with_sas);
            ucout << "File copy started successfully. copyID = " << copy_id << "\n";

            // Abort the copy of the file to blob storage.
            // Note that you call Abort on the target object, i.e. the blob, not the file.
            // If you were copying from one file to another on the file share, the target object would be a file.
            ucout << "Check the copy status. If pending, cancel the copy operation" << "\n";

            // Print out the copy state information.
            target_blob.download_attributes();
            ucout << "targetBlob.copystate.CopyId = " << target_blob.copy_state().copy_id() << "\n";

            // Do the actual abort copy.
            // This only works if the copy is still pending or ongoing.
            if (target_blob.copy_state().status() == copy_status::pending)
            {
                ucout << "Status is Pending; cancelling the copy operation.\n";
                // Call to stop the copy, passing in the copyId of the operation.
                // This won't work if it has already finished copying.
                target_blob.abort_copy_async(target_blob.copy_state().copy_id());
                ucout << "Cancelling the copy succeeded.\n";
            }
            else
            {
                // If this happens, try a larger file.
                ucout << "Cancellation of copy not performed; copy has already finished.\n";
            }

            ucout << "Setting up files to test WriteRange and ListRanges.\n";

            // **********************************************************
            // **** Write 2 ranges to a file, then list the ranges ******
            // **********************************************************

            // This is the code for trying out writing data to a range in a file, 
            //   and then listing those ranges.
            // Get a reference to a file and write a range of data to it      .
            // Then write another range to it.
            // Then list the ranges.

            ucout << "\n ** Write 2 ranges to a file\n";

            // Start at the very beginning of the file.
            int64_t start_offset = 0;

            // Set the destination file name -- this is the file on the file share that you're writing to.
            utility::string_t dest_file = U("rangeops.txt");
            cloud_file range_file = root_dir.get_file_reference(dest_file);

            // Create a string with 512 a's in it. This will be used to write the range.
            int test_stream_len = 512;
            utility::string_t text_to_stream;
            text_to_stream.resize(test_stream_len);
            text_to_stream.insert(0, test_stream_len, 'a');
                
            // Name to be used for the file when downloading it so you can inspect it locally
            utility::string_t download_file = U("range.txt");

            {
                auto ms = concurrency::streams::bytestream::open_istream(std::move(utility::conversions::to_utf8string(text_to_stream)));
                    
                // Max size of the output file; have to specify this when you create the file
                // I picked this number arbitrarily.
                long maxFileSize = 65536;

                ucout << "Write first range.\n";

                // If the file doesn't exist, create it.
                // The maximum file size is passed in. It has to be big enough to hold
                //   all the data you're going to write, so don't set it to 256k and try to write two 256-k blocks to it. 
                if (!range_file.exists())
                {
                    ucout << "File doesn't exist, create empty file to write ranges to.\n";

                    // Create a file with a maximum file size of 64k. 
                    range_file.create(maxFileSize);
                    ucout << "Empty file created successfully.\n";
                }

                // Write the stream to the file starting at startOffset for the length of the stream.
                ucout << "Writing range to file.\n"; 
                utility::string_t md5;

                range_file.write_range(ms, start_offset, md5);

                // Download the file to your temp directory so you can inspect it locally.
                ucout << "Downloading file to examine.\n";
                range_file.download_to_file(download_file);
                ucout << "Successfully downloaded file with ranges in it to examine.\n";
            }

            // Now add the second range, but don't make it adjacent to the first one, or it will show only 
            //   one range, with the two combined. Put it like 2000 spaces away. When you get the range back, it will 
            //   start at the position at the 512-multiple border prior or equal to the beginning of the data written,
            //   and it will end at the 512-multliple border after the actual end of the data.
            //For example, if you write to 2000-3000, the range will be the 512-multiple prior to 2000, which is 
            //   position 1536, or offset 1535 (because it's 0-based).
            //   And the right offset of the range will be the 512-multiple after 3000, which is position 3072,
            //   or offset 3071 (because it's 0-based).
            ucout << "\n ** Getting ready to write second range to file.\n";

            start_offset += test_stream_len + 2000; //randomly selected number

            // Create a string with 512 b's in it. This will be used to write the range.
            text_to_stream.insert(0, test_stream_len, 'b');
            text_to_stream.resize(512);

            {
                auto ms = concurrency::streams::bytestream::open_istream(std::move(utility::conversions::to_utf8string(text_to_stream)));

                // Write the stream to the file starting at startOffset for the length of the stream.
                ucout << "Write second range to file.\n";
                range_file.write_range(ms, start_offset, utility::string_t());
                ucout << "Successful writing second range to file.\n";

                // Download the file to your temp directory so you can examine it.
                ucout << "Downloading file to examine.\n";
                range_file.download_to_file(download_file);
                ucout << "Successfully downloaded file with ranges in it to examine.\n";
            }

            // ************************************
            // Query and view the list of ranges.
            // ************************************
            ucout << "\n ** Call to get the list of ranges\n.";
            auto list_of_ranges = range_file.list_ranges();
            ucout << "Successfully retrieved list of ranges.\n";
            for(file_range range : list_of_ranges)
            {
                ucout << "    --> filerange startOffset = " << range.start_offset() << "\n";
            }

            // *********************************
            // Clean up the range file on Azure
            // *********************************
            ucout << "Cleaning up files from Azure Storage.\n";

            cloud_file_sample.delete_file_if_exists();
            range_file.delete_file_if_exists();
            target_blob.delete_blob_if_exists();
            container.delete_container_if_exists();
            share.delete_share_if_exists();

        }
        catch (const std::exception& e) {
            std::cout << e.what();
        }

}