---
services: storage
platforms: cpp
author: seguler
---

# Azure Storage File Service Samples in C++

Demonstrates how to use the File Service.

- Azure File Storage offers shared storage for legacy applications using the standard SMB protocol. Azure virtual machines and cloud services can share file data across application components via mounted shares, and on-premises applications can access file data in a share via the File service REST API.. 

Note: This sample uses the Windows Azure Storage client library for C++ available through a Nuget package, which depends on the Visual Studio version you will use for development. For Visual Studio 2015, use the Nuget package "wastorage.v140". For Visual Studio 2013, use "wastorage.v120".

If you don't have a Microsoft Azure subscription you can get a FREE trial account [here](http://go.microsoft.com/fwlink/?LinkId=330212).

## Running this sample in Windows

This sample can be run using an Azure Subscription. The sample will prompt the user to enter an account name and key.

Note: The sample will download files to 'downloadfolder' in local environment where it is run and  upload files to 'targetfolder' in your Azure Storage account. These folders can be changed in getting_started.cpp.

To run the sample using Visual Studio:

1. Create a Storage Account through the Azure Portal and note down your account name and account key.
2. Set breakpoints and run the project using F10.
3. Enter in your account name and key when prompted.

## Running this sample in Linux

In order to compile and run this sample in Linux, you first need to get a local copy of the C++ REST SDK (aka Casablanca) from [here] (https://github.com/Microsoft/cpprestsdk/wiki/How-to-build-for-Linux) and the Azure Storage Client Library for C++ from [here] (https://github.com/Azure/azure-storage-cpp). Both libraries must be compiled following the instructions described in the corresponding Github repositories.
Once you have these libraries, follow these instructions to build the samples,

- Clone the project using git:
```bash
git clone https://github.com/Azure-Samples/storage-file-cpp-getting-started.git
```
The project is cloned to a folder called `storage-file-cpp-getting-started`. Always use the master branch, which contains the latest release.

- Build the samples:
```bash
cd storage-file-cpp-getting-started/storage-File-cpp-getting-started
export AZURESTORAGE_DIR=<path to azure-storage-cpp library>
cmake .
make
```

The sample is generated under `storage-file-cpp-getting-started/Binaries/`.

## More information
- [What is a Storage Account](http://azure.microsoft.com/en-us/documentation/articles/storage-whatis-account/)
- [How to use File Storage from C++](https://azure.microsoft.com/en-us/documentation/articles/storage-introduction/)
- [File Service Concepts](https://msdn.microsoft.com/library/dn166972.aspx)
- [File Service REST API](https://msdn.microsoft.com/library/dn167006.aspx)
