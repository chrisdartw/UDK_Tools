@echo -off

for %a run (0 9)
  if exist fs%a:\QualifyByDateTime_X64.efi then
    fs%a:
    dmpstore MemoryConfig -guid C94F8C4D-9B9A-45FE-8A55-238B67302643 -s fs%a:\MemoryConfig.bin
    QualifyByDateTime_X64 fs%a:\MrcData\Sku1 fs%a:\MemoryConfig.bin
    dmpstore MemoryConfig -guid C94F8C4D-9B9A-45FE-8A55-238B67302643 -d
    goto Done
  endif
endfor

# https://github.com/tianocore/edk2-ShellBinPkg
:Done
#@echo -on
# mm 80 01 ;IO :77
#mm 80 77 -w 1 -io
#reset -w

#reset