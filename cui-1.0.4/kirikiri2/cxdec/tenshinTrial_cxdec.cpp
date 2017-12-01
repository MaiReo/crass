#include "cxdec.h"

static int xcode_building_stage0(struct cxdec_xcode_status *xcode, int stage);
static int xcode_building_stage1(struct cxdec_xcode_status *xcode, int stage);

static BYTE EncryptionControlBlock[4096] = {
		0x20, 0x45, 0x6e, 0x63, 0x72, 0x79, 0x70, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x63, 0x6f, 0x6e, 0x74, 
		0x72, 0x6f, 0x6c, 0x20, 0x62, 0x6c, 0x6f, 0x63, 0x6b, 0x20, 0x2d, 0x2d, 0x20, 0x53, 0x74, 0x61, 
		0x74, 0x69, 0x63, 0x61, 0x6c, 0x6c, 0x79, 0x20, 0x6f, 0x72, 0x20, 0x64, 0x79, 0x6e, 0x61, 0x6d, 
		0x69, 0x63, 0x61, 0x6c, 0x6c, 0x79, 0x2c, 0x20, 0x64, 0x69, 0x72, 0x65, 0x63, 0x74, 0x6c, 0x79, 
		0x20, 0x6f, 0x72, 0x20, 0x69, 0x6e, 0x64, 0x69, 0x72, 0x65, 0x63, 0x74, 0x6c, 0x79, 0x2c, 0x20, 
		0x75, 0x73, 0x69, 0x6e, 0x67, 0x20, 0x74, 0x68, 0x69, 0x73, 0x20, 0x70, 0x72, 0x6f, 0x67, 0x72, 
		0x61, 0x6d, 0x20, 0x61, 0x6e, 0x64, 0x2f, 0x6f, 0x72, 0x20, 0x62, 0x6c, 0x6f, 0x63, 0x6b, 0x20, 
		0x66, 0x72, 0x6f, 0x6d, 0x20, 0x6f, 0x74, 0x68, 0x65, 0x72, 0x20, 0x70, 0x72, 0x6f, 0x67, 0x72, 
		0x61, 0x6d, 0x73, 0x20, 0x77, 0x69, 0x6c, 0x6c, 0x20, 0x62, 0x65, 0x20, 0x69, 0x6c, 0x6c, 0x65, 
		0x67, 0x61, 0x6c, 0x20, 0x62, 0x79, 0x20, 0x74, 0x68, 0x65, 0x20, 0x6c, 0x69, 0x63, 0x65, 0x6e, 
		0x73, 0x65, 0x20, 0x61, 0x67, 0x72, 0x65, 0x65, 0x6d, 0x65, 0x6e, 0x74, 0x2e, 0x20, 0x82, 0xb1, 
		0x82, 0xcc, 0x83, 0x76, 0x83, 0x8d, 0x83, 0x4f, 0x83, 0x89, 0x83, 0x80, 0x82, 0xe2, 0x83, 0x75, 
		0x83, 0x8d, 0x83, 0x62, 0x83, 0x4e, 0x82, 0xf0, 0x81, 0x41, 0x90, 0xc3, 0x93, 0x49, 0x82, 0xc5, 
		0x82, 0xa0, 0x82, 0xea, 0x93, 0xae, 0x93, 0x49, 0x82, 0xc5, 0x82, 0xa0, 0x82, 0xea, 0x81, 0x41, 
		0x92, 0xbc, 0x90, 0xda, 0x93, 0x49, 0x82, 0xc5, 0x82, 0xa0, 0x82, 0xea, 0x8a, 0xd4, 0x90, 0xda, 
		0x93, 0x49, 0x82, 0xc5, 0x82, 0xa0, 0x82, 0xea, 0x81, 0x41, 0x91, 0xbc, 0x82, 0xcc, 0x83, 0x76, 
		0x83, 0x8d, 0x83, 0x4f, 0x83, 0x89, 0x83, 0x80, 0x82, 0xa9, 0x82, 0xe7, 0x97, 0x70, 0x82, 0xa2, 
		0x82, 0xe9, 0x82, 0xb1, 0x82, 0xc6, 0x82, 0xcd, 0x83, 0x89, 0x83, 0x43, 0x83, 0x5a, 0x83, 0x93, 
		0x83, 0x58, 0x82, 0xc9, 0x82, 0xe6, 0x82, 0xe8, 0x8b, 0xd6, 0x82, 0xb6, 0x82, 0xe7, 0x82, 0xea, 
		0x82, 0xc4, 0x82, 0xa2, 0x82, 0xdc, 0x82, 0xb7, 0x81, 0x42, 0x0a, 0x81, 0xa1, 0x83, 0x58, 0x83, 
		0x67, 0x81, 0x5b, 0x83, 0x8a, 0x81, 0x5b, 0x0a, 0x0a, 0x8e, 0xe5, 0x90, 0x6c, 0x8c, 0xf6, 0x81, 
		0x45, 0x90, 0xe7, 0x8d, 0xce, 0x8f, 0x74, 0x8e, 0xf7, 0x82, 0xcc, 0x93, 0xfa, 0x8f, 0xed, 0x82, 
		0xcd, 0x8e, 0x8a, 0x82, 0xc1, 0x82, 0xc4, 0x95, 0xbd, 0x89, 0xb8, 0x82, 0xc8, 0x82, 0xe0, 0x82, 
		0xcc, 0x82, 0xc5, 0x82, 0xa0, 0x82, 0xc1, 0x82, 0xbd, 0x81, 0x42, 0x0a, 0x0a, 0x8c, 0x67, 0x91, 
		0xd1, 0x97, 0x8e, 0x82, 0xc6, 0x82, 0xb9, 0x82, 0xce, 0x90, 0x85, 0x82, 0xcc, 0x92, 0x86, 0x0a, 
		0x96, 0xb3, 0x82, 0xad, 0x82, 0xb5, 0x82, 0xbd, 0x8d, 0xe0, 0x95, 0x7a, 0x82, 0xcd, 0x90, 0x94, 
		0x92, 0x6d, 0x82, 0xea, 0x82, 0xb8, 0x0a, 0x8e, 0xa9, 0x93, 0x5d, 0x8e, 0xd4, 0x91, 0x86, 0x82, 
		0xa2, 0x82, 0xc5, 0x82, 0xe8, 0x82, 0xe1, 0x8e, 0x96, 0x8c, 0xcc, 0x82, 0xc9, 0x91, 0x98, 0x82, 
		0xa2, 0x81, 0x41, 0x8a, 0x58, 0x82, 0xf0, 0x95, 0xe0, 0x82, 0xaf, 0x82, 0xce, 0x97, 0x8d, 0x82, 
		0xdc, 0x82, 0xea, 0x82, 0xe9, 0x0a, 0x0a, 0x81, 0x63, 0x81, 0x63, 0x81, 0x63, 0x82, 0xbd, 0x82, 
		0xc6, 0x82, 0xa6, 0x91, 0xbc, 0x82, 0xcc, 0x90, 0x6c, 0x82, 0xa9, 0x82, 0xe7, 0x82, 0xb7, 0x82, 
		0xea, 0x82, 0xce, 0x95, 0x73, 0x8d, 0x4b, 0x82, 0xc6, 0x8c, 0xc4, 0x82, 0xd7, 0x82, 0xe9, 0x82, 
		0xe6, 0x82, 0xa4, 0x82, 0xc8, 0x82, 0xb1, 0x82, 0xc6, 0x82, 0xbe, 0x82, 0xc1, 0x82, 0xbd, 0x82, 
		0xc6, 0x82, 0xb5, 0x82, 0xc4, 0x82, 0xe0, 0x81, 0x41, 0x82, 0xb1, 0x82, 0xcc, 0x92, 0xf6, 0x93, 
		0x78, 0x82, 0xcc, 0x82, 0xb1, 0x82, 0xc6, 0x82, 0xcd, 0x83, 0x67, 0x83, 0x89, 0x83, 0x75, 0x83, 
		0x8b, 0x82, 0xcc, 0x82, 0xa4, 0x82, 0xbf, 0x82, 0xc9, 0x93, 0xfc, 0x82, 0xe7, 0x82, 0xc8, 0x82, 
		0xa2, 0x82, 0xcc, 0x82, 0xaa, 0x81, 0x41, 0x94, 0xde, 0x82, 0xc9, 0x82, 0xc6, 0x82, 0xc1, 0x82, 
		0xc4, 0x81, 0x77, 0x95, 0x81, 0x92, 0xca, 0x81, 0x78, 0x82, 0xcc, 0x93, 0xfa, 0x8f, 0xed, 0x82, 
		0xc5, 0x82, 0xa0, 0x82, 0xe9, 0x81, 0x42, 0x0a, 0x82, 0xbb, 0x82, 0xf1, 0x82, 0xc8, 0x81, 0x41, 
		0x90, 0x73, 0x82, 0xab, 0x82, 0xc8, 0x82, 0xa2, 0x96, 0xef, 0x89, 0xee, 0x8e, 0x96, 0x82, 0xc6, 
		0x90, 0xdc, 0x82, 0xe8, 0x8d, 0x87, 0x82, 0xa2, 0x82, 0xf0, 0x82, 0xc2, 0x82, 0xaf, 0x81, 0x41, 
		0x95, 0xbd, 0x98, 0x61, 0x81, 0x69, 0x81, 0x48, 0x81, 0x6a, 0x82, 0xc9, 0x95, 0xe9, 0x82, 0xe7, 
		0x82, 0xb7, 0x8f, 0x74, 0x8e, 0xf7, 0x82, 0xcc, 0x8c, 0xb3, 0x82, 0xc9, 0x81, 0x67, 0x83, 0x5c, 
		0x83, 0x8c, 0x81, 0x68, 0x82, 0xcd, 0x96, 0x4b, 0x82, 0xea, 0x82, 0xbd, 0x81, 0x42, 0x0a, 0x0a, 
		0x82, 0xa0, 0x82, 0xe9, 0x93, 0xfa, 0x81, 0x41, 0x93, 0xcb, 0x91, 0x52, 0x93, 0xcd, 0x82, 0xaf, 
		0x82, 0xe7, 0x82, 0xea, 0x82, 0xbd, 0x91, 0xee, 0x94, 0x7a, 0x95, 0xd6, 0x81, 0x42, 0x0a, 0x88, 
		0xb6, 0x96, 0xbc, 0x82, 0xe0, 0x82, 0xc8, 0x82, 0xa2, 0x81, 0x41, 0x8d, 0xb7, 0x8f, 0x6f, 0x90, 
		0x6c, 0x82, 0xe0, 0x8f, 0x91, 0x82, 0xa9, 0x82, 0xea, 0x82, 0xc4, 0x82, 0xa2, 0x82, 0xc8, 0x82, 
		0xa2, 0x81, 0x77, 0x90, 0xb6, 0x95, 0xa8, 0x81, 0x78, 0x82, 0xc6, 0x82, 0xa2, 0x82, 0xa4, 0x83, 
		0x56, 0x81, 0x5b, 0x83, 0x8b, 0x82, 0xbe, 0x82, 0xaf, 0x82, 0xaa, 0x93, 0x5c, 0x82, 0xe7, 0x82, 
		0xea, 0x82, 0xbd, 0x89, 0xd7, 0x95, 0xa8, 0x81, 0x42, 0x0a, 0x82, 0xa2, 0x82, 0xa9, 0x82, 0xc9, 
		0x83, 0x67, 0x83, 0x89, 0x83, 0x75, 0x83, 0x8b, 0x82, 0xdc, 0x82, 0xdd, 0x82, 0xea, 0x82, 0xcc, 
		0x93, 0xfa, 0x8f, 0xed, 0x82, 0xf0, 0x89, 0xdf, 0x82, 0xb2, 0x82, 0xb7, 0x8f, 0x74, 0x8e, 0xf7, 
		0x82, 0xc5, 0x82, 0xa0, 0x82, 0xc1, 0x82, 0xc4, 0x82, 0xe0, 0x81, 0x41, 0x82, 0xbb, 0x82, 0xea, 
		0x82, 0xcd, 0x97, 0x5c, 0x91, 0x7a, 0x82, 0xc5, 0x82, 0xab, 0x82, 0xe9, 0x82, 0xe6, 0x82, 0xa4, 
		0x82, 0xc8, 0x92, 0x86, 0x90, 0x67, 0x82, 0xc5, 0x82, 0xcd, 0x82, 0xc8, 0x82, 0xa9, 0x82, 0xc1, 
		0x82, 0xbd, 0x81, 0x42, 0x0a, 0x82, 0xdc, 0x82, 0xb3, 0x82, 0xa9, 0x81, 0x67, 0x8f, 0x97, 0x82, 
		0xcc, 0x8e, 0x71, 0x81, 0x68, 0x82, 0xaa, 0x94, 0x7a, 0x92, 0x42, 0x82, 0xb3, 0x82, 0xea, 0x82, 
		0xc4, 0x82, 0xad, 0x82, 0xe9, 0x82, 0xbe, 0x82, 0xc8, 0x82, 0xf1, 0x82, 0xc4, 0x81, 0x63, 0x81, 
		0x63, 0x81, 0x42, 0x0a, 0x0a, 0x82, 0xb1, 0x82, 0xea, 0x82, 0xb1, 0x82, 0xbb, 0x82, 0xaa, 0x81, 
		0x41, 0x8f, 0x74, 0x8e, 0xf7, 0x82, 0xaa, 0x8e, 0xe8, 0x82, 0xc9, 0x82, 0xb5, 0x82, 0xbd, 0x81, 
		0x77, 0x90, 0x56, 0x82, 0xbd, 0x82, 0xc8, 0x82, 0xe9, 0x95, 0x73, 0x8d, 0x4b, 0x81, 0x78, 0x81, 
		0x63, 0x81, 0x63, 0x81, 0x63, 0x81, 0x63, 0x82, 0xc5, 0x82, 0xcd, 0x82, 0xc8, 0x82, 0xad, 0x81, 
		0x77, 0x8f, 0x89, 0x82, 0xdf, 0x82, 0xc4, 0x82, 0xcc, 0x8d, 0x4b, 0x89, 0x5e, 0x81, 0x78, 0x82, 
		0xc5, 0x82, 0xa0, 0x82, 0xe8, 0x81, 0x41, 0x94, 0x67, 0x97, 0x90, 0x82, 0xcc, 0x96, 0x8b, 0x8a, 
		0x4a, 0x82, 0xaf, 0x82, 0xc5, 0x82, 0xa0, 0x82, 0xc1, 0x82, 0xbd, 0x81, 0x42, 0x0a, 0x0a, 0x8f, 
		0x74, 0x8e, 0xf7, 0x82, 0xcc, 0x95, 0x73, 0x8d, 0x4b, 0x82, 0xf0, 0x90, 0x40, 0x82, 0xa4, 0x82, 
		0xbd, 0x82, 0xdf, 0x82, 0xc9, 0x97, 0x88, 0x82, 0xbd, 0x81, 0x49, 0x81, 0x40, 0x82, 0xc6, 0x8c, 
		0xbe, 0x82, 0xa4, 0x8f, 0xad, 0x8f, 0x97, 0x82, 0xcd, 0x8e, 0xa9, 0x82, 0xe7, 0x82, 0xf0, 0x81, 
		0x67, 0x90, 0x5f, 0x97, 0x6c, 0x81, 0x68, 0x82, 0xc6, 0x96, 0xbc, 0x8f, 0xe6, 0x82, 0xe8, 0x81, 
		0x41, 0x94, 0xde, 0x82, 0xf0, 0x83, 0x67, 0x83, 0x89, 0x83, 0x75, 0x83, 0x8b, 0x82, 0xa9, 0x82, 
		0xe7, 0x8e, 0xe7, 0x82, 0xe9, 0x96, 0xbc, 0x96, 0xda, 0x82, 0xcc, 0x82, 0xe0, 0x82, 0xc6, 0x89, 
		0xc6, 0x82, 0xc9, 0x8b, 0x8f, 0x8d, 0xc0, 0x82, 0xe9, 0x82, 0xb1, 0x82, 0xc6, 0x82, 0xc9, 0x82, 
		0xc8, 0x82, 0xe9, 0x82, 0xcc, 0x82, 0xbe, 0x82, 0xaa, 0x81, 0x63, 0x81, 0x63, 0x81, 0x42, 0x0a, 
		0x94, 0xde, 0x8f, 0x97, 0x82, 0xcc, 0x8d, 0x73, 0x93, 0xae, 0x82, 0xaa, 0x90, 0x56, 0x82, 0xbd, 
		0x82, 0xc8, 0x96, 0xe2, 0x91, 0xe8, 0x82, 0xf0, 0x88, 0xf8, 0x82, 0xab, 0x8b, 0x4e, 0x82, 0xb1, 
		0x82, 0xb5, 0x81, 0x41, 0x8f, 0xac, 0x82, 0xb3, 0x82, 0xc8, 0x83, 0x67, 0x83, 0x89, 0x83, 0x75, 
		0x83, 0x8b, 0x82, 0xcd, 0x82, 0xa2, 0x82, 0xc2, 0x82, 0xcc, 0x8a, 0xd4, 0x82, 0xc9, 0x82, 0xa9, 
		0x8e, 0xfc, 0x82, 0xe8, 0x82, 0xcc, 0x90, 0x6c, 0x8a, 0xd4, 0x82, 0xe0, 0x8a, 0xaa, 0x82, 0xab, 
		0x8d, 0x9e, 0x82, 0xf1, 0x82, 0xbe, 0x91, 0xe5, 0x91, 0x9b, 0x93, 0xae, 0x82, 0xc9, 0x81, 0x49, 
		0x0a, 0x0a, 0x0a, 0x82, 0xbb, 0x82, 0xf1, 0x82, 0xc8, 0x83, 0x6e, 0x83, 0x60, 0x83, 0x83, 0x83, 
		0x81, 0x83, 0x60, 0x83, 0x83, 0x82, 0xc8, 0x96, 0x88, 0x93, 0xfa, 0x82, 0xcc, 0x92, 0x86, 0x82, 
		0xc5, 0x81, 0x41, 0x8f, 0x74, 0x8e, 0xf7, 0x82, 0xaa, 0x8e, 0xe8, 0x82, 0xc9, 0x82, 0xb5, 0x82, 
		0xbd, 0x81, 0x77, 0x8d, 0x4b, 0x89, 0x5e, 0x81, 0x78, 0x82, 0xcd, 0x88, 0xea, 0x91, 0xcc, 0x89, 
		0xbd, 0x82, 0xf0, 0x82, 0xe0, 0x82, 0xbd, 0x82, 0xe7, 0x82, 0xb7, 0x82, 0xcc, 0x82, 0xa9, 0x81, 
		0x48, 0x0a, 0x90, 0x5f, 0x97, 0x6c, 0x82, 0xc6, 0x96, 0xbc, 0x8f, 0xe6, 0x82, 0xe9, 0x8f, 0xad, 
		0x8f, 0x97, 0x82, 0xcc, 0x96, 0xda, 0x93, 0x49, 0x82, 0xcd, 0x81, 0x48, 0x0a, 0x8f, 0x74, 0x8e, 
		0xf7, 0x82, 0xc9, 0x95, 0x74, 0x82, 0xab, 0x82, 0xdc, 0x82, 0xc6, 0x82, 0xa4, 0x95, 0x73, 0x8d, 
		0x4b, 0x82, 0xcd, 0x8f, 0xc1, 0x82, 0xa6, 0x82, 0xe9, 0x82, 0xcc, 0x82, 0xa9, 0x81, 0x48, 0x0a, 
		0x0a, 0x0a, 0x93, 0x9a, 0x82, 0xa6, 0x82, 0xcd, 0x82, 0xbb, 0x82, 0xa4, 0x81, 0x41, 0x82, 0xb1, 
		0x82, 0xea, 0x82, 0xa9, 0x82, 0xe7, 0x8e, 0x6e, 0x82, 0xdc, 0x82, 0xe9, 0x91, 0x9b, 0x97, 0x90, 
		0x82, 0xcc, 0x93, 0xfa, 0x81, 0x58, 0x82, 0xcc, 0x92, 0x86, 0x82, 0xc9, 0x81, 0x49, 0x0a, 0x0a, 
		0x0a, 0x81, 0xa1, 0x89, 0x4b, 0x89, 0xd4, 0x94, 0x56, 0x8d, 0xb2, 0x8b, 0x76, 0x96, 0xe9, 0x95, 
		0x50, 0x0a, 0x0a, 0x20, 0x90, 0x94, 0x95, 0x53, 0x94, 0x4e, 0x82, 0xe0, 0x82, 0xcc, 0x8a, 0xd4, 
		0x81, 0x41, 0x90, 0x5e, 0x93, 0xfa, 0x89, 0x48, 0x8e, 0x73, 0x82, 0xf0, 0x8e, 0xe7, 0x82, 0xe9, 
		0x93, 0x79, 0x92, 0x6e, 0x90, 0x5f, 0x82, 0xc6, 0x82, 0xb5, 0x82, 0xc4, 0x91, 0xb6, 0x8d, 0xdd, 
		0x82, 0xb5, 0x82, 0xc4, 0x82, 0xab, 0x82, 0xbd, 0x90, 0x5f, 0x97, 0x6c, 0x81, 0x42, 0x0a, 0x90, 
		0xe7, 0x8d, 0xce, 0x8f, 0x74, 0x8e, 0xf7, 0x82, 0xcc, 0x95, 0x73, 0x8d, 0x4b, 0x82, 0xf0, 0x90, 
		0x40, 0x82, 0xa4, 0x82, 0xbd, 0x82, 0xdf, 0x82, 0xc9, 0x94, 0xde, 0x82, 0xcc, 0x89, 0xc6, 0x82, 
		0xc9, 0x8b, 0x8f, 0x8d, 0xc0, 0x82, 0xe9, 0x8e, 0x96, 0x82, 0xc9, 0x82, 0xc8, 0x82, 0xe9, 0x82, 
		0xcc, 0x82, 0xbe, 0x82, 0xaa, 0x81, 0x41, 0x96, 0xca, 0x94, 0x92, 0x82, 0xa2, 0x8e, 0x96, 0x82, 
		0xaa, 0x8d, 0x44, 0x82, 0xab, 0x82, 0xc8, 0x90, 0xab, 0x8a, 0x69, 0x82, 0xcc, 0x82, 0xb9, 0x82, 
		0xa2, 0x82, 0xa9, 0x83, 0x67, 0x83, 0x89, 0x83, 0x75, 0x83, 0x8b, 0x83, 0x81, 0x81, 0x5b, 0x83, 
		0x4a, 0x81, 0x5b, 0x82, 0xc9, 0x82, 0xc8, 0x82, 0xc1, 0x82, 0xc4, 0x82, 0xb5, 0x82, 0xdc, 0x82, 
		0xa4, 0x82, 0xb1, 0x82, 0xc6, 0x82, 0xe0, 0x91, 0xbd, 0x82, 0xa2, 0x81, 0x42, 0x0a, 0x90, 0x6c, 
		0x82, 0xf0, 0x8e, 0x8e, 0x82, 0xb7, 0x82, 0xe6, 0x82, 0xa4, 0x82, 0xc8, 0x8e, 0x96, 0x82, 0xf0, 
		0x8c, 0xbe, 0x82, 0xc1, 0x82, 0xc4, 0x94, 0xbd, 0x89, 0x9e, 0x82, 0xf0, 0x8c, 0xa9, 0x82, 0xe9, 
		0x82, 0xcc, 0x82, 0xaa, 0x8d, 0xc5, 0x8b, 0xdf, 0x82, 0xcc, 0x83, 0x7d, 0x83, 0x43, 0x83, 0x75, 
		0x81, 0x5b, 0x83, 0x80, 0x81, 0x42, 0x0a, 0x89, 0xbd, 0x8e, 0x96, 0x82, 0xc9, 0x91, 0xce, 0x82, 
		0xb5, 0x82, 0xc4, 0x82, 0xe0, 0x8e, 0xb7, 0x92, 0x85, 0x90, 0x53, 0x82, 0xcd, 0x94, 0x96, 0x82, 
		0xa2, 0x82, 0xaa, 0x81, 0x41, 0x8b, 0x43, 0x82, 0xc9, 0x82, 0xc8, 0x82, 0xe9, 0x90, 0x6c, 0x8a, 
		0xd4, 0x82, 0xc9, 0x82, 0xcd, 0x83, 0x84, 0x83, 0x4c, 0x83, 0x82, 0x83, 0x60, 0x82, 0xf0, 0x8f, 
		0xc4, 0x82, 0xad, 0x88, 0xea, 0x96, 0xca, 0x82, 0xe0, 0x81, 0x42, 0x0a, 0x0a, 0x0a, 0x81, 0xa1, 
		0x97, 0xb3, 0x92, 0x5f, 0x20, 0x83, 0x8b, 0x83, 0x8a, 0x0a, 0x0a, 0x20, 0x90, 0x5f, 0x97, 0x6c, 
		0x82, 0xf0, 0x92, 0xc7, 0x82, 0xa2, 0x82, 0xa9, 0x82, 0xaf, 0x82, 0xc4, 0x82, 0xe2, 0x82, 0xc1, 
		0x82, 0xc4, 0x82, 0xab, 0x82, 0xbd, 0x81, 0x41, 0x8c, 0xcf, 0x8e, 0xa8, 0x82, 0xcc, 0x8f, 0x97, 
		0x82, 0xcc, 0x8e, 0x71, 0x81, 0x42, 0x0a, 0x92, 0x67, 0x82, 0xa9, 0x82, 0xa2, 0x82, 0xc6, 0x82, 
		0xb1, 0x82, 0xeb, 0x82, 0xc6, 0x82, 0xa8, 0x89, 0xd9, 0x8e, 0x71, 0x82, 0xaa, 0x91, 0xe5, 0x8d, 
		0x44, 0x82, 0xab, 0x82, 0xc5, 0x81, 0x41, 0x8e, 0xef, 0x96, 0xa1, 0x82, 0xcd, 0x82, 0xb5, 0x82, 
		0xc1, 0x82, 0xdb, 0x82, 0xcc, 0x8e, 0xe8, 0x93, 0xfc, 0x82, 0xea, 0x81, 0x42, 0x0a, 0x90, 0x5f, 
		0x97, 0x6c, 0x82, 0xaa, 0x8b, 0xea, 0x98, 0x4a, 0x82, 0xb5, 0x82, 0xc4, 0x82, 0xa2, 0x82, 0xe9, 
		0x82, 0xcc, 0x82, 0xcd, 0x8e, 0xe5, 0x90, 0x6c, 0x8c, 0xf6, 0x82, 0xcc, 0x82, 0xb9, 0x82, 0xa2, 
		0x82, 0xbe, 0x82, 0xc6, 0x8e, 0x76, 0x82, 0xa2, 0x8d, 0x9e, 0x82, 0xdd, 0x81, 0x41, 0x88, 0xea, 
		0x95, 0xfb, 0x93, 0x49, 0x82, 0xc9, 0x93, 0x47, 0x8e, 0x8b, 0x82, 0xb5, 0x82, 0xc4, 0x82, 0xa2, 
		0x82, 0xe9, 0x81, 0x42, 0x0a, 0x8b, 0xbb, 0x96, 0xa1, 0x82, 0xf0, 0x82, 0xd0, 0x82, 0xa9, 0x82, 
		0xea, 0x82, 0xe9, 0x82, 0xe0, 0x82, 0xcc, 0x82, 0xaa, 0x82, 0xa0, 0x82, 0xe9, 0x82, 0xc6, 0x81, 
		0x41, 0x93, 0xf5, 0x82, 0xa2, 0x82, 0xf0, 0x82, 0xa9, 0x82, 0xa2, 0x82, 0xbe, 0x82, 0xe8, 0xe4, 
		0x72, 0x82, 0xdf, 0x82, 0xc4, 0x82, 0xdd, 0x82, 0xbd, 0x82, 0xe8, 0x82, 0xb7, 0x82, 0xe9, 0x95, 
		0xc8, 0x82, 0xaa, 0x82, 0xa0, 0x82, 0xe9, 0x81, 0x42, 0x0a, 0x8d, 0x44, 0x88, 0xd3, 0x82, 0xf0, 
		0x8e, 0x9d, 0x82, 0xc2, 0x91, 0x8a, 0x8e, 0xe8, 0x82, 0xc9, 0x82, 0xcd, 0x95, 0xdf, 0x8a, 0x6c, 
		0x82, 0xb5, 0x82, 0xbd, 0x8f, 0xac, 0x93, 0xae, 0x95, 0xa8, 0x82, 0xf0, 0x83, 0x76, 0x83, 0x8c, 
		0x83, 0x5b, 0x83, 0x93, 0x83, 0x67, 0x82, 0xb7, 0x82, 0xe9, 0x82, 0xb1, 0x82, 0xc6, 0x82, 0xe0, 
		0x81, 0x49, 0x81, 0x48, 0x0a, 0x0a, 0x0a, 0x81, 0xa1, 0x90, 0xe7, 0x8d, 0xce, 0x20, 0x8d, 0xb2, 
		0x93, 0xde, 0x0a, 0x0a, 0x20, 0x8c, 0x5a, 0x82, 0xcc, 0x8e, 0x96, 0x82, 0xaa, 0x91, 0xe5, 0x8d, 
		0x44, 0x82, 0xab, 0x82, 0xc8, 0x8e, 0xe5, 0x90, 0x6c, 0x8c, 0xf6, 0x82, 0xcc, 0x96, 0x85, 0x81, 
		0x42, 0x0a, 0x90, 0x85, 0x93, 0xe3, 0x8a, 0x77, 0x89, 0x80, 0x82, 0xc9, 0x92, 0xca, 0x82, 0xa4, 
		0x88, 0xea, 0x94, 0x4e, 0x90, 0xb6, 0x82, 0xc5, 0x81, 0x41, 0x90, 0x86, 0x8e, 0x96, 0x82, 0xe2, 
		0x90, 0xf4, 0x91, 0xf3, 0x93, 0x99, 0x81, 0x41, 0x90, 0xe7, 0x8d, 0xce, 0x89, 0xc6, 0x82, 0xcc, 
		0x89, 0xc6, 0x8e, 0x96, 0x91, 0x53, 0x94, 0xca, 0x82, 0xf0, 0x82, 0xb1, 0x82, 0xc8, 0x82, 0xb5, 
		0x82, 0xc4, 0x82, 0xa2, 0x82, 0xe9, 0x81, 0x42, 0x0a, 0x8e, 0xef, 0x96, 0xa1, 0x82, 0xcd, 0x8c, 
		0x5a, 0x8a, 0xcf, 0x8e, 0x40, 0x81, 0x45, 0x8c, 0x5a, 0x82, 0xcc, 0x90, 0xa2, 0x98, 0x62, 0x82, 
		0xf0, 0x8f, 0xc4, 0x82, 0xad, 0x8e, 0x96, 0x81, 0x42, 0x95, 0x81, 0x92, 0x69, 0x82, 0xcd, 0x92, 
		0x4e, 0x82, 0xc9, 0x82, 0xc5, 0x82, 0xe0, 0x97, 0x44, 0x82, 0xb5, 0x82, 0xad, 0x82, 0xa8, 0x82, 
		0xc6, 0x82, 0xc8, 0x82, 0xb5, 0x82, 0xa2, 0x90, 0xab, 0x8a, 0x69, 0x82, 0xc8, 0x82, 0xcc, 0x82, 
		0xbe, 0x82, 0xaa, 0x81, 0x41, 0x8c, 0x5a, 0x82, 0xaa, 0x97, 0x8d, 0x82, 0xde, 0x82, 0xc6, 0x88, 
		0xd3, 0x8a, 0x4f, 0x82, 0xc8, 0x8d, 0x73, 0x93, 0xae, 0x97, 0xcd, 0x82, 0xf0, 0x94, 0xad, 0x8a, 
		0xf6, 0x82, 0xb5, 0x82, 0xbd, 0x82, 0xe8, 0x82, 0xb7, 0x82, 0xe9, 0x81, 0x42, 0x0a, 0x97, 0xf6, 
		0x82, 0xc9, 0x8a, 0xd6, 0x82, 0xb5, 0x82, 0xc4, 0x82, 0xcd, 0x89, 0x9c, 0x8e, 0xe8, 0x82, 0xc5, 
		0x81, 0x41, 0x8c, 0x5a, 0x82, 0xc6, 0x93, 0xf1, 0x90, 0x6c, 0x82, 0xab, 0x82, 0xe8, 0x82, 0xc9, 
		0x82, 0xc8, 0x82, 0xe9, 0x82, 0xc6, 0x96, 0x5c, 0x91, 0x96, 0x8b, 0x43, 0x96, 0xa1, 0x82, 0xc9, 
		0x82, 0xc8, 0x82, 0xc1, 0x82, 0xc4, 0x82, 0xb5, 0x82, 0xdc, 0x82, 0xa4, 0x8f, 0x97, 0x82, 0xcc, 
		0x8e, 0x71, 0x81, 0x42, 0x0a, 0x0a, 0x0a, 0x81, 0xa1, 0x8e, 0x52, 0x90, 0x81, 0x20, 0x88, 0xa8, 
		0x0a, 0x0a, 0x20, 0x8d, 0xd7, 0x82, 0xa9, 0x82, 0xa2, 0x8e, 0x96, 0x82, 0xcd, 0x8b, 0x43, 0x82, 
		0xc9, 0x82, 0xb5, 0x82, 0xc8, 0x82, 0xa2, 0x81, 0x41, 0x92, 0x7c, 0x82, 0xf0, 0x8a, 0x84, 0x82, 
		0xc1, 0x82, 0xbd, 0x82, 0xe6, 0x82, 0xa4, 0x82, 0xc8, 0x90, 0xab, 0x8a, 0x69, 0x82, 0xcc, 0x8e, 
		0xe5, 0x90, 0x6c, 0x8c, 0xf6, 0x82, 0xcc, 0x97, 0x63, 0x93, 0xe9, 0x90, 0xf5, 0x81, 0x42, 0x0a, 
		0x96, 0xbe, 0x82, 0xe9, 0x82, 0xa2, 0x90, 0xab, 0x8a, 0x69, 0x82, 0xc5, 0x83, 0x8b, 0x83, 0x62, 
		0x83, 0x4e, 0x83, 0x58, 0x82, 0xe0, 0x97, 0xc7, 0x82, 0xad, 0x81, 0x41, 0x92, 0x4e, 0x82, 0xc9, 
		0x82, 0xc5, 0x82, 0xe0, 0x95, 0xbd, 0x93, 0x99, 0x82, 0xc9, 0x90, 0xda, 0x82, 0xb7, 0x82, 0xe9, 
		0x82, 0xcc, 0x82, 0xc5, 0x81, 0x41, 0x92, 0x6a, 0x8f, 0x97, 0x82, 0xc6, 0x82, 0xe0, 0x82, 0xc9, 
		0x90, 0x6c, 0x8b, 0x43, 0x82, 0xaa, 0x82, 0xa0, 0x82, 0xe9, 0x81, 0x42, 0x0a, 0x82, 0xbe, 0x82, 
		0xaa, 0x81, 0x41, 0x97, 0xf6, 0x88, 0xa4, 0x82, 0xc9, 0x8a, 0xd6, 0x82, 0xb5, 0x82, 0xc4, 0x82, 
		0xcd, 0x82, 0xa9, 0x82, 0xc8, 0x82, 0xe8, 0x93, 0xdd, 0x8a, 0xb4, 0x82, 0xc5, 0x81, 0x41, 0x8e, 
		0xfc, 0x88, 0xcd, 0x82, 0xf0, 0x95, 0xf0, 0x82, 0xea, 0x82, 0xb3, 0x82, 0xb9, 0x82, 0xc4, 0x82, 
		0xb5, 0x82, 0xdc, 0x82, 0xa4, 0x8e, 0x96, 0x82, 0xe0, 0x81, 0x42, 0x0a, 0x82, 0xa2, 0x82, 0xc2, 
		0x82, 0xe0, 0x95, 0x73, 0x89, 0x5e, 0x82, 0xc8, 0x8e, 0xe5, 0x90, 0x6c, 0x8c, 0xf6, 0x82, 0xf0, 
		0x90, 0x53, 0x94, 0x7a, 0x82, 0xb5, 0x81, 0x41, 0x97, 0x6c, 0x81, 0x58, 0x82, 0xc8, 0x82, 0xa8, 
		0x8e, 0xe7, 0x82, 0xe8, 0x82, 0xf0, 0x8f, 0x57, 0x82, 0xdf, 0x82, 0xc4, 0x82, 0xcd, 0x83, 0x76, 
		0x83, 0x8c, 0x83, 0x5b, 0x83, 0x93, 0x83, 0x67, 0x82, 0xb5, 0x82, 0xc4, 0x82, 0xa2, 0x82, 0xe9, 
		0x82, 0xcc, 0x82, 0xbe, 0x82, 0xaa, 0x81, 0x41, 0x96, 0xa2, 0x82, 0xbe, 0x82, 0xc9, 0x8c, 0xf8, 
		0x89, 0xca, 0x82, 0xcd, 0x82, 0xdd, 0x82, 0xe7, 0x82, 0xea, 0x82, 0xc8, 0x82, 0xa2, 0x82, 0xe6, 
		0x82, 0xa4, 0x82, 0xbe, 0x81, 0x42, 0x0a, 0x0a, 0x0a, 0x81, 0xa1, 0x90, 0xe7, 0x8d, 0xce, 0x20, 
		0x8f, 0x74, 0x8e, 0xf7, 0x0a, 0x0a, 0x20, 0x96, 0x7b, 0x8d, 0xec, 0x82, 0xcc, 0x8e, 0xe5, 0x90, 
		0x6c, 0x8c, 0xf6, 0x0a, 0x90, 0xb6, 0x82, 0xdc, 0x82, 0xea, 0x82, 0xc2, 0x82, 0xab, 0x95, 0x73, 
		0x89, 0x5e, 0x82, 0xc8, 0x91, 0xcc, 0x8e, 0xbf, 0x82, 0xc5, 0x90, 0x46, 0x81, 0x58, 0x82, 0xc6, 
		0x8b, 0xea, 0x98, 0x4a, 0x82, 0xf0, 0x82, 0xb5, 0x82, 0xc4, 0x82, 0xab, 0x82, 0xbd, 0x82, 0xaa, 
		0x81, 0x41, 0x89, 0xc6, 0x91, 0xb0, 0x82, 0xe2, 0x97, 0x46, 0x92, 0x42, 0x82, 0xcc, 0x8e, 0x78, 
		0x82, 0xa6, 0x82, 0xe0, 0x82, 0xa0, 0x82, 0xe8, 0x81, 0x41, 0x8d, 0xa1, 0x82, 0xcd, 0x91, 0x4f, 
		0x8c, 0xfc, 0x82, 0xab, 0x82, 0xc9, 0x95, 0xe9, 0x82, 0xe7, 0x82, 0xb5, 0x82, 0xc4, 0x82, 0xa2, 
		0x82, 0xe9, 0x81, 0x42, 0x0a, 0x8a, 0xee, 0x96, 0x7b, 0x93, 0x49, 0x82, 0xc9, 0x82, 0xcd, 0x97, 
		0x44, 0x82, 0xb5, 0x82, 0xa2, 0x90, 0xab, 0x8a, 0x69, 0x82, 0xc5, 0x81, 0x41, 0x96, 0xca, 0x93, 
		0x7c, 0x8c, 0xa9, 0x82, 0xaa, 0x82, 0xa2, 0x82, 0xa2, 0x81, 0x42, 0x0a, 0x8a, 0x58, 0x82, 0xc9, 
		0x8f, 0x6f, 0x82, 0xe9, 0x82, 0xc6, 0x97, 0x8d, 0x82, 0xdc, 0x82, 0xea, 0x82, 0xe9, 0x8e, 0x96, 
		0x82, 0xaa, 0x91, 0xbd, 0x82, 0xa9, 0x82, 0xc1, 0x82, 0xbd, 0x82, 0xb9, 0x82, 0xa2, 0x82, 0xa9, 
		0x81, 0x41, 0x83, 0x50, 0x83, 0x93, 0x83, 0x4a, 0x82, 0xe2, 0x8d, 0x72, 0x8e, 0x96, 0x82, 0xc9, 
		0x82, 0xcd, 0x8a, 0xb5, 0x82, 0xea, 0x82, 0xc4, 0x82, 0xa2, 0x82, 0xe9, 0x81, 0x42, 0x0a, 0x0a, 
		0x0a, 0x81, 0xa1, 0x8f, 0xed, 0x94, 0xd5, 0x20, 0x82, 0xdc, 0x82, 0xd0, 0x82, 0xeb, 0x0a, 0x0a, 
		0x20, 0x8e, 0xe5, 0x90, 0x6c, 0x8c, 0xf6, 0x82, 0xaa, 0x92, 0xca, 0x82, 0xa4, 0x90, 0x85, 0x93, 
		0xe3, 0x8a, 0x77, 0x89, 0x80, 0x82, 0xcc, 0x90, 0xe6, 0x94, 0x79, 0x82, 0xc5, 0x81, 0x41, 0x8a, 
		0x77, 0x90, 0xb6, 0x89, 0xef, 0x92, 0xb7, 0x81, 0x42, 0x0a, 0x90, 0xac, 0x90, 0xd1, 0x97, 0x44, 
		0x8f, 0x47, 0x82, 0xc5, 0x97, 0x65, 0x8e, 0x70, 0x92, 0x5b, 0x97, 0xed, 0x81, 0x41, 0x92, 0x4e, 
		0x82, 0xc9, 0x82, 0xc5, 0x82, 0xe0, 0x97, 0x44, 0x82, 0xb5, 0x82, 0xad, 0x89, 0xb8, 0x82, 0xe2, 
		0x25, 0x2f, 0xa4, 0x7c, 0xb2, 0x4f, 0x08, 0xe1, 0x11, 0x23, 0x15, 0x22, 0xe2, 0xfd, 0x7f, 0x60, 
		0x20, 0x71, 0x00, 0xee, 0x92, 0x38, 0x2e, 0x28, 0x5d, 0xcd, 0x46, 0x6b, 0xd0, 0xd6, 0xf8, 0xc5, 
		0xe4, 0xce, 0x82, 0x2e, 0x54, 0x71, 0x58, 0xb9, 0x30, 0xa1, 0x04, 0x51, 0xbc, 0xdd, 0x83, 0xc9, 
		0x05, 0x98, 0xb7, 0xc1, 0xa2, 0x33, 0x74, 0x67, 0x7c, 0xff, 0x14, 0x29, 0x12, 0x8a, 0xfa, 0x01, 
		0x66, 0x02, 0x1e, 0x94, 0x03, 0x4b, 0x5b, 0x60, 0x32, 0xcd, 0x30, 0x5f, 0xf6, 0x03, 0xed, 0x3d, 
		0x30, 0x2f, 0x7e, 0xe1, 0x3d, 0x0d, 0x2b, 0x83, 0x2b, 0x31, 0x92, 0xbf, 0x56, 0x6b, 0x39, 0x5f, 
		0x68, 0x14, 0xfc, 0xff, 0xa5, 0x20, 0x0f, 0xf1, 0xad, 0xc4, 0x1d, 0x9c, 0x60, 0xb3, 0x0f, 0x85, 
		0x0e, 0x97, 0x5a, 0x9d, 0x5b, 0x30, 0xe6, 0x56, 0x83, 0xec, 0x08, 0xd9, 0x2e, 0x69, 0x0a, 0x51, 
		0x6a, 0x10, 0xf6, 0xd8, 0x31, 0x71, 0xfc, 0xd0, 0xbb, 0xe9, 0xa8, 0xed, 0xcc, 0x07, 0x3d, 0x40, 
		0x93, 0x09, 0xe7, 0xc7, 0xf7, 0x1a, 0x4c, 0xf9, 0x71, 0x4f, 0xbd, 0x34, 0x0d, 0x46, 0x14, 0x23, 
		0xf3, 0x19, 0x66, 0xdb, 0xb8, 0x17, 0x77, 0x80, 0xb8, 0x30, 0x2a, 0xbb, 0x1f, 0x6b, 0xa2, 0xb2, 
		0xc3, 0x8d, 0x03, 0x32, 0x9f, 0x0d, 0xc8, 0xde, 0xa3, 0xa8, 0x64, 0xe3, 0xfe, 0x99, 0x13, 0x2f, 
		0x49, 0xe3, 0xae, 0xd6, 0x48, 0x4f, 0x3e, 0xae, 0xd8, 0xa6, 0x56, 0xf2, 0xa4, 0xa0, 0xfb, 0xb1, 
		0x8d, 0x86, 0x50, 0x4b, 0xf9, 0x4e, 0xd8, 0xba, 0x1d, 0xda, 0x2f, 0x0d, 0x8c, 0x4c, 0x86, 0x29, 
		0x0a, 0x37, 0x31, 0xee, 0x08, 0x18, 0x63, 0x47, 0xef, 0xe7, 0xf1, 0xe1, 0xbb, 0xba, 0x4a, 0x6a, 
		0xee, 0x56, 0x03, 0x4c, 0xab, 0x46, 0xcf, 0x02, 0xb9, 0x6f, 0x14, 0x2c, 0x09, 0xa0, 0x02, 0x54, 
		0x77, 0xa4, 0x77, 0xda, 0x81, 0x75, 0xfa, 0x7d, 0x29, 0x58, 0x04, 0x3f, 0xc4, 0x23, 0x25, 0xca, 
		0xea, 0xf3, 0x77, 0x0a, 0x70, 0xc5, 0xd2, 0xd3, 0x11, 0xf4, 0xb0, 0x4f, 0x95, 0x26, 0x6e, 0x4a, 
		0xa6, 0x3a, 0x2d, 0xd6, 0x52, 0xbe, 0x6a, 0x57, 0xf2, 0xdd, 0x5d, 0x44, 0xe8, 0x18, 0xf4, 0x85, 
		0x46, 0x7a, 0x9b, 0x27, 0xe7, 0x5b, 0x20, 0xee, 0x94, 0x38, 0xb1, 0x58, 0xfd, 0x76, 0x35, 0x37, 
		0x6e, 0xad, 0xb3, 0xa0, 0x9a, 0x20, 0x33, 0xed, 0x0d, 0x00, 0x5b, 0x2b, 0x1f, 0x59, 0x76, 0xa2, 
		0x1d, 0x0a, 0xa0, 0xdf, 0x45, 0x9f, 0x33, 0xd8, 0x57, 0x6e, 0x8b, 0xbe, 0x8c, 0xe9, 0x17, 0x9e, 
		0xfd, 0x4a, 0xac, 0x8e, 0xc0, 0x19, 0x12, 0x2f, 0x90, 0xe2, 0x85, 0x47, 0x89, 0x6b, 0xcd, 0xbb, 
		0xe1, 0x06, 0x7c, 0xbf, 0x41, 0x2c, 0x20, 0xd7, 0xc4, 0xf2, 0xf5, 0x7c, 0xc2, 0x3f, 0xb7, 0xc3, 
		0x54, 0xdb, 0xe2, 0x45, 0x2d, 0xfd, 0xde, 0xcf, 0x55, 0xa2, 0x74, 0x39, 0xc8, 0x35, 0x1e, 0x41, 
		0x90, 0xe5, 0xeb, 0x68, 0x56, 0xdc, 0x4d, 0x07, 0x84, 0x9d, 0xb8, 0x8f, 0xb4, 0x8b, 0x8f, 0xae, 
		0xb8, 0x83, 0x30, 0x82, 0x5e, 0x75, 0x3f, 0xc1, 0x88, 0xb9, 0x05, 0xbe, 0x02, 0xf8, 0x83, 0xe6, 
		0xc6, 0xb2, 0xb8, 0x01, 0x53, 0xf2, 0x25, 0x19, 0x49, 0x91, 0xf7, 0x0b, 0xd9, 0xa3, 0x18, 0xbd, 
		0x40, 0xb5, 0xb6, 0xd4, 0xb4, 0xad, 0xb1, 0xd1, 0xae, 0xee, 0xd9, 0x8f, 0x01, 0xb0, 0x2a, 0xa0, 
		0x1a, 0x0d, 0xd7, 0xb2, 0xac, 0xd6, 0x66, 0x92, 0x85, 0x32, 0x52, 0xd6, 0x3f, 0x72, 0xd4, 0xe1, 
		0x5c, 0x3d, 0xa8, 0xdf, 0x26, 0x93, 0xc7, 0x5b, 0x0b, 0x63, 0xa5, 0xee, 0x96, 0x76, 0xe2, 0x0b, 
		0xbd, 0x29, 0xcf, 0xb9, 0x59, 0xf0, 0x64, 0xb8, 0x00, 0x67, 0x90, 0x6a, 0x44, 0x28, 0xc5, 0x5f, 
		0xbb, 0xe8, 0x5f, 0x99, 0xac, 0x7c, 0xd2, 0xe3, 0xb8, 0xb9, 0x73, 0xb0, 0x8f, 0xa0, 0x95, 0xe6, 
		0xf1, 0xfe, 0xe9, 0xa7, 0xd3, 0x6e, 0xb4, 0x18, 0x27, 0xbe, 0x1f, 0x96, 0xa0, 0x45, 0xc3, 0x10, 
		0xe2, 0x95, 0xf8, 0xc1, 0x53, 0x10, 0xbe, 0xbf, 0x25, 0xd6, 0xcf, 0x7c, 0xe6, 0x0c, 0x69, 0xb4, 
		0x4d, 0x86, 0x83, 0x23, 0xbc, 0x93, 0x82, 0xe1, 0xbb, 0xa6, 0x1d, 0xbd, 0xc9, 0xb3, 0x09, 0x21, 
		0x2b, 0x4b, 0x41, 0x53, 0xbf, 0x6b, 0xc6, 0xf8, 0x04, 0x02, 0x9b, 0x8f, 0x65, 0x3a, 0x9e, 0xbc, 
		0x3d, 0x61, 0x50, 0x24, 0x38, 0xb2, 0x4d, 0xe9, 0x3d, 0xd5, 0xec, 0xc5, 0x0c, 0x1b, 0xa8, 0x84, 
		0x80, 0x14, 0xae, 0x74, 0x93, 0xfa, 0x2b, 0x6c, 0x58, 0x2f, 0x9a, 0xfa, 0x19, 0x8f, 0x44, 0x1a, 
		0x6e, 0xc2, 0x06, 0xba, 0x6b, 0xb8, 0xe5, 0x47, 0x06, 0x01, 0x94, 0x99, 0x0a, 0xac, 0x33, 0xec, 
		0x67, 0xbb, 0xda, 0x1b, 0x89, 0x95, 0x01, 0x9e, 0xcd, 0xce, 0x69, 0xe2, 0x95, 0x34, 0x72, 0x2c, 
		0x64, 0xe1, 0x2e, 0xf6, 0x23, 0x51, 0xb3, 0xd6, 0xc4, 0x38, 0x30, 0xb7, 0xa6, 0xe9, 0x42, 0x63, 
		0xdf, 0xa4, 0xdf, 0xc4, 0x6f, 0xd1, 0xc1, 0x54, 0x62, 0x95, 0xa2, 0xbe, 0x00, 0x56, 0x90, 0x15, 
		0x77, 0xc8, 0xe8, 0xed, 0x30, 0x7e, 0xe1, 0x48, 0xe4, 0x59, 0x9f, 0xf1, 0x4a, 0x28, 0xc0, 0x87, 
		0x85, 0x42, 0xad, 0xff, 0x16, 0x19, 0x38, 0x17, 0xd7, 0xff, 0xe9, 0x1c, 0xe3, 0x75, 0xed, 0x1b, 
		0xc0, 0x08, 0x64, 0x18, 0x62, 0xa6, 0xae, 0x10, 0x2b, 0x19, 0x54, 0xaf, 0xf1, 0x95, 0xfd, 0x48, 
		0x05, 0xd0, 0x7a, 0x60, 0x63, 0x40, 0x37, 0x14, 0x73, 0x08, 0xe0, 0x8c, 0xde, 0x67, 0xdc, 0xab, 
		0x2e, 0xc2, 0xaa, 0x07, 0xb4, 0x74, 0xad, 0xe6, 0x32, 0xb1, 0x98, 0xaa, 0x7c, 0x2d, 0x1d, 0xaa, 
		0x10, 0x0a, 0xfb, 0xfc, 0x9f, 0xd7, 0x13, 0xd1, 0x8e, 0xd5, 0xd0, 0x22, 0x12, 0x54, 0x20, 0x73, 
		0x06, 0x40, 0x44, 0x87, 0xef, 0x93, 0xc2, 0xa6, 0x53, 0x6c, 0x33, 0x31, 0xe0, 0x45, 0xc7, 0x08, 
		0xe9, 0x51, 0xd0, 0xd0, 0xa9, 0xf7, 0xa3, 0x81, 0x9a, 0x6a, 0x2a, 0x88, 0xe8, 0xe9, 0x62, 0xe6, 
		0x4d, 0xcb, 0x8f, 0x43, 0xa4, 0x87, 0x6c, 0xcf, 0xbf, 0xbb, 0x46, 0x98, 0x94, 0x34, 0xa7, 0x7f, 
		0xbb, 0x6b, 0x66, 0xbb, 0xe1, 0x8c, 0x15, 0xae, 0x84, 0x94, 0xdc, 0xb9, 0x16, 0x16, 0x33, 0x4c, 
		0xdd, 0xf4, 0x7e, 0x21, 0x35, 0xa5, 0xe6, 0xa8, 0xc9, 0x92, 0x85, 0xb4, 0xd7, 0x11, 0x3d, 0x6f, 
		0xf3, 0xe3, 0x34, 0xa3, 0xee, 0x55, 0xf8, 0x87, 0x55, 0x1f, 0x5d, 0x46, 0xb0, 0xb0, 0x63, 0xff, 
		0x37, 0x5c, 0x16, 0x1e, 0xdb, 0x96, 0x84, 0x36, 0x1b, 0x2a, 0xb5, 0xbf, 0x35, 0xde, 0x1c, 0x5d, 
		0xc0, 0xc3, 0xaf, 0x6c, 0xf8, 0xe6, 0x33, 0x4e, 0x26, 0xae, 0x25, 0x93, 0x03, 0xe2, 0xd7, 0x2f, 
		0xae, 0x3d, 0xb3, 0x9e, 0x90, 0x59, 0xee, 0x57, 0x9f, 0x4e, 0x73, 0x59, 0xe6, 0xef, 0x9c, 0x72, 
		0xb9, 0x6f, 0x25, 0x94, 0x3d, 0x58, 0xad, 0x98, 0xe5, 0xb7, 0xd0, 0x9a, 0x38, 0x15, 0xc9, 0x9d, 
		0x57, 0x60, 0xb2, 0x06, 0xa4, 0x70, 0x4c, 0xfa, 0x46, 0xdf, 0x5d, 0xa4, 0x23, 0x56, 0xa9, 0x1c, 
		0xb7, 0x19, 0xc5, 0xe9, 0xdd, 0x84, 0x54, 0xc1, 0xd4, 0x05, 0xa2, 0xa6, 0xca, 0x4e, 0xf8, 0x62, 
		0xcf, 0x26, 0x46, 0xbe, 0xbb, 0x1f, 0x1e, 0x63, 0xc3, 0x51, 0xe7, 0xc6, 0xa5, 0xc3, 0xd1, 0x8b, 
		0xd4, 0xce, 0x96, 0xd0, 0x10, 0x2c, 0x67, 0x65, 0x79, 0xb5, 0x0e, 0x4a, 0x46, 0x4e, 0xa5, 0xd9, 
		0x74, 0x01, 0x34, 0xc6, 0x8b, 0x90, 0xf8, 0x9b, 0x1d, 0x5d, 0x18, 0x7b, 0xcf, 0xa2, 0xbb, 0x45, 
};

