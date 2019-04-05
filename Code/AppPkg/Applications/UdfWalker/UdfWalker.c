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
#include  <Uefi.h>
#include  <Library/UefiLib.h>
#include  <Library/ShellCEntryLib.h>
#include  <Library/ShellLib.h>
#include  <Library/MemoryAllocationLib.h>
#include <Library/FileHandleLib.h>

#include "FileSystemOperations.h"

#define SafeFree(ptr)   do {if ((ptr) != NULL) {FreePool(ptr);(ptr) = NULL;}} while (0)

EFI_STATUS
DirectoryWalker (
  IN   EFI_FILE_HANDLE                 FileHandle,
  IN   UDF_VOLUME_INFO                 *Volume,
  IN   UDF_FILE_INFO                   *Parent,
  IN   UDF_LONG_ALLOCATION_DESCRIPTOR  *Icb,
  IN   CONST UINTN                     CharMax,
  IN   CONST CHAR16                    *String
  )
{
  EFI_STATUS               Status = EFI_SUCCESS;
  UDF_READ_DIRECTORY_INFO  ReadDirInfo = {0};
  UDF_FILE_INFO            ReadFile = {0};
  CHAR16                   FoundFileName[UDF_FILENAME_LENGTH] = {0};
  UINT64                   Size = 0;
  UINT64                   Lsn = 0;

  for (;;) {
    UDF_FILE_IDENTIFIER_DESCRIPTOR  *FileIdentifierDesc = NULL;
    Status = ReadDirectoryEntry (
               FileHandle,
               Volume,
               Icb,
               Parent->FileEntry,
               &ReadDirInfo,
               &FileIdentifierDesc
               );
    if (EFI_ERROR (Status)) {
      if (Status == EFI_DEVICE_ERROR) {
        Status = EFI_NOT_FOUND;
      }
      break;
    }
    if (0 == (FileIdentifierDesc->FileCharacteristics & PARENT_FILE)) {
      Status = GetFileNameFromFid (FileIdentifierDesc, ARRAY_SIZE (FoundFileName), FoundFileName);
      if (EFI_ERROR (Status)) {
        break;
      }

      ReadFile.FileIdentifierDesc = FileIdentifierDesc;
      Status = FindFileEntry (
                 FileHandle,
                 Volume,
                 &FileIdentifierDesc->Icb,
                 &ReadFile.FileEntry
                 );
      if (EFI_ERROR (Status)) {
        break;
      }

      Status = GetFileSize (FileHandle, Volume, &ReadFile, &Size, &Lsn);
      if (EFI_ERROR (Status)) {
        break;
      }

      CHAR16 *String2 = (CHAR16 *)AllocateCopyPool (CharMax * sizeof (*String2), String);
      if (NULL == String2) {
        Status = EFI_OUT_OF_RESOURCES;
        break;
      }
      if (0 != StrnLenS (String2, CharMax)) {
        StrCpyS (String2, CharMax, L"\\");
      }
      StrCpyS (String2, CharMax, FoundFileName);

      if (FileIdentifierDesc->FileCharacteristics & DIRECTORY_FILE) {
        Print (L"[%8s %10s] %s\n", L"", L"", String2);
        Status = DirectoryWalker (FileHandle, Volume, &ReadFile, &FileIdentifierDesc->Icb, CharMax, String2);
      } else {
        Print (L"[%8ld %10ld] %s\n", Lsn, Size, String2);
        Status = EFI_SUCCESS;
      }
      SafeFree (String2);
      if (EFI_ERROR (Status)) {
        break;
      }

      SafeFree (ReadFile.FileEntry);
    }

    SafeFree (FileIdentifierDesc);
  }

  SafeFree (ReadDirInfo.DirectoryData);
  return (Status == EFI_NOT_FOUND ? EFI_SUCCESS : Status);
}

/***
  Print a welcoming message.

  Establishes the main structure of the application.

  @retval  0         The application exited normally.
  @retval  Other     An error occurred.
***/
INTN
EFIAPI
ShellAppMain (
  IN UINTN        Argc,
  IN CHAR16       **Argv
  )
{
  EFI_STATUS        Status = EFI_SUCCESS;
  EFI_FILE_HANDLE   FileHandle;

  Print (L"We have %d arguments:\n", Argc);
  for (UINTN i = 0; i < Argc; ++i) {
    Print (L"[%d] %s\n", i, Argv[i]);
  }

  if (Argc < 2) {
    Print (L"No Argc!\n");
    return EFI_SUCCESS;
  }

  //Open the file given by the parameter
  Status = ShellOpenFileByName (Argv[1],
                                (SHELL_FILE_HANDLE *)&FileHandle,
                                EFI_FILE_MODE_READ,
                                0);

  if (Status != RETURN_SUCCESS) {
    Print (L"OpenFile failed!\n");
    return EFI_SUCCESS;
  }

  do {
    UINT64            FileSize = 0;
    UDF_VOLUME_INFO   Volume = { 0 };
    UINT64            VolumeSize = 0;
    UINT64            FreeSpaceSize = 0;
    CHAR16            VolumeLabel[64] = { 0 };
    UDF_FILE_INFO     File = { 0 };
    UDF_FILE_INFO     Parent = { 0 };

    //Get file size
    Status = FileHandleGetSize (FileHandle, &FileSize);
    if (EFI_ERROR (Status)) {
      break;
    }

    Status = ReadUdfVolumeInformation (FileHandle, &Volume);
    if (EFI_ERROR (Status)) {
      break;
    }
    Status = GetVolumeSize (FileHandle, &Volume, &VolumeSize, &FreeSpaceSize);
    if (EFI_ERROR (Status)) {
      break;
    }
    Status = GetVolumeLabel (&Volume, ARRAY_SIZE (VolumeLabel), VolumeLabel);
    if (EFI_ERROR (Status)) {
      break;
    }
    Print (L"File Name    : %s\n", Argv[1]);
    Print (L"File Size    : %ld\n", FileSize);
    Print (L"Volume Label : %s\n", VolumeLabel);

    Status = FindRootDirectory (FileHandle, &Volume, &File);
    if (EFI_ERROR (Status)) {
      break;
    }

    Parent.FileEntry = File.FileEntry;
    Status = DirectoryWalker (FileHandle,
                              &Volume,
                              &Parent,
                              &Volume.FileSetDesc.RootDirectoryIcb,
                              UDF_PATH_LENGTH,
                              L"");

    CleanupFileInformation (&File);
  } while (0);

  FileHandleClose (FileHandle);
  return EFI_SUCCESS;
}
