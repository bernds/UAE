copy d:\amiga\winuae.exe d:\projects\winuae\distribution
makensis.exe winuae
del winuae.zip
cd d:\projects\winuae\distribution
zip -9 -r d:\projects\winuae\src\od-win32\winuae.zip *
cd d:\amiga
del installwinuae*.exe
del winuae*.zip
cd d:\projects\winuae\src\od-win32
copy installwinuae.exe d:\amiga
copy winuae.zip d:\amiga
cd d:\amiga
rename installwinuae.exe InstallWinUAE0823.exe
rename winuae.zip WinUAE0823.zip
zip -9 WinUAEMini0823 winuae_mini.exe
cd d:\projects\winuae\src\od-win32
del winuae.zip
