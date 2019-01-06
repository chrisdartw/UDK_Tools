
README
===========================
Collect some EFI tools here.

****

## Index
* [Requirement](#requirement)
* [Installation](#installation)
* [Modification](#modification)


### Requirement
___
[Python 2.7.15  x86 ](https://www.python.org/ftp/python/2.7.15/python-2.7.15.msi)
[Python 2.7.15 amd64](https://www.python.org/ftp/python/2.7.15/python-2.7.15.amd64.msi)

[vs2015 pro_enu u3](http://download.microsoft.com/download/e/b/c/ebc2c43f-3821-4a0b-82b1-d05368af1604/vs2015.3.pro_enu.iso)
[vs2015 ent_enu u3](http://download.microsoft.com/download/8/4/3/843ec655-1b67-46c3-a7a4-10a1159cfa84/vs2015.3.ent_enu.iso)
[vs2015 com_enu u3](http://download.microsoft.com/download/b/e/d/bedddfc4-55f4-4748-90a8-ffe38a40e89f/vs2015.3.com_enu.iso)

[vs2015 pro_cht u3](http://download.microsoft.com/download/6/c/f/6cfe65ad-a72a-4828-8477-6abf91daa920/vs2015.3.pro_cht.iso)
[vs2015 ent_cht u3](http://download.microsoft.com/download/9/d/f/9df3e0f6-519d-43ac-952c-ff1d3bd5e486/vs2015.3.ent_cht.iso)
[vs2015 com_cht u3](http://download.microsoft.com/download/7/6/d/76dd809a-d4ae-4e0e-9a24-ad55576e5c8a/vs2015.3.com_cht.iso)

[UDK2018](https://github.com/tianocore/edk2/archive/vUDK2018.zip)
[Edk2-BaseTools-win32](https://github.com/tianocore/edk2-BaseTools-win32/archive/master.zip)
[Nasm 2.14.02](https://www.nasm.us/pub/nasm/releasebuilds/2.14.02/nasm-2.14.02.zip)
[iasl 20181213](https://acpica.org/sites/acpica/files/iasl-win-20181213.zip)


### Installation
___
![vs2015](http://www.lab-z.com/wp-content/uploads/2018/05/vs2015.png "vs2015")
![sdk](http://www.lab-z.com/wp-content/uploads/2018/05/sdk.png "sdk")


### Modification
___
> \StdLib\Include\sys\EfiCdefs.h
```diff
+     #define __STDC_HOSTED__     1
- //  #define __STDC_HOSTED__     1
```
