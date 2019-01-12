//
// @file
//
//  Copyright (c) 2013 - 2014, Intel Corporation. All rights reserved.<BR>
//
//  This program and the accompanying materials
//  are licensed and made available under the terms and conditions of the BSD License
//  which accompanies this distribution.  The full text of the license may be found at
//  http://opensource.org/licenses/bsd-license.php.
//
//  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
//  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
//
//

#include <Uefi.h>
#include <PiDxe.h>
#include <IndustryStandard/Pci.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Library/UefiLib.h>
#include <Library/HiiLib.h>
#include <Library/ShellLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/PciExpressLib.h>
#include <Protocol/LegacyBios.h>

#define CPUID_VERSION_INFO                      0x01
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32  SteppingId        : 4; ///< [Bits   3:0] Stepping ID
    UINT32  Model             : 4; ///< [Bits   7:4] Model
    UINT32  FamilyId          : 4; ///< [Bits  11:8] Family
    UINT32  ProcessorType     : 2; ///< [Bits 13:12] Processor Type
    UINT32  Reserved1         : 2; ///< [Bits 15:14] Reserved
    UINT32  ExtendedModelId   : 4; ///< [Bits 19:16] Extended Model ID
    UINT32  ExtendedFamilyId  : 8; ///< [Bits 27:20] Extended Family ID
    UINT32  Reserved2         : 4; ///< Reserved
  } Bits;
  ///
  /// All bit fields as a 32-bit value
  ///
  UINT32    Uint32;
} CPUID_VERSION_INFO_EAX;
#define CPUID_BRAND_STRING1                     0x80000002
#define CPUID_BRAND_STRING2                     0x80000003
#define CPUID_BRAND_STRING3                     0x80000004
///
/// This constant defines the maximum length of the CPU brand string. According to the
/// IA manual, the brand string is in EAX through EDX (thus 16 bytes) after executing
/// the CPUID instructions with EAX as 80000002, 80000003, 80000004.
///
#define MAXIMUM_CPU_BRAND_STRING_LENGTH 48
///
/// Structure to hold the return value of AsmCpuid instruction
///
typedef struct {
  UINT32 RegEax; ///< Value of EAX.
  UINT32 RegEbx; ///< Value of EBX.
  UINT32 RegEcx; ///< Value of ECX.
  UINT32 RegEdx; ///< Value of EDX.
} EFI_CPUID_REGISTER;

typedef union {
  EFI_CPUID_REGISTER   Cpuid[3];
  CHAR8                BrandString[MAXIMUM_CPU_BRAND_STRING_LENGTH + 1];
} CPUID_BRAND_STRING;

#define LEGACY_BIOS_INT_10             0x10
#define VBE_OEM_EXTENSION_SERVICES     0x4F14
#define GET_VBIOS_VERSION_SUBFUNCTION  0x0087
#define LEGACY_BIOS_INT_15             0x15
#define INTEL_GET_VIDEO_BIOS_INFO      0x5F01

