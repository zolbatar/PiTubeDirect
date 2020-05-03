#include "tuberom.h"
uint16_t tuberom_f100[0x800] = {
0x8000,0x7C01,0x4800,0x0000,0x8000,0x7800,0x4800,0x0001,0x8000,0xFFCB,0x4002,0x8000,0x0C33,0x4003,0x8000,0x061F,
0x4004,0x8903,0x4904,0x7002,0x0811,0x8000,0x0C0C,0x2800,0x09B7,0x8000,0x0000,0x2800,0x7FEE,0x2800,0x0B9E,0x8000,
0x002A,0x2800,0x7FEE,0x8000,0x0000,0x4003,0x8000,0x0A9B,0x4004,0x2800,0x7FF1,0x0194,0x0834,0x8000,0x0660,0x4003,
0x2800,0x7FF7,0xF800,0x081F,0x8000,0x007E,0x4003,0x2800,0x7FF4,0x2800,0x09C3,0x8000,0x0C2D,0x2800,0x09B7,0x2800,
0x09C3,0xF800,0x081F,0x047B,0x3000,0x3000,0x8000,0x7C01,0x4800,0x0000,0x2800,0x09C3,0x8000,0x07FA,0x4002,0x8802,
0x9000,0x0001,0x2800,0x09B7,0x2800,0x09C3,0x01F0,0xF800,0x081F,0x3000,0x3000,0x3000,0x8003,0xC000,0x0080,0x0181,
0x0875,0x8003,0x4901,0x8000,0x0004,0x2800,0x0BA5,0x8004,0x2800,0x0BA5,0x8B01,0x4003,0x8901,0x2800,0x0BA5,0x2800,
0x0B9E,0x4004,0x8B01,0x4003,0x3000,0x8000,0x0000,0x4002,0x8003,0xB000,0x0082,0x0191,0x08AD,0xB000,0x0083,0x0191,
0x08A9,0xB000,0x0084,0x0191,0x08A5,0x8000,0x0006,0x2800,0x0BA5,0x8004,0x2800,0x0BA5,0x8005,0x2800,0x0BA5,0x8003,
0x2800,0x0BA5,0x8003,0xB000,0x009D,0x0191,0x08A2,0x2800,0x0B9E,0x4901,0x2800,0x0B9E,0x4005,0x2800,0x0B9E,0x4004,
0x8B01,0x4002,0x8B01,0x4003,0x3000,0x8000,0x77FF,0x4003,0x3000,0x8000,0x1000,0x4003,0x3000,0x8000,0x0000,0x4003,
0x3000,0x8004,0x4901,0x8003,0x4901,0x2800,0x0912,0x8003,0x0191,0x08CC,0x8000,0x0002,0x2800,0x0BA5,0x8B01,0x2800,
0x0BAE,0x2800,0x0B9E,0x0087,0x08CC,0x8B01,0x4003,0x4901,0x2800,0x08D5,0x2800,0x08D1,0x8B01,0x4003,0x8B01,0x4004,
0x3000,0x8800,0x07EC,0x4002,0xF802,0x8000,0x0001,0x6003,0x8000,0x0001,0x5003,0x2800,0x09AD,0xB000,0x002A,0x0191,
0x08D8,0xB000,0x002F,0x0191,0x0907,0x8000,0x090D,0x4004,0x8000,0x0001,0xA003,0x4005,0x8000,0x0001,0x5004,0x8804,
0x4006,0x0191,0x0905,0x8000,0x0001,0x5005,0x8805,0x4007,0xB000,0x002E,0x0191,0x0905,0x8000,0x00DF,0xC007,0x4007,
0xB006,0x0181,0x090A,0xF800,0x08EC,0x8005,0x4003,0x8000,0x0001,0x5003,0x2800,0x09AD,0x4004,0x3000,0x0052,0x0055,
0x004E,0x0000,0x8004,0x4901,0x8005,0x4901,0x8006,0x4901,0x8007,0x4901,0x8B03,0x8903,0x2800,0x09AD,0x8004,0xB000,
0x002A,0x0191,0x091B,0x8000,0x095B,0x4004,0x8000,0x0001,0xA003,0x4005,0x8905,0x8904,0x4006,0x0193,0x0940,0x8805,
0x4007,0xC000,0x00DF,0xB006,0x0191,0x092A,0x8B04,0x8904,0x4006,0x0183,0x0937,0x8000,0x002E,0xB007,0x0181,0x0926,
0x8005,0x4003,0x8006,0x4004,0x2800,0x095B,0x8B01,0x4007,0x8B01,0x4006,0x8B01,0x4005,0x8B01,0x4004,0x3000,0x2800,
0x0BC8,0x2800,0x095B,0x8000,0x0000,0x4003,0x3000,0x8000,0x0001,0x4003,0x3000,0xF804,0x002E,0x8957,0x0047,0x004F,
0x894F,0x8957,0x3000,0x3000,0x3000,0x8003,0x0191,0x0A57,0x4901,0x8004,0x4901,0x8005,0x4901,0x8006,0x4901,0x8003,
0x4005,0x8004,0x4006,0x8000,0x0008,0x2800,0x0BA5,0x8005,0x2800,0x0BA5,0x8005,0xB000,0x0015,0x0184,0x0982,0x8000,
0x0000,0x4005,0x8005,0x9000,0x0A2D,0x4002,0x8802,0x2800,0x0BA5,0x8005,0x9000,0x0A2D,0x4002,0x8802,0x4003,0x8006,
0x4004,0x2800,0x09CC,0x8005,0x9000,0x0A42,0x4002,0x8802,0x2800,0x0BA5,0x8005,0x9000,0x0A42,0x4002,0x8802,0x4003,
0x8006,0x4004,0x2800,0x09FE,0x8B01,0x4006,0x8B01,0x4005,0x8B01,0x4004,0x8B01,0x4003,0x3000,0x8000,0x0001,0x6003,
0x8903,0xB000,0x0020,0x0191,0x09B0,0x4004,0x3000,0x4003,0x8B03,0x8903,0xC000,0x00FF,0x0191,0x09C2,0x2800,0x7FEE,
0xF800,0x09B9,0x3000,0x8000,0x000A,0x2800,0x7FEE,0x8000,0x000D,0x2800,0x7FEE,0x3000,0x8005,0x4901,0x8006,0x4901,
0x8000,0xFFFF,0x9003,0x4006,0x0193,0x09F9,0x8006,0x0021,0x9004,0x4004,0x8804,0x4005,0x8006,0xC000,0x0001,0x4003,
0x0194,0x09EB,0x8005,0x0078,0x2800,0x0BA5,0x8000,0x0001,0x6006,0x0193,0x09F9,0x2800,0x0BA5,0x8000,0x0001,0x6006,
0x0193,0x09F9,0x8000,0x0001,0x6004,0x8804,0x4005,0xF800,0x09E2,0x8B01,0x4006,0x8B01,0x4005,0x3000,0x8005,0x4901,
0x8006,0x4901,0x8000,0x0001,0x6006,0x0193,0x0A28,0x8006,0x0021,0x5004,0x8000,0x0000,0x4005,0x0380,0x0006,0x0A17,
0x2800,0x0B9E,0x0068,0x5005,0x8000,0x0001,0x6006,0x0193,0x0A1E,0x2800,0x0B9E,0xC000,0x00FF,0x5005,0x8005,0x4B04,
0x8000,0x0000,0x4005,0x8000,0x0001,0x6006,0x0183,0x0A10,0x8B01,0x4006,0x8B01,0x4005,0x3000,0x0010,0x0000,0x0005,
0x0000,0x0005,0x0004,0x0005,0x0008,0x000E,0x0004,0x0001,0x0001,0x0005,0x0000,0x0008,0x0019,0x0010,0x000D,0x0000,
0x0008,0x0080,0x0010,0x0005,0x0000,0x0005,0x0000,0x0005,0x0000,0x0000,0x0000,0x0005,0x0009,0x0005,0x0000,0x0008,
0x0019,0x0001,0x000D,0x000D,0x0080,0x0008,0x0080,0x8004,0x4901,0x8000,0x000A,0x2800,0x0BA5,0x8004,0x9000,0x0003,
0x4002,0x8802,0x2800,0x0BA5,0x8004,0x9000,0x0002,0x4002,0x8802,0x2800,0x0BA5,0x8004,0x9000,0x0001,0x4002,0x8802,
0x2800,0x0BA5,0x8000,0x0007,0x2800,0x0BA5,0x8000,0x0000,0x2800,0x0BA5,0x2800,0x0B9E,0xC000,0x0080,0x0181,0x0A95,
0x8000,0x0000,0x4005,0x8804,0x4004,0x2800,0x0B9E,0x4002,0x4804,0x8000,0x0001,0x5004,0x8000,0x0001,0x5005,0x8002,
0xB000,0x000D,0x0181,0x0A85,0xD002,0x8B01,0x4004,0x8000,0x0000,0x4003,0x3800,0x0660,0x018C,0x0020,0x00FF,0x9000,
0x0030,0x4002,0x8800,0x7EF8,0x0086,0x0AA2,0x8002,0x4800,0x7EF9,0x3000,0x8000,0x0000,0x2800,0x0BA5,0x2800,0x0B9E,
0x2800,0x0B9E,0x3000,0x4901,0x8003,0x4901,0xF800,0x0624,0x8800,0x7EFE,0xC000,0x0080,0x0181,0x0AEB,0x8800,0x7EF8,
0x0097,0x0AC4,0xF800,0x0626,0x8800,0x7EF9,0x4003,0x0097,0x0AE2,0x8004,0x4901,0x8005,0x4901,0x2800,0x0B93,0x4005,
0x2800,0x0B93,0x4004,0x2800,0x0B93,0x4003,0x2800,0x0AE0,0x8B01,0x4005,0x8B01,0x4004,0x8B01,0x4003,0x8B01,0x3000,
0xF800,0x0640,0x8003,0x5003,0x8003,0x4800,0x07FE,0x8B01,0x4003,0x8B01,0x3000,0x8800,0x7EFF,0x4003,0x0087,0x0B0D,
0x8004,0x4901,0x2800,0x0B9E,0x8000,0x0660,0x4004,0x2800,0x0B9E,0x4804,0x8000,0x0001,0x5004,0x2800,0x0B9E,0x4003,
0x4804,0x8000,0x0001,0x5004,0x8003,0x0181,0x0AFD,0x8000,0x0660,0x4800,0x07FA,0xF800,0x0846,0x8004,0x4901,0x8005,
0x4901,0x8003,0x4004,0x2800,0x0BB8,0x8004,0xB000,0x0005,0x0191,0x0B3E,0x2800,0x0BB8,0x2800,0x0BB8,0x2800,0x0BB8,
0x0068,0x4005,0x2800,0x0BB8,0x5005,0x8800,0x7EFD,0x8800,0x7EFD,0x2800,0x0BB8,0x8004,0x9004,0x1000,0xF800,0x0B46,
0xF800,0x0B69,0xF800,0x0B91,0xF800,0x0B91,0xF800,0x0B8E,0xF800,0x0B3E,0xF800,0x0B91,0xF800,0x0B91,0x8B01,0x4005,
0x8B01,0x4004,0x8B01,0x4003,0x8B01,0x3000,0x8000,0x0000,0x4004,0x8800,0x7EFE,0x4003,0x0097,0x0B3E,0x8800,0x7EFC,
0x4003,0x0086,0x0B49,0x8000,0x0000,0xA004,0x0193,0x0B65,0x8805,0x4004,0x4800,0x7EFD,0x8000,0x0001,0x5005,0x0328,
0x0004,0x03EF,0x0004,0xF800,0x0B49,0x4800,0x7EFD,0xF800,0x0B46,0x8000,0x0000,0x4004,0x8800,0x7EFE,0x0097,0x0B3E,
0x8800,0x7EFC,0x0087,0x0B6C,0x8000,0x0000,0xA004,0x0193,0x0B7F,0x8800,0x7EFD,0x00EF,0x4004,0xF800,0x0B6C,0x8004,
0xC000,0x00FF,0x4004,0x8800,0x7EFD,0x0068,0x9004,0x4004,0x4805,0x8000,0x0001,0x5005,0xF800,0x0B69,0x8005,0x4800,
0x07EC,0xF800,0x0B3E,0x8800,0x7EF8,0x0097,0x0B9B,0x8800,0x7EFE,0x0087,0x0B93,0x8800,0x7EF9,0x3000,0x8800,0x7EFA,
0x0087,0x0B9E,0x8800,0x7EFB,0x3000,0x4002,0x8800,0x7EFA,0x0086,0x0BA6,0x8002,0x4800,0x7EFB,0x3000,0x4003,0x8B03,
0x8903,0x2800,0x0BA5,0xB000,0x000D,0x0181,0x0BB0,0x3000,0x8800,0x7EFE,0x0087,0x0BB8,0x8800,0x7EFF,0x3000,0x4002,
0x8800,0x7EFE,0x0086,0x0BC0,0x8002,0x4800,0x7EFF,0x3000,0x8005,0x4901,0x2800,0x09AD,0x8000,0x0000,0x4004,0x8000,
0x0004,0x4005,0x2800,0x0BE0,0x0194,0x0BDD,0x8000,0x0001,0x6005,0x0181,0x0BD2,0xD000,0x0000,0x8B01,0x4005,0x3000,
0x8005,0x4901,0x8803,0x4005,0xB000,0x002F,0x0194,0x0BFB,0xB000,0x0039,0x0194,0x0C00,0xC000,0x00DF,0x4005,0x8000,
0x0007,0x6005,0x8005,0xB000,0x0039,0x0194,0x0BFB,0xB000,0x003F,0x0194,0x0C00,0xC000,0x0000,0x8B01,0x4005,0x3800,
0x0364,0x0004,0x8000,0x000F,0xC005,0x5004,0x8903,0xD000,0x0000,0x8B01,0x4005,0x3800,0x000A,0x0046,0x0065,0x0072,
0x0072,0x0061,0x006E,0x0074,0x0069,0x0020,0x0046,0x0031,0x0030,0x0030,0x002D,0x004C,0x0020,0x0028,0x0033,0x0032,
0x004B,0x0020,0x0077,0x006F,0x0072,0x0064,0x0073,0x0029,0x000A,0x000A,0x000D,0x0000,0x0011,0x0045,0x0073,0x0063,
0x0061,0x0070,0x0065,0x0000,0xF800,0x0845,0xF800,0x0846,0xF800,0x0AB8,0xF800,0x0845,0xF800,0x08B1,0xF800,0x085C,
0xF800,0x0965,0xF800,0x0AA1,0xF800,0x0AAA,0xF800,0x0962,0xF800,0x0859,0xF800,0x085A,0xF800,0x085B,0xF800,0x0964,
0xF800,0x0963,0xF800,0x0845,0xF800,0x0844,0xF800,0x0845,0xF800,0x0845,0xF800,0x0845,0xF800,0x0845,0xF800,0x0845,
0xF800,0x0845,0xF800,0x0845,0xF800,0x0844,0xF800,0x0844,0xF800,0x0844,0x8000,0x0FFF,0x4003,0x8000,0xFDED,0x4002,
0x8000,0x0002,0x4903,0x7002,0x0C70,0x8000,0xFF61,0x400A,0x8000,0x0000,0x4005,0x400B,0x4004,0x8000,0x0212,0x4006,
0x9000,0x1000,0x4009,0x8000,0x0000,0x4007,0x8809,0x0043,0x9809,0x9809,0x5007,0x8006,0x0041,0x4011,0x8B11,0x8007,
0x4010,0x2800,0x0D37,0x8010,0x4007,0x8011,0x4809,0x8000,0x0001,0x6006,0x0191,0x0CA7,0x8B09,0x8007,0x4010,0x8006,
0x4011,0x2800,0x0D58,0x8011,0x4007,0xF800,0x0C86,0x8007,0x4010,0x8000,0x000A,0x4011,0x2800,0x0D37,0x8010,0x9005,
0x4008,0x8011,0x4005,0x8008,0xB000,0x0009,0x0181,0x0CBB,0x8B04,0x700A,0x0C7D,0x8008,0xB000,0x000A,0x0181,0x0CD0,
0x8000,0x0000,0x4008,0x890B,0x800B,0x2800,0x0A9F,0x8004,0x0191,0x0CEA,0x8000,0x0000,0x2800,0x0A9F,0x7004,0x0CCA,
0x800A,0xB000,0xFF61,0x0191,0x0CEA,0x800B,0x2800,0x0A9F,0x800A,0xB000,0xFF62,0x0181,0x0CE1,0x8000,0x002E,0x2800,
0x7FEE,0x8004,0x0191,0x0CEA,0x8000,0x0009,0x2800,0x0A9F,0x7004,0x0CE4,0x8008,0x400B,0x700A,0x0C7D,0x800B,0x2800,
0x0A9F,0x3000,0x8000,0x0002,0x2800,0x0A9F,0x8000,0x002E,0x2800,0x7FEE,0x8000,0x00A1,0xA000,0x0000,0x4002,0x8000,
0x0FFF,0x4003,0x8000,0x0001,0x4903,0x7002,0x0D02,0x8000,0x0000,0x4800,0x1000,0x8000,0x009E,0xA000,0x0000,0x4007,
0x8000,0x0000,0x4004,0x8000,0x00A1,0x4006,0x8000,0x1000,0x9006,0x4003,0x8000,0xFF5F,0x4006,0x8803,0x400C,0x0063,
0x900C,0x900C,0x9004,0x4005,0x4010,0x8006,0xA000,0x0001,0x4011,0x2800,0x0D37,0x8010,0x4004,0x8011,0x4B03,0x7006,
0x0D1D,0x8004,0x2800,0x0A9F,0x7007,0x0D10,0x3000,0x8000,0x0000,0x400F,0x400E,0x8000,0xFFF0,0x400D,0x0341,0x000F,
0x0341,0x000E,0x038F,0x0010,0x0D47,0x03E0,0x000E,0x8011,0xB00E,0x0193,0x0D4F,0x8011,0x600E,0x03E0,0x000F,0x0341,
0x0010,0x700D,0x0D3E,0x800E,0x4011,0x800F,0x4010,0x3000,0x8000,0xFFF1,0x400D,0x8011,0x400F,0x8000,0x0000,0x4011,
0x0380,0x000F,0x0D65,0x8010,0x5011,0x0301,0x000F,0x0361,0x0010,0x700D,0x0D60,0x3000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
};
uint16_t tuberom_f100_high[0x100] = {
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xF800,0x0AAA,0x0000,0xF800,0x0AA1,0x0000,0xF800,0x063C,
0x0000,0xF800,0x063A,0x0000,0xF800,0x0638,0x0000,0xF800,0x0636,0x0000,0xF800,0x0634,0x0000,0xF800,0x0632,0x0000,
0xF800,0x0630,0x0000,0xB000,0x000D,0x0181,0x7FEE,0xF800,0x09C3,0x0000,0x0000,0x0000,0x8000,0x000D,0xF800,0x062E,
0x0000,0xF800,0x062C,0x0000,0xF800,0x062A,0x0000,0xF800,0x0628,0x0000,0x0000,0x0000,0x0000,0x0000,0xF800,0x0AB3,
};
