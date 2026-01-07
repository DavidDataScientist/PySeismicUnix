# PowerShell version of mkoffs.sh
# Get offset.h for mkhdr.c from segy.h
# Usage: .\mkoffs.ps1 < segy.h (preprocessed)

param(
    [Parameter(ValueFromPipeline=$true)]
    [string]$InputText
)

# Read all input
$input = $InputText
if (-not $input) {
    $input = [Console]::In.ReadToEnd()
}

# Process the input similar to sed script
$lines = $input -split "`n"
$output = @()
$i = 1

foreach ($line in $lines) {
    # Match lines between "int tracl" and "unass["
    if ($line -match "int tracl") {
        $inRange = $true
    }
    if ($line -match "unass\[") {
        $inRange = $false
        continue
    }
    
    if ($inRange -and $line -match ";") {
        # Remove semicolon and everything after
        $line = $line -replace ";.*", ""
        
        # Skip tracl and unass lines
        if ($line -match "tracl|unass\[") {
            continue
        }
        
        # Extract field name (last word)
        $fields = $line -split "\s+"
        if ($fields.Length -ge 1) {
            $field = $fields[$fields.Length - 1]
            Write-Host "`thdr[$i].offs ="
            Write-Host "`t`toffsetof(segy, $field);"
            $i++
        }
    }
}

