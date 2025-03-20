#!/bin/bash

# This script reverts the sparse-checkout setup by disabling sparse-checkout
# and restoring the full repository structure.

echo "---------------------------------------------------------"
echo "             #######    #####  #   #  #####  #"
echo "             #          #      ##  #    #    #"
echo "             #          ###    # # #    #    #"
echo "             #          #      #  ##    #    #"
echo "             #######    #####  #   #    #    #"
echo "---------------------------------------------------------"
echo "---------------------------------------------------------"

# Check if Git is installed
echo "Checking if Git is installed..."
if ! command -v git &>/dev/null; then
    echo "Git is not installed or not available in the PATH. Please install Git and try again."
    exit 1
fi

# Check if sparse-checkout is enabled
echo "Checking if sparse-checkout is enabled..."
if ! git sparse-checkout disable &>/dev/null; then
    echo "Sparse-checkout was not enabled or failed to disable. Exiting..."
    exit 1
fi

# Pull the full repository content from the current branch
echo "Restoring the full repository structure by pulling the complete content..."
if ! git pull --recurse-submodules; then
    echo "Failed to pull the full repository. Exiting..."
    exit 1
fi

# Confirm that the repository is fully restored
echo "The sparse-checkout configuration has been removed."
echo "The full repository structure has been restored."
