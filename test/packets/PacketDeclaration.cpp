#ifndef DEBUG_PREPROCESSOR
#include "gtest/gtest.h"
#include "Packet/MessageBuffer.h"
#include "Packet/EncodedInt.h"
#include "Packet/EncodingRandomized.h"
#include "Packet/EncodingScrambled.h"
#include <vector>
#endif

#include "Packet/PacketDeclaration.h"
#include "Packet/PacketBaseMessage.h"

enum EnumNoClass : uint16_t {
	ENC_Item1,
	ENC_Item2
};

#define TEST_OBJECT_SUB_DEF(_) \
	_(simple)(bool,     b) \
	_(simple)(uint32_t, i)
CREATE_STRUCT(TEST_OBJECT_SUB);

#define TEST_SIMPLE_SUB_DEF(_) \
	_(simple)(bool,                           b                       ) \
	_(simple)(char,                           c                       ) \
	_(simple)(int8_t,                         i8                      ) \
	_(simple)(uint8_t,                        u8                      ) \
	_(simple)(int16_t,                        i16                     ) \
	_(simple)(uint16_t,                       u16                     ) \
	_(simple)(int32_t,                        i32                     ) \
	_(simple)(uint32_t,                       u32                     ) \
	_(simple)(int64_t,                        i64                     ) \
	_(simple)(uint64_t,                       u64                     ) \
	_(simple)(float,                          f                       ) \
	_(simple)(double,                         d                       ) \
	_(simple)(long double,                    ld                      ) \
	_(simple)(EnumNoClass,                    enc                     ) \
	_(simple)(EncodedInt<EncodingRandomized>, er                      ) \
	_(simple)(EncodedInt<EncodingScrambled>,  es                      ) \
	_(simple)(TEST_OBJECT_SUB,                obj                     ) \
	_(string)(                                str   , 20              ) \
	\
	_(simple)(bool,                           v1_b  ,     version == 1) \
	_(simple)(char,                           v1_c  ,     version == 1) \
	_(simple)(int8_t,                         v1_i8 ,     version == 1) \
	_(simple)(uint8_t,                        v1_u8 ,     version == 1) \
	_(simple)(int16_t,                        v1_i16,     version == 1) \
	_(simple)(uint16_t,                       v1_u16,     version == 1) \
	_(simple)(int32_t,                        v1_i32,     version == 1) \
	_(simple)(uint32_t,                       v1_u32,     version == 1) \
	_(simple)(int64_t,                        v1_i64,     version == 1) \
	_(simple)(uint64_t,                       v1_u64,     version == 1) \
	_(simple)(float,                          v1_f  ,     version == 1) \
	_(simple)(double,                         v1_d  ,     version == 1) \
	_(simple)(long double,                    v1_ld ,     version == 1) \
	_(simple)(EnumNoClass,                    v1_enc,     version == 1) \
	_(simple)(EncodedInt<EncodingRandomized>, v1_er ,     version == 1) \
	_(simple)(EncodedInt<EncodingScrambled>,  v1_es ,     version == 1) \
	_(simple)(TEST_OBJECT_SUB,                v1_obj,     version == 1) \
	_(string)(                                v1_str, 20, version == 1) \
	\
	_(simple)(bool,                           v2_b  ,     version == 2, true) \
	_(simple)(char,                           v2_c  ,     version == 2, 'c') \
	_(simple)(int8_t,                         v2_i8 ,     version == 2, INT8_MIN) \
	_(simple)(uint8_t,                        v2_u8 ,     version == 2, UINT8_MAX) \
	_(simple)(int16_t,                        v2_i16,     version == 2, INT16_MIN) \
	_(simple)(uint16_t,                       v2_u16,     version == 2, UINT16_MAX) \
	_(simple)(int32_t,                        v2_i32,     version == 2, INT32_MIN) \
	_(simple)(uint32_t,                       v2_u32,     version == 2, UINT32_MAX) \
	_(simple)(int64_t,                        v2_i64,     version == 2, INT64_MIN) \
	_(simple)(uint64_t,                       v2_u64,     version == 2, UINT64_MAX) \
	_(simple)(float,                          v2_f  ,     version == 2, 54.8498f) \
	_(simple)(double,                         v2_d  ,     version == 2, 64.894794499884) \
	_(simple)(long double,                    v2_ld ,     version == 2, 66.49846416519898984984984984l) \
	_(simple)(EnumNoClass,                    v2_enc,     version == 2, ENC_Item2) \
	_(simple)(EncodedInt<EncodingRandomized>, v2_er ,     version == 2, UINT32_MAX) \
	_(simple)(EncodedInt<EncodingScrambled>,  v2_es ,     version == 2, UINT32_MAX) \
	_(simple)(TEST_OBJECT_SUB,                v2_obj,     version == 2, ((TEST_OBJECT_SUB){true, 42})) \
	_(string)(                                v2_str, 20, version == 2, "test string")
