@echo -off

for %a run (0 9)
  if exist fs%a:\LongRunShellTool_RELEASE_X64.efi then
    fs%a:
    LongRunShellTool_RELEASE_X64 -n >>a TestLog.txt
    echo "LongRunShellTool_RELEASE_X64 -sr -s 10 -a 0x1800 -t 5"
    LongRunShellTool_RELEASE_X64 -sr -s 10 -a 0x1800 -t 5
    goto Done
  endif
endfor

#for %a run (0 9)
#  if exist fs%a:\LongRunShellTool_RELEASE_X64.efi then
#    fs%a:
#    LongRunShellTool_RELEASE_X64 -n >>a TestLog.txt
#    echo "LongRunShellTool_RELEASE_X64 -wr -s 10 -a 0x1800 -t 5"
#    LongRunShellTool_RELEASE_X64 -wr -s 10 -a 0x1800 -t 5
#    goto Done
#  endif
#endfor

# https://github.com/tianocore/edk2-ShellBinPkg
:Done
@echo -on
mm 80 01 ;IO :77