@echo off
:: This script sets up sparse-checkout to only check out the directories
:: for the correct development branch on the remote server

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
::>nul 2>&1
IF ERRORLEVEL 1 (
    echo Git is not installed or not available in the PATH. Please install Git and try again.
    pause
    exit /b 1
)

:: Check if sparse-checkout was disabled
echo Checking if sparse-checkout was disabled...
git sparse-checkout disable >nul 2>&1
IF ERRORLEVEL 1 (
    echo Failed to check sparse-checkout. Exiting...
    pause
    exit /b 1
)

:: Initialize sparse-checkout in the current repository
echo Initializing sparse-checkout...
git sparse-checkout init
IF ERRORLEVEL 1 (
    echo Failed to initialize sparse-checkout. Exiting...
    pause
    exit /b 1
)

:: Set the paths for sparse-checkout to include only the desired directories
echo Setting sparse-checkout directories: Software/Server and Software/DevServer...
git sparse-checkout set ../Software/Server ../Software/DevServer
IF ERRORLEVEL 1 (
    echo Failed to set sparse-checkout directories. Exiting...
    pause
    exit /b 1
)

:: Update the working directory to reflect the sparse-checkout setup
echo Applying sparse-checkout changes by pulling the latest from the branch sw_server_staging...
git pull origin sw_server_staging
IF ERRORLEVEL 1 (
    echo Failed to pull from the branch. Exiting...
    pause
    exit /b 1
)

:: Confirm the sparse-checkout has been configured
echo Sparse-checkout setup complete. The following directories are now checked out:
echo - Software/Server
echo - Software/DevServer
echo - Software/README.md
pause
