# flash_q8max_default.ps1
# Recovery script: flashes the stock 'keychron' firmware, discarding your
# custom keymap. Use this if your custom firmware misbehaves badly.
# Same DFU-mode prep: Cable mode, hold Esc while plugging in.
param()

$ErrorActionPreference = "Stop"

Write-Host "Flashing stock 'keychron' keymap to keychron/q8_max/ansi_encoder..." -ForegroundColor Cyan
Write-Host "Make sure the keyboard is in DFU mode (hold Esc while plugging in)." -ForegroundColor Yellow

qmk flash -kb keychron/q8_max/ansi_encoder -km keychron

if ($LASTEXITCODE -ne 0) {
    Write-Host "Flash failed! The keyboard should still have its previous firmware." -ForegroundColor Red
    exit $LASTEXITCODE
}

Write-Host "Flash complete! Stock firmware restored." -ForegroundColor Green
