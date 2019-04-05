/** @file
  OSTA Universal Disk Format (UDF) definitions.

  Copyright (C) 2014-2017 Paulo Alcantara <pcacjr@zytor.com>

  This program and the accompanying materials are licensed and made available
  under the terms and conditions of the BSD License which accompanies this
  distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS, WITHOUT
  WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
**/

#ifndef __UDF_H__
#define __UDF_H__

#pragma pack(push,1)

#define UDF_BEA_IDENTIFIER   "BEA01"
#define UDF_NSR2_IDENTIFIER  "NSR02"
#define UDF_NSR3_IDENTIFIER  "NSR03"
#define UDF_TEA_IDENTIFIER   "TEA01"

#define UDF_LOGICAL_SECTOR_SHIFT  11
#define UDF_LOGICAL_SECTOR_SIZE   ((UINT64)(1ULL << UDF_LOGICAL_SECTOR_SHIFT))
#define UDF_VRS_START_OFFSET      ((UINT64)(16ULL << UDF_LOGICAL_SECTOR_SHIFT))

typedef enum {
  UdfPrimaryVolumeDescriptor          = 1,
  UdfAnchorVolumeDescriptorPointer    = 2,
  UdfVolumeDescriptorPointer          = 3,
  UdfImplemenationUseVolumeDescriptor = 4,
  UdfPartitionDescriptor              = 5,
  UdfLogicalVolumeDescriptor          = 6,
  UdfUnallocatedSpaceDescriptor       = 7,
  UdfTerminatingDescriptor            = 8,
  UdfLogicalVolumeIntegrityDescriptor = 9,
  UdfFileSetDescriptor                = 256,
  UdfFileIdentifierDescriptor         = 257,
  UdfAllocationExtentDescriptor       = 258,
  UdfFileEntry                        = 261,
  UdfExtendedFileEntry                = 266,
} UDF_VOLUME_DESCRIPTOR_ID;

typedef struct {
  UINT16  TagIdentifier;               // 00 02
  UINT16  DescriptorVersion;           // 02 02
  UINT8   TagChecksum;                 // 04 01
  UINT8   Reserved;                    // 05 01
  UINT16  TagSerialNumber;             // 06 02
  UINT16  DescriptorCRC;               // 08 02
  UINT16  DescriptorCRCLength;         // 0a 02
  UINT32  TagLocation;                 // 0c 04
} UDF_DESCRIPTOR_TAG;

typedef struct {
  UINT32  ExtentLength;                // 00 04
  UINT32  ExtentLocation;              // 04 04
} UDF_EXTENT_AD;

typedef struct {
  UINT8           CharacterSetType;       // 00 01
  UINT8           CharacterSetInfo[63];   // 01 63
} UDF_CHAR_SPEC;

typedef struct {
  UINT8           Flags;                    // 00 01
  UINT8           Identifier[23];           // 01 23
  union {
    //
    // Domain Entity Identifier
    //
    struct {
      UINT16      UdfRevision;              // 18 02
      UINT8       DomainFlags;              // 1a 01
      UINT8       Reserved[5];              // 1b 05
    } Domain;
    //
    // UDF Entity Identifier
    //
    struct {
      UINT16      UdfRevision;              // 18 02
      UINT8       OSClass;                  // 1a 01
      UINT8       OSIdentifier;             // 1b 01
      UINT8       Reserved[4];              // 1c 04
    } Entity;
    //
    // Implementation Entity Identifier
    //
    struct {
      UINT8       OSClass;                  // 18 01
      UINT8       OSIdentifier;             // 19 01
      UINT8       ImplementationUseArea[6]; // 1a 06
    } ImplementationEntity;
    //
    // Application Entity Identifier
    //
    struct {
      UINT8       ApplicationUseArea[8];    // 18 08
    } ApplicationEntity;
    //
    // Raw Identifier Suffix
    //
    struct {
      UINT8       Data[8];                  // 18 08
    } Raw;
  } Suffix;
} UDF_ENTITY_ID;

typedef struct {
  UINT32        LogicalBlockNumber;        // 00 04
  UINT16        PartitionReferenceNumber;  // 04 02
} UDF_LB_ADDR;

typedef struct {
  UINT32                           ExtentLength;          // 00 04
  UDF_LB_ADDR                      ExtentLocation;        // 04 06
  UINT8                            ImplementationUse[6];  // 0a 06
} UDF_LONG_ALLOCATION_DESCRIPTOR;

typedef struct {
  UDF_DESCRIPTOR_TAG  DescriptorTag;
  UDF_EXTENT_AD       MainVolumeDescriptorSequenceExtent;        // 10  08
  UDF_EXTENT_AD       ReserveVolumeDescriptorSequenceExtent;     // 18  08
  UINT8               Reserved[480];                             // 20 1e0
} UDF_ANCHOR_VOLUME_DESCRIPTOR_POINTER;

typedef struct {
  UDF_DESCRIPTOR_TAG              DescriptorTag;                    //  00 10
  UINT32                          VolumeDescriptorSequenceNumber;   //  10 04
  UDF_CHAR_SPEC                   DescriptorCharacterSet;           //  14 40
  UINT8                           LogicalVolumeIdentifier[128];     //  54 80
  UINT32                          LogicalBlockSize;                 //  d4 04
  UDF_ENTITY_ID                   DomainIdentifier;                 //  d8 20
  UDF_LONG_ALLOCATION_DESCRIPTOR  LogicalVolumeContentsUse;         //  f8 10
  UINT32                          MapTableLength;                   // 108 04
  UINT32                          NumberOfPartitionMaps;            // 10c 04
  UDF_ENTITY_ID                   ImplementationIdentifier;         // 110 20
  UINT8                           ImplementationUse[128];           // 130 80
  UDF_EXTENT_AD                   IntegritySequenceExtent;          // 1b0 08
  UINT8                           PartitionMaps[6];                 // 1b8 06
} UDF_LOGICAL_VOLUME_DESCRIPTOR;

#pragma pack(pop)

#endif
