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
		0x82, 0xc4, 0x82, 0xa2, 0x82, 0xdc, 0x82, 0xb7, 0x81, 0x42, 0x0a, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 
		0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 
		0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 
		0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 
		0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 
		0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
		0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
		0x20, 0x20, 0x20, 0x20, 0x81, 0x77, 0x20, 0x46, 0x61, 0x69, 0x72, 0x43, 0x68, 0x69, 0x6c, 0x64, 
		0x20, 0x81, 0x78, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
		0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
		0x20, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
		0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
		0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
		0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
		0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x0a, 0x20, 0x20, 0x20, 
		0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
		0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
		0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
		0x20, 0x20, 0x20, 0x20, 0x32, 0x30, 0x30, 0x37, 0x20, 0x94, 0x4e, 0x20, 0x31, 0x32, 0x20, 0x8c, 
		0x8e, 0x20, 0x31, 0x34, 0x20, 0x93, 0xfa, 0x0a, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 
		0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 
		0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 
		0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 
		0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 
		0x2d, 0x2d, 0x0a, 0x0a, 0x81, 0x40, 0x82, 0xb1, 0x82, 0xcc, 0x93, 0x78, 0x82, 0xcd, 0x81, 0x75, 
		0x46, 0x61, 0x69, 0x72, 0x43, 0x68, 0x69, 0x6c, 0x64, 0x81, 0x76, 0x82, 0xf0, 0x82, 0xa8, 0x94, 
		0x83, 0x82, 0xa2, 0x8f, 0xe3, 0x82, 0xb0, 0x82, 0xa2, 0x82, 0xbd, 0x82, 0xbe, 0x82, 0xab, 0x81, 
		0x41, 0x90, 0xbd, 0x82, 0xc9, 0x97, 0x4c, 0x82, 0xe8, 0x93, 0xef, 0x82, 0xa4, 0x82, 0xb2, 0x82, 
		0xb4, 0x82, 0xa2, 0x82, 0xdc, 0x82, 0xb7, 0x81, 0x42, 0x0a, 0x81, 0x40, 0x41, 0x4c, 0x63, 0x6f, 
		0x74, 0x83, 0x58, 0x83, 0x5e, 0x83, 0x62, 0x83, 0x74, 0x88, 0xea, 0x93, 0xaf, 0x81, 0x41, 0x8c, 
		0xfa, 0x82, 0xad, 0x8c, 0xe4, 0x97, 0xe7, 0x90, 0x5c, 0x82, 0xb5, 0x8f, 0xe3, 0x82, 0xb0, 0x82, 
		0xdc, 0x82, 0xb7, 0x81, 0x42, 0x0a, 0x0a, 0x81, 0x40, 0x82, 0xb1, 0x82, 0xcc, 0x83, 0x68, 0x83, 
		0x4c, 0x83, 0x85, 0x83, 0x81, 0x83, 0x93, 0x83, 0x67, 0x83, 0x74, 0x83, 0x40, 0x83, 0x43, 0x83, 
		0x8b, 0x82, 0xc9, 0x82, 0xcd, 0x81, 0x41, 0x96, 0x7b, 0x83, 0x5c, 0x83, 0x74, 0x83, 0x67, 0x82, 
		0xcc, 0x91, 0x80, 0x8d, 0xec, 0x95, 0xfb, 0x96, 0x40, 0x82, 0xc8, 0x82, 0xc7, 0x82, 0xf0, 0x8b, 
		0x4c, 0x8d, 0xda, 0x82, 0xb5, 0x82, 0xc4, 0x82, 0xa2, 0x82, 0xdc, 0x82, 0xb7, 0x81, 0x42, 0x0a, 
		0x81, 0x40, 0x82, 0xb2, 0x88, 0xea, 0x93, 0xc7, 0x82, 0xcc, 0x8f, 0xe3, 0x81, 0x41, 0x82, 0xa8, 
		0x8a, 0x79, 0x82, 0xb5, 0x82, 0xdd, 0x82, 0xa2, 0x82, 0xbd, 0x82, 0xbe, 0x82, 0xab, 0x82, 0xdc, 
		0x82, 0xb7, 0x82, 0xe6, 0x82, 0xa4, 0x82, 0xa8, 0x8a, 0xe8, 0x82, 0xa2, 0x82, 0xa2, 0x82, 0xbd, 
		0x82, 0xb5, 0x82, 0xdc, 0x82, 0xb7, 0x81, 0x42, 0x0a, 0x0a, 0x0a, 0x0a, 0x82, 0x50, 0x81, 0x44, 
		0x83, 0x43, 0x83, 0x93, 0x83, 0x58, 0x83, 0x67, 0x81, 0x5b, 0x83, 0x8b, 0x95, 0xfb, 0x96, 0x40, 
		0x0a, 0x0a, 0x81, 0x40, 0x81, 0x45, 0x83, 0x51, 0x81, 0x5b, 0x83, 0x80, 0x83, 0x66, 0x83, 0x42, 
		0x83, 0x58, 0x83, 0x4e, 0x82, 0xf0, 0x44, 0x56, 0x44, 0x2d, 0x52, 0x4f, 0x4d, 0x83, 0x68, 0x83, 
		0x89, 0x83, 0x43, 0x83, 0x75, 0x82, 0xc9, 0x83, 0x5a, 0x83, 0x62, 0x83, 0x67, 0x82, 0xb5, 0x82, 
		0xc4, 0x82, 0xad, 0x82, 0xbe, 0x82, 0xb3, 0x82, 0xa2, 0x81, 0x42, 0x0a, 0x81, 0x40, 0x81, 0x45, 
		0x83, 0x56, 0x83, 0x58, 0x83, 0x65, 0x83, 0x80, 0x82, 0xaa, 0x83, 0x49, 0x81, 0x5b, 0x83, 0x67, 
		0x83, 0x89, 0x83, 0x93, 0x82, 0xc9, 0x91, 0xce, 0x89, 0x9e, 0x82, 0xb5, 0x82, 0xc4, 0x82, 0xa2, 
		0x82, 0xe9, 0x8f, 0xea, 0x8d, 0x87, 0x82, 0xcd, 0x81, 0x41, 0x83, 0x5a, 0x83, 0x62, 0x83, 0x67, 
		0x83, 0x41, 0x83, 0x62, 0x83, 0x76, 0x83, 0x81, 0x83, 0x6a, 0x83, 0x85, 0x81, 0x5b, 0x82, 0xaa, 
		0x8b, 0x4e, 0x93, 0xae, 0x0a, 0x81, 0x40, 0x81, 0x40, 0x82, 0xb5, 0x82, 0xdc, 0x82, 0xb7, 0x81, 
		0x42, 0x0a, 0x81, 0x40, 0x81, 0x40, 0x83, 0x49, 0x81, 0x5b, 0x83, 0x67, 0x83, 0x89, 0x83, 0x93, 
		0x82, 0xc9, 0x91, 0xce, 0x89, 0x9e, 0x82, 0xb5, 0x82, 0xc4, 0x82, 0xa2, 0x82, 0xc8, 0x82, 0xa2, 
		0x8f, 0xea, 0x8d, 0x87, 0x82, 0xcd, 0x81, 0x41, 0x83, 0x58, 0x83, 0x5e, 0x81, 0x5b, 0x83, 0x67, 
		0x83, 0x81, 0x83, 0x6a, 0x83, 0x85, 0x81, 0x5b, 0x82, 0xa9, 0x82, 0xe7, 0x81, 0x77, 0x83, 0x74, 
		0x83, 0x40, 0x83, 0x43, 0x83, 0x8b, 0x96, 0xbc, 0x82, 0xf0, 0x0a, 0x81, 0x40, 0x81, 0x40, 0x8e, 
		0x77, 0x92, 0xe8, 0x82, 0xb5, 0x82, 0xc4, 0x8e, 0xc0, 0x8d, 0x73, 0x81, 0x69, 0x52, 0x81, 0x6a, 
		0x81, 0x78, 0x82, 0xf0, 0x91, 0x49, 0x91, 0xf0, 0x82, 0xb5, 0x81, 0x41, 0x8e, 0x51, 0x8f, 0xc6, 
		0x83, 0x7b, 0x83, 0x5e, 0x83, 0x93, 0x82, 0xf0, 0x89, 0x9f, 0x82, 0xb5, 0x82, 0xc4, 0x44, 0x56, 
		0x44, 0x2d, 0x52, 0x4f, 0x4d, 0x93, 0xe0, 0x82, 0xc9, 0x82, 0xa0, 0x82, 0xe9, 0x0a, 0x81, 0x40, 
		0x81, 0x40, 0x81, 0x77, 0x73, 0x74, 0x61, 0x72, 0x74, 0x75, 0x70, 0x2e, 0x65, 0x78, 0x65, 0x81, 
		0x78, 0x82, 0xf0, 0x8e, 0xc0, 0x8d, 0x73, 0x82, 0xb5, 0x82, 0xc4, 0x83, 0x5a, 0x83, 0x62, 0x83, 
		0x67, 0x83, 0x41, 0x83, 0x62, 0x83, 0x76, 0x83, 0x81, 0x83, 0x6a, 0x83, 0x85, 0x81, 0x5b, 0x82, 
		0xf0, 0x8b, 0x4e, 0x93, 0xae, 0x82, 0xb5, 0x82, 0xc4, 0x82, 0xad, 0x82, 0xbe, 0x82, 0xb3, 0x82, 
		0xa2, 0x81, 0x42, 0x0a, 0x81, 0x40, 0x81, 0x45, 0x81, 0x75, 0x83, 0x43, 0x83, 0x93, 0x83, 0x58, 
		0x83, 0x67, 0x81, 0x5b, 0x83, 0x8b, 0x81, 0x76, 0x83, 0x7b, 0x83, 0x5e, 0x83, 0x93, 0x82, 0xf0, 
		0x89, 0x9f, 0x82, 0xb7, 0x82, 0xc6, 0x83, 0x43, 0x83, 0x93, 0x83, 0x58, 0x83, 0x67, 0x81, 0x5b, 
		0x83, 0x89, 0x81, 0x5b, 0x82, 0xaa, 0x97, 0xa7, 0x82, 0xbf, 0x8f, 0xe3, 0x82, 0xaa, 0x82, 0xe8, 
		0x82, 0xdc, 0x82, 0xb7, 0x81, 0x42, 0x0a, 0x81, 0x40, 0x81, 0x45, 0x89, 0xe6, 0x96, 0xca, 0x82, 
		0xcc, 0x8e, 0x77, 0x8e, 0xa6, 0x82, 0xc9, 0x8f, 0x5d, 0x82, 0xc1, 0x82, 0xc4, 0x81, 0x41, 0x83, 
		0x5a, 0x83, 0x62, 0x83, 0x67, 0x83, 0x41, 0x83, 0x62, 0x83, 0x76, 0x82, 0xf0, 0x8d, 0x73, 0x82, 
		0xc1, 0x82, 0xc4, 0x82, 0xad, 0x82, 0xbe, 0x82, 0xb3, 0x82, 0xa2, 0x81, 0x42, 0x0a, 0x0a, 0x0a, 
		0x0a, 0x82, 0x51, 0x81, 0x44, 0x83, 0x51, 0x81, 0x5b, 0x83, 0x80, 0x82, 0xcc, 0x8b, 0x4e, 0x93, 
		0xae, 0x95, 0xfb, 0x96, 0x40, 0x0a, 0x0a, 0x81, 0x40, 0x81, 0x45, 0x83, 0x58, 0x83, 0x5e, 0x81, 
		0x5b, 0x83, 0x67, 0x83, 0x81, 0x83, 0x6a, 0x83, 0x85, 0x81, 0x5b, 0x82, 0xcc, 0x81, 0x75, 0x83, 
		0x76, 0x83, 0x8d, 0x83, 0x4f, 0x83, 0x89, 0x83, 0x80, 0x81, 0x76, 0x81, 0xa8, 0x81, 0x75, 0x41, 
		0x4c, 0x63, 0x6f, 0x74, 0x81, 0x76, 0x81, 0xa8, 0x81, 0x75, 0x46, 0x61, 0x69, 0x72, 0x43, 0x68, 
		0x69, 0x6c, 0x64, 0x81, 0x76, 0x82, 0xcc, 0x92, 0x86, 0x82, 0xc9, 0x82, 0xa0, 0x82, 0xe9, 0x0a, 
		0x81, 0x40, 0x81, 0x40, 0x81, 0x75, 0x46, 0x61, 0x69, 0x72, 0x43, 0x68, 0x69, 0x6c, 0x64, 0x81, 
		0x76, 0x82, 0xf0, 0x91, 0x49, 0x91, 0xf0, 0x82, 0xb5, 0x82, 0xc4, 0x83, 0x51, 0x81, 0x5b, 0x83, 
		0x80, 0x82, 0xf0, 0x8b, 0x4e, 0x93, 0xae, 0x82, 0xb5, 0x82, 0xc4, 0x82, 0xad, 0x82, 0xbe, 0x82, 
		0xb3, 0x82, 0xa2, 0x81, 0x42, 0x0a, 0x0a, 0x0a, 0x0a, 0x82, 0x52, 0x81, 0x44, 0x83, 0x41, 0x83, 
		0x93, 0x83, 0x43, 0x83, 0x93, 0x83, 0x58, 0x83, 0x67, 0x81, 0x5b, 0x83, 0x8b, 0x95, 0xfb, 0x96, 
		0x40, 0x0a, 0x0a, 0x81, 0x40, 0x81, 0x45, 0x88, 0xc8, 0x89, 0xba, 0x82, 0xcc, 0x82, 0xc7, 0x82, 
		0xbf, 0x82, 0xe7, 0x82, 0xcc, 0x8e, 0xe8, 0x8f, 0x87, 0x82, 0xc5, 0x82, 0xe0, 0x83, 0x41, 0x83, 
		0x93, 0x83, 0x43, 0x83, 0x93, 0x83, 0x58, 0x83, 0x67, 0x81, 0x5b, 0x83, 0x8b, 0x82, 0xc5, 0x82, 
		0xab, 0x82, 0xdc, 0x82, 0xb7, 0x81, 0x42, 0x0a, 0x0a, 0x81, 0x40, 0x81, 0x75, 0x83, 0x51, 0x81, 
		0x5b, 0x83, 0x80, 0x95, 0x74, 0x91, 0xae, 0x82, 0xcc, 0x83, 0x5a, 0x83, 0x62, 0x83, 0x67, 0x83, 
		0x41, 0x83, 0x62, 0x83, 0x76, 0x83, 0x81, 0x83, 0x6a, 0x83, 0x85, 0x81, 0x5b, 0x82, 0xf0, 0x8e, 
		0x67, 0x82, 0xa4, 0x81, 0x76, 0x0a, 0x0a, 0x81, 0x40, 0x81, 0x45, 0x83, 0x51, 0x81, 0x5b, 0x83, 
		0x80, 0x83, 0x66, 0x83, 0x42, 0x83, 0x58, 0x83, 0x4e, 0x82, 0xf0, 0x44, 0x56, 0x44, 0x2d, 0x52, 
		0x4f, 0x4d, 0x83, 0x68, 0x83, 0x89, 0x83, 0x43, 0x83, 0x75, 0x82, 0xc9, 0x83, 0x5a, 0x83, 0x62, 
		0x83, 0x67, 0x82, 0xb5, 0x82, 0xc4, 0x82, 0xad, 0x82, 0xbe, 0x82, 0xb3, 0x82, 0xa2, 0x81, 0x42, 
		0x0a, 0x81, 0x40, 0x81, 0x45, 0x83, 0x56, 0x83, 0x58, 0x83, 0x65, 0x83, 0x80, 0x82, 0xaa, 0x83, 
		0x49, 0x81, 0x5b, 0x83, 0x67, 0x83, 0x89, 0x83, 0x93, 0x82, 0xc9, 0x91, 0xce, 0x89, 0x9e, 0x82, 
		0xb5, 0x82, 0xc4, 0x82, 0xa2, 0x82, 0xe9, 0x8f, 0xea, 0x8d, 0x87, 0x82, 0xcd, 0x81, 0x41, 0x83, 
		0x5a, 0x83, 0x62, 0x83, 0x67, 0x83, 0x41, 0x83, 0x62, 0x83, 0x76, 0x83, 0x81, 0x83, 0x6a, 0x83, 
		0x85, 0x81, 0x5b, 0x82, 0xaa, 0x8b, 0x4e, 0x93, 0xae, 0x82, 0xb5, 0x82, 0xdc, 0x82, 0xb7, 0x81, 
		0x42, 0x0a, 0x81, 0x40, 0x81, 0x40, 0x83, 0x49, 0x81, 0x5b, 0x83, 0x67, 0x83, 0x89, 0x83, 0x93, 
		0x82, 0xc9, 0x91, 0xce, 0x89, 0x9e, 0x82, 0xb5, 0x82, 0xc4, 0x82, 0xa2, 0x82, 0xc8, 0x82, 0xa2, 
		0x8f, 0xea, 0x8d, 0x87, 0x82, 0xcd, 0x81, 0x41, 0x83, 0x58, 0x83, 0x5e, 0x81, 0x5b, 0x83, 0x67, 
		0x83, 0x81, 0x83, 0x6a, 0x83, 0x85, 0x81, 0x5b, 0x82, 0xa9, 0x82, 0xe7, 0x81, 0x77, 0x83, 0x74, 
		0x83, 0x40, 0x83, 0x43, 0x83, 0x8b, 0x96, 0xbc, 0x82, 0xf0, 0x8e, 0x77, 0x92, 0xe8, 0x82, 0xb5, 
		0x82, 0xc4, 0x0a, 0x81, 0x40, 0x81, 0x40, 0x8e, 0xc0, 0x8d, 0x73, 0x81, 0x69, 0x52, 0x81, 0x6a, 
		0x81, 0x78, 0x82, 0xf0, 0x91, 0x49, 0x91, 0xf0, 0x82, 0xb5, 0x81, 0x41, 0x8e, 0x51, 0x8f, 0xc6, 
		0x83, 0x7b, 0x83, 0x5e, 0x83, 0x93, 0x82, 0xf0, 0x89, 0x9f, 0x82, 0xb5, 0x82, 0xc4, 0x44, 0x56, 
		0x44, 0x2d, 0x52, 0x4f, 0x4d, 0x93, 0xe0, 0x82, 0xc9, 0x82, 0xa0, 0x82, 0xe9, 0x81, 0x77, 0x73, 
		0x74, 0x61, 0x72, 0x74, 0x75, 0x70, 0x2e, 0x65, 0x78, 0x65, 0x81, 0x78, 0x82, 0xf0, 0x8e, 0xc0, 
		0x8d, 0x73, 0x0a, 0x81, 0x40, 0x81, 0x40, 0x82, 0xb5, 0x82, 0xc4, 0x83, 0x5a, 0x83, 0x62, 0x83, 
		0x67, 0x83, 0x41, 0x83, 0x62, 0x83, 0x76, 0x83, 0x81, 0x83, 0x6a, 0x83, 0x85, 0x81, 0x5b, 0x82, 
		0xf0, 0x8b, 0x4e, 0x93, 0xae, 0x82, 0xb5, 0x82, 0xc4, 0x82, 0xad, 0x82, 0xbe, 0x82, 0xb3, 0x82, 
		0xa2, 0x81, 0x42, 0x0a, 0x81, 0x40, 0x81, 0x45, 0x81, 0x75, 0x83, 0x41, 0x83, 0x93, 0x83, 0x43, 
		0x83, 0x93, 0x83, 0x58, 0x83, 0x67, 0x81, 0x5b, 0x83, 0x8b, 0x81, 0x76, 0x83, 0x7b, 0x83, 0x5e, 
		0x83, 0x93, 0x82, 0xf0, 0x89, 0x9f, 0x82, 0xb5, 0x82, 0xc4, 0x83, 0x41, 0x83, 0x93, 0x83, 0x43, 
		0x83, 0x93, 0x83, 0x58, 0x83, 0x67, 0x81, 0x5b, 0x83, 0x8b, 0x82, 0xf0, 0x8d, 0x73, 0x82, 0xc1, 
		0x82, 0xc4, 0x82, 0xad, 0x82, 0xbe, 0x82, 0xb3, 0x82, 0xa2, 0x81, 0x42, 0x0a, 0x0a, 0x0a, 0x81, 
		0x40, 0x81, 0x75, 0x83, 0x52, 0x83, 0x93, 0x83, 0x67, 0x83, 0x8d, 0x81, 0x5b, 0x83, 0x8b, 0x83, 
		0x70, 0x83, 0x6c, 0x83, 0x8b, 0x82, 0xcc, 0x81, 0x75, 0x83, 0x76, 0x83, 0x8d, 0x83, 0x4f, 0x83, 
		0x89, 0x83, 0x80, 0x82, 0xcc, 0x92, 0xc7, 0x89, 0xc1, 0x82, 0xc6, 0x8d, 0xed, 0x8f, 0x9c, 0x81, 
		0x76, 0x82, 0xf0, 0x8e, 0x67, 0x82, 0xa4, 0x81, 0x76, 0x0a, 0x0a, 0x81, 0x40, 0x81, 0x45, 0x83, 
		0x58, 0x83, 0x5e, 0x81, 0x5b, 0x83, 0x67, 0x83, 0x81, 0x83, 0x6a, 0x83, 0x85, 0x81, 0x5b, 0x82, 
		0xcc, 0x81, 0x75, 0x83, 0x76, 0x83, 0x8d, 0x83, 0x4f, 0x83, 0x89, 0x83, 0x80, 0x81, 0x76, 0x81, 
		0xa8, 0x81, 0x75, 0x90, 0xdd, 0x92, 0xe8, 0x81, 0x76, 0x81, 0xa8, 0x81, 0x75, 0x83, 0x52, 0x83, 
		0x93, 0x83, 0x67, 0x83, 0x8d, 0x81, 0x5b, 0x83, 0x8b, 0x83, 0x70, 0x83, 0x6c, 0x83, 0x8b, 0x81, 
		0x76, 0x82, 0xcc, 0x92, 0x86, 0x82, 0xc9, 0x82, 0xa0, 0x82, 0xe9, 0x0a, 0x81, 0x40, 0x81, 0x40, 
		0x81, 0x75, 0x83, 0x76, 0x83, 0x8d, 0x83, 0x4f, 0x83, 0x89, 0x83, 0x80, 0x82, 0xcc, 0x92, 0xc7, 
		0x89, 0xc1, 0x82, 0xc6, 0x8d, 0xed, 0x8f, 0x9c, 0x81, 0x76, 0x82, 0xf0, 0x91, 0x49, 0x91, 0xf0, 
		0x82, 0xb5, 0x82, 0xc4, 0x82, 0xad, 0x82, 0xbe, 0x82, 0xb3, 0x82, 0xa2, 0x81, 0x42, 0x0a, 0x81, 
		0x40, 0x81, 0x45, 0x8c, 0xbb, 0x8d, 0xdd, 0x83, 0x43, 0x83, 0x93, 0x83, 0x58, 0x83, 0x67, 0x81, 
		0x5b, 0x83, 0x8b, 0x82, 0xb3, 0x82, 0xea, 0x82, 0xc4, 0x82, 0xa2, 0x82, 0xe9, 0x83, 0x76, 0x83, 
		0x8d, 0x83, 0x4f, 0x83, 0x89, 0x83, 0x80, 0x82, 0xa9, 0x82, 0xe7, 0x81, 0x75, 0x46, 0x61, 0x69, 
		0x72, 0x43, 0x68, 0x69, 0x6c, 0x64, 0x81, 0x76, 0x82, 0xf0, 0x91, 0x49, 0x91, 0xf0, 0x82, 0xb5, 
		0x82, 0xc4, 0x81, 0x75, 0x95, 0xcf, 0x8d, 0x58, 0x82, 0xc6, 0x8d, 0xed, 0x0a, 0x81, 0x40, 0x81, 
		0x40, 0x8f, 0x9c, 0x81, 0x76, 0x82, 0xf0, 0x83, 0x4e, 0x83, 0x8a, 0x83, 0x62, 0x83, 0x4e, 0x82, 
		0xb5, 0x82, 0xc4, 0x82, 0xad, 0x82, 0xbe, 0x82, 0xb3, 0x82, 0xa2, 0x81, 0x42, 0x0a, 0x81, 0x40, 
		0x81, 0x45, 0x83, 0x5a, 0x83, 0x62, 0x83, 0x67, 0x83, 0x41, 0x83, 0x62, 0x83, 0x76, 0x83, 0x81, 
		0x83, 0x6a, 0x83, 0x85, 0x81, 0x5b, 0x82, 0xaa, 0x8b, 0x4e, 0x93, 0xae, 0x82, 0xb5, 0x82, 0xdc, 
		0x82, 0xb7, 0x82, 0xcc, 0x82, 0xc5, 0x81, 0x75, 0x83, 0x41, 0x83, 0x93, 0x83, 0x43, 0x83, 0x93, 
		0x83, 0x58, 0x83, 0x67, 0x81, 0x5b, 0x83, 0x8b, 0x81, 0x76, 0x83, 0x7b, 0x83, 0x5e, 0x83, 0x93, 
		0x82, 0xf0, 0x89, 0x9f, 0x82, 0xb5, 0x82, 0xc4, 0x83, 0x41, 0x83, 0x93, 0x83, 0x43, 0x0a, 0x81, 
		0x40, 0x81, 0x40, 0x83, 0x93, 0x83, 0x58, 0x83, 0x67, 0x81, 0x5b, 0x83, 0x8b, 0x82, 0xf0, 0x8d, 
		0x73, 0x82, 0xc1, 0x82, 0xc4, 0x82, 0xad, 0x82, 0xbe, 0x82, 0xb3, 0x82, 0xa2, 0x81, 0x42, 0x0a, 
		0x0a, 0x0a, 0x0a, 0x82, 0x53, 0x81, 0x44, 0x8a, 0xee, 0x96, 0x7b, 0x91, 0x80, 0x8d, 0xec, 0x0a, 
		0x0a, 0x81, 0x40, 0x81, 0x45, 0x83, 0x65, 0x83, 0x4c, 0x83, 0x58, 0x83, 0x67, 0x91, 0x97, 0x82, 
		0xe8, 0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 0x81, 0x46, 0x8d, 0xb6, 0x83, 0x4e, 0x83, 
		0x8a, 0x83, 0x62, 0x83, 0x4e, 0x81, 0x5e, 0x83, 0x7a, 0x83, 0x43, 0x81, 0x5b, 0x83, 0x8b, 0x89, 
		0xba, 0x89, 0xf1, 0x93, 0x5d, 0x81, 0x5e, 0x45, 0x6e, 0x74, 0x65, 0x72, 0x81, 0x5e, 0x53, 0x70, 
		0x61, 0x63, 0x65, 0x0a, 0x81, 0x40, 0x81, 0x45, 0x83, 0x56, 0x83, 0x58, 0x83, 0x65, 0x83, 0x80, 
		0x83, 0x81, 0x83, 0x6a, 0x83, 0x85, 0x81, 0x5b, 0x95, 0x5c, 0x8e, 0xa6, 0x81, 0x46, 0x89, 0x45, 
		0x83, 0x4e, 0x83, 0x8a, 0x83, 0x62, 0x83, 0x4e, 0x81, 0x5e, 0x45, 0x73, 0x63, 0x0a, 0x81, 0x40, 
		0x81, 0x45, 0x83, 0x5a, 0x81, 0x5b, 0x83, 0x75, 0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 
		0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 0x81, 0x46, 0x83, 0x56, 0x83, 0x58, 0x83, 0x65, 0x83, 0x80, 
		0x83, 0x81, 0x83, 0x6a, 0x83, 0x85, 0x81, 0x5b, 0x82, 0xa9, 0x82, 0xe7, 0x81, 0x75, 0x83, 0x5a, 
		0x81, 0x5b, 0x83, 0x75, 0x81, 0x76, 0x81, 0x5e, 0x46, 0x32, 0x0a, 0x81, 0x40, 0x81, 0x45, 0x83, 
		0x8d, 0x81, 0x5b, 0x83, 0x68, 0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 0x81, 
		0x40, 0x81, 0x40, 0x81, 0x46, 0x83, 0x56, 0x83, 0x58, 0x83, 0x65, 0x83, 0x80, 0x83, 0x81, 0x83, 
		0x6a, 0x83, 0x85, 0x81, 0x5b, 0x82, 0xa9, 0x82, 0xe7, 0x81, 0x75, 0x83, 0x8d, 0x81, 0x5b, 0x83, 
		0x68, 0x81, 0x76, 0x81, 0x5e, 0x46, 0x33, 0x0a, 0x81, 0x40, 0x81, 0x45, 0x8a, 0xc2, 0x8b, 0xab, 
		0x90, 0xdd, 0x92, 0xe8, 0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 
		0x81, 0x46, 0x83, 0x56, 0x83, 0x58, 0x83, 0x65, 0x83, 0x80, 0x83, 0x81, 0x83, 0x6a, 0x83, 0x85, 
		0x81, 0x5b, 0x82, 0xa9, 0x82, 0xe7, 0x81, 0x75, 0x8a, 0xc2, 0x8b, 0xab, 0x90, 0xdd, 0x92, 0xe8, 
		0x81, 0x76, 0x81, 0x5e, 0x46, 0x38, 0x0a, 0x81, 0x40, 0x81, 0x45, 0x83, 0x45, 0x83, 0x42, 0x83, 
		0x93, 0x83, 0x68, 0x83, 0x45, 0x8f, 0xc1, 0x8b, 0x8e, 0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 0x81, 
		0x46, 0x83, 0x56, 0x83, 0x58, 0x83, 0x65, 0x83, 0x80, 0x83, 0x81, 0x83, 0x6a, 0x83, 0x85, 0x81, 
		0x5b, 0x82, 0xa9, 0x82, 0xe7, 0x81, 0x75, 0x83, 0x45, 0x83, 0x42, 0x83, 0x93, 0x83, 0x68, 0x83, 
		0x45, 0x8f, 0xc1, 0x8b, 0x8e, 0x81, 0x76, 0x0a, 0x81, 0x40, 0x81, 0x45, 0x89, 0xe6, 0x96, 0xca, 
		0x90, 0xd8, 0x82, 0xe8, 0x91, 0xd6, 0x82, 0xa6, 0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 
		0x81, 0x46, 0x83, 0x56, 0x83, 0x58, 0x83, 0x65, 0x83, 0x80, 0x83, 0x81, 0x83, 0x6a, 0x83, 0x85, 
		0x81, 0x5b, 0x82, 0xa9, 0x82, 0xe7, 0x81, 0x75, 0x89, 0xe6, 0x96, 0xca, 0x90, 0xd8, 0x82, 0xe8, 
		0x91, 0xd6, 0x82, 0xa6, 0x81, 0x76, 0x81, 0x5e, 0x46, 0x34, 0x0a, 0x81, 0x40, 0x81, 0x45, 0x83, 
		0x58, 0x83, 0x4c, 0x83, 0x62, 0x83, 0x76, 0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 0x81, 
		0x40, 0x81, 0x40, 0x81, 0x46, 0x83, 0x51, 0x81, 0x5b, 0x83, 0x80, 0x89, 0xe6, 0x96, 0xca, 0x89, 
		0x45, 0x89, 0xba, 0x82, 0xcc, 0x81, 0x75, 0x53, 0x6b, 0x69, 0x70, 0x81, 0x76, 0x83, 0x7b, 0x83, 
		0x5e, 0x83, 0x93, 0x81, 0x5e, 0x46, 0x35, 0x81, 0x5e, 0x46, 0x0a, 0x81, 0x40, 0x81, 0x45, 0x83, 
		0x87, 0x20, 0x03, 0x93, 0x50, 0xda, 0xe3, 0xab, 0x34, 0x9f, 0x13, 0x0e, 0xfb, 0x52, 0xa0, 0xff, 
		0xe2, 0xf2, 0x50, 0xd0, 0x2b, 0x2b, 0x38, 0x95, 0x3c, 0xc7, 0x69, 0x13, 0x6a, 0xa3, 0x31, 0xcb, 
		0x3f, 0xaf, 0x2a, 0xa3, 0x36, 0x9c, 0x99, 0x1c, 0x39, 0x21, 0x2b, 0xa7, 0x8a, 0x85, 0x85, 0x6b, 
		0xae, 0x3b, 0x1e, 0x09, 0x82, 0x7f, 0x06, 0x9f, 0xde, 0x29, 0x0b, 0x66, 0xbe, 0x15, 0xbf, 0xe3, 
		0x24, 0x79, 0x0a, 0xb1, 0x0a, 0xad, 0x5c, 0x16, 0xf8, 0x8a, 0x73, 0x64, 0xc8, 0x47, 0xed, 0xdf, 
		0x1d, 0xd8, 0xd9, 0x78, 0xfe, 0x5e, 0x3d, 0x20, 0xa5, 0x16, 0x85, 0x02, 0xa1, 0xd1, 0x5b, 0x75, 
		0x8a, 0x4e, 0x56, 0xd5, 0xec, 0x54, 0x3d, 0x4d, 0x5b, 0x44, 0x5a, 0x38, 0xae, 0x61, 0x7d, 0x6a, 
		0x20, 0xdd, 0x8b, 0x09, 0x18, 0xd3, 0xa8, 0x3b, 0x12, 0xe6, 0x60, 0xeb, 0x75, 0xf4, 0x91, 0xbf, 
		0x08, 0x99, 0xd2, 0x4d, 0x12, 0xb0, 0x78, 0x08, 0x0d, 0x75, 0xd0, 0xba, 0x33, 0x62, 0x15, 0x18, 
		0xb5, 0x75, 0xe4, 0x1c, 0xf2, 0xfb, 0xad, 0x2b, 0xad, 0xd4, 0x69, 0x4c, 0xc1, 0xae, 0x71, 0xd2, 
		0x4c, 0x96, 0x1b, 0xae, 0x58, 0xf2, 0x19, 0x40, 0xc0, 0xdd, 0xdf, 0xd6, 0x7a, 0x6a, 0x75, 0x8d, 
		0xf6, 0x20, 0xba, 0x64, 0xc0, 0x9b, 0x31, 0xf9, 0x20, 0xf1, 0x10, 0x22, 0x0c, 0x73, 0x8e, 0x63, 
		0x12, 0x07, 0x5c, 0xfc, 0x9c, 0x0f, 0x2e, 0xf3, 0x77, 0x8d, 0xe0, 0xb9, 0x36, 0xc1, 0x95, 0x02, 
		0x08, 0x2f, 0x24, 0xb9, 0x3a, 0xbb, 0x1f, 0x0a, 0xfa, 0xd1, 0xec, 0xac, 0x25, 0xcb, 0x0f, 0x7d, 
		0xb1, 0x7e, 0x19, 0x00, 0xff, 0x8f, 0xdf, 0x44, 0xdc, 0x9a, 0xc5, 0xe7, 0x1c, 0x41, 0x09, 0x30, 
		0xad, 0x0a, 0x7e, 0x48, 0xed, 0x95, 0xc4, 0x7e, 0x8e, 0x8a, 0x55, 0xb0, 0x6d, 0x93, 0x6f, 0x6d, 
		0x40, 0xf6, 0x18, 0xe9, 0x38, 0xd2, 0x53, 0xaa, 0x7c, 0xed, 0xfe, 0xb8, 0xba, 0x09, 0x46, 0xe9, 
		0x65, 0xdb, 0x78, 0x80, 0x37, 0x06, 0x85, 0x50, 0x32, 0x88, 0x68, 0x38, 0xd8, 0xaa, 0xc0, 0x03, 
		0xda, 0x3b, 0xe2, 0xf8, 0x12, 0xb6, 0x36, 0x5f, 0x59, 0x55, 0x53, 0x45, 0x6d, 0xfa, 0x46, 0xde, 
		0xb5, 0x60, 0x86, 0x24, 0x4f, 0x15, 0x85, 0xcb, 0x92, 0x1b, 0x17, 0x9e, 0xf0, 0xc5, 0x62, 0x26, 
		0xbe, 0x9e, 0x56, 0x44, 0x9b, 0x71, 0x18, 0xf1, 0x65, 0xa5, 0xb0, 0x7d, 0xea, 0x09, 0x06, 0xd8, 
		0xc7, 0xad, 0xe0, 0x4e, 0xed, 0xc7, 0xc3, 0x81, 0x2c, 0xe6, 0xe5, 0xe5, 0x04, 0x2d, 0xd0, 0xcf, 
		0x12, 0xf3, 0x65, 0xb7, 0xe4, 0x4d, 0x6f, 0xc3, 0x7a, 0x6f, 0xc3, 0x25, 0xe7, 0x52, 0x18, 0xa2, 
		0xaa, 0xa8, 0xe2, 0xa8, 0x5b, 0x78, 0xfb, 0xd4, 0x84, 0x93, 0x2d, 0x62, 0xe2, 0xbb, 0x04, 0xca, 
		0x3d, 0x93, 0xd6, 0xfd, 0xa4, 0xa6, 0x34, 0x7b, 0x94, 0xef, 0xff, 0x10, 0x61, 0x03, 0x61, 0xcc, 
		0x30, 0xcc, 0x0c, 0xf4, 0x0b, 0x97, 0x7f, 0x4f, 0xdf, 0x4b, 0x0c, 0xad, 0x01, 0x23, 0xfd, 0xbb, 
		0x2b, 0xaa, 0x02, 0xcb, 0xf6, 0xb0, 0x61, 0x94, 0x86, 0xa4, 0x2c, 0xae, 0x65, 0x8e, 0x5c, 0xe8, 
		0x2d, 0x57, 0xa3, 0xf4, 0x72, 0xd7, 0x06, 0x7e, 0x68, 0x8f, 0xe1, 0x33, 0x6c, 0x96, 0xd0, 0x84, 
		0xcc, 0x49, 0x19, 0x64, 0x38, 0x9c, 0x0a, 0xdb, 0xa3, 0x8c, 0xa3, 0x6a, 0xe7, 0xa9, 0x05, 0x9e, 
		0xfe, 0x1b, 0xad, 0x01, 0xc7, 0x2f, 0x4c, 0xee, 0xa0, 0x73, 0x32, 0x37, 0x2d, 0x1f, 0xe3, 0xe2, 
		0xd2, 0xd6, 0xda, 0xf3, 0x4b, 0x76, 0xc1, 0xb3, 0x04, 0x8f, 0x95, 0xc9, 0x80, 0x9d, 0xdd, 0xfa, 
		0xbc, 0x41, 0x88, 0x06, 0x57, 0xaa, 0x0e, 0x89, 0xc6, 0x4d, 0xa9, 0xba, 0xa8, 0x58, 0x32, 0xe0, 
		0x55, 0x38, 0x5e, 0xe8, 0x67, 0xda, 0x9f, 0x94, 0xd7, 0x67, 0x75, 0x60, 0xfa, 0x8b, 0xca, 0x9b, 
		0xec, 0xf3, 0xa3, 0xf5, 0x81, 0xa8, 0x9f, 0x73, 0x34, 0x0a, 0xb3, 0x18, 0x0f, 0x22, 0x8c, 0xa0, 
		0x13, 0xdc, 0xfe, 0xf0, 0x46, 0xab, 0x20, 0x77, 0x5a, 0xd1, 0xf9, 0x4c, 0x54, 0x93, 0xc8, 0x24, 
		0xb3, 0x2d, 0xc3, 0x4e, 0xbb, 0x0c, 0xa5, 0xf9, 0xd3, 0x38, 0x31, 0xfd, 0xf0, 0xf0, 0x57, 0xd9, 
		0x4b, 0x82, 0xd7, 0x25, 0x34, 0xda, 0x26, 0x6a, 0x65, 0x0f, 0xfc, 0xba, 0x31, 0xab, 0x90, 0xcd, 
		0x3b, 0x86, 0xef, 0xae, 0x0c, 0x8c, 0x5c, 0xcd, 0xa3, 0xfd, 0x15, 0x5a, 0x55, 0x8d, 0xc7, 0x42, 
		0x7e, 0x50, 0xdc, 0x30, 0x27, 0x04, 0x15, 0x91, 0x10, 0x35, 0xfd, 0xbe, 0x19, 0x2d, 0x87, 0x62, 
		0x72, 0xd0, 0x73, 0x2c, 0x42, 0xc3, 0x50, 0xd4, 0x67, 0xbf, 0xbd, 0x73, 0x5e, 0x42, 0x2a, 0xa9, 
		0x17, 0xbf, 0x13, 0xbf, 0x13, 0xd6, 0xd4, 0x90, 0xc5, 0xef, 0xc1, 0x30, 0xa7, 0x49, 0xe2, 0x31, 
		0xd9, 0x79, 0xbf, 0xfb, 0xab, 0x22, 0x26, 0xf3, 0xb3, 0x7b, 0x82, 0x63, 0xd4, 0x31, 0x4f, 0x70, 
		0x52, 0x4a, 0x56, 0x43, 0xb3, 0xbf, 0x62, 0x24, 0x58, 0x29, 0xcd, 0x59, 0xbb, 0x30, 0x0b, 0x1d, 
		0x86, 0xbf, 0x68, 0x5d, 0xbc, 0x9b, 0x84, 0x3a, 0x85, 0x00, 0x97, 0x05, 0x7f, 0x0e, 0x09, 0xfd, 
		0x5d, 0xf3, 0xc4, 0x02, 0x55, 0x76, 0x0e, 0xe9, 0xdc, 0x65, 0xfc, 0xd8, 0xb7, 0x19, 0xed, 0xa0, 
		0xa4, 0x33, 0x45, 0xec, 0x7f, 0xf6, 0x14, 0x07, 0x1b, 0x21, 0xd0, 0x8a, 0x6b, 0x9f, 0x41, 0x48, 
		0xbc, 0xbe, 0xbd, 0x17, 0xda, 0x74, 0x3d, 0x93, 0xec, 0x49, 0x3e, 0xf0, 0x54, 0x3c, 0x5b, 0xb3, 
		0x9f, 0xfe, 0x73, 0x92, 0x7d, 0x8d, 0x63, 0x00, 0xb6, 0x37, 0xc8, 0xc7, 0xba, 0x04, 0x30, 0x5f, 
		0x58, 0x03, 0xc7, 0xae, 0xbb, 0xc4, 0x26, 0x58, 0x72, 0x0f, 0xd6, 0x39, 0x1b, 0x78, 0x8c, 0xe7, 
		0x39, 0x09, 0xe2, 0x80, 0x1a, 0x80, 0x98, 0xcd, 0xef, 0xec, 0x59, 0x66, 0xa2, 0xda, 0x28, 0x54, 
		0xdb, 0x00, 0x8e, 0x0c, 0x80, 0x9a, 0x99, 0x33, 0xb0, 0xb3, 0x4b, 0x8d, 0x6d, 0x51, 0xb8, 0xde, 
		0x19, 0x54, 0xc5, 0xba, 0x38, 0x32, 0x14, 0xa9, 0xb0, 0x50, 0x37, 0x53, 0xeb, 0x97, 0x7b, 0x4b, 
		0x76, 0x45, 0xff, 0xf4, 0x48, 0x39, 0x64, 0xa2, 0x3b, 0x49, 0x67, 0x1b, 0xad, 0xcb, 0x1c, 0xc0, 
		0xa8, 0xb1, 0x15, 0x97, 0xce, 0x19, 0x16, 0x36, 0xb7, 0x79, 0x25, 0x80, 0xa4, 0x42, 0x81, 0xff, 
		0xad, 0xe7, 0x4c, 0x41, 0x45, 0xac, 0x5b, 0x36, 0xfb, 0x35, 0x4a, 0x36, 0xd1, 0x73, 0x0a, 0xc9, 
		0xd4, 0x45, 0x6c, 0x34, 0x7c, 0xd2, 0x9a, 0xa8, 0x49, 0xbb, 0xbd, 0x6f, 0xdc, 0x77, 0x75, 0x7c, 
		0xd7, 0xcd, 0x7b, 0x61, 0x3e, 0xbf, 0x69, 0xe7, 0x2e, 0x62, 0x26, 0x85, 0xf7, 0xd3, 0x28, 0x97, 
		0x5f, 0xcd, 0x8d, 0x7f, 0x60, 0x0b, 0x33, 0x37, 0x6a, 0x95, 0xdd, 0x0c, 0xca, 0x27, 0x77, 0x25, 
		0x20, 0x77, 0x26, 0x95, 0x3e, 0x53, 0xc8, 0xc0, 0x95, 0x3c, 0x04, 0x73, 0xbc, 0x50, 0x30, 0x5e, 
		0x5e, 0x47, 0xfb, 0x8e, 0x5f, 0xb7, 0x58, 0x2f, 0x06, 0x45, 0xa3, 0x04, 0x37, 0x46, 0xac, 0x9d, 
		0x08, 0xa2, 0x3c, 0x75, 0xd1, 0xed, 0x4a, 0xcd, 0x33, 0x86, 0xfd, 0x70, 0x84, 0xaf, 0xdf, 0xd7, 
		0xbb, 0x46, 0x7d, 0xc8, 0xf0, 0x8f, 0x85, 0x73, 0x5d, 0x8e, 0x87, 0x45, 0xef, 0x1c, 0xb9, 0xe0, 
		0x62, 0xa5, 0x6e, 0x59, 0xdc, 0xa8, 0x93, 0x33, 0xa2, 0xe4, 0x81, 0x2c, 0xc6, 0x0c, 0xf6, 0xfc, 
		0xb5, 0xaf, 0xb0, 0x59, 0x20, 0x5c, 0x35, 0xe5, 0x6f, 0x99, 0x10, 0x26, 0x16, 0x86, 0x5b, 0x3a, 
};

