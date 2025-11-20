#include <stdint.h>

volatile const uint32_t
crcStart __attribute__((used, section("crc_foot_start_address"), space(prog), address(0x1FFFC))) = 0xFFFFFFFF;