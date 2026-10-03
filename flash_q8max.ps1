# flash_q8max.ps1
# Flashes the custom 'alex' keymap firmware to your Q8 Max.
# Before running:
#   1. Set the keyboard's mode switch to Cable (wired).
#   2. Unplug USB, hold Esc (physical Esc key position) or use reset button,
#      plug USB back in while holding, and release.
param()

$ErrorActionPreference = "Stop"

Write-Host "Flashing 'alex' keymap to keychron/q8_max/ansi_encoder..." -ForegroundColor Cyan
qmk flash -kb keychron/q8_max/ansi_encoder -km alex

if ($LASTEXITCODE -ne 0) {
    Write-Host "Flash failed! Your keyboard should still have its old firmware." -ForegroundColor Red
    exit $LASTEXITCODE
}

Write-Host "Flash complete!" -ForegroundColor Green