EFI_STATUS
EFIAPI
VbiosTestEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  CPUID_BRAND_STRING           CpuName      = {0};
  CPUID_VERSION_INFO_EAX       CpuVersion   = {0};
  UINT16                       GtDid        = 0;
  EFI_STATUS                   Status       = EFI_SUCCESS;
  EFI_LEGACY_BIOS_PROTOCOL    *LegacyBios   = NULL;
  EFI_IA32_REGISTER_SET        RegisterSet  = {0};
  UINT16                       BuildNum1    = 0;
  UINT16                       BuildNum2    = 0;
  UINT16                       BuildNum3    = 0;
  BOOLEAN                      Test         = FALSE;

  AsmCpuid (CPUID_BRAND_STRING1,
            &CpuName.Cpuid[0].RegEax, &CpuName.Cpuid[0].RegEbx,
            &CpuName.Cpuid[0].RegEcx, &CpuName.Cpuid[0].RegEdx);
  AsmCpuid (CPUID_BRAND_STRING2,
            &CpuName.Cpuid[1].RegEax, &CpuName.Cpuid[1].RegEbx,
            &CpuName.Cpuid[1].RegEcx, &CpuName.Cpuid[1].RegEdx);
  AsmCpuid (CPUID_BRAND_STRING3,
            &CpuName.Cpuid[2].RegEax, &CpuName.Cpuid[2].RegEbx,
            &CpuName.Cpuid[2].RegEcx, &CpuName.Cpuid[2].RegEdx);

  AsmCpuid (CPUID_VERSION_INFO, &CpuVersion.Uint32, NULL, NULL, NULL);

  Print (L"CPUID : %x , Brand : %a\n", CpuVersion.Uint32, &CpuName.BrandString[0]);

  Status = gBS->LocateProtocol (
             &gEfiLegacyBiosProtocolGuid,
             NULL,
             (VOID **) &LegacyBios
             );
  if (EFI_ERROR (Status) || (NULL == LegacyBios)) {
    Print (L"LocateProtocol gEfiLegacyBiosProtocolGuid %r\n", Status);
    return RETURN_SUCCESS;
  }


  ZeroMem (&RegisterSet, sizeof (EFI_IA32_REGISTER_SET));
  RegisterSet.X.AX = VBE_OEM_EXTENSION_SERVICES;
  RegisterSet.X.BX = GET_VBIOS_VERSION_SUBFUNCTION;
  Test = LegacyBios->Int86 (LegacyBios, LEGACY_BIOS_INT_10, &RegisterSet);
  Print (L"INT 10h: RegA/B/C/DX : %04x %04x %04x %04x T#:%d    (VBIOS provided)\n",
         RegisterSet.X.AX, RegisterSet.X.BX,
         RegisterSet.X.CX, RegisterSet.X.DX, Test);
  if (RegisterSet.X.AX == 0x004F) {
    BuildNum1 = 0;
    BuildNum1 += RegisterSet.H.CH;
    BuildNum1 *= 100;
    BuildNum1 += RegisterSet.H.CL;
  } else {
    BuildNum1 = 9999;
  }


  ZeroMem (&RegisterSet, sizeof (EFI_IA32_REGISTER_SET));
  RegisterSet.X.AX = INTEL_GET_VIDEO_BIOS_INFO;
  Test = LegacyBios->Int86 (LegacyBios, LEGACY_BIOS_INT_10, &RegisterSet);
  Print (L"INT 10h: RegA/B/C/DX : %04x %04x %04x %04x T#:%d    (VBIOS provided)\n",
         RegisterSet.X.AX, RegisterSet.X.BX,
         RegisterSet.X.CX, RegisterSet.X.DX, Test);
  if (RegisterSet.X.AX == 0x005F) {
    BuildNum2 = 0;
    BuildNum2 += ((RegisterSet.E.EBX & 0x0000000F) >>  0) * 1;
    BuildNum2 += ((RegisterSet.E.EBX & 0x00000F00) >>  8) * 10;
    BuildNum2 += ((RegisterSet.E.EBX & 0x000F0000) >> 16) * 100;
    BuildNum2 += ((RegisterSet.E.EBX & 0x0F000000) >> 24) * 1000;
  } else {
    BuildNum2 = 9999;
  }


  ZeroMem (&RegisterSet, sizeof (EFI_IA32_REGISTER_SET));
  RegisterSet.X.AX = INTEL_GET_VIDEO_BIOS_INFO;
  Test = LegacyBios->Int86 (LegacyBios, LEGACY_BIOS_INT_15, &RegisterSet);
  Print (L"INT 15h: RegA/B/C/DX : %04x %04x %04x %04x T#:%d    (SBIOS provided)\n",
         RegisterSet.X.AX, RegisterSet.X.BX,
         RegisterSet.X.CX, RegisterSet.X.DX, Test);
  if (RegisterSet.X.AX == 0x005F) {
    BuildNum3 = 0;
    BuildNum3 += (UINT8) ((RegisterSet.H.DH & 0xF) * 10 + (RegisterSet.H.DL & 0xF));
    BuildNum3 *= 100;
    BuildNum3 += (UINT8) ((RegisterSet.H.BH & 0xF) * 10 + (RegisterSet.H.BL & 0xF));
  } else {
    BuildNum3 = 9999;
  }


  GtDid = PciExpressRead16 (PCI_EXPRESS_LIB_ADDRESS (0, 2, 0, PCI_DEVICE_ID_OFFSET));
  Print (L"GtDid : %04x , Build#1 %04d , Build#2 %04d , Build#3 %04d (9999 for invalid)\n",
         GtDid, BuildNum1, BuildNum2, BuildNum3);

  return RETURN_SUCCESS;
}
