# build-q8max.ps1
# Compiles your Q8 Max firmware.
# Usage:
#   .\build-q8max.ps1              # builds the 'alex' keymap
#   .\build-q8max.ps1 -Keymap keychron   # builds a different keymap
param(
    [string]$Keymap = "alex"
)

$ErrorActionPreference = "Stop"

$Keyboard = "keychron/q8_max/ansi_encoder"

Write-Host "Compiling $Keyboard with keymap '$Keymap'..." -ForegroundColor Cyan

qmk compile -kb $Keyboard -km $Keymap

if ($LASTEXITCODE -ne 0) {
    Write-Host "Build failed!" -ForegroundColor Red
    exit $LASTEXITCODE
}

Write-Host "Build succeeded! Look for the .bin file in the repo root." -ForegroundColor Green
