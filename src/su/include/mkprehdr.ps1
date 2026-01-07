# PowerShell version of mkprehdr.sh
# Make hdr.h from segy.h
# Usage: .\mkprehdr.ps1 < segy.h (preprocessed)

param(
    [Parameter(ValueFromPipeline=$true)]
    [string]$InputText
)

# Read all input
$input = $InputText
if (-not $input) {
    $input = $input = [Console]::In.ReadToEnd()
}

# Process the input similar to sed script
$lines = $input -split "`n"
$output = @()

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
        
        # Replace type names
        $line = $line -replace "unsigned short", "u"
        $line = $line -replace "unsigned long", "v"
        $line = $line -replace "\bshort\b", "h"
        $line = $line -replace "\blong\b", "l"
        $line = $line -replace "\bfloat\b", "f"
        $line = $line -replace "\bdouble\b", "z"
        $line = $line -replace "\bint\b", "i"
        $line = $line -replace "\bchar\b", "s"
        
        # Skip unass lines
        if ($line -match "unass\[") {
            continue
        }
        
        # Extract field name (second word)
        $fields = $line -split "\s+"
        if ($fields.Length -ge 2) {
            $type = $fields[0]
            $name = $fields[1]
            $output += "`t{`"$name`",`t`"$type`",`t0},"
        }
    }
}

# Output in awk format
Write-Host "static struct {"
Write-Host "`tchar *key;`tchar *type;`tint offs;"
Write-Host "} hdr[] = {"
foreach ($line in $output) {
    Write-Host $line
}
Write-Host "};"

