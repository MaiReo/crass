﻿#include "cxdec.h"

static int xcode_building_stage0(struct cxdec_xcode_status *xcode, int stage);
static int xcode_building_stage1(struct cxdec_xcode_status *xcode, int stage);

static BYTE EncryptionControlBlock[4096] = {
		0x20, 0x45, 0x6e, 0x63, 0x72, 0x79, 0x70, 0x74, 
		0x69, 0x6f, 0x6e, 0x20, 0x63, 0x6f, 0x6e, 0x74, 
		0x72, 0x6f, 0x6c, 0x20, 0x62, 0x6c, 0x6f, 0x63, 
		0x6b, 0x20, 0x2d, 0x2d, 0x20, 0x53, 0x74, 0x61, 
		0x74, 0x69, 0x63, 0x61, 0x6c, 0x6c, 0x79, 0x20, 
		0x6f, 0x72, 0x20, 0x64, 0x79, 0x6e, 0x61, 0x6d, 
		0x69, 0x63, 0x61, 0x6c, 0x6c, 0x79, 0x2c, 0x20, 
		0x64, 0x69, 0x72, 0x65, 0x63, 0x74, 0x6c, 0x79, 
		0x20, 0x6f, 0x72, 0x20, 0x69, 0x6e, 0x64, 0x69, 
		0x72, 0x65, 0x63, 0x74, 0x6c, 0x79, 0x2c, 0x20, 
		0x75, 0x73, 0x69, 0x6e, 0x67, 0x20, 0x74, 0x68, 
		0x69, 0x73, 0x20, 0x70, 0x72, 0x6f, 0x67, 0x72, 
		0x61, 0x6d, 0x20, 0x61, 0x6e, 0x64, 0x2f, 0x6f, 
		0x72, 0x20, 0x62, 0x6c, 0x6f, 0x63, 0x6b, 0x20, 
		0x66, 0x72, 0x6f, 0x6d, 0x20, 0x6f, 0x74, 0x68, 
		0x65, 0x72, 0x20, 0x70, 0x72, 0x6f, 0x67, 0x72, 
		0x61, 0x6d, 0x73, 0x20, 0x77, 0x69, 0x6c, 0x6c, 
		0x20, 0x62, 0x65, 0x20, 0x69, 0x6c, 0x6c, 0x65, 
		0x67, 0x61, 0x6c, 0x20, 0x62, 0x79, 0x20, 0x74, 
		0x68, 0x65, 0x20, 0x6c, 0x69, 0x63, 0x65, 0x6e, 
		0x73, 0x65, 0x20, 0x61, 0x67, 0x72, 0x65, 0x65, 
		0x6d, 0x65, 0x6e, 0x74, 0x2e, 0x20, 0x82, 0xb1, 
		0x82, 0xcc, 0x83, 0x76, 0x83, 0x8d, 0x83, 0x4f, 
		0x83, 0x89, 0x83, 0x80, 0x82, 0xe2, 0x83, 0x75, 
		0x83, 0x8d, 0x83, 0x62, 0x83, 0x4e, 0x82, 0xf0, 
		0x81, 0x41, 0x90, 0xc3, 0x93, 0x49, 0x82, 0xc5, 
		0x82, 0xa0, 0x82, 0xea, 0x93, 0xae, 0x93, 0x49, 
		0x82, 0xc5, 0x82, 0xa0, 0x82, 0xea, 0x81, 0x41, 
		0x92, 0xbc, 0x90, 0xda, 0x93, 0x49, 0x82, 0xc5, 
		0x82, 0xa0, 0x82, 0xea, 0x8a, 0xd4, 0x90, 0xda, 
		0x93, 0x49, 0x82, 0xc5, 0x82, 0xa0, 0x82, 0xea, 
		0x81, 0x41, 0x91, 0xbc, 0x82, 0xcc, 0x83, 0x76, 
		0x83, 0x8d, 0x83, 0x4f, 0x83, 0x89, 0x83, 0x80, 
		0x82, 0xa9, 0x82, 0xe7, 0x97, 0x70, 0x82, 0xa2, 
		0x82, 0xe9, 0x82, 0xb1, 0x82, 0xc6, 0x82, 0xcd, 
		0x83, 0x89, 0x83, 0x43, 0x83, 0x5a, 0x83, 0x93, 
		0x83, 0x58, 0x82, 0xc9, 0x82, 0xe6, 0x82, 0xe8, 
		0x8b, 0xd6, 0x82, 0xb6, 0x82, 0xe7, 0x82, 0xea, 
		0x82, 0xc4, 0x82, 0xa2, 0x82, 0xdc, 0x82, 0xb7, 
		0x81, 0x42, 0x0a, 0x82, 0xbb, 0x82, 0xea, 0x82, 
		0xcd, 0x81, 0x41, 0x97, 0x64, 0x89, 0xf6, 0x82, 
		0xc6, 0x90, 0x6c, 0x82, 0xaa, 0x8b, 0xa4, 0x82, 
		0xc9, 0x90, 0xb6, 0x82, 0xab, 0x82, 0xbd, 0x93, 
		0xfa, 0x96, 0x7b, 0x81, 0x42, 0x0a, 0x0a, 0x95, 
		0x91, 0x91, 0xe4, 0x82, 0xcd, 0x93, 0xfa, 0x96, 
		0x7b, 0x81, 0x41, 0x8e, 0x9e, 0x82, 0xcd, 0x8d, 
		0x5d, 0x8c, 0xcb, 0x81, 0x42, 0x0a, 0x97, 0x64, 
		0x82, 0xc6, 0x90, 0x6c, 0x82, 0xcd, 0x8c, 0xdd, 
		0x82, 0xa2, 0x82, 0xc9, 0x8b, 0xab, 0x8a, 0x45, 
		0x82, 0xf0, 0x95, 0xdb, 0x82, 0xbf, 0x82, 0xc2, 
		0x82, 0xc2, 0x82, 0xe0, 0x81, 0x41, 0x8e, 0x9e, 
		0x82, 0xc9, 0x8d, 0xac, 0x82, 0xb4, 0x82, 0xe8, 
		0x81, 0x41, 0x8e, 0x9e, 0x82, 0xc9, 0x91, 0x8a, 
		0x94, 0xbd, 0x82, 0xb5, 0x82, 0xc8, 0x82, 0xaa, 
		0x82, 0xe7, 0x8b, 0xa4, 0x90, 0xb6, 0x82, 0xb5, 
		0x82, 0xc4, 0x82, 0xa2, 0x82, 0xbd, 0x8e, 0x9e, 
		0x91, 0xe3, 0x81, 0x42, 0x0a, 0x97, 0x64, 0x82, 
		0xc6, 0x90, 0x6c, 0x8a, 0xd4, 0x82, 0xcc, 0x8a, 
		0xd4, 0x82, 0xc9, 0x8e, 0x59, 0x82, 0xdc, 0x82, 
		0xea, 0x82, 0xbd, 0x94, 0xbc, 0x97, 0x64, 0x81, 
		0x45, 0x97, 0x95, 0x8a, 0xdb, 0x82, 0xcd, 0x8d, 
		0x5d, 0x8c, 0xcb, 0x82, 0xcc, 0x92, 0xac, 0x82, 
		0xc5, 0x89, 0xbd, 0x82, 0xc5, 0x82, 0xe0, 0x89, 
		0xae, 0x82, 0xf0, 0x89, 0x63, 0x82, 0xf1, 0x82, 
		0xc5, 0x82, 0xa2, 0x82, 0xbd, 0x81, 0x42, 0x0a, 
		0x82, 0xbb, 0x82, 0xb1, 0x82, 0xc9, 0x82, 0xa0, 
		0x82, 0xe9, 0x93, 0xfa, 0x81, 0x75, 0x8d, 0x73, 
		0x95, 0xfb, 0x95, 0x73, 0x96, 0xbe, 0x82, 0xc9, 
		0x82, 0xc8, 0x82, 0xc1, 0x82, 0xbd, 0x95, 0x76, 
		0x82, 0xf0, 0x91, 0x7b, 0x82, 0xb5, 0x82, 0xc4, 
		0x97, 0x7e, 0x82, 0xb5, 0x82, 0xa2, 0x81, 0x76, 
		0x82, 0xc6, 0x82, 0xa2, 0x82, 0xa4, 0x88, 0xcb, 
		0x97, 0x8a, 0x82, 0xaa, 0x95, 0x91, 0x82, 0xa2, 
		0x8d, 0x9e, 0x82, 0xf1, 0x82, 0xc5, 0x82, 0xad, 
		0x82, 0xe9, 0x81, 0x42, 0x0a, 0x82, 0xbb, 0x82, 
		0xb1, 0x82, 0xc5, 0x97, 0x95, 0x8a, 0xdb, 0x82, 
		0xcd, 0x8b, 0xa4, 0x82, 0xc9, 0x95, 0xe9, 0x82, 
		0xe7, 0x82, 0xb5, 0x82, 0xc4, 0x82, 0xa2, 0x82, 
		0xe9, 0x97, 0x64, 0x81, 0x45, 0x97, 0x8b, 0x89, 
		0xa4, 0x82, 0xc6, 0x8c, 0xca, 0x94, 0x92, 0x82, 
		0xc6, 0x8b, 0xa4, 0x82, 0xc9, 0x88, 0xcb, 0x97, 
		0x8a, 0x8e, 0xe5, 0x82, 0xcc, 0x8c, 0xb3, 0x82, 
		0xc9, 0x8c, 0xfc, 0x82, 0xa9, 0x82, 0xa4, 0x81, 
		0x42, 0x0a, 0x82, 0xb7, 0x82, 0xe9, 0x82, 0xc6, 
		0x8a, 0xef, 0x96, 0xad, 0x82, 0xc8, 0x8c, 0xbb, 
		0x8f, 0xdb, 0x82, 0xaa, 0x8b, 0x4e, 0x82, 0xb1, 
		0x82, 0xc1, 0x82, 0xc4, 0x82, 0xa2, 0x82, 0xe9, 
		0x82, 0xb1, 0x82, 0xc6, 0x82, 0xf0, 0x92, 0x6d, 
		0x82, 0xe9, 0x81, 0x42, 0x0a, 0x0a, 0x2d, 0x2d, 
		0x2d, 0x82, 0xdc, 0x82, 0xe9, 0x82, 0xc5, 0x93, 
		0x81, 0x82, 0xc9, 0x96, 0xa3, 0x93, 0xfc, 0x82, 
		0xe7, 0x82, 0xea, 0x82, 0xbd, 0x82, 0xe6, 0x82, 
		0xa4, 0x82, 0xc9, 0x81, 0x41, 0x90, 0x6c, 0x82, 
		0xaa, 0x8e, 0x70, 0x82, 0xf0, 0x8f, 0xc1, 0x82, 
		0xb5, 0x81, 0x41, 0x8b, 0xb6, 0x82, 0xc1, 0x82, 
		0xc4, 0x82, 0xa2, 0x82, 0xad, 0x2d, 0x2d, 0x2d, 
		0x0a, 0x0a, 0x8d, 0x5d, 0x8c, 0xcb, 0x82, 0xc5, 
		0x8b, 0x4e, 0x82, 0xb1, 0x82, 0xc1, 0x82, 0xc4, 
		0x82, 0xa2, 0x82, 0xe9, 0x97, 0x64, 0x93, 0x81, 
		0x8e, 0x96, 0x8c, 0x8f, 0x81, 0x42, 0x0a, 0x8e, 
		0x96, 0x8c, 0x8f, 0x82, 0xf0, 0x89, 0xf0, 0x82, 
		0xab, 0x90, 0x69, 0x82, 0xdf, 0x82, 0xc4, 0x82, 
		0xa2, 0x82, 0xad, 0x82, 0xa4, 0x82, 0xbf, 0x81, 
		0x41, 0x97, 0x95, 0x8a, 0xdb, 0x82, 0xcd, 0x8e, 
		0x96, 0x8c, 0x8f, 0x82, 0xc9, 0x89, 0x42, 0x82, 
		0xb3, 0x82, 0xea, 0x82, 0xbd, 0x90, 0x5b, 0xe0, 
		0x4b, 0x82, 0xcc, 0x88, 0xc5, 0x82, 0xf0, 0x8c, 
		0xa9, 0x82, 0xe9, 0x81, 0x63, 0x81, 0x42, 0x0a, 
		0x0a, 0x0a, 0x93, 0x6f, 0x8f, 0xea, 0x90, 0x6c, 
		0x95, 0xa8, 0x0a, 0x0a, 0x0a, 0x97, 0x95, 0x8a, 
		0xdb, 0x81, 0x69, 0x82, 0xe7, 0x82, 0xf1, 0x82, 
		0xdc, 0x82, 0xe9, 0x81, 0x6a, 0x94, 0xbc, 0x97, 
		0x64, 0x0a, 0x8d, 0x5d, 0x8c, 0xcb, 0x82, 0xcc, 
		0x8a, 0x58, 0x81, 0x45, 0x90, 0xf3, 0x91, 0x90, 
		0x82, 0xc5, 0x90, 0x6c, 0x8a, 0xd4, 0x82, 0xc6, 
		0x82, 0xb5, 0x82, 0xc4, 0x81, 0x41, 0x89, 0xbd, 
		0x82, 0xc5, 0x82, 0xe0, 0x89, 0xae, 0x82, 0xf0, 
		0x89, 0x63, 0x82, 0xf1, 0x82, 0xc5, 0x95, 0xe9, 
		0x82, 0xe7, 0x82, 0xb5, 0x82, 0xc4, 0x82, 0xa2, 
		0x82, 0xe9, 0x81, 0x42, 0x0a, 0x8e, 0x4f, 0x8d, 
		0xce, 0x82, 0xcc, 0x8d, 0xa0, 0x82, 0xc9, 0x95, 
		0xea, 0x90, 0x65, 0x82, 0xaa, 0x8e, 0x80, 0x96, 
		0x53, 0x82, 0xb5, 0x81, 0x41, 0x82, 0xbb, 0x82, 
		0xea, 0x88, 0xc8, 0x97, 0x88, 0x97, 0x8b, 0x89, 
		0xa4, 0x81, 0x41, 0x8c, 0xca, 0x94, 0x92, 0x82, 
		0xf0, 0x82, 0xcd, 0x82, 0xb6, 0x82, 0xdf, 0x82, 
		0xc6, 0x82, 0xb7, 0x82, 0xe9, 0x97, 0x64, 0x92, 
		0x42, 0x82, 0xc6, 0x95, 0xe9, 0x82, 0xe7, 0x82, 
		0xb5, 0x82, 0xc4, 0x82, 0xa2, 0x82, 0xe9, 0x81, 
		0x42, 0x0a, 0x0a, 0x81, 0x75, 0x89, 0xb4, 0x82, 
		0xcd, 0x94, 0xbc, 0x95, 0xaa, 0x82, 0xc5, 0x81, 
		0x41, 0x97, 0xbc, 0x95, 0xfb, 0x82, 0xbe, 0x81, 
		0x42, 0x82, 0xbe, 0x82, 0xa9, 0x82, 0xe7, 0x82, 
		0xb1, 0x82, 0xbb, 0x95, 0xaa, 0x82, 0xa9, 0x82, 
		0xe9, 0x82, 0xb1, 0x82, 0xc6, 0x82, 0xe0, 0x82, 
		0xa0, 0x82, 0xe9, 0x81, 0x76, 0x0a, 0x0a, 0x97, 
		0x64, 0x82, 0xc6, 0x82, 0xb5, 0x82, 0xc4, 0x82, 
		0xcc, 0x94, 0x5c, 0x97, 0xcd, 0x82, 0xcd, 0x94, 
		0xea, 0x82, 0xcc, 0x89, 0x8a, 0x82, 0xf0, 0x8f, 
		0x6f, 0x82, 0xb5, 0x81, 0x41, 0x8d, 0x55, 0x8c, 
		0x82, 0x8f, 0x6f, 0x97, 0x88, 0x82, 0xe9, 0x82, 
		0xb1, 0x82, 0xc6, 0x81, 0x42, 0x0a, 0x8d, 0x44, 
		0x82, 0xab, 0x82, 0xc8, 0x82, 0xe0, 0x82, 0xcc, 
		0x82, 0xcd, 0x98, 0x61, 0x89, 0xd9, 0x8e, 0x71, 
		0x82, 0xcc, 0x8b, 0xe0, 0x92, 0xd5, 0x81, 0x42, 
		0x82, 0xa2, 0x82, 0xc2, 0x82, 0xcc, 0x8a, 0xd4, 
		0x82, 0xc9, 0x82, 0xa9, 0x8f, 0x57, 0x82, 0xdc, 
		0x82, 0xc1, 0x82, 0xc4, 0x82, 0xab, 0x82, 0xbd, 
		0x91, 0xbc, 0x82, 0xcc, 0x97, 0x64, 0x89, 0xf6, 
		0x82, 0xc9, 0x90, 0x67, 0x8e, 0x78, 0x93, 0x78, 
		0x82, 0xcc, 0x88, 0xea, 0x90, 0xd8, 0x82, 0xf0, 
		0x82, 0xb3, 0x82, 0xb9, 0x82, 0xc4, 0x82, 0xa2, 
		0x82, 0xe9, 0x81, 0x42, 0x97, 0x64, 0x89, 0xf6, 
		0x92, 0x42, 0x82, 0xcd, 0x8a, 0xec, 0x81, 0x58, 
		0x82, 0xc6, 0x82, 0xb5, 0x82, 0xc4, 0x82, 0xbb, 
		0x82, 0xea, 0x82, 0xf0, 0x82, 0xb7, 0x82, 0xe9, 
		0x82, 0xcc, 0x82, 0xc5, 0x8e, 0xc0, 0x8d, 0xdb, 
		0x82, 0xcc, 0x90, 0xb6, 0x8a, 0x88, 0x94, 0x5c, 
		0x97, 0xcd, 0x82, 0xcd, 0x92, 0xe1, 0x82, 0xa2, 
		0x81, 0x42, 0x0a, 0x8e, 0xa9, 0x95, 0xaa, 0x82, 
		0xaa, 0x94, 0xbc, 0x97, 0x64, 0x82, 0xc5, 0x81, 
		0x41, 0x82, 0xc8, 0x82, 0xa8, 0x82, 0xa9, 0x82, 
		0xc2, 0x97, 0xcd, 0x82, 0xaa, 0x82, 0xa0, 0x82, 
		0xe9, 0x8f, 0x8a, 0x88, 0xd7, 0x82, 0xa9, 0x91, 
		0xbc, 0x82, 0xcc, 0x95, 0x73, 0x8d, 0x4b, 0x82, 
		0xc8, 0x8b, 0xab, 0x8b, 0xf6, 0x82, 0xcc, 0x97, 
		0x64, 0x82, 0xcc, 0x96, 0xca, 0x93, 0x7c, 0x82, 
		0xe0, 0x8c, 0xa9, 0x82, 0xc4, 0x82, 0xa2, 0x82, 
		0xe9, 0x81, 0x42, 0x0a, 0x82, 0xa2, 0x82, 0xc2, 
		0x82, 0xe0, 0x89, 0x8c, 0x8a, 0xc7, 0x82, 0xc5, 
		0x89, 0x8c, 0x91, 0x90, 0x82, 0xf0, 0x8b, 0x7a, 
		0x82, 0xc1, 0x82, 0xc4, 0x82, 0xa2, 0x82, 0xe9, 
		0x82, 0xaa, 0x81, 0x41, 0x89, 0x8c, 0x8a, 0xc7, 
		0x82, 0xcc, 0x92, 0x86, 0x82, 0xc9, 0x82, 0xcd, 
		0x8b, 0xc9, 0x8a, 0x79, 0x92, 0xb1, 0x82, 0xc6, 
		0x82, 0xa2, 0x82, 0xa4, 0x97, 0x64, 0x82, 0xaa, 
		0x8f, 0x5a, 0x82, 0xf1, 0x82, 0xc5, 0x82, 0xa8, 
		0x82, 0xe8, 0x81, 0x41, 0x82, 0xa2, 0x82, 0xb4, 
		0x82, 0xc6, 0x8c, 0xbe, 0x82, 0xa4, 0x8e, 0x9e, 
		0x82, 0xc9, 0x82, 0xcd, 0x8f, 0xac, 0x91, 0xbe, 
		0x93, 0x81, 0x82, 0xc9, 0x95, 0xcf, 0x82, 0xed, 
		0x82, 0xe8, 0x97, 0x95, 0x8a, 0xdb, 0x82, 0xcc, 
		0x95, 0x90, 0x8a, 0xed, 0x82, 0xc6, 0x82, 0xc8, 
		0x82, 0xe9, 0x81, 0x42, 0x0a, 0x91, 0xcc, 0x8f, 
		0x70, 0x81, 0x45, 0x8c, 0x95, 0x8f, 0x70, 0x8b, 
		0xa4, 0x82, 0xc9, 0x93, 0xbe, 0x88, 0xd3, 0x81, 
		0x42, 0x0a, 0x0a, 0x0a, 0x97, 0x8b, 0x89, 0xa4, 
		0x81, 0x69, 0x82, 0xe7, 0x82, 0xa2, 0x82, 0xa8, 
		0x82, 0xa4, 0x81, 0x6a, 0x97, 0x64, 0x0a, 0x97, 
		0x95, 0x8a, 0xdb, 0x82, 0xcc, 0x95, 0xea, 0x90, 
		0x65, 0x82, 0xc9, 0x8e, 0xe5, 0x90, 0x6c, 0x8c, 
		0xf6, 0x82, 0xcc, 0x8c, 0xe3, 0x82, 0xeb, 0x8f, 
		0x82, 0x82, 0xf0, 0x97, 0x8a, 0x82, 0xdc, 0x82, 
		0xea, 0x82, 0xc4, 0x82, 0xa9, 0x82, 0xe7, 0x82, 
		0xb8, 0x82, 0xc1, 0x82, 0xc6, 0x88, 0xea, 0x8f, 
		0x8f, 0x82, 0xc9, 0x82, 0xa2, 0x82, 0xe9, 0x81, 
		0x42, 0x0a, 0x82, 0xbb, 0x82, 0xea, 0x88, 0xc8, 
		0x97, 0x88, 0x97, 0x95, 0x8a, 0xdb, 0x82, 0xcc, 
		0x8b, 0xb3, 0x88, 0xe7, 0x81, 0x41, 0x82, 0xdc, 
		0x82, 0xbd, 0x90, 0x67, 0x82, 0xcc, 0x8e, 0xfc, 
		0x82, 0xe8, 0x82, 0xcc, 0x90, 0xa2, 0x98, 0x62, 
		0x82, 0xc8, 0x82, 0xc7, 0x82, 0xf0, 0x88, 0xf8, 
		0x82, 0xab, 0x8e, 0xf3, 0x82, 0xaf, 0x82, 0xc4, 
		0x82, 0xa2, 0x82, 0xe9, 0x81, 0x42, 0x0a, 0x0a, 
		0x81, 0x75, 0x82, 0xa8, 0x91, 0x4f, 0x82, 0xcc, 
		0x95, 0xea, 0x90, 0x65, 0x82, 0xcd, 0x81, 0x41, 
		0x82, 0xa8, 0x91, 0x4f, 0x82, 0xaa, 0x90, 0x6c, 
		0x82, 0xc6, 0x82, 0xb5, 0x82, 0xc4, 0x90, 0xb6, 
		0x82, 0xab, 0x82, 0xe9, 0x82, 0xb1, 0x82, 0xc6, 
		0x82, 0xf0, 0x8a, 0xe8, 0x82, 0xc1, 0x82, 0xc4, 
		0x82, 0xa2, 0x82, 0xbd, 0x81, 0x42, 0x89, 0xb4, 
		0x82, 0xcd, 0x82, 0xbb, 0x82, 0xea, 0x82, 0xf0, 
		0x8e, 0xe7, 0x82, 0xe8, 0x91, 0xb1, 0x82, 0xaf, 
		0x82, 0xc4, 0x8d, 0x73, 0x82, 0xad, 0x82, 0xbe, 
		0x82, 0xaf, 0x82, 0xbe, 0x81, 0x76, 0x0a, 0x0a, 
		0x93, 0xac, 0x82, 0xa2, 0x82, 0xcd, 0x93, 0xf7, 
		0x92, 0x65, 0x90, 0xed, 0x82, 0xaa, 0x92, 0x86, 
		0x90, 0x53, 0x81, 0x42, 0x97, 0x8b, 0x81, 0x41, 
		0x82, 0xcc, 0x96, 0xbc, 0x82, 0xcc, 0x92, 0xca, 
		0x82, 0xe8, 0x97, 0x8b, 0x82, 0xf0, 0x93, 0x5a, 
		0x82, 0xc1, 0x82, 0xc4, 0x8d, 0x55, 0x8c, 0x82, 
		0x82, 0xb7, 0x82, 0xe9, 0x82, 0xb1, 0x82, 0xc6, 
		0x82, 0xaa, 0x91, 0xbd, 0x82, 0xa2, 0x81, 0x42, 
		0x0a, 0x96, 0x7b, 0x94, 0x5c, 0x82, 0xc5, 0x93, 
		0xae, 0x82, 0xad, 0x97, 0x64, 0x92, 0x42, 0x82, 
		0xcc, 0x92, 0x86, 0x82, 0xc9, 0x82, 0xa0, 0x82, 
		0xc1, 0x82, 0xc4, 0x8e, 0x76, 0x97, 0xb6, 0x90, 
		0x5b, 0x82, 0xad, 0x81, 0x41, 0x8d, 0x8b, 0x9a, 
		0xf8, 0x82, 0xc8, 0x8e, 0x96, 0x82, 0xf0, 0x8d, 
		0x44, 0x82, 0xdc, 0x82, 0xc8, 0x82, 0xa2, 0x8e, 
		0xbf, 0x8e, 0xc0, 0x8d, 0x84, 0x8c, 0x92, 0x82, 
		0xf0, 0x8e, 0x7c, 0x82, 0xc6, 0x82, 0xb7, 0x82, 
		0xe9, 0x90, 0xab, 0x8a, 0x69, 0x81, 0x42, 0x0a, 
		0x0a, 0x0a, 0x8c, 0xca, 0x94, 0x92, 0x28, 0x82, 
		0xb1, 0x82, 0xb5, 0x82, 0xeb, 0x29, 0x97, 0x64, 
		0x0a, 0x82, 0xa2, 0x82, 0xc2, 0x82, 0xcc, 0x8d, 
		0xa0, 0x82, 0xa9, 0x82, 0xe7, 0x82, 0xa9, 0x97, 
		0x95, 0x8a, 0xdb, 0x82, 0xc6, 0x8b, 0xa4, 0x82, 
		0xc9, 0x8b, 0x8f, 0x82, 0xe9, 0x97, 0x64, 0x81, 
		0x42, 0x0a, 0x97, 0x8b, 0x89, 0xa4, 0x82, 0xaa, 
		0x90, 0x6c, 0x8a, 0xd4, 0x82, 0xc6, 0x82, 0xb5, 
		0x82, 0xc4, 0x90, 0xb6, 0x82, 0xab, 0x82, 0xe9, 
		0x82, 0xb1, 0x82, 0xc6, 0x82, 0xf0, 0x8e, 0xe5, 
		0x90, 0x6c, 0x8c, 0xf6, 0x82, 0xc9, 0x8a, 0xa9, 
		0x82, 0xdf, 0x82, 0xe9, 0x82, 0xcc, 0x82, 0xc9, 
		0x91, 0xce, 0x82, 0xb5, 0x81, 0x41, 0x8c, 0xca, 
		0x94, 0x92, 0x82, 0xcd, 0x97, 0x64, 0x82, 0xc6, 
		0x82, 0xb5, 0x82, 0xc4, 0x90, 0xb6, 0x82, 0xab, 
		0x82, 0xe9, 0x82, 0xb1, 0x82, 0xc6, 0x82, 0xf0, 
		0x8a, 0xa9, 0x82, 0xdf, 0x82, 0xc4, 0x82, 0xa2, 
		0x82, 0xad, 0x81, 0x42, 0x0a, 0x0a, 0x81, 0x75, 
		0x82, 0xa8, 0x82, 0xe2, 0x81, 0x63, 0x81, 0x41, 
		0x82, 0xbd, 0x82, 0xbe, 0x82, 0xcc, 0x8b, 0x56, 
		0x8e, 0xae, 0x82, 0xc8, 0x82, 0xcc, 0x82, 0xc9, 
		0x81, 0x41, 0x90, 0xe3, 0x82, 0xf0, 0x93, 0xfc, 
		0x82, 0xea, 0x82, 0xe7, 0x82, 0xea, 0x82, 0xc4, 
		0x8b, 0xbb, 0x95, 0xb1, 0x82, 0xb5, 0x82, 0xbd, 
		0x82, 0xa9, 0x81, 0x48, 0x81, 0x76, 0x0a, 0x0a, 
		0x8e, 0xa9, 0x95, 0xaa, 0x82, 0xc5, 0x82, 0xcd, 
		0x82, 0xa0, 0x82, 0xdc, 0x82, 0xe8, 0x90, 0xed, 
		0x82, 0xed, 0x82, 0xb8, 0x81, 0x41, 0x8f, 0x5d, 
		0x91, 0xae, 0x82, 0xc9, 0x90, 0xed, 0x82, 0xed, 
		0x82, 0xb9, 0x82, 0xe9, 0x81, 0x42, 0x8f, 0x5d, 
		0x91, 0xae, 0x82, 0xaa, 0x8f, 0xc1, 0x96, 0xc5, 
		0x82, 0xb5, 0x82, 0xc4, 0x82, 0xe0, 0x8d, 0x5c, 
		0x82, 0xed, 0x82, 0xc8, 0x82, 0xa2, 0x82, 0xc6, 
		0x8e, 0x76, 0x82, 0xc1, 0x82, 0xc4, 0x82, 0xa2, 
		0x82, 0xe9, 0x81, 0x42, 0x0a, 0x8e, 0xa9, 0x95, 
		0xaa, 0x82, 0xcc, 0x96, 0x7b, 0x88, 0xd3, 0x82, 
		0xcd, 0x82, 0xa0, 0x82, 0xdc, 0x82, 0xe8, 0x8c, 
		0xa9, 0x82, 0xb9, 0x82, 0xb8, 0x81, 0x41, 0x97, 
		0x95, 0x8a, 0xdb, 0x82, 0xcc, 0x8c, 0xbe, 0x97, 
		0x74, 0x82, 0xc9, 0x8f, 0x5d, 0x82, 0xa4, 0x81, 
		0x42, 0x8f, 0x5d, 0x91, 0xae, 0x82, 0xb7, 0x82, 
		0xe9, 0x89, 0xf5, 0x8a, 0x79, 0x82, 0xc9, 0x82, 
		0xb5, 0x82, 0xbd, 0x82, 0xaa, 0x82, 0xc1, 0x82, 
		0xc4, 0x82, 0xa2, 0x82, 0xe9, 0x97, 0x64, 0x82, 
		0xe7, 0x82, 0xb5, 0x82, 0xa2, 0x97, 0x64, 0x81, 
		0x42, 0x0a, 0x0a, 0x0a, 0x8d, 0xf7, 0x97, 0x86, 
		0x81, 0x69, 0x82, 0xb3, 0x82, 0xad, 0x82, 0xe7, 
		0x81, 0x6a, 0x90, 0x6c, 0x8a, 0xd4, 0x0a, 0x81, 
		0x40, 0x81, 0x40, 0x92, 0xb7, 0x82, 0xa2, 0x94, 
		0xaf, 0x82, 0xf0, 0x8c, 0xe3, 0x82, 0xeb, 0x82, 
		0xc5, 0x82, 0xdc, 0x82, 0xc6, 0x82, 0xdf, 0x81, 
		0x41, 0x91, 0xe5, 0x93, 0x81, 0x82, 0xf0, 0x93, 
		0xf1, 0x96, 0x7b, 0x8d, 0xb7, 0x82, 0xb5, 0x82, 
		0xc4, 0x82, 0xa2, 0x82, 0xe9, 0x98, 0x51, 0x90, 
		0x6c, 0x95, 0x97, 0x82, 0xcc, 0x92, 0x6a, 0x81, 
		0x42, 0x0a, 0x81, 0x40, 0x95, 0x5c, 0x8f, 0xee, 
		0x82, 0xf0, 0x95, 0xcf, 0x82, 0xa6, 0x82, 0xe9, 
		0x82, 0xb1, 0x82, 0xc6, 0x82, 0xcd, 0x8f, 0xad, 
		0x82, 0xc8, 0x82, 0xad, 0x91, 0xbc, 0x8e, 0xd2, 
		0x82, 0xc6, 0x82, 0xa0, 0x82, 0xdc, 0x82, 0xe8, 
		0x8a, 0xd6, 0x82, 0xed, 0x82, 0xe7, 0x82, 0xc8, 
		0x82, 0xa2, 0x90, 0xab, 0x8a, 0x69, 0x82, 0xbe, 
		0x82, 0xaa, 0x81, 0x41, 0x88, 0xea, 0x93, 0x78, 
		0x89, 0xf9, 0x82, 0xc9, 0x93, 0xfc, 0x82, 0xea, 
		0x82, 0xbd, 0x91, 0x8a, 0x8e, 0xe8, 0x82, 0xc9, 
		0x91, 0xce, 0x82, 0xb5, 0x82, 0xc4, 0x82, 0xcd, 
		0x8f, 0xee, 0x82, 0xaa, 0x8c, 0xfa, 0x82, 0xa2, 
		0x81, 0x42, 0x0a, 0x0a, 0x0a, 0x81, 0x75, 0x81, 
		0x63, 0x8e, 0x45, 0x82, 0xb5, 0x82, 0xcd, 0x82, 
		0xb5, 0x82, 0xc8, 0x82, 0xa2, 0x81, 0x42, 0x82, 
		0xbd, 0x82, 0xbe, 0x8f, 0xc1, 0x82, 0xb7, 0x82, 
		0xbe, 0x82, 0xaf, 0x82, 0xbe, 0x81, 0x63, 0x82, 
		0xa8, 0x91, 0x4f, 0x82, 0xcc, 0x81, 0x41, 0x97, 
		0x64, 0x82, 0xcc, 0x94, 0xbc, 0x90, 0x67, 0x82, 
		0xf0, 0x81, 0x76, 0x0a, 0x0a, 0x81, 0x40, 0x93, 
		0xcb, 0x91, 0x52, 0x97, 0x95, 0x8a, 0xdb, 0x82, 
		0xc9, 0x94, 0x8d, 0x97, 0x64, 0x93, 0x81, 0x82, 
		0xf0, 0x93, 0xcb, 0x82, 0xab, 0x82, 0xc2, 0x82, 
		0xaf, 0x8b, 0x87, 0x92, 0x6e, 0x82, 0xc9, 0x97, 
		0xa7, 0x82, 0xbd, 0x82, 0xb9, 0x82, 0xe9, 0x82, 
		0xe0, 0x81, 0x41, 0x81, 0x40, 0x82, 0xac, 0x82, 
		0xe8, 0x82, 0xac, 0x82, 0xe8, 0x82, 0xcc, 0x82, 
		0xc6, 0x82, 0xb1, 0x82, 0xeb, 0x82, 0xc5, 0x8d, 
		0xa1, 0x93, 0xfa, 0x8f, 0xe5, 0x82, 0xc9, 0x8e, 
		0xd7, 0x96, 0x82, 0x82, 0xf0, 0x82, 0xb3, 0x82, 
		0xea, 0x93, 0xa6, 0x82, 0xb0, 0x82, 0xe7, 0x82, 
		0xea, 0x82, 0xe9, 0x81, 0x42, 0x89, 0xc1, 0x82, 
		0xa6, 0x82, 0xc4, 0x81, 0x75, 0x8f, 0x4f, 0x93, 
		0xb9, 0x82, 0xcc, 0x94, 0xc6, 0x8d, 0xdf, 0x8e, 
		0xd2, 0x81, 0x76, 0x8c, 0xc4, 0x82, 0xce, 0x82, 
		0xed, 0x82, 0xe8, 0x82, 0xb3, 0x82, 0xea, 0x82, 
		0xbd, 0x82, 0xb1, 0x82, 0xc6, 0x82, 0xc9, 0x95, 
		0xae, 0x8a, 0x53, 0x82, 0xb5, 0x81, 0x41, 0x8d, 
		0xa1, 0x93, 0xfa, 0x8f, 0xe5, 0x82, 0xc6, 0x82, 
		0xcd, 0x82, 0xbb, 0x82, 0xcc, 0x8c, 0xe3, 0x82, 
		0xe0, 0x8c, 0xa2, 0x89, 0x8e, 0x82, 0xcc, 0x92, 
		0x87, 0x82, 0xc6, 0x82, 0xc8, 0x82, 0xe9, 0x81, 
		0x42, 0x0a, 0x81, 0x40, 0x8e, 0x9d, 0x82, 0xc1, 
		0x82, 0xc4, 0x82, 0xa2, 0x82, 0xe9, 0x94, 0x8d, 
		0x97, 0x64, 0x93, 0x81, 0x82, 0xcd, 0x94, 0xf1, 
		0x8f, 0xed, 0x82, 0xc9, 0x97, 0xcd, 0x82, 0xc9, 
		0x88, 0xec, 0x82, 0xea, 0x82, 0xc4, 0x82, 0xa8, 
		0x82, 0xe8, 0x81, 0x41, 0x82, 0xbb, 0x82, 0xcc, 
		0x93, 0x81, 0x82, 0xf0, 0x8e, 0x67, 0x82, 0xa4, 
		0x88, 0xd7, 0x82, 0xc9, 0x82, 0xa0, 0x82, 0xe9, 
		0x8c, 0x5f, 0x96, 0xf1, 0x82, 0xf0, 0x82, 0xb5, 
		0x82, 0xc4, 0x82, 0xa2, 0x82, 0xe9, 0x81, 0x42, 
		0x8d, 0xf7, 0x97, 0x86, 0x81, 0x41, 0x82, 0xc6, 
		0x82, 0xa2, 0x82, 0xa4, 0x89, 0xd8, 0x82, 0xe2, 
		0x82, 0xa9, 0x82, 0xc8, 0x96, 0xbc, 0x82, 0xcd, 
		0x82, 0xbb, 0x82, 0xcc, 0x88, 0xd7, 0x82, 0xcc, 
		0x82, 0xe0, 0x82, 0xcc, 0x82, 0xc5, 0x82, 0xa0, 
		0x82, 0xe9, 0x81, 0x42, 0x0a, 0x81, 0x40, 0x81, 
		0x40, 0x0a, 0x0a, 0x8b, 0x76, 0x8e, 0x9c, 0x81, 
		0x40, 0x8d, 0xa1, 0x93, 0xfa, 0x8f, 0xe5, 0x0a, 
		0x81, 0x69, 0x82, 0xad, 0x82, 0xb6, 0x81, 0x40, 
		0x82, 0xab, 0x82, 0xe5, 0x82, 0xa4, 0x82, 0xb7, 
		0x82, 0xaf, 0x81, 0x6a, 0x90, 0x6c, 0x8a, 0xd4, 
		0x0a, 0x81, 0x40, 0x97, 0x95, 0x8a, 0xdb, 0x82, 
		0xcc, 0x8a, 0xeb, 0x8b, 0x40, 0x82, 0xf0, 0x93, 
		0xf1, 0x93, 0x78, 0x82, 0xc9, 0x82, 0xed, 0x82, 
		0xbd, 0x82, 0xc1, 0x82, 0xc4, 0x8f, 0x95, 0x82, 
		0xaf, 0x82, 0xc4, 0x82, 0xad, 0x82, 0xea, 0x82, 
		0xbd, 0x8e, 0x98, 0x81, 0x42, 0x82, 0xc6, 0x82, 
		0xa0, 0x82, 0xe9, 0x8a, 0xf8, 0x96, 0x7b, 0x82, 
		0xcc, 0x8e, 0x9f, 0x92, 0x6a, 0x96, 0x56, 0x82, 
		0xbe, 0x82, 0xaa, 0x81, 0x41, 0x8e, 0x9f, 0x92, 
		0x6a, 0x82, 0xcc, 0x88, 0xd7, 0x89, 0xc6, 0x93, 
		0xc2, 0x91, 0x8a, 0x91, 0xb1, 0x8c, 0xa0, 0x82, 
		0xcd, 0x8e, 0x9d, 0x82, 0xc1, 0x82, 0xc4, 0x82, 
		0xa2, 0x82, 0xc8, 0x82, 0xa2, 0x81, 0x42, 0x82, 
		0xb5, 0x82, 0xa9, 0x82, 0xb5, 0x82, 0xa2, 0x82, 
		0xc2, 0x82, 0xe0, 0x89, 0xc6, 0x91, 0xb0, 0x82, 
		0xcc, 0x8e, 0x96, 0x82, 0xf0, 0x91, 0xe5, 0x8e, 
		0x96, 0x82, 0xc9, 0x8e, 0x76, 0x82, 0xc1, 0x82, 
		0xc4, 0x82, 0xa2, 0x82, 0xe9, 0x81, 0x42, 0x0a, 
		0x81, 0x40, 0x97, 0x44, 0x82, 0xb5, 0x82, 0xb0, 
		0x82, 0xc8, 0x8a, 0xe7, 0x82, 0xc5, 0x82, 0xcd, 
		0x82, 0xa0, 0x82, 0xe9, 0x82, 0xaa, 0x81, 0x41, 
		0x93, 0xb9, 0x8f, 0xea, 0x82, 0xc5, 0x8e, 0x74, 
		0x94, 0xcd, 0x91, 0xe3, 0x82, 0xf0, 0x93, 0x77, 
		0x82, 0xdf, 0x82, 0xe9, 0x82, 0xd9, 0x82, 0xc7, 
		0x82, 0xcc, 0x8c, 0x95, 0x82, 0xcc, 0x92, 0x42, 
		0x90, 0x6c, 0x82, 0xc5, 0x81, 0x41, 0x90, 0x6c, 
		0x96, 0x5d, 0x82, 0xe0, 0x8c, 0xfa, 0x82, 0xa2, 
		0x81, 0x42, 0x0a, 0x0a, 0x81, 0x75, 0x92, 0x6a, 
		0x82, 0xf0, 0x8f, 0x50, 0x82, 0xc1, 0x82, 0xc4, 
		0x82, 0xa8, 0x93, 0xea, 0x82, 0xc9, 0x82, 0xc8, 
		0x82, 0xe9, 0x88, 0xca, 0x82, 0xc8, 0x82, 0xe7, 
		0x81, 0x41, 0x89, 0x41, 0x8a, 0xd4, 0x82, 0xc5, 
		0x82, 0xe0, 0x94, 0x83, 0x82, 0xa6, 0x82, 0xce, 
		0x1e, 0x9a, 0x24, 0xcf, 0xe2, 0x08, 0xf9, 0x0c, 
		0xdc, 0x5b, 0x16, 0x52, 0x00, 0xb7, 0x39, 0xc8, 
		0x88, 0xf9, 0x69, 0xc4, 0xbc, 0x6a, 0x7b, 0x7f, 
		0x52, 0xfb, 0xf2, 0xe7, 0xd7, 0xdc, 0xcb, 0xb2, 
		0x9d, 0x8a, 0x20, 0xe7, 0xc1, 0x78, 0xd9, 0xb3, 
		0xed, 0xac, 0xcf, 0x9d, 0xc6, 0xc8, 0x3d, 0x00, 
		0x7d, 0x86, 0xc6, 0x34, 0x92, 0x74, 0x97, 0xdb, 
		0x6b, 0x96, 0xa9, 0x74, 0xab, 0xb5, 0x6e, 0x2d, 
		0x48, 0xe8, 0xad, 0xdd, 0xbd, 0x79, 0x8c, 0x52, 
		0x9c, 0xec, 0x5a, 0x64, 0xda, 0x2f, 0x53, 0x98, 
		0x54, 0x7f, 0x08, 0x39, 0x5d, 0xe8, 0x76, 0xf1, 
		0x82, 0x4e, 0xe5, 0x18, 0x8a, 0x60, 0x2b, 0x33, 
		0x00, 0x42, 0x5c, 0x99, 0xeb, 0x08, 0x1e, 0x85, 
		0x4f, 0x8a, 0x08, 0x77, 0x28, 0x67, 0xd6, 0x83, 
		0xc7, 0x55, 0xe6, 0xa1, 0xdd, 0xb0, 0x9f, 0x6c, 
		0xf7, 0x18, 0x8e, 0xc8, 0x87, 0x1e, 0xb4, 0x9b, 
		0x3a, 0x3a, 0x53, 0xaa, 0x68, 0x76, 0xbf, 0x28, 
		0x00, 0x3b, 0x0c, 0x4d, 0xaf, 0x72, 0x34, 0x86, 
		0xa6, 0xbe, 0x75, 0xf6, 0x39, 0x50, 0x6c, 0xba, 
		0x3f, 0x67, 0x74, 0xb9, 0x4e, 0x30, 0x14, 0x7d, 
		0xed, 0x18, 0x9e, 0x6e, 0xef, 0x95, 0x47, 0x6a, 
		0xbc, 0xc6, 0x9f, 0xdf, 0x84, 0xa4, 0xaf, 0x8b, 
		0xd1, 0x02, 0x08, 0x9e, 0xd7, 0x6c, 0x4c, 0x1e, 
		0x4f, 0x06, 0xb6, 0x79, 0x9a, 0x2b, 0x65, 0xb1, 
		0xb6, 0x39, 0x4b, 0x8b, 0xa8, 0xbe, 0xce, 0xa2, 
		0x24, 0x4a, 0xfa, 0x28, 0x7e, 0x35, 0x4e, 0xd1, 
		0xc0, 0xbf, 0x86, 0x4d, 0xa7, 0xbf, 0x84, 0x30, 
		0x74, 0x54, 0x0b, 0x17, 0x40, 0x8d, 0x97, 0xfc, 
		0x46, 0x3a, 0x6b, 0x28, 0xf6, 0x9b, 0xaf, 0x79, 
		0xed, 0x63, 0x6f, 0xcd, 0xfa, 0xb3, 0xd9, 0xd8, 
		0xfd, 0x95, 0x3e, 0x80, 0x67, 0x46, 0x99, 0x4a, 
		0x7c, 0x42, 0x2c, 0x24, 0x55, 0x23, 0x2e, 0xdf, 
		0x80, 0x4f, 0xa6, 0x03, 0xf6, 0xee, 0x1e, 0x5a, 
		0x60, 0x88, 0x51, 0xc1, 0xf4, 0xae, 0x3f, 0x29, 
		0xa6, 0xfb, 0x80, 0xb3, 0xac, 0x13, 0x57, 0x1b, 
		0xca, 0x19, 0xd7, 0xda, 0x71, 0x41, 0x04, 0x22, 
		0xdc, 0xff, 0xa7, 0xba, 0x85, 0x38, 0xdf, 0x38, 
		0x5c, 0x41, 0x76, 0x40, 0x55, 0xbb, 0xc3, 0xda, 
		0xed, 0x78, 0x1f, 0x79, 0x92, 0x0c, 0x5d, 0x27, 
		0x67, 0x9f, 0x5e, 0x43, 0x03, 0xbc, 0x34, 0xf2, 
		0xf8, 0xd5, 0x6b, 0x04, 0xe6, 0x60, 0x8a, 0x96, 
		0xd0, 0xda, 0x1b, 0x84, 0xc1, 0x73, 0xda, 0xec, 
		0x98, 0xaa, 0xd4, 0xc8, 0xf2, 0x1a, 0x5d, 0x82, 
		0x5e, 0xf5, 0xe4, 0xb0, 0x4d, 0x3e, 0x9c, 0x0e, 
		0x73, 0xb1, 0x96, 0x98, 0x00, 0xb6, 0xfd, 0xdf, 
		0xf5, 0xb1, 0xde, 0xe1, 0x66, 0xc0, 0x4e, 0xa0, 
		0xb4, 0x04, 0x6e, 0xac, 0x8d, 0x2b, 0xdf, 0x5b, 
		0x7d, 0xcc, 0xa6, 0x72, 0x64, 0x88, 0xc2, 0xba, 
		0x32, 0x85, 0x8d, 0xc2, 0xc0, 0x77, 0xd9, 0x3f, 
		0xff, 0x4c, 0x65, 0x8a, 0x73, 0xf6, 0x0d, 0x2b, 
		0x4c, 0xf8, 0xaa, 0x32, 0x4a, 0x00, 0xfd, 0xde, 
		0xa8, 0x1b, 0x60, 0xf2, 0x32, 0x9c, 0xff, 0x7f, 
		0xac, 0x57, 0x7c, 0x65, 0x51, 0x66, 0x62, 0x56, 
		0x80, 0x44, 0x55, 0x4b, 0xdb, 0xb8, 0x18, 0x18, 
		0x44, 0xd1, 0x5e, 0x7d, 0x11, 0x3f, 0xa8, 0xce, 
		0x56, 0xea, 0x53, 0xfb, 0x76, 0x8f, 0x2c, 0xb7, 
		0xef, 0x86, 0x61, 0x62, 0xc7, 0x2b, 0xb4, 0x1a, 
		0x56, 0xf4, 0x6c, 0xf0, 0xf6, 0x30, 0x48, 0x94, 
		0x51, 0x6d, 0xc4, 0x43, 0x8e, 0x62, 0xe1, 0x03, 
		0x93, 0xea, 0xa4, 0xec, 0x71, 0x9c, 0xbc, 0xe3, 
		0x1a, 0x6e, 0x9d, 0x33, 0x8b, 0xc3, 0x67, 0x23, 
		0xb9, 0xfd, 0x68, 0x8c, 0xc0, 0x89, 0x35, 0x70, 
		0xff, 0x3a, 0xe6, 0x3f, 0x02, 0xe7, 0x01, 0x59, 
		0x7d, 0xdb, 0x0f, 0xaa, 0x28, 0x15, 0xa2, 0xd8, 
		0xe2, 0x44, 0x2a, 0xe6, 0xf6, 0xad, 0x31, 0x2e, 
		0xb0, 0xc2, 0xdb, 0x01, 0x3b, 0xc4, 0xbd, 0xd5, 
		0xdc, 0x81, 0xe6, 0x3f, 0x73, 0x4f, 0x22, 0x3b, 
		0xc8, 0x45, 0x8e, 0x2a, 0x04, 0x88, 0xa5, 0x2b, 
		0x8e, 0xcd, 0x6f, 0x08, 0x36, 0xbc, 0x45, 0xcd, 
		0x99, 0x3b, 0x80, 0xdb, 0xa7, 0x45, 0x13, 0x5c, 
		0x87, 0x08, 0x69, 0x26, 0xb8, 0x6e, 0xa2, 0x7f, 
		0x3b, 0x72, 0x76, 0x7a, 0xc7, 0x9f, 0xb2, 0xfc, 
		0xad, 0x17, 0xec, 0x40, 0xdf, 0xd8, 0x2b, 0x06, 
		0x56, 0x85, 0xd0, 0xa8, 0x2d, 0xc7, 0x7b, 0xe1, 
		0x4b, 0x6e, 0x16, 0x59, 0x0f, 0x81, 0x36, 0xbd, 
		0xe4, 0x6f, 0x2a, 0xa7, 0x6d, 0x53, 0x79, 0x76, 
		0xcc, 0xbe, 0x85, 0xc6, 0xc8, 0xad, 0x11, 0x1e, 
		0x0c, 0xd1, 0x0c, 0xf7, 0x4b, 0x85, 0x12, 0xa7, 
		0x70, 0x64, 0x2a, 0x54, 0xf9, 0x14, 0x23, 0xfd, 
		0xcd, 0x5d, 0xe6, 0xd6, 0x11, 0x23, 0xee, 0xb1, 
		0xa1, 0xb8, 0x6d, 0x55, 0x4c, 0x7a, 0xe9, 0x44, 
		0x74, 0x91, 0xc6, 0x7f, 0xef, 0x41, 0x34, 0x04, 
		0x04, 0xdf, 0xb6, 0x73, 0xf9, 0x38, 0x86, 0x9e, 
		0x2d, 0x05, 0xa5, 0x3e, 0x59, 0x92, 0x59, 0x04, 
		0xf5, 0xfa, 0x23, 0xbf, 0xe7, 0xa3, 0x66, 0xba, 
		0xdb, 0xce, 0x27, 0x7b, 0x5b, 0xbe, 0x03, 0xd3, 
		0x2a, 0x18, 0x5f, 0xac, 0xea, 0x05, 0x0c, 0xa5, 
		0x4c, 0xc0, 0xf9, 0x1b, 0x85, 0x76, 0x96, 0x88, 
		0x93, 0xd0, 0xe1, 0xb7, 0x7f, 0xa0, 0xe2, 0x44, 
		0xa3, 0x66, 0x29, 0x24, 0xbb, 0x94, 0xc5, 0x2f, 
		0x98, 0xdb, 0xce, 0x73, 0x77, 0x92, 0x3a, 0x67, 
		0x72, 0xac, 0xd9, 0xba, 0x16, 0x8b, 0xd5, 0x41, 
		0x1c, 0x68, 0x0c, 0x87, 0x39, 0xdd, 0x0c, 0x39, 
		0x40, 0xb7, 0x5d, 0x5f, 0x77, 0x42, 0x65, 0x0d, 
		0x99, 0x1b, 0xad, 0x83, 0x21, 0x4d, 0xd6, 0x50, 
		0xc1, 0x2b, 0x3f, 0x82, 0x92, 0xde, 0x59, 0x71, 
		0x03, 0xc9, 0x5c, 0xfa, 0x41, 0xc8, 0x11, 0x8a, 
		0x3c, 0x75, 0x26, 0x60, 0x81, 0x17, 0x11, 0x00, 
		0x09, 0x5c, 0x0d, 0x1a, 0x3e, 0x89, 0xec, 0xdf, 
		0x5b, 0xb3, 0x26, 0xa7, 0x53, 0x02, 0x3c, 0x1f, 
		0x92, 0xf5, 0xb9, 0xa2, 0x46, 0x36, 0xba, 0x24, 
		0x35, 0x6a, 0xfb, 0xa1, 0x9c, 0xe9, 0xba, 0xf5, 
		0xdd, 0x69, 0x65, 0x0b, 0xf6, 0xeb, 0x28, 0x96, 
		0x77, 0xde, 0xa1, 0x37, 0x86, 0x8f, 0x5a, 0xb9, 
		0x15, 0x4b, 0x92, 0x12, 0xb9, 0xcf, 0x4d, 0x0e, 
		0x0d, 0xf0, 0x59, 0xa0, 0xdf, 0xdf, 0x27, 0x17, 
		0xcb, 0x9b, 0x2c, 0x0c, 0x46, 0x9d, 0x12, 0x0d, 
		0xf1, 0xdb, 0x14, 0x00, 0xad, 0xee, 0xd1, 0x3d, 
		0x9b, 0x6a, 0x39, 0xf3, 0x72, 0x8c, 0x68, 0xe1, 
		0xa6, 0x39, 0x3e, 0x6c, 0x37, 0x37, 0xb2, 0x6a, 
		0x8b, 0xa9, 0xde, 0x67, 0x2f, 0x9b, 0xac, 0xee, 
		0x29, 0x1d, 0x7b, 0x26, 0x1d, 0xa0, 0xe6, 0x30, 
		0x4a, 0x72, 0xe6, 0x51, 0x6b, 0x81, 0xcd, 0x88, 
		0x0c, 0x78, 0x8e, 0xfc, 0x63, 0xf1, 0x88, 0xc3, 
		0x2e, 0xc3, 0x8d, 0x6c, 0x75, 0x7a, 0x15, 0xe9, 
		0x41, 0xd8, 0x53, 0x9f, 0x82, 0x64, 0x33, 0x5a, 
		0xd8, 0x6d, 0x53, 0xcc, 0x24, 0x7e, 0x01, 0x47, 
		0x5c, 0x36, 0x42, 0xdb, 0x78, 0x22, 0x1d, 0xbc, 
		0x94, 0xf5, 0xcc, 0x7b, 0x40, 0x0c, 0x39, 0x70, 
		0x71, 0x57, 0x9d, 0xaa, 0xd8, 0x14, 0xd0, 0x56, 
		0x94, 0x34, 0xbe, 0x8a, 0x5f, 0x6c, 0x4c, 0x20, 
		0x36, 0x0e, 0xc3, 0x13, 0x5c, 0xb3, 0xa4, 0x43, 
		0x75, 0xdb, 0x83, 0x0d, 0x90, 0xbd, 0x1d, 0x4a, 
		0x84, 0x39, 0xbd, 0x51, 0x71, 0x28, 0xf1, 0xe4, 
		0x5c, 0x2a, 0x7b, 0xc2, 0xf3, 0x87, 0xcb, 0xed, 
		0x11, 0x62, 0x11, 0x79, 0xcc, 0x5f, 0x2b, 0x1b, 
		0x59, 0x57, 0xc2, 0x9b, 0x23, 0xca, 0xc8, 0x61, 
		0x58, 0x9f, 0xfe, 0x4c, 0xa5, 0x31, 0xc7, 0x99, 
};

