# ══════════════════════════════════════════════════════════════
#  Gidit — Build Script (Windows / PowerShell)
#  Usage: .\scripts\build.ps1 [-Config Debug|Release] [-Clean] [-Package]
# ══════════════════════════════════════════════════════════════
param(
    [ValidateSet("Debug","Release")]
    [string]$Config = "Release",
    [switch]$Clean,
    [switch]$Package,
    [switch]$Install
)

$ErrorActionPreference = "Stop"
$ScriptDir  = Split-Path -Parent $MyInvocation.MyCommand.Path
$ProjectDir = Split-Path -Parent $ScriptDir
$BuildDir   = Join-Path $ProjectDir "build"

function Write-Info  { Write-Host "[gidit] $args" -ForegroundColor Cyan }
function Write-Ok    { Write-Host "[  ok ] $args" -ForegroundColor Green }
function Write-Fail  { Write-Host "[fail] $args" -ForegroundColor Red; exit 1 }

Write-Info "Platform: Windows"

# ── Clean ──
if ($Clean) {
    Write-Info "Cleaning build directory..."
    if (Test-Path $BuildDir) { Remove-Item -Recurse -Force $BuildDir }
    Write-Ok "Clean complete"
    exit 0
}

# ── Check dependencies ──
Write-Info "Checking dependencies..."

# CMake
$cmake = Get-Command cmake -ErrorAction SilentlyContinue
if (-not $cmake) { Write-Fail "cmake not found. Install: https://cmake.org/download/" }
Write-Ok "cmake found: $($cmake.Source)"

# Visual Studio / MSVC
$vsWhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
if (Test-Path $vsWhere) {
    $vsPath = & $vsWhere -latest -property installationPath 2>$null
    if ($vsPath) { Write-Ok "Visual Studio found: $vsPath" }
} else {
    Write-Info "vswhere not found — CMake will use its own generator detection"
}

# Qt6
$Qt6Paths = @(
    $env:CMAKE_PREFIX_PATH,
    $env:Qt6_DIR,
    "C:\Qt",
    "$env:USERPROFILE\Qt"
) | Where-Object { $_ -and (Test-Path $_) }

$Qt6Found = $false
foreach ($p in $Qt6Paths) {
    $configs = Get-ChildItem -Path $p -Recurse -Filter "Qt6Config.cmake" -ErrorAction SilentlyContinue | Select-Object -First 1
    if ($configs) {
        $Qt6Dir = Split-Path -Parent (Split-Path -Parent $configs.FullName)
        $env:CMAKE_PREFIX_PATH = "$Qt6Dir;$env:CMAKE_PREFIX_PATH"
        Write-Ok "Qt6 found: $Qt6Dir"
        $Qt6Found = $true
        break
    }
}
if (-not $Qt6Found) {
    Write-Info "Qt6 not auto-detected. If build fails, set CMAKE_PREFIX_PATH."
    Write-Info "  Install Qt6: https://www.qt.io/download-qt-installer"
    Write-Info "  Then: `$env:CMAKE_PREFIX_PATH = 'C:\Qt\6.7.0\msvc2022_64'"
}

# ── Configure ──
Write-Info "Configuring ($Config)..."
$cmakeArgs = @(
    "-S", $ProjectDir,
    "-B", $BuildDir,
    "-DCMAKE_BUILD_TYPE=$Config"
)
if ($env:CMAKE_PREFIX_PATH) {
    $cmakeArgs += "-DCMAKE_PREFIX_PATH=$env:CMAKE_PREFIX_PATH"
}

& cmake @cmakeArgs
if ($LASTEXITCODE -ne 0) { Write-Fail "CMake configure failed" }

# ── Build ──
$jobs = [Environment]::ProcessorCount
Write-Info "Building with $jobs parallel jobs..."
& cmake --build $BuildDir --config $Config -j $jobs
if ($LASTEXITCODE -ne 0) { Write-Fail "Build failed" }

Write-Ok "Build complete!"

# ── Package ──
if ($Package) {
    Write-Info "Packaging..."
    Push-Location $BuildDir
    & cpack -C $Config
    Pop-Location
    Write-Ok "Package created in $BuildDir"
}

# ── Install ──
if ($Install) {
    Write-Info "Installing..."
    & cmake --install $BuildDir --config $Config
    Write-Ok "Installed"
}

# ── Output ──
$Binary = Join-Path $BuildDir "$Config\Gidit.exe"
if (-not (Test-Path $Binary)) { $Binary = Join-Path $BuildDir "Gidit.exe" }
if (Test-Path $Binary) {
    Write-Ok "Binary: $Binary"
    Write-Host ""
    Write-Info "Run with:  & '$Binary'"
}
