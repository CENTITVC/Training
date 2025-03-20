@echo off
:: This script reverts the sparse-checkout setup by disabling sparse-checkout
:: and restoring the full repository structure.

echo ---------------------------------------------------------
echo             #######    #####  #   #  #####  #
echo             #          #      ##  #    #    #
echo             #          ###    # # #    #    #
echo             #          #      #  ##    #    #
echo             #######    #####  #   #    #    #
echo ---------------------------------------------------------
echo ---------------------------------------------------------

:: Check if Git is installed
echo Checking if Git is installed...
git --version
IF ERRORLEVEL 1 (
    echo Git is not installed or not available in the PATH. Please install Git and try again.
    pause
    exit /b 1
)

:: Check if sparse-checkout is enabled
echo Checking if sparse-checkout is enabled...
git sparse-checkout disable >nul 2>&1
IF ERRORLEVEL 1 (
    echo Sparse-checkout was not enabled or failed to disable. Exiting...
    pause
    exit /b 1
)

:: Pull the full repository content from the current branch
echo Restoring the full repository structure by pulling the complete content...
git pull --recurse-submodules
IF ERRORLEVEL 1 (
    echo Failed to pull the full repository. Exiting...
    pause
    exit /b 1
)

:: Confirm that the repository is fully restored
echo The sparse-checkout configuration has been removed.
echo The full repository structure has been restored.
pause