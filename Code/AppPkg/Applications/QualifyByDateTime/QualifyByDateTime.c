/** @file
    A simple, basic, EDK II native, "hello" application to verify that
    we can build applications without LibC.

    Copyright (c) 2010 - 2011, Intel Corporation. All rights reserved.<BR>
    This program and the accompanying materials
    are licensed and made available under the terms and conditions of the BSD License
    which accompanies this distribution. The full text of the license may be found at
    http://opensource.org/licenses/bsd-license.

    THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
**/
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/ShellCEntryLib.h>
#include <Library/FileHandleLib.h>
#include <Library/ShellLib.h>
#include <Library/PrintLib.h>

#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>

/***
  Print a welcoming message.

  Establishes the main structure of the application.

  @retval  0         The application exited normally.
  @retval  Other     An error occurred.
***/
INTN
EFIAPI
ShellAppMain (
  IN UINTN Argc,
  IN CHAR16 **Argv
  )
{
  EFI_TIME          TimeBuffer        = {0};
  EFI_STATUS        Status            = EFI_SUCCESS;
  EFI_FILE_HANDLE   FileHandle        = NULL;
  UINTN             FileSize          = 0;
  CHAR16            FileName[0x200]   = {0};
  VOID              *AllocatedBuffer  = NULL;
  EFI_FILE_INFO     *FileInfo         = NULL;

  for (UINTN i = 0; i < Argc; ++i) {
    Print (L"[%d] %s\n", i, Argv[i]);
  }

  if (Argc != 3) {
    return EFI_INVALID_PARAMETER;
  }

  Status = gRT->GetTime (&TimeBuffer, NULL);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  do {

    Status = ShellOpenFileByName (Argv[2], &FileHandle, EFI_FILE_MODE_READ, 0);
    if (EFI_ERROR (Status)) {
      break;
    }

    FileInfo = FileHandleGetInfo (FileHandle);
    if (FileInfo == NULL) {
      Status = EFI_DEVICE_ERROR;
      break;
    }

    FileSize = (UINTN)FileInfo->FileSize;
    UnicodeSPrint (
      FileName,
      sizeof (FileName),
      L"%s\\%04d%02d%02d_%02d\\%04d%02d%02d_%02d%02d%02d_%s",
      Argv[1],
      TimeBuffer.Year,
      TimeBuffer.Month,
      TimeBuffer.Day,
      TimeBuffer.Hour,
      TimeBuffer.Year,
      TimeBuffer.Month,
      TimeBuffer.Day,
      TimeBuffer.Hour,
      TimeBuffer.Minute,
      TimeBuffer.Second,
      FileInfo->FileName
      );

    FreePool (FileInfo);
    FileInfo = NULL;

    Print (L"cp %s %s", Argv[2], FileName);

    AllocatedBuffer = AllocateZeroPool (FileSize);
    if (AllocatedBuffer == NULL) {
      Status = EFI_OUT_OF_RESOURCES;
      break;
    }

    Status = ShellReadFile (FileHandle, &FileSize, AllocatedBuffer);
    if (EFI_ERROR (Status)) {
      break;
    }

    Status = ShellCloseFile (&FileHandle);
    if (EFI_ERROR (Status)) {
      break;
    }
    FileHandle = NULL;

    for (UINTN Index = 0; (Index < ARRAY_SIZE (FileName)) && (FileName[Index] != L'\0'); ++Index) {
      if (FileName[Index] == L'\\') {

        if ((Index > 0) && (FileName[Index - 1] == L':')) {
          continue;
        }

        FileName[Index] = L'\0';

        //Print(L"Folder Name %s\n", FileName);

        Status = ShellOpenFileByName (FileName, &FileHandle, EFI_FILE_MODE_CREATE | EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE, EFI_FILE_DIRECTORY);
        if (EFI_ERROR (Status)) {
          break;
        }

        Status = ShellCloseFile (&FileHandle);
        if (EFI_ERROR (Status)) {
          break;
        }
        FileHandle = NULL;

        FileName[Index] = L'\\';
      }
    }
    if (EFI_ERROR (Status)) {
      break;
    }

    Status = ShellOpenFileByName (FileName, &FileHandle, EFI_FILE_MODE_CREATE | EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE, 0);
    if (EFI_ERROR (Status)) {
      break;
    }

    Status = ShellWriteFile (FileHandle, &FileSize, AllocatedBuffer);
    if (EFI_ERROR (Status)) {
      break;
    }

    Status = ShellCloseFile (&FileHandle);
    if (EFI_ERROR (Status)) {
      break;
    }
    FileHandle = NULL;

  } while (FALSE);

  if (FileInfo != NULL) {
    FreePool (FileInfo);
    FileInfo = NULL;
  }

  if (AllocatedBuffer != NULL) {
    FreePool (AllocatedBuffer);
    AllocatedBuffer = NULL;
  }

  if (FileHandle != NULL) {
    Status = ShellCloseFile (&FileHandle);
    FileHandle = NULL;
  }

  if (EFI_ERROR (Status)) {
    Print (L" ERR.\n");
  } else {
    Print (L" OK.\n");
  }

  return Status;
}
