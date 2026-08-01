<#
.SYNOPSIS
    Converts C/C++ source files to UTF-8 without BOM and normalizes line endings to LF.

.DESCRIPTION
    Recursively scans a directory for C/C++ source files, converts Windows CRLF
    line endings to Unix LF format, and saves files as UTF-8 without BOM.

    If the directory is a Git repository (or a Git repository is specified),
    only files that are not ignored by Git are processed. Otherwise, all matching
    files are processed.

.PARAMETER Path
    Root directory to scan.

.PARAMETER Extensions
    File extensions to process.

.PARAMETER GitPath
    Path to the root of a Git repository. If omitted, the script will check if
    Path is a Git repository and use it automatically if applicable.

.EXAMPLE
    .\Convert-CppEncoding.ps1 -Path "C:\Projects\MyProject"

.EXAMPLE
    .\Convert-CppEncoding.ps1 -GitPath "C:\Projects\MyProject"

.EXAMPLE
    .\Convert-CppEncoding.ps1 -Path "." -Extensions "*.h","*.cpp"
#>

param(
    [Parameter()]
    [string]$Path = "../",

    [Parameter()]
    [string[]]$Extensions = @("*.h", "*.cpp"),

    [Parameter()]
    [string]$GitPath
)

# Resolve and confirm the target path
try {
    $resolvedPath = (Resolve-Path $Path).Path
}
catch {
    Write-Host "Path does not exist: $Path" -ForegroundColor Red
    exit 1
}

Write-Host ""
Write-Host "Target directory:"
Write-Host $resolvedPath -ForegroundColor Yellow
Write-Host ""

$confirmation = Read-Host "Is this path correct? (Y/N)"

if ($confirmation -notmatch '^(Y|y)$') {
    Write-Host "Operation cancelled."
    exit 0
}

# Use the absolute path from now on
$Path = $resolvedPath

# UTF-8 encoding without BOM
$utf8NoBom = New-Object System.Text.UTF8Encoding($false)

function Normalize-File {
    param(
        [Parameter(Mandatory)]
        [string]$FilePath
    )

    # Read file content
    $content = [System.IO.File]::ReadAllText($FilePath)

    # Normalize line endings
    $content = $content -replace "`r`n", "`n"

    # Convert tabs to 4 spaces
    $content = $content -replace "`t", "    "
    
    # Write back as UTF-8 without BOM
    [System.IO.File]::WriteAllText(
        $FilePath,
        $content,
        $utf8NoBom
    )

    Write-Host "Converted $FilePath"

    return $true
}

# Counter for converted files
$convertedCount = 0

# Determine Git repository to use
if (-not $GitPath) {
    if (Test-Path (Join-Path $Path ".git")) {
        $GitPath = $Path
    }
}

# Build file list
if ($GitPath -and (Test-Path (Join-Path $GitPath ".git"))) {

    Write-Host "Git repository detected. Ignoring files excluded by Git."

    $extensionsRegex = ($Extensions |
        ForEach-Object { [Regex]::Escape($_.TrimStart('*')) }) -join '|'

    $files = git -C $GitPath ls-files --cached --others --exclude-standard |
        Where-Object { $_ -match "($extensionsRegex)$" } |
        ForEach-Object { Join-Path $GitPath $_ }
}
else {

    Write-Host "No Git repository detected. Scanning all matching files."

    $files = Get-ChildItem `
        -Path $Path `
        -Recurse `
        -Include $Extensions `
        -File |
        Select-Object -ExpandProperty FullName
}

# Process files
foreach ($file in $files) {
    if (Normalize-File -FilePath $file) {
        $convertedCount++
    }
}

Write-Host "Conversion completed. $convertedCount file(s) converted."