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
#include <list>
#include <cstdio>
#include <iostream>
#include <string>
#include "was/common.h"
#include "was/storage_account.h"
#include "was/file.h"
#include "cpprest/filestream.h"
#include "advanced.h"

using namespace azure::storage;


advanced::advanced()
{
}


advanced::~advanced()
{
}

/// <summary>
/// Test some of the advanced file storage operations.
/// </summary>
void advanced::run_file_operations(utility::string_t connection_string) {

        //***** Setup *****//
        // Retrieve storage account information from connection string
        // How to create a storage connection string - http://msdn.microsoft.com/en-us/library/azure/ee758697.aspx
        cloud_storage_account storage_account = cloud_storage_account::parse(connection_string);

        ucout << "Instantiating file client for the advanced sample.\n";

        // Create a file client for interacting with the file service.
        cloud_file_client file_client = storage_account.create_cloud_file_client();

        // List shares
        list_shares_sample(file_client);

        // CORS Rules
        cors_sample(file_client);

        // Share Properties
        share_properties_sample(file_client);

        // Share Metadata
        share_metadata_sample(file_client);

        // Directory Properties
        directory_properties_sample(file_client);

        // Directory Metadata
        directory_metadata_sample(file_client);

        // File Properties
        file_properties_sample(file_client);

        // File Metadata
        file_metadata_sample(file_client);
}

/// <summary>
/// List shares sample for the File service
/// </summary>
/// <param name="file_client"></param>
void advanced::list_shares_sample(cloud_file_client file_client)
{
    
        ucout << "\n ** List Shares Sample starting...\n";

        // Keep a list of the file shares so you can compare this list 
        //   against the list of shares that we retrieve .
        std::list <utility::string_t> share_list;

        try
        {
            
            // Create 3 file shares.
            for (int i = 0; i < 3; i++)
            {
                // Set the name of the share, then add it to the generic list.
                utility::string_t share_name = U("demotest-") + utility::conversions::to_string_t(std::to_string(rand()));
                share_list.push_back(share_name);

                // Create the share with this name.
                ucout << "Creating share with name: " << share_name << "\n";
                cloud_file_share file_share = file_client.get_share_reference(share_name);

                file_share.create_if_not_exists();
                ucout << "Share created successfully.\n";

            }

            //ucout << share_list.size();
            ucout << "List of shares in the storage account:\n";

            // List the file shares with prefix 'demotest-' for this storage account 
            share_result_iterator shares = file_client.list_shares(U("demotest-"));
            for (cloud_file_share share : shares) {
                ucout << "......." << share.name() << "\n";
            }

        }
        catch (const storage_exception& ex_storage)
        {
            ucout << ex_storage.what();
            ucout << "Please make sure your storage account has storage file endpoint enabled and specified correctly in the app.config - then restart the sample.\n";
            ucout << "Press any key to exit.\n";
            std::getchar();
            throw;
        }
        catch (const std::exception& e) {
            ucout << e.what();
        }

        // Clean up - This will delete shares created in this method
        if (!share_list.empty())
        {
            // Now clean up after yourself, using the list of shares that you created in case there were other shares in the account.
            for(utility::string_t share : share_list)
            {
                cloud_file_share file_share = file_client.get_share_reference(share);
                file_share.delete_share_if_exists();
            }
        }

}

/// <summary>
/// Query the Cross-Origin Resource Sharing (CORS) rules for the File service
/// </summary>
/// <param name="file_client"></param>
void advanced::cors_sample(cloud_file_client file_client) {

        try {

            ucout << "\n ** Cors Sample starting...\n";

            // Get original CORS rule
            service_properties original_properties = file_client.download_service_properties();
            service_properties properties = original_properties;

            // Add CORS rule
            ucout << "Add CORS rule\n";

            // Enable cors rule
            service_properties_includes includes;
            includes.set_cors(true);

            // Configure cors rule
            std::vector<service_properties::cors_rule> cors_rules;
            service_properties::cors_rule cors_rule;
            cors_rule.allowed_headers().push_back(U("*"));
            cors_rule.allowed_methods().push_back(web::http::methods::GET);
            cors_rule.allowed_origins().push_back(U("www.ab.com"));
            cors_rule.exposed_headers().push_back(U("*"));
            cors_rule.set_max_age(std::chrono::seconds(30));
            cors_rules.push_back(cors_rule);

            // Upload cors rule
            properties.set_cors(cors_rules);
            file_client.upload_service_properties(properties, includes);

            // Clean up - revert to the original cors settings
            file_client.upload_service_properties_async(original_properties, includes);

        }
        catch (const storage_exception& ex_storage)
        {
            ucout << ex_storage.what();
            ucout << "Please make sure your storage account has storage file endpoint enabled and specified correctly in the app.config - then restart the sample.\n";
            ucout << "Press any key to exit.\n";
            std::getchar();
            throw;
        }
        catch (const std::exception& e) {
            ucout << e.what();
        }

}