static int xcode_building_first_stage(struct cxdec_xcode_status *xcode)
{
  	switch (xcode_rand(xcode) % 3) {
  	case 2:
		// MOV EAX, xcode_rand(xcode)
		if (!push_bytexcode(xcode, 0xb8)
				|| !push_dwordxcode(xcode, xcode_rand(xcode)))
			return 0;
		break;
	case 0:
		// MOV ESI, EncryptionControlBlock
		// MOV EAX, DWORD PTR DS:[ESI+((xcode_rand(xcode) & 0x3ff) << 2)]
		if (!push_bytexcode(xcode, 0xbe)
				|| !push_dwordxcode(xcode, (DWORD)EncryptionControlBlock)
				|| !push_2bytesxcode(xcode, 0x8b, 0x86)
				|| !push_dwordxcode(xcode, (xcode_rand(xcode) & 0x3ff) << 2))
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
	case 6:
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
		// INC EAX
		if (!push_bytexcode(xcode, 0x40))
			return 0;
        break;
	case 2:
		// NOT EAX
		if (!push_2bytesxcode(xcode, 0xf7, 0xd0))
			return 0;
		break;
	case 7:
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
	case 5:
		// NEG EAX
        if (!push_2bytesxcode(xcode, 0xf7, 0xd8))
			return 0;
        break;
	case 3:
		// DEC EAX
		if (!push_bytexcode(xcode, 0x48))
			return 0;
		break;
	case 1:
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
	case 0:
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
	case 5:
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
    case 3:
    	// SUB EAX, EBX
		if (!push_2bytesxcode(xcode, 0x29, 0xd8))
			return 0;
		break;
    case 4:
    	// NEG EAX, ADD EAX, EBX
		if (!push_2bytesxcode(xcode, 0xf7, 0xd8) 
				|| !push_2bytesxcode(xcode, 0x01, 0xd8))
			return 0;
		break;
    case 0:
		// ADD EAX, EBX
		if (!push_2bytesxcode(xcode, 0x01, 0xd8))
			return 0;
		break;
    case 2:
    	// IMUL EAX, EBX
		if (!push_3bytesxcode(xcode, 0x0f,  0xaf, 0xc3))
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
	}
	// POP EBX
	return push_bytexcode(xcode, 0x5b);
}

struct cxdec_callback FairChild_cxdec_callback = {
	"FairChild",
	{ 0x231, 0x52e },
	xcode_building_stage1
};
;
