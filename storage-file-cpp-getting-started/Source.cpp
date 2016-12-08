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
#include "was/storage_account.h"
#include "was/file.h"
#include "cpprest/filestream.h"
#include "getting_started.h"
#include "advanced.h"

using namespace azure::storage;

utility::string_t account, key;

int main(int argc, const char *argv[])
{
    
    std::cout << "Azure Storage File Sample\n ";

    //Getting reference to the storage account
    std::cout << "Please enter your account name\n";
    std::getline(ucin, account);
    std::cout << "Please enter your account key\n";
    std::getline(ucin, key);

    // Retrieve storage account information from connection string
    utility::string_t connection_string = U("DefaultEndpointsProtocol=http;AccountName=") + account + U(";AccountKey=") + key;

    getting_started basic_file;
    basic_file.run_file_operations(connection_string);

    ucout << "\n";

    advanced advanced_file;
    advanced_file.run_file_operations(connection_string);

    std::cout << "Press any key to exit.\n";
    std::getchar();
    return 0;
}