/// <summary>
/// Manage share properties
/// </summary>
/// <param name="file_client"></param>
/// <returns></returns>
void advanced::share_properties_sample(cloud_file_client file_client)
{

        ucout << "\n ** Share Properties Sample starting...\n";
        
        // Create the share name -- use a random value in the name so it's unique.
        utility::string_t share_name = U("demotest-") + utility::conversions::to_string_t(std::to_string(rand()));

        cloud_file_share share = file_client.get_share_reference(share_name);

        // Set share properties
        ucout << "Set share properties\n";
        share.properties().set_quota(100);

        try
        {
            
            // Create Share
            ucout << "Creating the share \n";
            share.create_if_not_exists();

            // Fetch share attributes
            // in this case this call is not need but is included for demo purposes
            share.download_attributes();
            ucout << "Get share properties:\n";
            ucout << "Quota: " << share.properties().quota() << "\n";
            ucout << "ETag: " << share.properties().etag() << "\n";
            ucout << "Last modified: " << share.properties().last_modified().to_string() << "\n";

            // Clean up - delete the share
            share.delete_share_if_exists();

        }
        catch (const storage_exception& ex_storage)
        {
            ucout << ex_storage.what();
            ucout << "Please make sure your storage account has storage file endpoint enabled and specified correctly in the app.config - then restart the sample.\n";
            ucout << "Press any key to exit.\n";
            std::getchar();
            throw;
        }
        catch (const std::exception& e) {
            ucout << e.what();
        }     

}

/// <summary>
/// Manage share metadata
/// </summary>
/// <param name="file_client"></param>
/// <returns></returns>
void advanced::share_metadata_sample(cloud_file_client file_client)
{
        
        ucout << "\n ** Share Metada Sample starting...\n";

        // Create the share name -- use a random string in the name so it's unique.
        utility::string_t share_name = U("demotest-") + utility::conversions::to_string_t(std::to_string(rand()));

        cloud_file_share share = file_client.get_share_reference(share_name);

        // Set share metadata
        ucout << "Set share metadata\n";
        share.metadata()[U("key1")] = U("value");

        try
        {

            // Create the share
            ucout << "Create Share\n";
            share.create_if_not_exists();

            // Fetch share attributes
            // in this case this call is not needed but is included for demo purposes
            share.download_attributes();
            ucout << "Get share metadata:\n";
            for (auto key : share.metadata())
                ucout << key.first << key.second << "\n";

            // Clean up - delete the share
            share.delete_share_if_exists();
            
        }
        catch (const storage_exception& ex_storage)
        {
            ucout << ex_storage.what();
            ucout << "Please make sure your storage account has storage file endpoint enabled and specified correctly in the app.config - then restart the sample.\n";
            ucout << "Press any key to exit.\n";
            std::getchar();
            throw;
        }
        catch (const std::exception& e) {
            ucout << e.what();
        }

}

/// <summary>
/// Get directory properties
/// </summary>
/// <param name="file_client"></param>
/// <returns></returns>
void advanced::directory_properties_sample(cloud_file_client file_client)
{

        ucout << "\n ** Directory Properties Sample starting...\n";

        // Create the share name -- use a random string in the name so it's unique.
        utility::string_t share_name = U("demotest-") + utility::conversions::to_string_t(std::to_string(rand()));

        cloud_file_share share = file_client.get_share_reference(share_name);

        try
        {

            // Create Share
            ucout << "Creating the share and the directory \n";
            share.create_if_not_exists();

            // Create a directory
            cloud_file_directory directory = share.get_directory_reference(U("test"));
            directory.create_if_not_exists();


            // Fetch share attributes
            // in this case this call is not need but is included for demo purposes
            directory.download_attributes();
            ucout << "Get share properties:\n";
            ucout << "ETag: " << directory.properties().etag() << "\n";
            ucout << "Last modified: " << directory.properties().last_modified().to_string() << "\n";

            // Clean up - delete the share and the directory
            directory.delete_directory_if_exists(); 
            share.delete_share_if_exists();

        }
        catch (const storage_exception& ex_storage)
        {
            ucout << ex_storage.what();
            ucout << "Please make sure your storage account has storage file endpoint enabled and specified correctly in the app.config - then restart the sample.\n";
            ucout << "Press any key to exit.\n";
            std::getchar();
            throw;
        }
        catch (const std::exception& e) {
            ucout << e.what();
        }

}

