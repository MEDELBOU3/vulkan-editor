# Build script for MinGW/MSYS2
if (!(Test-Path "CMakeLists.txt")) {
    Write-Host "Error: Run this script from the project root (where CMakeLists.txt is)." -ForegroundColor Red
    exit 1
}

# Forced cleanup
if (Test-Path "build") {
    Write-Host "Cleaning up old build directory..."
    Remove-Item -Path build -Recurse -Force -ErrorAction SilentlyContinue
    if (Test-Path "build") {
        # If still exists, try moving it or renaming (common Windows locking issue)
        $randomName = "build_old_" + (Get-Random)
        Rename-Item -Path build -NewName $randomName -ErrorAction SilentlyContinue
    }
}

mkdir build
cd build

# Point to MSYS2 compilers specifically to avoid NMake/cl.exe confusion
$env:CC = "C:/msys64/ucrt64/bin/gcc.exe"
$env:CXX = "C:/msys64/ucrt64/bin/g++.exe"

Write-Host "Configuring Project..." -ForegroundColor Cyan
# Using MinGW generator and pointing to MSYS2 Vulkan paths
# We use the mingw64 Vulkan path since we found it there earlier
cmake .. -G "MinGW Makefiles" `
    -DCMAKE_BUILD_TYPE=Release `
    -DVulkan_INCLUDE_DIR="C:/msys64/mingw64/include" `
    -DVulkan_LIBRARY="C:/msys64/mingw64/lib/libvulkan.dll.a"

if ($LASTEXITCODE -eq 0) {
    Write-Host "Building project..." -ForegroundColor Green
    cmake --build .
} else {
    Write-Host "Configuration failed." -ForegroundColor Red
    Write-Host "Please check if you have internet access for FetchContent or if Vulkan is installed." -ForegroundColor Yellow
    Write-Host "Try: pacman -S mingw-w64-ucrt-x86_64-vulkan-devel" -ForegroundColor Gray
}