CREATE_STRUCT(TEST_SIMPLE_SUB);

#define TEST_ARRAY_SUB_DEF(_) \
	_(array) (bool,                           b    , 4               ) \
	_(array) (char,                           c    , 4               ) \
	_(array) (int8_t,                         i8   , 4               ) \
	_(array) (uint8_t,                        u8   , 4               ) \
	_(array) (int16_t,                        i16  , 4               ) \
	_(array) (uint16_t,                       u16  , 4               ) \
	_(array) (int32_t,                        i32  , 4               ) \
	_(array) (uint32_t,                       u32  , 4               ) \
	_(array) (int64_t,                        i64  , 4               ) \
	_(array) (uint64_t,                       u64  , 4               ) \
	_(array) (float,                          f    , 4               ) \
	_(array) (double,                         d    , 4               ) \
	_(array) (long double,                    ld   , 4               ) \
	_(array) (EnumNoClass,                    enc  , 4               ) \
	_(array) (EncodedInt<EncodingRandomized>, er   , 4               ) \
	_(array) (EncodedInt<EncodingScrambled>,  es   , 4               ) \
	_(array) (TEST_OBJECT_SUB,                obj  , 4               ) \
	\
	_(array)(bool,                            v1_b  , 5,  version == 1) \
	_(array)(char,                            v1_c  , 5,  version == 1) \
	_(array)(int8_t,                          v1_i8 , 5,  version == 1) \
	_(array)(uint8_t,                         v1_u8 , 5,  version == 1) \
	_(array)(int16_t,                         v1_i16, 5,  version == 1) \
	_(array)(uint16_t,                        v1_u16, 5,  version == 1) \
	_(array)(int32_t,                         v1_i32, 5,  version == 1) \
	_(array)(uint32_t,                        v1_u32, 5,  version == 1) \
	_(array)(int64_t,                         v1_i64, 5,  version == 1) \
	_(array)(uint64_t,                        v1_u64, 5,  version == 1) \
	_(array)(float,                           v1_f  , 5,  version == 1) \
	_(array)(double,                          v1_d  , 5,  version == 1) \
	_(array)(long double,                     v1_ld , 5,  version == 1) \
	_(array)(EnumNoClass,                     v1_enc, 5,  version == 1) \
	_(array)(EncodedInt<EncodingRandomized>,  v1_er , 5,  version == 1) \
	_(array)(EncodedInt<EncodingScrambled>,   v1_es , 5,  version == 1) \
	_(array)(TEST_OBJECT_SUB,                 v1_obj, 5,  version == 1) \
CREATE_STRUCT(TEST_ARRAY_SUB);

