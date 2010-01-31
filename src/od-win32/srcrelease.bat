del winuae.zip
rm -rf bak
mkdir bak
cp d:/projects/winuae bak/ -r
cd bak
cd winuae
rm -rf distribution
del *.obj *.ilk *.exe *.pdb *.pch *.idb /s
cd src

del cpudefs.c
del blit.h
del blitfunc.c
del blitfunc.h
del blittable.c
del cputbl.h
del cpustbl.c
del compemu.c
del comptbl.h
del compstbl.c
del cpuemu_0.c
del cpuemu_5.c
del cpuemu_6.c

cd od-win32

cd build68k_msvc
rm -f build68k.exe build68k_msvc.plg
rm -rf debug
rm -rf release
cd ..

cd genblitter_msvc
rm -f genblitter.exe genblitter_msvc.plg
rm -rf debug
rm -rf release
cd ..

cd gencomp_msvc
rm -f gencomp.exe gencomp_msvc.plg
rm -rf debug
rm -rf release
cd ..

cd gencpu_msvc
rm -f gencpu.exe gencpu_msvc.plg
rm -rf debug
rm -rf release
cd ..

cd winuae_msvc
rm -f winuae_msvc.plg
rm -f winuae_msvc.ncb
rm -rf debug
rm -rf release
cd ..

cd miniuae
rm -f winuae_msvc.plg
rm -f winuae_msvc.ncb
rm -rf debug
rm -rf release
cd ..

cd winuae_nogui
rm -rf debug
rm -rf release
cd ..

cd soundcheck
rm -rf debug
rm -rf release
cd ..

rmdir bak
cd ..
cd ..
cd ..

zip -9 -r winuaesrc *

move winuaesrc.zip d:\amiga
