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

#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Library/UefiLib.h>
#include <Library/HiiLib.h>
#include <Library/ShellLib.h>

#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

#define STALL_1_MILLI_SECOND  1000  // stall 1 ms
#define STALL_1_SECONDS       1000 * STALL_1_MILLI_SECOND

#define R_PCH_ACPI_PM1_STS                        0x00
#define B_PCH_ACPI_PM1_STS_RTC                    0x0400

#define R_PCH_ACPI_PM1_EN                         0x02
#define B_PCH_ACPI_PM1_EN_RTC                     0x0400


STATIC CONST SHELL_PARAM_ITEM LongRunParamList[] = {
  //
  //  -cr   ColdReset
  //  -wr   WarmReset
  //  -sr   ShutdownReset
  //  -pcr  PowerCycleReset   <EfiResetPlatformSpecific, unsupported>
  //  -gr   GlobalReset       <EfiResetPlatformSpecific, unsupported>
  //  -gre  GlobalResetWithEc <EfiResetPlatformSpecific, unsupported>
  //
  //  -s xx     Delay xx seconds to action
  //  -a 0x1800 PMBASE ACPI Base Address
  //  -t xx     SetWakeupTime after xx seconds
  //
  {L"/?",   TypeFlag},
  {L"-h",   TypeFlag},
  {L"-cr",  TypeFlag},
  {L"-wr",  TypeFlag},
  {L"-sr",  TypeFlag},
  {L"-pcr", TypeFlag},
  {L"-gr",  TypeFlag},
  {L"-gre", TypeFlag},

  {L"-s",   TypeValue},
  {L"-a",   TypeValue},
  {L"-t",   TypeValue},
  {L"-n",   TypeFlag},
  {NULL,    TypeMax},
};

STATIC
BOOLEAN
IsLeapYear (
  IN UINT16 Year
  )
{
  return (Year % 4 == 0) && ((Year % 100 != 0) || (Year % 400 == 0));
}

