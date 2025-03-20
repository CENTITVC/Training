#!/bin/bash
# This script sets up sparse-checkout to only check out the directories
# for the correct development branch on the remote server

echo "---------------------------------------------------------"
echo "            #######    #####  #   #  #####  #"
echo "            #          #      ##  #    #    #"
echo "            #          ###    # # #    #    #"
echo "            #          #      #  ##    #    #"
echo "            #######    #####  #   #    #    #"
echo "---------------------------------------------------------"
echo "---------------------------------------------------------"

# Check if Git is installed
echo "Checking if Git is installed..."
if ! command -v git &> /dev/null
then
    echo "Git is not installed or not available in the PATH. Please install Git and try again."
    exit 1
fi

# Check if sparse-checkout was disabled
echo "Checking if sparse-checkout was disabled..."
git sparse-checkout disable > /dev/null 2>&1
if [ $? -ne 0 ]; then
    echo "Failed to check sparse-checkout. Exiting..."
    exit 1
fi

# Initialize sparse-checkout in the current repository
echo "Initializing sparse-checkout..."
git sparse-checkout init
if [ $? -ne 0 ]; then
    echo "Failed to initialize sparse-checkout. Exiting..."
    exit 1
fi

# Set the paths for sparse-checkout to include only the desired directories
echo "Setting sparse-checkout directories: Software/Server and Software/DevServer..."
git sparse-checkout set ../Software/Server ../Software/DevServer
if [ $? -ne 0 ]; then
    echo "Failed to set sparse-checkout directories. Exiting..."
    exit 1
fi

# Update the working directory to reflect the sparse-checkout setup
echo "Applying sparse-checkout changes by pulling the latest from the branch sw_server_staging..."
git pull origin sw_server_staging
if [ $? -ne 0 ]; then
    echo "Failed to pull from the branch. Exiting..."
    exit 1
fi

# Confirm the sparse-checkout has been configured
echo "Sparse-checkout setup complete. The following directories are now checked out:"
echo "- Software/Server"
echo "- Software/DevServer"
echo "- Software/README.md"