static int xcode_building_first_stage(struct cxdec_xcode_status *xcode)
{
  	switch (xcode_rand(xcode) % 3) {
  	case 1:
		// MOV ESI, EncryptionControlBlock
		// MOV EAX, DWORD PTR DS:[ESI+((xcode_rand(xcode) & 0x3ff) << 2)]
		if (!push_bytexcode(xcode, 0xbe)
				|| !push_dwordxcode(xcode, (DWORD)EncryptionControlBlock)
				|| !push_2bytesxcode(xcode, 0x8b, 0x86)
				|| !push_dwordxcode(xcode, (xcode_rand(xcode) & 0x3ff) << 2))
			return 0;
		break;
	case 2:
  		// MOV EAX, EDI
  		if (!push_2bytesxcode(xcode, 0x8b, 0xc7))
  			return 0;
		break;
  	case 0:
		// MOV EAX, xcode_rand(xcode)
		if (!push_bytexcode(xcode, 0xb8)
				|| !push_dwordxcode(xcode, xcode_rand(xcode)))
			return 0;
  		break;
  	}
  	return 1;
}

static int xcode_building_stage0(struct cxdec_xcode_status *xcode, int stage)
{
	if (stage == 1)
		return xcode_building_first_stage(xcode);

    if (xcode_rand(xcode) & 1) {
		if (!xcode_building_stage1(xcode, stage - 1))
			return 0;
    } else {
		if (!xcode_building_stage0(xcode, stage - 1))
			return 0;
    }

    switch (xcode_rand(xcode) & 7) {
	case 2:
		// NOT EAX
		if (!push_2bytesxcode(xcode, 0xf7, 0xd0))
			return 0;
        break;
	case 6:
		// MOV ESI, EncryptionControlBlock
		// AND EAX, 3FFh
		// MOV EAX, DWORD PTR DS:[ESI+EAX*4]
        if (!push_bytexcode(xcode, 0xbe)
  				|| !push_dwordxcode(xcode, (DWORD)EncryptionControlBlock)
				|| !push_bytexcode(xcode, 0x25)
				|| !push_dwordxcode(xcode, 0x3ff)
				|| !push_3bytesxcode(xcode, 0x8b, 0x04, 0x86))
			return 0;
        break;
	case 1:
		// DEC EAX
		if (!push_bytexcode(xcode, 0x48))
			return 0;
        break;
	case 0:
		// NEG EAX
        if (!push_2bytesxcode(xcode, 0xf7, 0xd8))
			return 0;
		break;
	case 7:
        if (xcode_rand(xcode) & 1) {
        	// ADD EAX, xcode_rand(xcode)
			if (!push_bytexcode(xcode, 0x05))
            	return 0;
        } else {
        	// SUB EAX, xcode_rand(xcode)
			if (!push_bytexcode(xcode, 0x2d))
            	return 0;
        }
        if (!push_dwordxcode(xcode, xcode_rand(xcode)))
        	return 0;
        break;
	case 4:
		// PUSH EBX
		// MOV EBX, EAX
		// AND EBX, AAAAAAAA
		// AND EAX, 55555555
		// SHR EBX, 1
		// SHL EAX, 1
		// OR EAX, EBX
		// POP EBX
		if (!push_bytexcode(xcode, 0x53)
				|| !push_2bytesxcode(xcode, 0x89, 0xc3)
				|| !push_6bytesxcode(xcode, 0x81, 0xe3, 0xaa, 0xaa, 0xaa, 0xaa)
				|| !push_5bytesxcode(xcode, 0x25, 0x55, 0x55, 0x55, 0x55)
				|| !push_2bytesxcode(xcode, 0xd1, 0xeb)
				|| !push_2bytesxcode(xcode, 0xd1, 0xe0)
				|| !push_2bytesxcode(xcode, 0x09, 0xd8)
				|| !push_bytexcode(xcode, 0x5b))
			return 0;
		break;
	case 3:
		// INC EAX
		if (!push_bytexcode(xcode, 0x40))
			return 0;
		break;
	case 5:
		// XOR EAX, xcode_rand(xcode)
		if (!push_bytexcode(xcode, 0x35) 
				|| !push_dwordxcode(xcode, xcode_rand(xcode)))
			return 0;
        break;
	}
	return 1;
}