#define TEST_DYNARRAY_SUB_DEF(_) \
	_(count) (int8_t,                            b                      ) \
	_(count) (int8_t,                            c                      ) \
	_(count) (int8_t,                            i8                     ) \
	_(count) (uint8_t,                           u8                     ) \
	_(count) (int16_t,                           i16                    ) \
	_(count) (uint16_t,                          u16                    ) \
	_(count) (int32_t,                           i32                    ) \
	_(count) (uint32_t,                          u32                    ) \
	_(count) (int32_t,                           i64                    ) \
	_(count) (uint32_t,                          u64                    ) \
	_(count) (int8_t,                            f                      ) \
	_(count) (int8_t,                            d                      ) \
	_(count) (int8_t,                            ld                     ) \
	_(count) (int8_t,                            enc                    ) \
	_(count) (int8_t,                            er                     ) \
	_(count) (int8_t,                            es                     ) \
	_(count) (int8_t,                            obj                    ) \
	_(count) (int8_t,                            strNT                  ) \
	_(count) (int8_t,                            str                    ) \
	\
	_(count) (int8_t,                            v1_b     ,  version == 1) \
	_(count) (int8_t,                            v1_c     ,  version == 1) \
	_(count) (int8_t,                            v1_i8    ,  version == 1) \
	_(count) (uint8_t,                           v1_u8    ,  version == 1) \
	_(count) (int16_t,                           v1_i16   ,  version == 1) \
	_(count) (uint16_t,                          v1_u16   ,  version == 1) \
	_(count) (int32_t,                           v1_i32   ,  version == 1) \
	_(count) (uint32_t,                          v1_u32   ,  version == 1) \
	_(count) (int32_t,                           v1_i64   ,  version == 1) \
	_(count) (uint32_t,                          v1_u64   ,  version == 1) \
	_(count) (int8_t,                            v1_f     ,  version == 1) \
	_(count) (int8_t,                            v1_d     ,  version == 1) \
	_(count) (int8_t,                            v1_ld    ,  version == 1) \
	_(count) (int8_t,                            v1_enc   ,  version == 1) \
	_(count) (int8_t,                            v1_er    ,  version == 1) \
	_(count) (int8_t,                            v1_es    ,  version == 1) \
	_(count) (int8_t,                            v1_obj   ,  version == 1) \
	_(count) (int8_t,                            v1_strNT ,  version == 1) \
	_(count) (int8_t,                            v1_str   ,  version == 1) \
	\
	_(dynarray) (bool,                           b                      ) \
	_(dynarray) (char,                           c                      ) \
	_(dynarray) (int8_t,                         i8                     ) \
	_(dynarray) (uint8_t,                        u8                     ) \
	_(dynarray) (int16_t,                        i16                    ) \
	_(dynarray) (uint16_t,                       u16                    ) \
	_(dynarray) (int32_t,                        i32                    ) \
	_(dynarray) (uint32_t,                       u32                    ) \
	_(dynarray) (int64_t,                        i64                    ) \
	_(dynarray) (uint64_t,                       u64                    ) \
	_(dynarray) (float,                          f                      ) \
	_(dynarray) (double,                         d                      ) \
	_(dynarray) (long double,                    ld                     ) \
	_(dynarray) (EnumNoClass,                    enc                    ) \
	_(dynarray) (EncodedInt<EncodingRandomized>, er                     ) \
	_(dynarray) (EncodedInt<EncodingScrambled>,  es                     ) \
	_(dynarray) (TEST_OBJECT_SUB,                obj                    ) \
	_(dynstring)(strNT,                          true                   ) \
	_(dynstring)(str,                            false                  ) \
	\
	_(dynarray) (bool,                           v1_b     ,  version == 1) \
	_(dynarray) (char,                           v1_c     ,  version == 1) \
	_(dynarray) (int8_t,                         v1_i8    ,  version == 1) \
	_(dynarray) (uint8_t,                        v1_u8    ,  version == 1) \
	_(dynarray) (int16_t,                        v1_i16   ,  version == 1) \
	_(dynarray) (uint16_t,                       v1_u16   ,  version == 1) \
	_(dynarray) (int32_t,                        v1_i32   ,  version == 1) \
	_(dynarray) (uint32_t,                       v1_u32   ,  version == 1) \
	_(dynarray) (int64_t,                        v1_i64   ,  version == 1) \
	_(dynarray) (uint64_t,                       v1_u64   ,  version == 1) \
	_(dynarray) (float,                          v1_f     ,  version == 1) \
	_(dynarray) (double,                         v1_d     ,  version == 1) \
	_(dynarray) (long double,                    v1_ld    ,  version == 1) \
	_(dynarray) (EnumNoClass,                    v1_enc   ,  version == 1) \
	_(dynarray) (EncodedInt<EncodingRandomized>, v1_er    ,  version == 1) \
	_(dynarray) (EncodedInt<EncodingScrambled>,  v1_es    ,  version == 1) \
	_(dynarray) (TEST_OBJECT_SUB,                v1_obj   ,  version == 1) \
	_(dynstring)(v1_strNT,                       true     ,  version == 1) \
	_(dynstring)(v1_str,                         false    ,  version == 1)
CREATE_STRUCT(TEST_DYNARRAY_SUB);

#define TEST_SIMPLE_DEF(_) \
CREATE_PACKET(TEST_SIMPLE, 21);

#pragma pack(push, 1)
struct TEST_SIMPLE_PRIMITIVE_PLAIN : public TS_MESSAGE {
	static const uint16_t packetID = 21;
	bool b;
	char c;
	int8_t i8;
	uint8_t u8;
	int16_t i16;
	uint16_t u16;
	int32_t i32;
	uint32_t u32;
	int64_t i64;
	uint64_t u64;
	float f;
	double d;
	long double ld;
	EnumNoClass enc;
	EncodedInt<EncodingRandomized> er;
	EncodedInt<EncodingScrambled> es;
};
#pragma pack(pop)

TEST(packet, simple_primitive) {
	TEST_SIMPLE_PRIMITIVE_PLAIN s;
	TS_MESSAGE::initMessage(&s);

	s.b = true;
	s.c = 15;
}