STATIC
EFI_STATUS
GetTimeAfterSeconds (
  IN       EFI_TIME       *FirstTime,
  IN       UINT32          AfterSeconds,
  IN OUT   EFI_TIME       *SecondTime
  )
{
  EFI_TIME                Time = {0};
  UINT32                  Reminder = 0;
  UINT8                   DayOfMonth = 0;
  STATIC UINT8            mDaysOfMonthInfo[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

  if ((FirstTime == NULL) || (SecondTime == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  if (AfterSeconds == 0) {
    CopyMem (SecondTime, FirstTime, sizeof (EFI_TIME));
  }

  CopyMem (&Time, FirstTime, sizeof (EFI_TIME));

  Reminder = AfterSeconds + (UINT32) Time.Second;
  Time.Second = Reminder % 60;
  Reminder = Reminder / 60;
  Reminder = Reminder + (UINT32) Time.Minute;
  Time.Minute = Reminder % 60;
  Reminder = Reminder / 60;
  Reminder = Reminder + (UINT32) Time.Hour;
  Time.Hour = Reminder % 24;
  Reminder = Reminder / 24;

  if (Reminder > 0) {
    Reminder = Reminder + (UINT32) Time.Day;
    if ((Time.Month == 2) && IsLeapYear (Time.Year)) {
      DayOfMonth = 29;
    } else {
      DayOfMonth = mDaysOfMonthInfo[Time.Month - 1];
    }
    if (Reminder > DayOfMonth) {
      Time.Day = (UINT8) Reminder - DayOfMonth;
      Reminder = 1;
    } else {
      Time.Day = (UINT8) Reminder;
      Reminder = 0;
    }
  }
  if (Reminder > 0) {
    if (Time.Month == 12) {
      Time.Month = 1;
      Time.Year = Time.Year + 1;
    } else {
      Time.Month = Time.Month + 1;
    }
  }

  CopyMem (SecondTime, &Time, sizeof (EFI_TIME));

  return EFI_SUCCESS;
}

/**
  The user Entry Point for Application. The user code starts with this function
  as the real entry point for the application.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
LongRunToolEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS              Status = EFI_SUCCESS;
  EFI_HII_HANDLE          mHiiHandle = NULL;
  LIST_ENTRY              *ParamPackage = NULL;
  BOOLEAN                 NoCommand = FALSE;
  CONST CHAR16            *Temp = NULL;
  UINTN                   ItemValue = 0;
  EFI_RESET_TYPE          ResetType = EfiResetPlatformSpecific;
  EFI_TIME                FirstTime = {0};
  EFI_TIME                SecondTime = {0};
  EFI_TIME_CAPABILITIES   Capabilities = {0};
  UINT16                  WakeSeconds = 0;
  UINT16                  AcpiBar = 0;
  UINT16                  DelaySeconds = 0;

  NoCommand = TRUE;
  mHiiHandle = HiiAddPackages (&gEfiCallerIdGuid, ImageHandle, LongRunShellToolStrings, NULL);
  Status = ShellCommandLineParseEx (LongRunParamList, &ParamPackage, NULL, TRUE, FALSE);
  if (EFI_ERROR (Status)) {
    ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_TOOLS_INVALID_INPUT), mHiiHandle);
    goto ON_EXIT;
  }

  if (ShellCommandLineGetFlag (ParamPackage, L"/?")) {
    ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_TOOLS_HELP2), mHiiHandle);
    NoCommand = FALSE;
    goto ON_EXIT;
  }
  if (ShellCommandLineGetFlag (ParamPackage, L"-h")) {
    ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_TOOLS_HELP2), mHiiHandle);
    NoCommand = FALSE;
    goto ON_EXIT;
  }
  if (ShellCommandLineGetFlag (ParamPackage, L"-a") && ShellCommandLineGetValue (ParamPackage, L"-a") == NULL) {
    ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_NO_VALUE), mHiiHandle, L"-a");
    NoCommand = FALSE;
    goto ON_EXIT;
  }
  if (ShellCommandLineGetFlag (ParamPackage, L"-s") && ShellCommandLineGetValue (ParamPackage, L"-s") == NULL) {
    ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_NO_VALUE), mHiiHandle, L"-s");
    NoCommand = FALSE;
    goto ON_EXIT;
  }
  if (ShellCommandLineGetFlag (ParamPackage, L"-t") && ShellCommandLineGetValue (ParamPackage, L"-t") == NULL) {
    ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_NO_VALUE), mHiiHandle, L"-t");
    NoCommand = FALSE;
    goto ON_EXIT;
  }
  if (ShellCommandLineGetFlag (ParamPackage, L"-s") && ShellCommandLineGetValue (ParamPackage, L"-s") != NULL) {
    if (!ShellCommandLineGetFlag (ParamPackage, L"-a")) {
      ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_GEN_TOO_FEW), mHiiHandle);
      NoCommand = FALSE;
      goto ON_EXIT;
    }
  }
  if (ShellCommandLineGetFlag (ParamPackage, L"-n")) {
    Status = gRT->GetTime (&FirstTime, &Capabilities);
    if (EFI_ERROR (Status)) {
      goto ON_EXIT;
    }
    Print (L"Now is  %04d/%02d/%02d  %02d:%02d:%02d\n",
           FirstTime.Year,
           FirstTime.Month,
           FirstTime.Day,
           FirstTime.Hour,
           FirstTime.Minute,
           FirstTime.Second
            );
    NoCommand = FALSE;
    goto ON_EXIT;
  }

  NoCommand = FALSE;
  Status = gRT->SetWakeupTime (FALSE, NULL);

  Temp = ShellCommandLineGetValue (ParamPackage, L"-s");
  if (Temp != NULL) {
    WakeSeconds = (UINT16) ShellStrToUintn (Temp);
  }
  Temp = ShellCommandLineGetValue (ParamPackage, L"-a");
  if (Temp != NULL) {
    AcpiBar = (UINT16) ShellStrToUintn (Temp);
  }
  Temp = ShellCommandLineGetValue (ParamPackage, L"-t");
  if (Temp != NULL) {
    DelaySeconds = (UINT16) ShellStrToUintn (Temp);
  }

  Status = gRT->GetTime (&FirstTime, &Capabilities);
  if (EFI_ERROR (Status)) {
    goto ON_EXIT;
  }
  Status = gRT->GetTime (&SecondTime, &Capabilities);
  if (EFI_ERROR (Status)) {
    goto ON_EXIT;
  }
  if (!EFI_ERROR (Status)) {
    Status = GetTimeAfterSeconds (&FirstTime, (UINT32) (WakeSeconds + DelaySeconds + 1), &SecondTime);
  }
  if ((WakeSeconds != 0) && (AcpiBar != 0)) {
    Print (L"Now is  %04d/%02d/%02d  %02d:%02d:%02d\n",
           FirstTime.Year,
           FirstTime.Month,
           FirstTime.Day,
           FirstTime.Hour,
           FirstTime.Minute,
           FirstTime.Second
            );
    Print (L"Wake at %04d/%02d/%02d  %02d:%02d:%02d\n",
           SecondTime.Year,
           SecondTime.Month,
           SecondTime.Day,
           SecondTime.Hour,
           SecondTime.Minute,
           SecondTime.Second
            );
    Print (L"ACPI BAR = 0x%04x\n", AcpiBar);
  }

  ResetType = EfiResetPlatformSpecific + 1;
  if (0) {
  } else if (ShellCommandLineGetFlag (ParamPackage, L"-cr")) {
    ResetType = EfiResetCold;
  } else if (ShellCommandLineGetFlag (ParamPackage, L"-wr")) {
    ResetType = EfiResetWarm;
  } else if (ShellCommandLineGetFlag (ParamPackage, L"-sr")) {
    ResetType = EfiResetShutdown;
  } else if (ShellCommandLineGetFlag (ParamPackage, L"-pcr")) {
    ResetType = EfiResetPlatformSpecific;
  } else if (ShellCommandLineGetFlag (ParamPackage, L"-gr")) {
    ResetType = EfiResetPlatformSpecific;
  } else if (ShellCommandLineGetFlag (ParamPackage, L"-gre")) {
    ResetType = EfiResetPlatformSpecific;
  }

  Temp = ShellCommandLineGetValue (ParamPackage, L"-t");
  if (Temp != NULL) {
    ItemValue = ShellStrToUintn (Temp);
    for (; ItemValue > 0; --ItemValue) {
      switch (ResetType) {
      case EfiResetCold:
        Print (L"\rRemain %d Seconds to Cold Reset...", ItemValue);
        break;
      case EfiResetWarm:
        Print (L"\rRemain %d Seconds to Warm Reset...", ItemValue);
        break;
      case EfiResetShutdown:
        Print (L"\rRemain %d Seconds to Shutdown...", ItemValue);
        break;
      case EfiResetPlatformSpecific:
        Print (L"\rRemain %d Seconds to Reset Platform Specific...", ItemValue);
        break;
      default:
        Print (L"\rRemain %d Seconds to action...", ItemValue);
        break;
      }
      gBS->Stall (STALL_1_SECONDS);
    }
    Print (L"\n");
  }

  if ((WakeSeconds != 0) && (AcpiBar != 0) && (ResetType == EfiResetShutdown)) {
    Status = gRT->SetWakeupTime (TRUE, &SecondTime);
    if (EFI_ERROR (Status)) {
      goto ON_EXIT;
    }
    ///
    /// Clear RTC PM1 status
    ///
    IoOr16 ((UINT16) AcpiBar + R_PCH_ACPI_PM1_STS, B_PCH_ACPI_PM1_STS_RTC);

    ///
    /// set RTC_EN bit in PM1_EN to wake up from the alarm
    ///
    IoOr16 ((UINT16) AcpiBar + R_PCH_ACPI_PM1_EN, B_PCH_ACPI_PM1_EN_RTC);
  }

  if (ResetType < EfiResetPlatformSpecific) {
    gRT->ResetSystem (ResetType, EFI_SUCCESS, 0, NULL);
  }

  if (NoCommand) {
    ShellPrintHiiEx (-1, -1, NULL, STRING_TOKEN (STR_TOOLS_INVALID_INPUT), mHiiHandle);
    goto ON_EXIT;
  }


ON_EXIT:
  ShellCommandLineFreeVarList (ParamPackage);
  HiiRemovePackages (mHiiHandle);
  return RETURN_SUCCESS;
}

