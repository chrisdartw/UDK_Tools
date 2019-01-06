/** @file
    A simple, basic, application showing how the Hello application could be
    built using the "Standard C Libraries" from StdLib.

    Copyright (c) 2010 - 2011, Intel Corporation. All rights reserved.<BR>
    This program and the accompanying materials
    are licensed and made available under the terms and conditions of the BSD License
    which accompanies this distribution. The full text of the license may be found at
    http://opensource.org/licenses/bsd-license.

    THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
**/
//#include  <Uefi.h>
//#include  <Library/UefiLib.h>
//#include  <Library/ShellCEntryLib.h>

#include  <stdio.h>
#include  <stdlib.h>
#include  <wchar.h>
#include  <time.h>

#define   PREFIX_CCDLL
#define   CC_STRICT

#if defined(PREFIX_CCDLL)
    #define prefix(func)  ccdll##func
    #define PREFIX(FUNC)  CCDLL##FUNC
    #include "src/list/extd-ccdll.h"
#elif defined(PREFIX_CCXLL)
    #define prefix(func)  ccxll##func
    #define PREFIX(FUNC)  CCXLL##FUNC
    #include "src/list/extd-ccxll.h"
#elif defined(PREFIX_CCSLL)
    #define prefix(func)  ccsll##func
    #define PREFIX(FUNC)  CCSLL##FUNC
    #include "src/list/extd-ccsll.h"
#endif

/***
  Demonstrates basic workings of the main() function by displaying a
  welcoming message.

  Note that the UEFI command line is composed of 16-bit UCS2 wide characters.
  The easiest way to access the command line parameters is to cast Argv as:
      wchar_t **wArgv = (wchar_t **)Argv;

  @param[in]  Argc    Number of argument tokens pointed to by Argv.
  @param[in]  Argv    Array of Argc pointers to command line tokens.

  @retval  0         The application exited normally.
  @retval  Other     An error occurred.
***/
#pragma warning(push)
#pragma warning(disable: 4706)
int
main (
  IN int Argc,
  IN char **Argv
  )
{
#pragma warning(push)
#pragma warning(disable: 4003)
  prefix()(int) list;
#pragma warning(pop)
  prefix(_init)(list);

  srand((unsigned)time(NULL));
  for (int cnt = 0; cnt < 10; cnt++)
    prefix(_push_back)(list, rand());

  prefix(_sort)(list);

  prefix(_iter_init)(ITER_NTH(list, prefix(_size)(list)), list, 0);

  int count = 0;
  PREFIX(_INCR)(ITER_NTH(list, prefix(_size)(list)))
    printf("list[%d] = %d\r\n", count++, LREF(ITER_NTH(list, prefix(_size)(list))));

  prefix(_free)(list);

  return EXIT_SUCCESS;
}
#pragma warning(pop)
