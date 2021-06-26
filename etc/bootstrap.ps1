Set-Location ".."
git submodule update --init --recursive
cmake -B "build" -S "."
Write-Host -NoNewLine 'Press any key to continue...';
$null = $Host.UI.RawUI.ReadKey('NoEcho,IncludeKeyDown');