static int xcode_building_stage1(struct cxdec_xcode_status *xcode, int stage)
{
	if (stage == 1)
		return xcode_building_first_stage(xcode);

	// PUSH EBX
	if (!push_bytexcode(xcode, 0x53))
		return 0;

	if (xcode_rand(xcode) & 1) {
		if (!xcode_building_stage1(xcode, stage - 1))
			return 0;
	} else {
		if (!xcode_building_stage0(xcode, stage - 1))
			return 0;
	}

	// MOV EBX, EAX
	if (!push_2bytesxcode(xcode, 0x89, 0xc3))
		return 0;

	if (xcode_rand(xcode) & 1) {
		if (!xcode_building_stage1(xcode, stage - 1))
			return 0;
	} else {
		if (!xcode_building_stage0(xcode, stage - 1))
			return 0;
	}

	switch (xcode_rand(xcode) % 6) {
	case 0:
		// ADD EAX, EBX
		if (!push_2bytesxcode(xcode, 0x01, 0xd8))
			return 0;
		break;
    case 1:
		// PUSH ECX
		// MOV ECX, EBX
		// AND ECX, 0F
		// SHR EAX, CL
		// POP ECX
		if (!push_bytexcode(xcode, 0x51)
				|| !push_2bytesxcode(xcode, 0x89, 0xd9)
				|| !push_3bytesxcode(xcode, 0x83, 0xe1, 0x0f)
				|| !push_2bytesxcode(xcode, 0xd3, 0xe8)
				|| !push_bytexcode(xcode, 0x59))
			return 0;
		break;
    case 3:
    	// PUSH ECX
    	// MOV ECX, EBX
    	// AND ECX, 0F
    	// SHL EAX, CL
    	// POP ECX
		if (!push_bytexcode(xcode, 0x51) 
				|| !push_2bytesxcode(xcode, 0x89, 0xd9)
				|| !push_3bytesxcode(xcode, 0x83, 0xe1, 0x0f)
				|| !push_2bytesxcode(xcode, 0xd3, 0xe0)
				|| !push_bytexcode(xcode, 0x59))
			return 0;
		break;
    case 5:
    	// NEG EAX, ADD EAX, EBX
		if (!push_2bytesxcode(xcode, 0xf7, 0xd8) 
				|| !push_2bytesxcode(xcode, 0x01, 0xd8))
			return 0;
		break;
    case 2:
    	// IMUL EAX, EBX
		if (!push_3bytesxcode(xcode, 0x0f,  0xaf, 0xc3))
			return 0;
		break;
    case 4:
    	// SUB EAX, EBX
		if (!push_2bytesxcode(xcode, 0x29, 0xd8))
			return 0;
		break;
	}
	// POP EBX
	return push_bytexcode(xcode, 0x5b);
}

struct cxdec_callback beniten_cxdec_callback = {
	"beniten",
	{ 0x15b, 0x660 },
	xcode_building_stage1
};
;