// @1E017420
static int xcode_building_first_stage(struct cxdec_xcode_status *xcode)
{
  	switch (xcode_rand(xcode) % 3) {
  	case 2:
		// MOV ESI, EncryptionControlBlock
		// MOV EAX, DWORD PTR DS:[ESI+((xcode_rand(xcode) & 0x3ff) << 2)]
		if (!push_bytexcode(xcode, 0xbe)
				|| !push_dwordxcode(xcode, (DWORD)EncryptionControlBlock)
				|| !push_2bytesxcode(xcode, 0x8b, 0x86)
				|| !push_dwordxcode(xcode, (xcode_rand(xcode) & 0x3ff) << 2))
			return 0;
		break;
	case 0:
		// MOV EAX, xcode_rand(xcode)
		if (!push_bytexcode(xcode, 0xb8)
				|| !push_dwordxcode(xcode, xcode_rand(xcode)))
			return 0;
		break;
  	case 1:
  		// MOV EAX, EDI
  		if (!push_2bytesxcode(xcode, 0x8b, 0xc7))
  			return 0;
  		break;
  	}
  	return 1;
}

// @1E017530
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
	case 7:
		// NOT EAX
		if (!push_2bytesxcode(xcode, 0xf7, 0xd0))
			return 0;
        break;
	case 3:
		// DEC EAX
		if (!push_bytexcode(xcode, 0x48))
			return 0;
        break;
	case 1:
		// XOR EAX, xcode_rand(xcode)
		if (!push_bytexcode(xcode, 0x35) 
				|| !push_dwordxcode(xcode, xcode_rand(xcode)))
			return 0;
		break;
	case 4:
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
	case 6:
		// INC EAX
		if (!push_bytexcode(xcode, 0x40))
			return 0;
        break;
	case 2:
		// NEG EAX
        if (!push_2bytesxcode(xcode, 0xf7, 0xd8))
			return 0;
		break;
	case 5:
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
	case 0:
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
	}
	return 1;
}

// @1E017790
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
	case 2:
		// ADD EAX, EBX
		if (!push_2bytesxcode(xcode, 0x01, 0xd8))
			return 0;
		break;
    case 5:
    	// SUB EAX, EBX
		if (!push_2bytesxcode(xcode, 0x29, 0xd8))
			return 0;
		break;
    case 4:
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
    	// IMUL EAX, EBX
		if (!push_3bytesxcode(xcode, 0x0f,  0xaf, 0xc3))
			return 0;
		break;
    case 0:
    	// NEG EAX, ADD EAX, EBX
		if (!push_2bytesxcode(xcode, 0xf7, 0xd8) 
				|| !push_2bytesxcode(xcode, 0x01, 0xd8))
			return 0;
		break;
    case 1:
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
	}
	// POP EBX
	return push_bytexcode(xcode, 0x5b);
}

struct cxdec_callback tenshin_Trial_cxdec_callback = {
	"tenshinTrial",
	{ 0x29f, 0x33f },
	xcode_building_stage1
};
;