/// <summary>
/// Manage directory metadata
/// </summary>
/// <param name="file_client"></param>
/// <returns></returns>
void advanced::directory_metadata_sample(cloud_file_client file_client)
{
        
        ucout << "\n ** Directory Metadata Sample starting...\n";

        // Create the share and directory -- use a random string in the name so it's unique.
        utility::string_t share_name = U("demotest-") + utility::conversions::to_string_t(std::to_string(rand()));

        cloud_file_share share = file_client.get_share_reference(share_name);
        cloud_file_directory directory = share.get_directory_reference(U("test"));

        // Set d irectorymetadata
        ucout << "Set directory metadata\n";
        directory.metadata()[U("key1")] = U("value1");
        directory.metadata()[U("key2")] = U("value2");

        try
        {

            // Create the share and the directory
            ucout << "Create Share\n";
            share.create_if_not_exists();
            directory.create_if_not_exists();

            // Fetch share attributes
            // in this case this call is not needed but is included for demo purposes
            directory.download_attributes();
            ucout << "Get share metadata\n";
            for (auto key : directory.metadata())
                ucout << key.first << key.second << "\n";

            // Clean up - delete the share and the directory
            share.delete_share_if_exists();
            directory.delete_directory_if_exists();

        }
        catch (const storage_exception& ex_storage)
        {
            ucout << ex_storage.what();
            ucout << "Please make sure your storage account has storage file endpoint enabled and specified correctly in the app.config - then restart the sample.\n";
            ucout << "Press any key to exit.\n";
            std::getchar();
            throw;
        }
        catch (const std::exception& e) {
            ucout << e.what();
        }

}

/// <summary>
/// Manage file properties
/// </summary>
/// <param name="file_client"></param>
/// <returns></returns>
void advanced::file_properties_sample(cloud_file_client file_client)
{

        ucout << "\n ** File Properties Sample starting...\n";

        // Create the share name -- use a random string in the name so it's unique.
        utility::string_t share_name = U("demotest-") + utility::conversions::to_string_t(std::to_string(rand()));

        cloud_file_share share = file_client.get_share_reference(share_name);
        cloud_file_directory directory = share.get_directory_reference(U("test"));

        try
        {

            // Create Share and Directory
            ucout << "Creating the share and the directory \n";
            share.create_if_not_exists();
            directory.create_if_not_exists();

            // Create a file
            cloud_file file = directory.get_file_reference(U("demofile"));

            // Set file properties
            file.properties().set_content_type(U("plain/text"));
            file.properties().set_content_encoding(U("UTF-8"));
            file.properties().set_content_language(U("en"));

            // Create file
            ucout << "Create file";
            file.create(1000);

            // Fetch file attributes
            // in this case this call is not need but is included for demo purposes
            file.download_attributes();
            ucout << "Get file properties:\n";
            ucout << "    ETag: " << file.properties().etag() << "\n";
            ucout << "    Content type: " << file.properties().content_type() << "\n";
            ucout << "    Cache control: " << file.properties().cache_control() << "\n";
            ucout << "    Content encoding: " << file.properties().content_encoding() << "\n";
            ucout << "    Content language: " << file.properties().content_language() << "\n";
            ucout << "    Content disposition: " << file.properties().content_disposition() << "\n";
            ucout << "  Content MD5: " << file.properties().content_md5() << "\n";
            ucout << "    Length: " << file.properties().length() << "\n";

            // Clean up - delete the created file, directory and share
            file.delete_file_if_exists();
            directory.delete_directory_if_exists();
            share.delete_share_if_exists();

        }
        catch (const storage_exception& ex_storage)
        {
            ucout << ex_storage.what();
            ucout << "Please make sure your storage account has storage file endpoint enabled and specified correctly in the app.config - then restart the sample.\n";
            ucout << "Press any key to exit.\n";
            std::getchar();
            throw;
        }
        catch (const std::exception& e) {
            ucout << e.what();
        }

}

/// <summary>
/// Manage file metadata
/// </summary>
/// <param name="file_client"></param>
/// <returns></returns>
void advanced::file_metadata_sample(cloud_file_client file_client)
{
        
        ucout << "\n ** File Metadata Sample starting...\n";

        // Create the share name -- use a rand in the name so it's unique.
        utility::string_t share_name = U("demotest-") + utility::conversions::to_string_t(std::to_string(rand()));

        cloud_file_share share = file_client.get_share_reference(share_name);
        cloud_file_directory directory = share.get_directory_reference(U("test"));

        try
        {

            // Create Share and Directory
            ucout << "Creating the share and the directory \n";
            share.create_if_not_exists();
            directory.create_if_not_exists();

            // Create a file
            cloud_file file = directory.get_file_reference(U("demofile"));

            // Set file metadata
            ucout << "Set file metadata\n";
            file.metadata()[U("key1")] = U("value1");
            file.metadata()[U("key2")] = U("value2");

            // Create file
            ucout << "Create file\n";
            file.create(1000);

            // Fetch file attributes
            // in this case this call is not need but is included for demo purposes
            file.download_attributes();
            ucout << "Get file metadata:\n";
            
            for(auto key_value : file.metadata())
            {
                ucout << key_value.first << key_value.second << "\n";
            }

            // Clean up - delete the created file, directory and share
            ucout << "Cleaning up files from Azure Storage.\n";

            file.delete_file_if_exists();
            directory.delete_directory_if_exists();
            share.delete_share_if_exists();
                     
        }
        catch (const storage_exception& ex_storage)
        {
            ucout << ex_storage.what();
            ucout << "Please make sure your storage account has storage file endpoint enabled and specified correctly in the app.config - then restart the sample.\n";
            ucout << "Press any key to exit.\n";
            std::getchar();
            throw;
        }
        catch (const std::exception& e) {
            ucout << e.what();
        }

}