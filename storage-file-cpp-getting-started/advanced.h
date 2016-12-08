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

using namespace azure::storage;

#pragma once
class advanced
{
public:
    advanced();
    ~advanced();

    static void run_file_operations(utility::string_t connection_string);

private: 
	static void list_shares_sample(cloud_file_client file_client);
    static void cors_sample(cloud_file_client file_client);
    static void share_properties_sample(cloud_file_client file_client);
    static void share_metadata_sample(cloud_file_client file_client);
    static void directory_properties_sample(cloud_file_client file_client);
    static void directory_metadata_sample(cloud_file_client file_client);
    static void file_properties_sample(cloud_file_client file_client);
    static void file_metadata_sample(cloud_file_client file_client);
}; 
